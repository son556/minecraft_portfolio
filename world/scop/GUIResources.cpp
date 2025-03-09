#include "pch.h"
#include "GUIResources.h"
#include "Texture.h"
#include "DeferredGraphics.h"

map<string, shared_ptr<Texture>> GUIResources::gui_resources;



void GUIResources::initialize()
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	gui_resources.insert(make_pair("widgets", make_shared<Texture>(
		device,
		L"./textures/gui/widgets.png")));
	gui_resources.insert(make_pair("minecraft", make_shared<Texture>(
		device,
		L"./textures/gui/minecraft.png")));
	gui_resources.insert(make_pair("tab_button", make_shared<Texture>(
		device,
		L"./textures/gui/tab_button.png")));
	gui_resources.insert(make_pair("tab_items", make_shared<Texture>(
		device, L"./textures/gui/tab_items.png")));
}

shared_ptr<Texture> const& GUIResources::getTexture(string str)
{
	map<string, shared_ptr<Texture>>::iterator it;
	it = gui_resources.find(str);
	if (it == gui_resources.end()) {
		cout << "no gui texture" << endl;
		assert(false);
	}
	return it->second;
}


