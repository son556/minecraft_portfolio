#include "pch.h"
#include "Inventory.h"
#include "DeferredGraphics.h"
#include "ConstantBuffer.h"
#include "Buffer.h"
#include "Block.h"
#include "Texture.h"
#include "OBlockItem.h"
#include "TBlockItem.h"

Inventory::Inventory(float w, float h)
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	vector<uint32> indices;
	vector<VertexDefer> vertices;
	Block::makeBox(w, h, vertices, indices);
	this->v_buffer = make_shared<Buffer<VertexDefer>>(
		device,
		vertices.data(), vertices.size(), D3D11_BIND_VERTEX_BUFFER);
	this->i_buffer = make_shared<Buffer<uint32>>(
		device, indices.data(), indices.size(), D3D11_BIND_INDEX_BUFFER);

	Mat tmp;
	this->constant_buffer = make_shared<ConstantBuffer>(
		device,
		d_graphic->getContext(),
		tmp
	);
	this->items.resize(3);
	for (int i = 0; i < 3; i++)
		this->items[i] = nullptr;

}

void Inventory::setGUIBuffer(ComPtr<ID3D11DeviceContext> const& context)
{
	uint32 stride = this->v_buffer->getStride();
	uint32 offset = this->v_buffer->getOffset();

	context->IASetVertexBuffers(0, 1,
		this->v_buffer->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(
		this->i_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT, 0);
	this->constant_buffer->update(this->gui_world.Transpose());
	context->VSSetConstantBuffers(0, 1, this->constant_buffer->getComPtr().GetAddressOf());
	shared_ptr<Texture> const& texture = 
		GUIResources::getTexture(GUITexture::INVENTORY);
	context->PSSetShaderResources(0, 1, texture->getComPtr().GetAddressOf());
}

void Inventory::setOpacityItemBuffer(ComPtr<ID3D11DeviceContext> const& context, int idx)
{
	if (this->items.size() >= idx || this->items[idx]->getBlockFlag())
		return;
	shared_ptr<OBlockItem> const& o_item =
		dynamic_pointer_cast<OBlockItem>(this->items[idx]);
	shared_ptr<Buffer<VertexDefer>> const& v_buff =
		o_item->getVertexBuffer();
	shared_ptr<Buffer<uint32>> const& i_buff =
		o_item->getIndexBuffer();
	uint32 stride = v_buff->getStride();
	uint32 offset = v_buff->getOffset();
	Mat world = o_item->getWorldMatrix().Transpose();
	this->constant_buffer->update(world);

	context->IASetVertexBuffers(0, 1,
		v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(i_buff->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT, 0);
	context->VSSetConstantBuffers(0, 1, this->constant_buffer->getComPtr().GetAddressOf());
	context->PSSetShaderResources(0, 1,
		o_item->getTexture()->getComPtr().GetAddressOf());
}

void Inventory::setTransParencyBuffer(
	ComPtr<ID3D11DeviceContext> const& context, 
	int idx
)
{
	if (this->items.size() >= idx || this->items[idx]->getBlockFlag() == false)
		return;

	shared_ptr<TBlockItem> const& t_item = dynamic_pointer_cast<TBlockItem>(this->items[idx]);
	shared_ptr<Buffer<VertexColor>> const& v_buff = t_item->getVertexBuffer();
	shared_ptr<Buffer<uint32>> const& i_buff = t_item->getIndexBuffer();
	uint32 stride = v_buff->getStride();
	uint32 offset = v_buff->getOffset();

	Mat world = t_item->getWorldMatrix().Transpose();
	this->constant_buffer->update(world);

	context->IASetVertexBuffers(0, 1,
		v_buff->getComPtr().GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(i_buff->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT, 0);
	context->VSSetConstantBuffers(0, 1, 
		this->constant_buffer->getComPtr().GetAddressOf());
}

int Inventory::getItemArraySize()
{
	return this->items.size();
}

shared_ptr<BlockItem> const& Inventory::getItem(int idx)
{
	return this->items[idx];
}

bool Inventory::getItemOTFlag(int idx)
{
	return this->items[idx]->getBlockFlag();
}

void Inventory::moveItem(int idx, vec3 const& new_pos)
{
	if (this->items[idx] == nullptr)
		return;
	// TODO: 아이템창 칸 안에 들어와 있는 지 확인
}

void Inventory::moveGUIPos(vec3 const& new_pos)
{
	this->gui_world = Mat::CreateTranslation(new_pos);
}

BlockType Inventory::getBlockType(int idx)
{
	return this->items[idx]->getBlockType();
}
