#include "pch.h"
#include "WaterInit.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "RasterizerState.h"
#include "PixelShader.h"
#include "MapUtils.h"
#include "Chunk.h"
#include "ConstantBuffer.h"
#include "SamplerState.h"
#include "TestCam.h"

WaterInit::WaterInit(MapUtils* m_info) : m_info(m_info)
{
	ComPtr<ID3D11Device> device = d_graphic->getDevice();
	this->d_buff = make_shared<DeferredBuffer>(2);
	this->d_buff->setRTVsAndSRVs(
		device,
		this->m_info->width,
		this->m_info->height
	);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"WaterInitVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pct.data(),
		InputLayouts::layout_pct.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"WaterInitPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_NONE
	);
	this->sampler_state = make_shared<SamplerState>(device);
}

void WaterInit::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

void WaterInit::render(
	CamType type, 
	ComPtr<ID3D11ShaderResourceView> const& depth_srv
)
{
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	d_graphic->renderBegin(this->d_buff.get());
	this->setPipe();
	context->VSSetConstantBuffers(0, 1,
		cam->getConstantBuffer(type)->getComPtr().GetAddressOf());
	context->PSSetShaderResources(0, 1, depth_srv.GetAddressOf());
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->w_chunk.render_flag == false)
				continue;
			this->m_info->chunks[i][j]->w_chunk.setBuffer(context);
		}
	}
}

ComPtr<ID3D11ShaderResourceView> WaterInit::getSRV(WaterRTVType type)
{
	if (type == WaterRTVType::POSITION)
		return this->d_buff->getSRV(0);
	return this->d_buff->getSRV(1);
}
