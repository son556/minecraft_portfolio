#pragma once

class DeferredGraphics;
class DeferredBuffer;
class VertexShader;
class PixelShader;
class RasterizerState;
template<typename T> class Buffer;
class ConstantBuffer;
class MapUtils;
class InputLayout;
class SamplerState;
struct VertexDefer;

class WaterReflection
{
public:
	WaterReflection(DeferredGraphics* d_graphic, MapUtils* m_info);
	WaterReflection() = delete;
	WaterReflection(WaterReflection const&) = delete;
	WaterReflection& operator=(WaterReflection const&) = delete;
	~WaterReflection() = default;
	void setPipe();
	void render(
		vec3 const& cam_pos,
		ComPtr<ID3D11ShaderResourceView> const& reflection_cub,
		ComPtr<ID3D11ShaderResourceView> const& water_pos
	);
	ComPtr<ID3D11ShaderResourceView> getSRV();


private:
	shared_ptr<Buffer<VertexDefer>> v_buff;
	shared_ptr<Buffer<uint32>> i_buff;

private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	shared_ptr<DeferredBuffer> d_buff;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<ConstantBuffer> constant_buffer;
	shared_ptr<SamplerState> sampler_state;
};

