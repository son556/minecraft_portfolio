#pragma once

class DeferredGraphics;
class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class GeoRender;
class Wallpaper;
class PBR;
class ConstantBuffer;
class MapUtils;
template<typename T> class Buffer;
struct VertexDefer;

class ReflectionCube
{
public:
	ReflectionCube(
		DeferredGraphics* graphic,
		MapUtils* m_info,
		GeoRender* g_render,
		Wallpaper* skybox,
		PBR* pbr
	);
	void render(vec3 const& cam_pos);
	ComPtr<ID3D11ShaderResourceView>& getSRV();

private:
	void init();
	void setPipe();

private:
	DeferredGraphics* d_graphic;
	MapUtils* m_info;
	GeoRender* geo_render;
	Wallpaper* sky_box;
	PBR* pbr;

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<ConstantBuffer> constant_buffer;

private:
	UINT width;
	UINT height;
	ComPtr<ID3D11RenderTargetView> rtvs[6];
	ComPtr<ID3D11DepthStencilView> dsv;
	ComPtr<ID3D11ShaderResourceView> srv;
	float clear_color[4] = { 0.f, 0.f, 0.f, 1.f };
	D3D11_VIEWPORT view_port = { 0, };
	Mat cam_views[6];
	Mat cam_proj;
};

