#include "pch.h"
#include "WaterReflection.h"
#include "OpacityRender.h"
#include "TestCam.h"
#include "DeferredGraphics.h"

WaterReflection::WaterReflection(MapUtils* m_info)
{
	SimpleMath::Plane p = SimpleMath::Plane(vec3(0, WATER_HEIGHT - 1, 0),
		vec3(0, 1, 0));
	cam->update(p);
	this->r_opt = make_shared<RenderOption>();
	this->r_opt->parallax_mapping = false;
	this->r_opt->reflection_flag = true;
	this->r_opt->shadow = true;
	this->r_opt->ssao = false;
	this->r_opt->ccw_flag = true;
	this->r_opt->geo_opt.ds_state = this->ds_state.Get();
	this->r_opt->geo_opt.ptr_dsv = this->dsv;
	this->r_opt->geo_opt.stencil_ref_num = &(this->stencil_ref_num);
	this->r_opt->geo_opt.clear_dsv = false;
	this->r_opt->ccw_flag = true;

	ComPtr<ID3D11Device> device = d_graphic->getDevice();
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.StencilEnable = true;
	desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	device->CreateDepthStencilState(&desc, this->ds_state.GetAddressOf());

	this->opacity_render = make_shared<OpacityRender>(m_info);
}

void WaterReflection::render()
{
	this->opacity_render->render(*(this->r_opt), CamType::REFLECTION_XZ);
}

ComPtr<ID3D11ShaderResourceView> WaterReflection::getSRV()
{
	return this->opacity_render->getSRV();
}

void WaterReflection::setDSV(ComPtr<ID3D11DepthStencilView> dsv)
{
	this->dsv = dsv;
}
