#include "pch.h"
#include "GUIResources.h"
#include "Texture.h"
#include "DeferredGraphics.h"

shared_ptr<Texture> GUIResources::gui_resources[3];



void GUIResources::initialize()
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	ComPtr<ID3D11DeviceContext> const& context = d_graphic->getContext();
	gui_resources[0] = make_shared<Texture>(
		device, L"./textures/gui/tab_items.png");

	gui_resources[1] = make_shared<Texture>(
		device, L"./textures/gui/inventory.png");
	
	gui_resources[2] = make_shared<Texture>(
		device, L"./textures/gui/select_item.png");
}

shared_ptr<Texture> const& GUIResources::getTexture(GUITexture tex_id)
{
	return gui_resources[static_cast<int>(tex_id)];
}


