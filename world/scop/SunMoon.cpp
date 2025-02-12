#include "pch.h"
#include "SunMoon.h"
#include "DeferredGraphics.h"
#include "DeferredBuffer.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RasterizerState.h"
#include "ConstantBuffer.h"
#include "Sun.h"
#include "Moon.h"
#include "Buffer.h"
#include "TestCam.h"

SunMoon::SunMoon(UINT width, UINT height)
{
	this->sun = make_shared<Sun>(30);
	this->moon = make_shared<Moon>(30);
	this->d_buffer = make_shared<DeferredBuffer>(1);
	ComPtr<ID3D11Device> device = d_graphic->getDevice();
	this->d_buffer->setRTVsAndSRVs(device, width, height);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"SunMoonVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pc.data(),
		InputLayouts::layout_pc.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"SunMoonPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	MVP tmp;
	this->constant_buff = make_shared<ConstantBuffer>(
		d_graphic->getDevice(),
		d_graphic->getContext(),
		tmp
	);
	this->ccw_rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK,
		true
	);
}

void SunMoon::render(CamType type, bool ccw_flag)
{
	ComPtr<ID3D11Device> device;
	device = d_graphic->getDevice();
	ComPtr<ID3D11DeviceContext> context;
	context = d_graphic->getContext();
	static float d = 90;
	float dt = XMConvertToRadians(d);
	
	//d += 0.04;
	MVP mvp = cam->getMVP(type);
	vec3 cam_pos = cam->getPos();
	vec3 move_pos = vec3(cam_pos.x + 299, 0, cam_pos.z);
	mvp.model = SimpleMath::Matrix::CreateTranslation(move_pos) * 
		SimpleMath::Matrix::CreateRotationZ(dt) * mvp.model;
	XMFLOAT4 s_pos = XMFLOAT4(0, 0, 0, 1);
	XMVECTOR sun_pos_vec = XMLoadFloat4(&s_pos);
	sun_pos_vec = XMVector4Transform(sun_pos_vec, mvp.model);
	XMStoreFloat4(&s_pos, sun_pos_vec);
	this->sun_pos = vec3(s_pos.x, s_pos.y, s_pos.z);

	mvp.model = mvp.model.Transpose();
	mvp.view = mvp.view.Transpose();
	mvp.proj = mvp.proj.Transpose();
	this->constant_buff->update(mvp);
	this->setPipe(ccw_flag);
	context->VSSetConstantBuffers(0, 1,
		this->constant_buff->getComPtr().GetAddressOf());
	d_graphic->renderBegin(this->d_buffer.get());

	// sun
	uint32 offset = this->sun->getVertexBuffer()->getOffset();
	uint32 stride = this->sun->getVertexBuffer()->getStride();
	context->IASetVertexBuffers(0, 1,
		this->sun->getVertexBuffer()->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->DrawIndexed(this->sun->getIndexBuffer()->getCount(),
		0, 0);

	// moon
	move_pos = vec3(cam_pos.x - 299, 0, cam_pos.z);
	mvp.model = SimpleMath::Matrix::CreateTranslation(move_pos) *
		SimpleMath::Matrix::CreateRotationZ(dt);
	XMFLOAT4 m_pos = XMFLOAT4(0, 0, 0, 1);
	XMVECTOR moon_pos_vec = XMLoadFloat4(&m_pos);
	moon_pos_vec = XMVector4Transform(moon_pos_vec, mvp.model);
	XMStoreFloat4(&m_pos, moon_pos_vec);
	this->moon_pos = vec3(m_pos.x, m_pos.y, m_pos.z);
	mvp.model = mvp.model.Transpose();
	this->constant_buff->update(mvp);

	offset = this->moon->getVertexBffer()->getOffset();
	stride = this->moon->getVertexBffer()->getStride();
	context->IASetVertexBuffers(0, 1,
		this->moon->getVertexBffer()->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->DrawIndexed(this->moon->getIndexBuffer()->getCount(),
		0, 0);
}

ComPtr<ID3D11ShaderResourceView> SunMoon::getSRV()
{
	return this->d_buffer->getSRV(0);
}

void SunMoon::setPipe(bool ccw_flag)
{
	ComPtr<ID3D11DeviceContext> context;
	context = d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	uint32 offset = this->sun->getIndexBuffer()->getOffset();
	uint32 stride = this->sun->getIndexBuffer()->getStride();
	context->IASetIndexBuffer(
		this->sun->getIndexBuffer()->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	if (ccw_flag == false)
		context->RSSetState(this->rasterizer_state->getComPtr().Get());
	else
		context->RSSetState(this->ccw_rasterizer_state->getComPtr().Get());
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);
}

vec3 SunMoon::getLightPos()
{
	if (this->sun_pos.y < 0.f)
		return this->moon_pos;
	return this->sun_pos;
}
