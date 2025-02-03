#pragma once

class OpacityRender;
struct RenderOption;
class MapUtils;
class DeferredBuffer;
class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class SamplerState;
class ConstantBuffer;
template<typename T> class Buffer;

struct VertexDefer;

class WaterReflection
{
public:
	WaterReflection(MapUtils* m_info);
	~WaterReflection() = default;
	void render();
	ComPtr<ID3D11ShaderResourceView> getSRV();
	void setDSV(ComPtr<ID3D11DepthStencilView> dsv);
	
private:
	void setPipe();
	void init(ComPtr<ID3D11Device>& device, MapUtils* m_info);

private:
	WaterReflection() = delete;
	WaterReflection(WaterReflection const&) = delete;
	WaterReflection& operator=(WaterReflection const&) = delete;

private:
	shared_ptr<Buffer<VertexDefer>> v_buff;
	shared_ptr <Buffer<uint32>> i_buff;
	shared_ptr<DeferredBuffer> d_buff;

private:
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;

private:
	shared_ptr<OpacityRender> opacity_render = nullptr;
	shared_ptr<RenderOption> r_opt;
	ComPtr<ID3D11DepthStencilView> dsv;
	ComPtr<ID3D11DepthStencilState> ds_state;
	int stencil_ref_num = 1;
};

