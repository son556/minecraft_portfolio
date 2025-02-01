#pragma once

class OpacityRender;
struct RenderOption;
class MapUtils;

class WaterReflection
{
public:
	WaterReflection(MapUtils* m_info);
	~WaterReflection() = default;
	void render();
	ComPtr<ID3D11ShaderResourceView> getSRV();
	void setDSV(ComPtr<ID3D11DepthStencilView> dsv);
	
private:
	WaterReflection() = delete;
	WaterReflection(WaterReflection const&) = delete;
	WaterReflection& operator=(WaterReflection const&) = delete;

private:
	shared_ptr<OpacityRender> opacity_render = nullptr;
	shared_ptr<RenderOption> r_opt;
	ComPtr<ID3D11DepthStencilView> dsv;
	ComPtr<ID3D11DepthStencilState> ds_state;
	int stencil_ref_num = 1;
};

