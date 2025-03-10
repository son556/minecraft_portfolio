#include "pch.h"
#include "GUI.h"
#include "Buffer.h"
#include "Block.h"
#include "DeferredGraphics.h"
#include "Texture.h"
#include "ConstantBuffer.h"

GUI::GUI(float w, float h, string gui_name)
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(w, h, vertices, indices);
	this->v_buffer = make_shared<Buffer<VertexDefer>>(
		device, vertices.data(), vertices.size(),
		D3D11_BIND_VERTEX_BUFFER);
	this->i_buffer = make_shared<Buffer<uint32>>(
		device, indices.data(), indices.size(),
		D3D11_BIND_INDEX_BUFFER);
	this->gui_name = gui_name;
	Mat tmp;
	this->constant_buffer = make_shared<ConstantBuffer>(
		device,
		d_graphic->getContext(),
		tmp
	);
}

void GUI::setGUIBuffer(ComPtr<ID3D11DeviceContext> const& context)
{
	uint32 stride = this->v_buffer->getStride();
	uint32 offset = this->v_buffer->getOffset();

	context->IASetVertexBuffers(0, 1,
		this->v_buffer->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->i_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT, 0);
	shared_ptr<Texture> const& texture = GUIResources::getTexture(this->gui_name);
	context->PSSetShaderResources(0, 1, texture->getComPtr().GetAddressOf());
}

void GUI::setOpacityItemBuffer(
	ComPtr<ID3D11DeviceContext> const& context,
	int idx
)
{
	shared_ptr<Buffer<VertexDefer>> const& v_buff = 
		this->opacity_items[idx].getVertexBuffer();
	shared_ptr<Buffer<uint32>> const& i_buff = 
		this->opacity_items[idx].getIndexBuffer();
	uint32 stride = v_buff->getStride();
	uint32 offset = v_buff->getOffset();
	Mat world = this->opacity_items[idx].getWorldMatrix().Transpose();
	this->constant_buffer->update(world);

	context->IASetVertexBuffers(0, 1, 
		v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(i_buff->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT, 0);
	context->VSSetConstantBuffers(0, 1, this->constant_buffer->getComPtr().GetAddressOf());
	context->PSSetShaderResources(0, 1,
		this->opacity_items[idx].getTexture()->getComPtr().GetAddressOf());
}

void GUI::setTransParencyBuffer(
	ComPtr<ID3D11DeviceContext> const& context,
	int idx
)
{
	shared_ptr<Buffer<VertexColor>> const& v_buff = this->tp_items[idx].getVertexBuffer();
	shared_ptr<Buffer<uint32>> const& i_buff = this->tp_items[idx].getIndexBuffer();
	uint32 stride = v_buff->getStride();
	uint32 offset = v_buff->getOffset();

	Mat world = this->tp_items[idx].getWorldMatrix().Transpose();
	this->constant_buffer->update(world);

	context->IASetVertexBuffers(0, 1,
		v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(i_buff->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT, 0);
	context->VSSetConstantBuffers(0, 1, this->constant_buffer->getComPtr().GetAddressOf());
}

int GUI::getItemArraySize(bool op_tp)
{
	if (op_tp)
		return this->opacity_items.size();
	return this->tp_items.size();
}

