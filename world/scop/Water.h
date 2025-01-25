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

class Water
{
public:
	Water(DeferredGraphics* d_graphic, MapUtils* m_info);
	void setPipe();
	void render_test(
		Mat const& cam_view,
		Mat const& cam_proj,
		ComPtr<ID3D11ShaderResourceView>& depth_srv,
		ComPtr<ID3D11RenderTargetView>& rtv
	);
private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<ConstantBuffer> constnat_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;
};

