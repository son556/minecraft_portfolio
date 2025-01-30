#pragma once

#include "WorldUtils.h"

class DeferredGraphics;
template<typename T> class Buffer;

class Moon
{
public:
	Moon(UINT radius);
	~Moon() = default;
	shared_ptr<Buffer<VertexColor>> getVertexBffer();
	shared_ptr<Buffer<uint32>> getIndexBuffer();

private:
	Moon() = delete;
	Moon(Moon const&) = delete;
	Moon& operator=(Moon const&) = delete;

private:
	shared_ptr<Buffer<VertexColor>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;
};

