#include "pch.h"
#include "WaterReflection.h"
#include "OpacityRender.h"
#include "TestCam.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "Buffer.h"
#include "Block.h"
#include "MapUtils.h"

WaterReflection::WaterReflection(MapUtils* m_info)
{
	this->r_opt = make_shared<RenderOption>();
	this->r_opt->parallax_mapping = false;
	this->r_opt->reflection_flag = true;
	this->r_opt->shadow = true;
	this->r_opt->ssao = false;
	this->r_opt->cave_shadow = false;
	this->r_opt->ccw_flag = true;
	this->r_opt->geo_opt.ccw_flag = true;
	this->r_opt->geo_opt.cut_flag = true;
	this->r_opt->geo_opt.cut_height = WATER_HEIGHT;

	ComPtr<ID3D11Device> device = d_graphic->getDevice();
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	desc.StencilEnable = true;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	HRESULT hr = device->CreateDepthStencilState(&desc, 
		this->ds_state.GetAddressOf());
	CHECK(hr);

	this->opacity_render = make_shared<OpacityRender>(m_info);
	this->init(device, m_info);
}

void WaterReflection::render()
{
	bool pre_under_water = under_water;
	if (cam->getPos().y < WATER_HEIGHT)
		under_water = true;
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	this->opacity_render->render(*(this->r_opt), CamType::REFLECTION_XZ);
	
	d_graphic->renderBegin(this->d_buff.get(), this->dsv, true, false);
	this->setPipe();
	context->OMSetDepthStencilState(this->ds_state.Get(), 1);
	context->PSSetShaderResources(0, 1,
		this->opacity_render->getSRV().GetAddressOf());
	context->PSSetShaderResources(1, 1,
		this->water_distortion_srv.GetAddressOf());
	context->DrawIndexed(this->i_buff->getCount(), 0, 0);
	
	context->OMSetDepthStencilState(nullptr, 0);
	under_water = pre_under_water;
}

ComPtr<ID3D11ShaderResourceView> WaterReflection::getSRV()
{
	return this->d_buff->getSRV(0);
}

void WaterReflection::setDSV(ComPtr<ID3D11DepthStencilView> dsv)
{
	this->dsv = dsv;
}

void WaterReflection::setWaterND(
	ComPtr<ID3D11ShaderResourceView> normal, 
	ComPtr<ID3D11ShaderResourceView> distortion
)
{
	this->water_normal_srv = normal;
	this->water_distortion_srv = distortion;
}

void WaterReflection::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	uint32 stride = this->v_buff->getStride();
	uint32 offset = this->v_buff->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->i_buff->getComPtr().Get(), DXGI_FORMAT_R32_UINT, 0);
	context->VSSetShader(this->vertex_shader->getComPtr().Get(), nullptr, 0);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(this->pixel_shader->getComPtr().Get(), nullptr, 0);
	context->PSSetSamplers(0, 1, this->sampler_state->getComPtr().GetAddressOf());
}

void WaterReflection::init(ComPtr<ID3D11Device>& device, MapUtils* m_info)
{
	this->d_buff = make_shared<DeferredBuffer>(1);
	this->d_buff->setRTVsAndSRVs(device, m_info->width, m_info->height);
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
		L"ReflectionVS.hlsl",
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
		D3D11_CULL_NONE
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"ReflectionPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->sampler_state = make_shared<SamplerState>(device);
}
