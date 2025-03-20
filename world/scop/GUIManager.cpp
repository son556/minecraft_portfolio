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
	int idx = static_cast<int>(GUITexture::TAB_ITEMS);
	this->gui_arr.resize(2);
	this->gui_arr[idx] = make_shared<TabItems>(0.8, 0.8);
	this->gui_arr[idx]->moveGUIPos(vec3(0, 0.2, 0));

	idx = static_cast<int>(GUITexture::INVENTORY);
	this->gui_arr[idx] = make_shared<Inventory>(0.3, 0.1);
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

BlockType GUIManager::getInventoryBlock(int idx)
{
	shared_ptr<Inventory>&& inven =
		dynamic_pointer_cast<Inventory>(this->gui_arr[static_cast<int>(GUITexture::INVENTORY)]);
	return inven->getBlockType(idx);
}

void GUIManager::selectInventoryItem(int idx)
{
	shared_ptr<Inventory>&& inven =
		dynamic_pointer_cast<Inventory>(this->gui_arr[static_cast<int>(GUITexture::INVENTORY)]);
	inven->selectItem(idx);
}
