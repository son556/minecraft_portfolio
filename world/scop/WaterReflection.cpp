#include "pch.h"
#include "WaterReflection.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "MapUtils.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "SamplerState.h"
#include "Buffer.h"
#include "ConstantBuffer.h"
#include "Block.h"
#include "RasterizerState.h"
#include "TestCam.h"

WaterReflection::WaterReflection(MapUtils* m_info) 
	: m_info(m_info)
{
	ComPtr<ID3D11Device> device = d_graphic->getDevice();

	this->d_buff = make_shared<DeferredBuffer>(1);
	this->d_buff->setRTVsAndSRVs(device, 
		this->m_info->width, this->m_info->height);
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
		D3D11_CULL_BACK
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"ReflectionPS.hlsl",
		"main",
		"ps_5_0"
	);
	vec4 dummy;
	this->constant_buffer = make_shared<ConstantBuffer>(
		device,
		d_graphic->getContext(),
		dummy
	);
	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampler_desc.MinLOD = 0;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	this->sampler_state = make_shared<SamplerState>(device, sampler_desc);
}

void WaterReflection::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	uint32 offset = this->v_buff->getOffset();
	uint32 stride = this->v_buff->getStride();
	context->IASetVertexBuffers(0, 1,
		this->v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->i_buff->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
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
	context->PSSetSamplers(0, 1, this->sampler_state->getComPtr().GetAddressOf());
}

void WaterReflection::render(
	ComPtr<ID3D11ShaderResourceView> const& reflection_cube,
	ComPtr<ID3D11ShaderResourceView> const& water_pos
)
{
	vec3 cam_pos = cam->getPos();
	vec4 tmp = vec4(cam_pos.x, cam_pos.y, cam_pos.z, 1);
	this->constant_buffer->update(tmp);

	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	d_graphic->renderBegin(this->d_buff.get());
	this->setPipe();
	context->PSSetShaderResources(0, 1, reflection_cube.GetAddressOf());
	context->PSSetShaderResources(1, 1, water_pos.GetAddressOf());
	context->PSSetConstantBuffers(0, 1, 
		this->constant_buffer->getComPtr().GetAddressOf());
	context->DrawIndexed(
		this->i_buff->getCount(),
		0, 0);
}

ComPtr<ID3D11ShaderResourceView> WaterReflection::getSRV()
{
	return this->d_buff->getSRV(0);
}
