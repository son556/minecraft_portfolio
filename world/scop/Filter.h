#pragma once

#include "Block.h"

class DeferredGraphics;
class DeferredBuffer;
class PixelShader;
class VertexShader;
class InputLayout;
template<typename T> class Buffer;
class SamplerState;
class RasterizerState;
class ConstantBuffer;

class Filter
{
public:
	Filter(
		UINT width,
		UINT height,
		wstring const& vs_path,
		wstring const& ps_path
	);
	~Filter() = default;
	void render();
	ComPtr<ID3D11ShaderResourceView> getSRV();
	void setStartSRV(ComPtr<ID3D11ShaderResourceView> srv);

private:
	Filter() = delete;
	Filter(Filter const&) = delete;
	Filter& operator=(Filter const&) = delete;

private:
	void setPipe();
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	shared_ptr<DeferredBuffer> d_buffer;
	D3D11_VIEWPORT view_port = { 0, };
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;

private:
	ComPtr<ID3D11ShaderResourceView> in_srv;
	shared_ptr<ConstantBuffer> constant_buffer;
};

