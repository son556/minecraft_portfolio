#include "pch.h"
#include "GUIManager.h"
#include "GUIResources.h"
#include "ItemResources.h"

GUIManager::GUIManager()
{
	GUIResources::initialize();
	ItemResources::initializeItemResources();
	float w = 0.8f;
	float r = 133.f / 194.f;
	this->gui_book.insert(make_pair(static_cast<string>("tab_items"), 
		make_shared<GUI>(w, w * r, "tab_items")));

}

void GUIManager::render()
{
	// temp
	map<string, shared_ptr<GUI>>::iterator it;
	for (it = this->gui_book.begin(); it != this->gui_book.end(); it++)
		this->gui_render.render(it->second.get());
}

ComPtr<ID3D11ShaderResourceView> const& GUIManager::getSRV()
{
	return this->gui_render.getSRV();
}
