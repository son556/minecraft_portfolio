#pragma once

#include "BlockItem.h"

template<typename T> class Buffer;
struct VertexDefer;
class Texture;

class OBlockItem : public BlockItem
{
public:
	OBlockItem() = default;
	~OBlockItem() = default;
	void setPos(vec3 const& pos);
	Mat const& getWorldMatrix();
	shared_ptr<Buffer<VertexDefer>> const& getVertexBuffer();
	shared_ptr<Buffer<uint32>> const& getIndexBuffer();
	shared_ptr<Texture>const& getTexture();
	void setInfo(BlockType const& item_type, bool tp_flag);
	virtual BlockType getBlockType() override;

private:
	virtual void copyFrom(BlockItem const& othrer) override;
	OBlockItem(OBlockItem const&) = delete;
	OBlockItem& operator=(const OBlockItem&) = delete;

private:
	BlockType item_type;
	Mat world = Mat::Identity;
	shared_ptr<Buffer<VertexDefer>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;

};

