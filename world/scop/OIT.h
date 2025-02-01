#pragma once

#include "Transparent.h"
#include "Composite.h"

class OIT
{
public:
	OIT(MapUtils* minfo);
	~OIT() = default;
	void setRTVandSRV(
		ComPtr<ID3D11RenderTargetView> rtv,
		ComPtr<ID3D11ShaderResourceView> srv,
		ComPtr<ID3D11ShaderResourceView> depth_srv
	);
	void setPipe();
	void render(CamType type);
	ComPtr<ID3D11ShaderResourceView> getSRV();

private:
	MapUtils* m_info;
	shared_ptr<DeferredBuffer> d_buff;
	Transparent tp;
	Composite cp;

private:
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;

private:
	ComPtr<ID3D11ShaderResourceView> depth_srv;
	ComPtr<ID3D11ShaderResourceView> solid_srv;
	ComPtr<ID3D11RenderTargetView> solid_rtv;
	shared_ptr<Buffer<VertexDefer>> v_buff;
	shared_ptr<Buffer<uint32>> i_buff;
};

