#pragma once

#include "MapUtils.h"

class DeferredGraphics;
class InputLayout;
class VertexShader;
class PixelShader;
class RasterizerState;
class ConstantBuffer;
class SamplerState;
template<typename T> class Buffer;

class Composite
{
public:
	Composite(MapUtils* minfo);
	~Composite() = default;

	void setPipe();
	void render(
		ComPtr<ID3D11RenderTargetView> solid_rtv,
		ComPtr<ID3D11ShaderResourceView> accum,
		ComPtr<ID3D11ShaderResourceView> reveal
	);

private:
	Composite() = delete;
	Composite(Composite const&) = delete;
	Composite& operator=(Composite const&) = delete;

private:
	shared_ptr<Buffer<VertexDefer>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;
	ComPtr<ID3D11BlendState> blend_state;

private:
	MapUtils* m_info;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<ConstantBuffer> constant_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
};

