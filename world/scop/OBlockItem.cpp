#include "pch.h"
#include "OBlockItem.h"
#include "DeferredGraphics.h"
#include "Block.h"
#include "ItemResources.h"
#include "Buffer.h"
#include "Texture.h"

OBlockItem::OBlockItem(string const& item_name) : item_name(item_name)
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	vector<VertexDefer> vertices;
	vector<uint32> indices;
	Block::makeBox(0.25, vertices, indices);
	this->v_buffer = make_shared<Buffer<VertexDefer>>(device,
		vertices.data(), vertices.size(), D3D11_BIND_VERTEX_BUFFER);
	this->i_buffer = make_shared<Buffer<uint32>>(device,
		indices.data(), indices.size(), D3D11_BIND_INDEX_BUFFER);
}

void OBlockItem::setPos(vec3 const& pos)
{
	this->world = Mat::CreateTranslation(pos);
}

Mat const& OBlockItem::getWorldMatrix()
{
	return this->world;
}

shared_ptr<Buffer<VertexDefer>> const& OBlockItem::getVertexBuffer()
{
	return this->v_buffer;
}

shared_ptr<Buffer<uint32>> const& OBlockItem::getIndexBuffer()
{
	return this->i_buffer;
}

shared_ptr<Texture> const& OBlockItem::getTexture()
{
	return ItemResources::getTexture(this->item_name);
}

