#include "pch.h"
#include "GUIManager.h"
#include "GUIResources.h"
#include "ItemResources.h"
#include "TabItems.h"
#include "Inventory.h"

GUIManager::GUIManager()
{
	GUIResources::initialize();
	ItemResources::initializeItemResources();
	float w = 0.8f;
	float r = 1.f;
	int idx = static_cast<int>(GUITexture::TAB_ITEMS);
	this->gui_arr.resize(2);
	this->gui_arr[idx] = make_shared<TabItems>(w, w * r);
	this->gui_arr[idx]->moveGUIPos(vec3(0, 0.2, 0));

	w = 0.3f;
	idx = static_cast<int>(GUITexture::INVENTORY);
	this->gui_arr[idx] = make_shared<Inventory>(w, 0.1);
	this->gui_arr[idx]->moveGUIPos(vec3(0, -0.9, 0));
}

void GUIManager::render(GUITexture idx)
{
	this->render();
	this->gui_render.render(this->gui_arr[static_cast<int>(idx)].get(), false);
}

void GUIManager::render()
{
	this->gui_render.render(
		this->gui_arr[static_cast<int>(GUITexture::INVENTORY)].get(), true);
}

ComPtr<ID3D11ShaderResourceView> const& GUIManager::getSRV()
{
	return this->gui_render.getSRV();
}
