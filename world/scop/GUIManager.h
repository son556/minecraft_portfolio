#pragma once

#include "GUI.h"
#include "GUIRender.h"

class GUIManager
{
public:
	GUIManager();
	~GUIManager() = default;
	void render();
	ComPtr<ID3D11ShaderResourceView> const& getSRV();

private:
	GUIManager(GUIManager const&) = delete;
	GUIManager& operator=(GUIManager const&) = delete;

private:
	map<string, shared_ptr<GUI>> gui_book;
	GUIRender gui_render;
};

