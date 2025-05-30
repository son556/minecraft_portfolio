#pragma once

#include "WorldUtils.h"

class MapUtils;
class DeferredGraphics;
class DeferredBuffer;
class RasterizerState;
class VertexShader;
class PixelShader;
class InputLayout;
class DepthMap;
class SamplerState;
template <typename T> class Buffer;
class ConstantBuffer;

class SsaoRender
{
public:
	SsaoRender(UINT width, UINT height);
	~SsaoRender() = default;
	void render(CamType type);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	shared_ptr<DeferredBuffer> getDBuffer();

private:
	SsaoRender() = delete;
	SsaoRender(SsaoRender const&) = delete;
	SsaoRender& operator=(SsaoRender const&) = delete;

private:
	void setPipe();
	void buildOffset();
	void buildRandomVecTexture();

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;
	shared_ptr<ConstantBuffer> cbuffer;

private:
	vec4 mOffsets[14];
	ComPtr<ID3D11ShaderResourceView> random_vec_SRV;
	D3D11_VIEWPORT view_port = { 0, };
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> sampler_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
};

