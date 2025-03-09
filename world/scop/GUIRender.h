#pragma once

class InputLayout;
class VertexShader;
class PixelShader;
class RasterizerState;
class SamplerState;
class GUI;

class GUIRender
{
public:
	GUIRender();
	~GUIRender() = default;
	void render(GUI* gui);

private:
	GUIRender(GUIRender const&) = delete;
	GUIRender& operator=(GUIRender const&) = delete;
	void setPipe(
		ComPtr<ID3D11DeviceContext> const& context,
		bool tp_flag
	);

private:
	D3D11_BLEND_DESC blend_desc;

private:
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;

	shared_ptr<InputLayout> input_layout_color;
	shared_ptr<VertexShader> vertex_shader_color;
	shared_ptr<PixelShader> pixel_shader_color;
};

