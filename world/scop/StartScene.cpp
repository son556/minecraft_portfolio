#include "pch.h"
#include "StartScene.h"
#include "DeferredGraphics.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "VertexShader.h"
#include "SamplerState.h"
#include "PixelShader.h"
#include "Texture.h"
#include "Block.h"
#include "Buffer.h"
#include "ConstantBuffer.h"

// 1 sky box 그리기(ndc box 기준 꽉 차게) 그 위에 버튼 그리기
StartScene::StartScene()
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	ZeroMemory(&depth_stencil_desc, sizeof(depth_stencil_desc));
	depth_stencil_desc.DepthEnable = false;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.StencilEnable = false;

	device->CreateDepthStencilState(&depth_stencil_desc,
		this->depth_stencil_state.GetAddressOf());

	// 배경
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(1, vertices, indices);
	this->v_buffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	vertices.clear();
	indices.clear();

	// button
	Block::makeBox(0.05, 0.5, vertices, indices);

}
