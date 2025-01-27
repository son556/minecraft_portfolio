#pragma once

class DeferredGraphics;
class DeferredBuffer;
class VertexShader;
class PixelShader;
class RasterizerState;
class InputLayout;
class MapUtils;
class ConstantBuffer;
class SamplerState;


enum class WaterRTVType {
	POSITION,
	COLOR
};

class WaterInit
{
public:
	WaterInit(DeferredGraphics* d_graphic, MapUtils* m_info);
	~WaterInit() = default;
	void setPipe();
	void render(
		Mat const& cam_view,
		Mat const& cam_proj,
		ComPtr<ID3D11ShaderResourceView> const& depth_srv
	);
	ComPtr<ID3D11ShaderResourceView> getSRV(WaterRTVType type);

private:
	WaterInit() = delete;
	WaterInit(WaterInit const&) = delete;
	WaterInit& operator=(WaterInit const&) = delete;

private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	shared_ptr<DeferredBuffer> d_buff;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<ConstantBuffer> constnat_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
};

