#pragma once

#include "BlockItem.h"

struct VertexColor;

class TBlockItem : public BlockItem
{
public:
	TBlockItem() = default;
	virtual ~TBlockItem() = default;
	void setPos(vec3 const& pos);
	Mat const& getWorldMatrix();
	shared_ptr<Buffer<VertexColor>> const& getVertexBuffer();
	shared_ptr<Buffer<uint32>> const& getIndexBuffer();
	void setInfo(BlockType item_type, vec4 const& color, bool tp_flag);
	virtual BlockType getBlockType() override;

private:
	TBlockItem(TBlockItem const&) = delete;
	TBlockItem& operator=(TBlockItem const&) = delete;

private:
	virtual void copyFrom(BlockItem const& othrer) override;

private:
	BlockType item_type;
	shared_ptr<Buffer<VertexColor>> v_buff;
	shared_ptr<Buffer<uint32>> i_buff;
	Mat world;
};

