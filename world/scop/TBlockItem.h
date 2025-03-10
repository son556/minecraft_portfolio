#pragma once

template<typename T> class Buffer;
struct VertexColor;

class TBlockItem
{
public:
	TBlockItem(vec4 const& color);
	~TBlockItem() = default;
	void setPos(vec3 const& pos);
	Mat const& getWorldMatrix();
	shared_ptr<Buffer<VertexColor>> const& getVertexBuffer();
	shared_ptr<Buffer<uint32>> const& getIndexBuffer();

private:
	TBlockItem() = delete;
	TBlockItem(TBlockItem const&) = delete;
	TBlockItem& operator=(TBlockItem const&) = delete;

private:
	shared_ptr<Buffer<VertexColor>> v_buff;
	shared_ptr<Buffer<uint32>> i_buff;
	Mat world;
};

