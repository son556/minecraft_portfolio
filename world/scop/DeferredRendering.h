#pragma once

#include "OpacityRender.h"
#include "OIT.h"
#include "Water.h"

class MapUtils;
template <typename T> class Buffer;
class Wallpaper;

class DeferredRendering
{
public:
	DeferredRendering(MapUtils* minfo);
	~DeferredRendering() = default;
	void Render();
	
private:
	DeferredRendering() = delete;
	DeferredRendering(DeferredRendering const&) = delete;
	DeferredRendering& operator=(DeferredRendering const&) = delete;

private:
	void setFinPipe();

private:
	shared_ptr<Buffer<VertexDefer>> vbuffer;
	shared_ptr<Buffer<uint32>> ibuffer;

private:
	MapUtils* m_info = nullptr;
	OpacityRender opacity_render;
	OIT oit;
	Water water;

private:
	shared_ptr<RasterizerState> rasterizer_state;

private:
	shared_ptr<VertexShader> fin_vs;
	shared_ptr<PixelShader> fin_ps;
	shared_ptr<InputLayout> fin_ia;
};

