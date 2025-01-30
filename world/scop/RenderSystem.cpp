#include "pch.h"
#include "RenderSystem.h"

RenderSystem::RenderSystem(MapUtils* minfo) 
	: m_info(minfo)
{
	this->r_deff = make_shared<DeferredRendering>(
		this->m_info
	);
}

void RenderSystem::Render()
{
	this->r_deff->Render();
}
