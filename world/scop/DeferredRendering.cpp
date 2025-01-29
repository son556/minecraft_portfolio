#include "pch.h"
#include "DeferredRendering.h"
#include "MapUtils.h"
#include "DeferredGraphics.h"
#include "RasterizerState.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "Wallpaper.h"
#include "Block.h"
#include "ConstantBuffer.h"
#include "DeferredBuffer.h"
#include "ReflectionCube.h"


struct cbuffer_pbr
{
	vec4 view_pos;
	vec4 light_pos;
	int ao_flag;
	int mettalic_flag;
	int roughness_flag;
	int dummy;
};

DeferredRendering::DeferredRendering(
	MapUtils* minfo,
	DeferredGraphics* defer_graphic
)
	: m_info(minfo), s_render(minfo, defer_graphic), 
	g_render(minfo, defer_graphic), ssao_render(defer_graphic, 
		minfo->width, minfo->height), 
	ssao_blur(defer_graphic, minfo->width, minfo->height),
	pbr(defer_graphic, minfo->width, minfo->height),
	cave_shadow(defer_graphic, m_info), oit(defer_graphic, minfo)
{
	this->d_graphic = defer_graphic;
	this->cube_map = make_shared<Wallpaper>(this->d_graphic,
		this->m_info->width, this->m_info->height);
	this->reflection_cube = make_shared<ReflectionCube>(
		this->d_graphic, this->m_info, 
		&(this->g_render), this->cube_map.get(), &(this->pbr));
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buffer = make_shared<DeferredBuffer>(1);
	this->d_buffer->setRTVsAndSRVs(device, 
		this->m_info->width, this->m_info->height);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"ResultVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"ResultPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pt.data(),
		InputLayouts::layout_pt.size(),
		this->vertex_shader->getBlob()
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->sampler_state = make_shared<SamplerState>(device);
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
	this->vbuffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->ibuffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->view_port.TopLeftX = 0.0f;
	this->view_port.TopLeftY = 0.0f;
	this->view_port.Width = this->m_info->width;
	this->view_port.Height = this->m_info->height;
	this->view_port.MinDepth = 0.f;
	this->view_port.MaxDepth = 1.f;

	// 결과 출력용
	this->fin_vs = make_shared<VertexShader>(
		device,
		L"FinishVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->fin_ia = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pt.data(),
		InputLayouts::layout_pt.size(),
		this->fin_vs->getBlob()
	);
	this->fin_ps = make_shared<PixelShader>(
		device,
		L"FinishPS.hlsl",
		"main",
		"ps_5_0"
	);
}

DeferredRendering::~DeferredRendering()
{
}

void DeferredRendering::ssaoBlur(
	int cnt, 
	Mat const& proj,
	Mat const& view
)
{
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();
	//// ssao blur width start
	this->d_graphic->renderBegin(
		this->ssao_blur.getWidthDBuffer().get());
	context->PSSetShaderResources(0, 1,
		this->g_render.getSRV(RTVIndex::w_normal).GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->g_render.getDepthSRV().GetAddressOf());
	context->PSSetShaderResources(2, 1,
		this->ssao_render.getSRV().GetAddressOf());
	this->ssao_blur.render(0, proj, view, 1.0f);

	//// ssao blur height start
	this->d_graphic->renderBegin(
		this->ssao_blur.getHeightDBuffer().get());
	context->PSSetShaderResources(0, 1,
		this->g_render.getSRV(RTVIndex::w_normal).GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->g_render.getDepthSRV().GetAddressOf());
	context->PSSetShaderResources(2, 1,
		this->ssao_blur.getWidthSRV().GetAddressOf());
	this->ssao_blur.render(1, proj, view, 1.0f);

	for (int i = 1; i < cnt; i++) {
		// ssao blur width start
		this->d_graphic->renderBegin(
			this->ssao_blur.getWidthDBuffer().get());
		context->PSSetShaderResources(0, 1,
			this->g_render.getSRV(RTVIndex::w_normal).GetAddressOf());
		context->PSSetShaderResources(1, 1,
			this->g_render.getDepthSRV().GetAddressOf());
		context->PSSetShaderResources(2, 1,
			this->ssao_blur.getHeightSRV().GetAddressOf());
		this->ssao_blur.render(0, proj, view, 1.0f);

		// ssao blur height start
		this->d_graphic->renderBegin(
			this->ssao_blur.getHeightDBuffer().get());
		context->PSSetShaderResources(0, 1,
			this->g_render.getSRV(RTVIndex::w_normal).GetAddressOf());
		context->PSSetShaderResources(1, 1,
			this->g_render.getDepthSRV().GetAddressOf());
		context->PSSetShaderResources(2, 1,
			this->ssao_blur.getWidthSRV().GetAddressOf());
		this->ssao_blur.render(1, proj, view, 1.0f);
	}
}

