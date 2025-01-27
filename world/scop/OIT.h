#pragma once

#include "Transparent.h"
#include "Composite.h"
#include "Water.h"

class TestRender;

class OIT
{
public:
	OIT(DeferredGraphics* graphic, MapUtils* minfo);
	~OIT();
	void setRTVandSRV(
		ComPtr<ID3D11RenderTargetView> rtv,
		ComPtr<ID3D11ShaderResourceView> srv,
		ComPtr<ID3D11ShaderResourceView> depth_srv
	);
	void setPipe();
	void render(vec3 const& cam_pos, Mat const& cam_view, Mat const& cam_proj);
	void setReflectionCube(
		ComPtr<ID3D11ShaderResourceView>& reflection_cube);
	ComPtr<ID3D11ShaderResourceView> getSRV();

private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	shared_ptr<DeferredBuffer> d_buff;
	Transparent tp;
	Composite cp;
	Water water;

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

	shared_ptr<TestRender> test_render;
};

