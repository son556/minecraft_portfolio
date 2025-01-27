#include "pch.h"
#include "ReflectionCube.h"
#include "DeferredGraphics.h"
#include "MapUtils.h"
#include "GeoRender.h"
#include "PBR.h"
#include "Wallpaper.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "RasterizerState.h"
#include "PixelShader.h"
#include "Buffer.h"
#include "ConstantBuffer.h"

ReflectionCube::ReflectionCube(
	DeferredGraphics* graphic, 
	MapUtils* m_info, 
	GeoRender* g_render, 
	Wallpaper* skybox, 
	PBR* pbr
) : d_graphic(graphic), m_info(m_info), geo_render(g_render), sky_box(skybox),
	pbr(pbr)
{
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"ReflectionCubeVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_pt.data(),
		InputLayouts::layout_pt.size(),
		this->vertex_shader->getBlob()
	);
	this->rasterizer_state = make_shared<RasterizerState>(
		device,
		D3D11_FILL_SOLID,
		D3D11_CULL_BACK
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"ReflectionCubePS.hlsl",
		"main",
		"ps_5_0"
	);
	MVP mvp;
	this->constant_buffer = make_shared<ConstantBuffer>(
		device,
		this->d_graphic->getContext(),
		mvp
	);
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
	this->vbuffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->ibuffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->width = 1024;
	this->height = 1024;
	this->view_port.Width = width;
	this->view_port.Height = height;
	this->view_port.MaxDepth = 1;
	this->view_port.MinDepth = 0;
	this->view_port.TopLeftX = 0;
	this->view_port.TopLeftY = 0;
	this->init();
	float width = static_cast<float>(this->m_info->width);
	float height = static_cast<float>(this->m_info->height);
	this->cam_proj = XMMatrixPerspectiveFovLH(
		XM_PI * 0.5,
		1,
		0.1, 1000);
}

void ReflectionCube::init()
{
	ComPtr<ID3D11Device> device = this->d_graphic->getDevice();
	D3D11_TEXTURE2D_DESC cube_desc = {};
	ZeroMemory(&cube_desc, sizeof(cube_desc));
	cube_desc.Width = width;
	cube_desc.Height = height;
	cube_desc.MipLevels = 1;
	cube_desc.ArraySize = 6;
	cube_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	cube_desc.SampleDesc.Count = 1;
	cube_desc.SampleDesc.Quality = 0;
	cube_desc.Usage = D3D11_USAGE_DEFAULT;
	cube_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	cube_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | 
		D3D11_RESOURCE_MISC_GENERATE_MIPS;

	ComPtr<ID3D11Texture2D> cube_tex;
	HRESULT hr = device->CreateTexture2D(&cube_desc, nullptr, cube_tex.GetAddressOf());
	CHECK(hr);

	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	ZeroMemory(&rtv_desc, sizeof(rtv_desc));
	rtv_desc.Format = cube_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
	rtv_desc.Texture2DArray.MipSlice = 0;
	rtv_desc.Texture2DArray.ArraySize = 1;

	for (int i = 0; i < 6; i++) {
		rtv_desc.Texture2DArray.FirstArraySlice = i;
		hr = device->CreateRenderTargetView(cube_tex.Get(),
			&rtv_desc, this->rtvs[i].GetAddressOf());
		CHECK(hr);
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = cube_desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.TextureCube.MipLevels = 1;
	srvDesc.TextureCube.MostDetailedMip = 0;

	hr = device->CreateShaderResourceView(cube_tex.Get(), 
		&srvDesc, this->srv.GetAddressOf());
	CHECK(hr);

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ComPtr<ID3D11Texture2D> depth_tex;
	hr = device->CreateTexture2D(&desc, nullptr, depth_tex.GetAddressOf());
	CHECK(hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	ZeroMemory(&dsv_desc, sizeof(dsv_desc));
	dsv_desc.Format = desc.Format;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	hr = device->CreateDepthStencilView(depth_tex.Get(), &dsv_desc,
		this->dsv.GetAddressOf());
	CHECK(hr);
}

void ReflectionCube::setPipe()
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	context->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);
	uint32 stride = this->vbuffer->getStride();
	uint32 offset = this->vbuffer->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->vbuffer->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->ibuffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->VSSetShader(
		this->vertex_shader->getComPtr().Get(),
		nullptr,
		0
	);
	context->RSSetState(this->rasterizer_state->getComPtr().Get());
	context->PSSetShader(
		this->pixel_shader->getComPtr().Get(),
		nullptr,
		0
	);

}

void ReflectionCube::render(vec3 const& cam_pos)
{
	ComPtr<ID3D11DeviceContext> context = this->d_graphic->getContext();
	this->cam_views[0] = XMMatrixLookToLH(cam_pos, vec3(1, 0, 0), vec3(0, 1, 0));
	this->cam_views[1] = XMMatrixLookToLH(cam_pos, vec3(-1, 0, 0), vec3(0, 1, 0));
	this->cam_views[2] = XMMatrixLookToLH(cam_pos, vec3(0, 1, 0), vec3(0, 0, -1));
	this->cam_views[3] = XMMatrixLookToLH(cam_pos, vec3(0, -1, 0), vec3(0, 0, 1));
	this->cam_views[4] = XMMatrixLookToLH(cam_pos, vec3(0, 0, 1), vec3(0, 1, 0));
	this->cam_views[5] = XMMatrixLookToLH(cam_pos, vec3(0, 0, -1), vec3(0, 1, 0));

	for (int i = 0; i < 6; i++) {
		// skybox render
		this->sky_box->render(cam_pos, this->cam_views[i], this->cam_proj);
		
		// geographic render
		this->geo_render->setParallaxFlag(false);
		this->geo_render->render(
			this->cam_views[i], 
			this->cam_proj, 
			cam_pos);
		
		// pbr render
		this->pbr->setRTV();
		context->PSSetShaderResources(1, 1,
			this->geo_render->getSRV(RTVIndex::w_normal).GetAddressOf());
		context->PSSetShaderResources(2, 1,
			this->geo_render->getSRV(RTVIndex::w_position).GetAddressOf());
		context->PSSetShaderResources(3, 1,
			this->geo_render->getSRV(RTVIndex::rma).GetAddressOf());
		vec3 lp = this->m_info->directional_light_pos;
		this->pbr->render(
			lp, cam_pos, 
			this->geo_render->getSRV(RTVIndex::color)
		);

		// reflection cube map
		context->OMSetRenderTargets(1,
			this->rtvs[i].GetAddressOf(), this->dsv.Get());
		context->ClearRenderTargetView(this->rtvs[i].Get(), clear_color);
		context->ClearDepthStencilView(this->dsv.Get(),
			D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		context->RSSetViewports(1, &(this->view_port));
		this->setPipe();
		context->PSSetShaderResources(0, 1,
			this->pbr->getAmbientLight().GetAddressOf());
		context->PSSetShaderResources(1, 1,
			this->pbr->getDirectLight().GetAddressOf());
		context->PSSetShaderResources(2, 1,
			this->sky_box->getSRV().GetAddressOf());
		context->DrawIndexed(
			this->ibuffer->getCount(),
			0, 0);
	}

	context->GenerateMips(this->srv.Get());
}

ComPtr<ID3D11ShaderResourceView>& ReflectionCube::getSRV()
{
	return this->srv;
}
