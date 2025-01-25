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
#include "Chunk.h"
#include "ConstantBuffer.h"
#include "SamplerState.h"

Water::Water(DeferredGraphics* d_graphic, MapUtils* m_info)
	: d_graphic(d_graphic), m_info(m_info)
{
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"WaterVS.hlsl",
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
		L"WaterPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_NONE
	);
	MVP mvp;
	this->constnat_buffer = make_shared<ConstantBuffer>(
		device,
		this->d_graphic->getContext(),
		mvp
	);
	this->sampler_state = make_shared<SamplerState>(device);
}

void Water::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
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

void Water::render_test(
	Mat const& cam_view, 
	Mat const& cam_proj, 
	ComPtr<ID3D11ShaderResourceView>& depth_srv, 
	ComPtr<ID3D11RenderTargetView>& rtv
)
{
	this->setPipe();
	MVP mvp;
	mvp.view = cam_view.Transpose();
	mvp.proj = cam_proj.Transpose();
	this->constnat_buffer->update(mvp);

	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	this->d_graphic->renderBegin(1, rtv.GetAddressOf(), nullptr, 
		false, true);
	context->VSSetConstantBuffers(0, 1,
		this->constnat_buffer->getComPtr().GetAddressOf());
	context->PSSetShaderResources(0, 1, depth_srv.GetAddressOf());
	for (int i = 0; i < this->m_info->size_h; i++) {
		for (int j = 0; j < this->m_info->size_w; j++) {
			if (this->m_info->chunks[i][j]->w_chunk.render_flag == false)
				continue;
			this->m_info->chunks[i][j]->w_chunk.setBuffer(context);
		}
	}
}
