#include "pch.h"
#include "Entity.h"
#include "DeferredGraphics.h"
#include "VertexShader.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "SamplerState.h"
#include "Character.h"
#include "DeferredBuffer.h"

Entity::Entity()
{
	ComPtr<ID3D11Device> device = d_graphic->getDevice();
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"EntityVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_ptn.data(),
		InputLayouts::layout_ptn.size(),
		this->vertex_shader->getBlob()
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"EntityPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->sampler_state = make_shared<SamplerState>(device);
	this->character = nullptr;
	this->d_buffer = make_shared<DeferredBuffer>(3);
	this->d_buffer->setRTVsAndSRVs(
		d_graphic->getDevice(),
		w_width,
		w_height
	);
}

void Entity::setRTV(ComPtr<ID3D11DepthStencilView> dsv)
{
	this->dsv = dsv;
}

void Entity::setCharacter(vec3 pos, vec3 radian_xyz)
{
	static int cnt = 0;
	if (cnt)
		return;
	Mat move = Mat::CreateTranslation(pos);
	Mat rot = Mat::CreateRotationX(radian_xyz.x) *
		Mat::CreateRotationY(radian_xyz.y) *
		Mat::CreateRotationZ(radian_xyz.z);
	this->character = make_shared<Character>(move, rot);
	cnt++;
}

void Entity::render(CamType type, bool shadow_flag)
{
	if (shadow_flag) {
		if (this->character)
			this->character->render(type, shadow_flag);
		return;
	}
	this->setPipe();
	d_graphic->renderBegin(this->d_buffer.get(), this->dsv, true, false);
	if (this->character)
		this->character->render(type, shadow_flag);
}

ComPtr<ID3D11ShaderResourceView> Entity::getSRV()
{
	return this->d_buffer->getSRV(0);
}

ComPtr<ID3D11ShaderResourceView> Entity::getSRVPos()
{
	return this->d_buffer->getSRV(1);
}

ComPtr<ID3D11ShaderResourceView> Entity::getSRVNormal()
{
	return this->d_buffer->getSRV(2);
}

void Entity::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr, 0);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr, 0);
	context->PSSetSamplers(0, 1, this->sampler_state->getComPtr().GetAddressOf());
}
