#pragma once

#include "DeferredRendering.h"

class Graphics;

class RenderSystem
{
public:
	RenderSystem(MapUtils* minfo);
	~RenderSystem() = default;
	void Render();

private:
	RenderSystem() = delete;
	RenderSystem(RenderSystem&) = delete;
	RenderSystem& operator=(RenderSystem&) = delete;

private:
	MapUtils* m_info;
	shared_ptr<DeferredRendering> r_deff;
};

