#include "pch.h"
#include "Water.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "RasterizerState.h"
#include "PixelShader.h"
#include "MapUtils.h"
#include "SamplerState.h"
#include "Buffer.h"
#include "Block.h"

#include "TestRender.h"

Water::Water(DeferredGraphics* d_graphic, MapUtils* m_info)
	: d_graphic(d_graphic), m_info(m_info), 
	water_init(d_graphic, m_info), water_reflection(d_graphic, m_info)
{
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->d_buff = make_shared<DeferredBuffer>(2);
	this->d_buff->setRTVsAndSRVs(
		device,
		this->m_info->width,
		this->m_info->height
	);
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
	this->v_buff = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->i_buff = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"WaterVS.hlsl",
		"main",
		"vs_5_0"
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
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"WaterPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->sampler_state = make_shared<SamplerState>(device);

	this->rt = make_shared<TestRender>(d_graphic, m_info);
}

void Water::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	uint32 stride = this->v_buff->getStride();
	uint32 offset = this->v_buff->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(this->i_buff->getComPtr().Get(), 
		DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr, 0);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(this->pixel_shader->getComPtr().Get(),
		nullptr, 0);
	context->PSSetSamplers(0, 1, 
		this->sampler_state->getComPtr().GetAddressOf());
}

void Water::render(
	vec3 const& cam_pos,
	Mat const& cam_view, 
	Mat const& cam_proj, 
	ComPtr<ID3D11ShaderResourceView> depth_srv, 
	ComPtr<ID3D11RenderTargetView>& rtv
)
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	
	// 물위치가 있는 곳을 그림
	this->water_init.render(cam_view, cam_proj, depth_srv);

	// 물 표면 반사를 그림
	this->water_reflection.render(cam_pos, this->reflection_cube,
		this->water_init.getSRV(WaterRTVType::POSITION));
	
	// 물 굴절 그림

	// 물 굴절 + 반사
	this->d_graphic->renderBegin(1, rtv.GetAddressOf(), nullptr, false, true);
	this->setPipe();
	context->PSSetShaderResources(0, 1,
		this->water_reflection.getSRV().GetAddressOf());
	context->DrawIndexed(
		this->i_buff->getCount(),
		0, 0);
}

void Water::setReflectionCube(
	ComPtr<ID3D11ShaderResourceView>& reflection_cube
)
{
	this->reflection_cube = reflection_cube;
}
