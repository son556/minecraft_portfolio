#pragma once

#include "GUI.h"
#include "GUIRender.h"

class GUIManager
{
public:
	GUIManager();
	~GUIManager() = default;
	void render(GUITexture idx);
	void render();
	ComPtr<ID3D11ShaderResourceView> const& getSRV();

private:
	GUIManager(GUIManager const&) = delete;
	GUIManager& operator=(GUIManager const&) = delete;

private:
	vector<shared_ptr<GUI>> gui_arr;
	GUIRender gui_render;
};

