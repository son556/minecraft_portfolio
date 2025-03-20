#include "pch.h"
#include "TBlockItem.h"
#include "Buffer.h"
#include "DeferredGraphics.h"
#include "WorldUtils.h"

void TBlockItem::setPos(vec3 const& pos)
{
	this->world = Mat::CreateTranslation(pos);
}

Mat const& TBlockItem::getWorldMatrix()
{
	return this->world;
}

shared_ptr<Buffer<VertexColor>> const& TBlockItem::getVertexBuffer()
{
	return this->v_buff;
}

shared_ptr<Buffer<uint32>> const& TBlockItem::getIndexBuffer()
{
	return this->i_buff;
}

void TBlockItem::setInfo(BlockType item_type, 
	vec4 const& color, bool tp_flag, float w_size, float h_size)
{
	BlockItem::setInfo(tp_flag);
	this->item_type = item_type;
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	float scale = 0.14;
	vec3 s = vec3(scale, scale, 0);
	if (w_size != -1)
		s = vec3(w_size, h_size, 0);
	vector<VertexColor> vertices;
	vertices.resize(4);
	vertices[0].pos = vec3(-0.5, 0.5, 0) * s;
	vertices[0].col = color;
	vertices[1].pos = vec3(0.5, 0.5, 0) * s;
	vertices[1].col = color;
	vertices[2].pos = vec3(0.5, -0.5, 0) * s;
	vertices[2].col = color;
	vertices[3].pos = vec3(-0.5, -0.5, 0) * s;
	vertices[3].col = color;

	static vector<uint32> indices = { 0, 1, 3, 1, 2, 3 };

	this->v_buff = make_shared<Buffer<VertexColor>>(
		device,
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->i_buff = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
}

BlockType TBlockItem::getBlockType()
{
	return this->item_type;
}

void TBlockItem::copyFrom(BlockItem const& other)
{
	BlockItem::copyFrom(other);
	TBlockItem const* oth = dynamic_cast<TBlockItem const*>(&other);
	this->i_buff = oth->i_buff;
	this->v_buff = oth->v_buff;
	this->world = oth->world;
	this->item_type = oth->item_type;
}


