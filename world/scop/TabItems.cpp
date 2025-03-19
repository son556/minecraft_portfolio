#include "pch.h"
#include "TabItems.h"
#include "DeferredGraphics.h"
#include "ConstantBuffer.h"
#include "Buffer.h"
#include "Block.h"
#include "Texture.h"
#include "OBlockItem.h"
#include "TBlockItem.h"

TabItems::TabItems(float w, float h)
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

	Mat tmp;
	this->constant_buffer = make_shared<ConstantBuffer>(
		device,
		d_graphic->getContext(),
		tmp
	);
	this->items.resize(9 * 6);
	for (int i = 0; i < this->items.size(); i++)
		this->items[i] = nullptr;

	this->items[0] = make_shared<OBlockItem>();
	dynamic_pointer_cast<OBlockItem>(this->items[0])->setInfo(BlockType::GRASS, false);
	this->items[1] = make_shared<TBlockItem>();
	dynamic_pointer_cast<TBlockItem>(this->items[1])->
		setInfo(BlockType::TRANSPARENCY_RED, vec4(1, 0, 0, 0.3), true);
	this->items[2] = make_shared<TBlockItem>();
	dynamic_pointer_cast<TBlockItem>(this->items[2])->
		setInfo(BlockType::TRANSPARENCY_BLUE, vec4(0, 0, 1, 0.3), true);
	this->items[3] = make_shared<TBlockItem>();
	dynamic_pointer_cast<TBlockItem>(this->items[3])->
		setInfo(BlockType::TRANSPARENCY_GREEN, vec4(0, 1, 0, 0.3), true);
}

void TabItems::setGUIBuffer(ComPtr<ID3D11DeviceContext> const& context)
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
	shared_ptr<Texture> const& texture = GUIResources::getTexture(GUITexture::TAB_ITEMS);
	context->PSSetShaderResources(0, 1, texture->getComPtr().GetAddressOf());
}

void TabItems::setOpacityItemBuffer(ComPtr<ID3D11DeviceContext> const& context, int idx)
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

void TabItems::setTransParencyBuffer(ComPtr<ID3D11DeviceContext> const& context, int idx)
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
	context->VSSetConstantBuffers(0, 1, this->constant_buffer->getComPtr().GetAddressOf());
}

bool TabItems::getItemOTFlag(int idx)
{
	return this->items[idx]->getBlockFlag();
}

shared_ptr<BlockItem> const& TabItems::getItem(int idx)
{
	return this->items[idx];
}

int TabItems::getItemArraySize()
{
	return this->items.size();
}

void TabItems::moveGUIPos(vec3 const& new_pos)
{
	this->gui_world = Mat::CreateTranslation(new_pos);
}

void TabItems::moveItem(int idx, vec3 const& new_pos)
{
	if (this->items[idx] == nullptr)
		return;
	// 아이템창 칸 안에 들어와 있는 지 확인해야 함

	int new_idx;
}

