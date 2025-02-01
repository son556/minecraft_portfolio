#pragma once

#include "WaterInit.h"
#include "WaterReflection.h"

class TestRender;

template<typename T> class Buffer;
class OpacityRender;
struct VertexDefer;
struct RenderOption;

class Water
{
public:
	Water(MapUtils* m_info);
	void setPipe();
	void render(
		ComPtr<ID3D11ShaderResourceView> depth_srv,
		ComPtr<ID3D11RenderTargetView> rtv
	);
	~Water() = default;

private:
	Water() = delete;
	Water(Water const&) = delete;
	Water& operator=(Water const&) = delete;

private:
	MapUtils* m_info;
	WaterInit water_init;
	WaterReflection water_reflection;
	shared_ptr<OpacityRender> o_render;

private:
	shared_ptr<DeferredBuffer> d_buff;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;

private:
	shared_ptr<Buffer<VertexDefer>> v_buff;
	shared_ptr<Buffer<uint32>> i_buff;
	ComPtr<ID3D11ShaderResourceView> reflection_cube;

private:
	shared_ptr<TestRender> rt;
};