void DeferredRendering::Render(
	Mat const& cam_view, 
	Mat const& cam_proj, 
	vec3 const& cam_pos
)
{
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();

	// reflection cube
	this->reflection_cube->render(cam_pos);
	this->oit.setReflectionCube(this->reflection_cube->getSRV());

	// cube map start
	this->cube_map->render(cam_pos, cam_view, cam_proj);
	this->m_info->directional_light_pos = 
		this->cube_map->getDirectionalLightPos();
	this->m_info->light_dir = XMVector3Normalize(cam_pos - 
		this->m_info->directional_light_pos);
	
	// geo render
	 this->g_render.setParallaxFlag(true);
	this->g_render.render(cam_view, cam_proj, cam_pos);

	// pbr render
	this->pbr.setRTV();
	this->setPBRShaderResources();
	vec3 lp = this->m_info->directional_light_pos;
	this->pbr.render(lp, cam_pos, this->g_render.getSRV(RTVIndex::color));

	// shadow map render
	this->s_render.renderCSM(cam_view, cam_proj);
	context->PSSetShaderResources(9, 1,
		this->g_render.getSRV(RTVIndex::w_position).GetAddressOf());
	context->PSSetShaderResources(10, 1,
		this->g_render.getSRV(RTVIndex::ssao_normal).GetAddressOf());
	this->s_render.render(cam_view, cam_proj);
	this->cave_shadow.render(cam_view, cam_proj);

	// ssao start
	this->d_graphic->renderBegin(
		this->ssao_render.getDBuffer().get());
	context->PSSetShaderResources(0, 1,
		this->g_render.getSRV(RTVIndex::ssao_normal).GetAddressOf()
	);
	context->PSSetShaderResources(1, 1,
		this->g_render.getSRV(RTVIndex::w_position).GetAddressOf()
	);
	context->PSSetShaderResources(
		2,
		1,
		this->g_render.getDepthSRV().GetAddressOf()
	);
	this->ssao_render.render(cam_view,cam_proj);
	
	// ssao blur start
	this->ssaoBlur(8, cam_proj, cam_view);

	// solid obj render result
	this->d_graphic->renderBegin(this->d_buffer.get());
	this->d_graphic->setViewPort(this->view_port);
	this->setPipe();
	context->PSSetShaderResources(0, 1,
		this->pbr.getAmbientLight().GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->pbr.getDirectLight().GetAddressOf());
	context->PSSetShaderResources(2, 1,
		this->s_render.getSRV().GetAddressOf());
	context->PSSetShaderResources(3, 1,
		this->ssao_blur.getHeightSRV().GetAddressOf());
	context->PSSetShaderResources(4, 1,
		this->cube_map->getSRV().GetAddressOf());
	context->PSSetShaderResources(5, 1,
		this->cave_shadow.getSRV().GetAddressOf());
	context->DrawIndexed(
		this->ibuffer->getCount(),
		0, 0);


	// oit render
	this->oit.setRTVandSRV(this->d_buffer->getRTV(0),
		this->d_buffer->getSRV(0), this->g_render.getDepthSRV());
	this->oit.render(cam_pos, cam_view, cam_proj);

	// result
	this->d_graphic->renderBegin();
	this->setFinPipe();
	context->PSSetShaderResources(0, 1,
		this->oit.getSRV().GetAddressOf());
	context->DrawIndexed(
		this->ibuffer->getCount(),
		0, 0);

	this->d_graphic->renderEnd();
}

void DeferredRendering::setPipe()
{
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	uint32 stride = this->vbuffer->getStride();
	uint32 offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->vbuffer->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->IASetIndexBuffer(
		this->ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->PSSetSamplers(0, 1,
		this->sampler_state->getComPtr().GetAddressOf());
}

void DeferredRendering::setFinPipe()
{
	ComPtr<ID3D11DeviceContext> context;
	context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->fin_ia->getComPtr().Get());
	uint32 stride = this->vbuffer->getStride();
	uint32 offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->vbuffer->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->IASetIndexBuffer(
		this->ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->VSSetShader(
		this->fin_vs->getComPtr().Get(),
		nullptr,
		0
	);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(
		this->fin_ps->getComPtr().Get(),
		nullptr,
		0
	);
}

void DeferredRendering::setPBRShaderResources()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->PSSetShaderResources(1, 1,
		this->g_render.getSRV(RTVIndex::w_normal).GetAddressOf());
	context->PSSetShaderResources(2, 1,
		this->g_render.getSRV(RTVIndex::w_position).GetAddressOf());
	context->PSSetShaderResources(3, 1,
		this->g_render.getSRV(RTVIndex::rma).GetAddressOf());
}
