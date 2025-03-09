#include "pch.h"
#include "GUIRender.h"
#include "DeferredGraphics.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "RasterizerState.h"
#include "PixelShader.h"
#include "SamplerState.h"
#include "GUI.h"


GUIRender::GUIRender()
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	
	ZeroMemory(&blend_desc, sizeof(this->blend_desc));
	this->blend_desc.AlphaToCoverageEnable = false;
	this->blend_desc.IndependentBlendEnable = false;

	this->blend_desc.RenderTarget[0].BlendEnable = true;
	this->blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	this->blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	this->blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	this->blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	this->blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	this->blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	this->blend_desc.RenderTarget[0].RenderTargetWriteMask =
		D3D11_COLOR_WRITE_ENABLE_ALL;
	
	this->vertex_shader = make_shared<VertexShader>(device, L"GUIRenderVS.hlsl",
		"main", "vs_5_0");
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pt.data(),
		InputLayouts::layout_pt.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(device, L"GUIRenderPS.hlsl",
		"main", "ps_5_0");
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_NONE
	);

	this->vertex_shader_color = make_shared<VertexShader>(device, L"GUIRenderColorVS.hlsl",
		"main", "vs_5_0");
	this->pixel_shader_color = make_shared<PixelShader>(device, L"GUIRenderColorPS.hlsl",
		"main", "ps_5_0");
	this->input_layout_color = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pc.data(),
		InputLayouts::layout_pc.size(),
		this->vertex_shader_color->getBlob()
	);
}

void GUIRender::render(GUI* gui)
{
	ComPtr<ID3D11DeviceContext> const& context = d_graphic->getContext();
}

void GUIRender::setPipe(
	ComPtr<ID3D11DeviceContext> const& context,
	bool tp_flag
)
{
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());

	if (tp_flag) {
		context->IASetInputLayout(this->input_layout_color->getComPtr().Get());
		context->VSSetShader(
			this->vertex_shader_color->getComPtr().Get(),
			nullptr,
			0
		);
		context->PSSetShader(
			this->pixel_shader_color->getComPtr().Get(),
			nullptr,
			0
		);
	}
	else {
		context->IASetInputLayout(this->input_layout->getComPtr().Get());
		context->VSSetShader(
			this->vertex_shader->getComPtr().Get(),
			nullptr,
			0
		);
		context->PSSetShader(
			this->pixel_shader->getComPtr().Get(),
			nullptr,
			0
		);
		context->PSSetSamplers(0, 1, this->sampler_state->getComPtr().GetAddressOf());
	}
}
