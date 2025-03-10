#pragma once

template<typename T> class Buffer;
struct VertexDefer;
class Texture;

class OBlockItem
{
public:
	OBlockItem(string const& item_name);
	void setPos(vec3 const& pos);
	Mat const& getWorldMatrix();
	shared_ptr<Buffer<VertexDefer>> const& getVertexBuffer();
	shared_ptr<Buffer<uint32>> const& getIndexBuffer();
	shared_ptr<Texture>const& getTexture();

private:
	OBlockItem(OBlockItem const&) = delete;
	OBlockItem& operator=(OBlockItem const&) = delete;

private:
	string const& item_name;
	Mat world;

private:
	shared_ptr<Buffer<VertexDefer>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;

};

