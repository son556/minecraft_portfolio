#pragma once

#include "ParallaxMapping.h"

class MapUtils;
class HullShader;
class DomainShader;
class DepthMap;

class GeoRender
{
public:
	GeoRender(MapUtils* minfo);
	~GeoRender() = default;
	void render(
		CamType type,
		D3D11_VIEWPORT* view_port = nullptr
	);
	void setParallaxFlag(bool flag);
	ComPtr<ID3D11ShaderResourceView> getSRV(RTVIndex idx);
	ComPtr<ID3D11ShaderResourceView> getNPSRV(RTVIndex idx);
	ComPtr<ID3D11ShaderResourceView> getDepthSRV();
	ComPtr<ID3D11DepthStencilView> getDSV();
	ComPtr<ID3D11RenderTargetView> getRTV(RTVIndex idx);

private:
	GeoRender() = delete;
	GeoRender(GeoRender const&) = delete;
	GeoRender& operator=(GeoRender const&) = delete;

private:
	void setPipe();
	void setConstantBuffer(CamType type);
	void parallaxRender(vec3 const& cam_pos);

private:
	MapUtils* m_info;
	shared_ptr<ParallaxMapping> parallax_mapping;
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<SamplerState> linear_state;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<HullShader> hull_shader;
	shared_ptr<DomainShader> domain_shader;

private:
	bool parallax_flag = true;
	shared_ptr<ConstantBuffer> mvp_cbuffer;
	shared_ptr<ConstantBuffer> cam_pos_cbuffer;
	shared_ptr<ConstantBuffer> eye_pos_cbuffer;
	shared_ptr<TextureArray> texture_array_color;
	shared_ptr<TextureArray> texture_array_normal;
	shared_ptr<TextureArray> texture_array_s;
	shared_ptr<DepthMap> depth_map;
};

