#pragma once

class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class Texture;
class SamplerState;
template<typename T> class Buffer;
class ConstantBuffer;
struct VertexDefer;

class StartScene
{
public:
	StartScene();
	~StartScene() = default;

private:
	StartScene& operator=(StartScene const&) = delete;
	StartScene(StartScene const&) = delete;

private:
	shared_ptr<Buffer<VertexDefer>> v_letter_buffer;
	shared_ptr<Buffer<VertexDefer>> v_button_buffer;
	shared_ptr<Buffer<VertexDefer>> v_title_buffer;
	shared_ptr<Buffer<VertexDefer>> v_buffer;
	shared_ptr<uint32> i_buffer;
	shared_ptr<uint32> i_cube_buffer;
	float cam_radian = 0;

private:
	ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;

private:
	shared_ptr<Texture> start_button;
	shared_ptr<Texture> select_start_button;
	shared_ptr<Texture> exit_button;
	shared_ptr<Texture> select_exit_button;
	shared_ptr<Texture> title;
	shared_ptr<Texture> sky_box;
};

