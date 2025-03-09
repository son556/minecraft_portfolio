#include "pch.h"
#include "ItemResources.h"
#include "Texture.h"
#include "DeferredGraphics.h"

map<string, shared_ptr<Texture>>ItemResources::item_textures;

void ItemResources::initializeItemResources()
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();
	item_textures.insert(make_pair("grass", make_shared<Texture>(
		device,
		L"./textures/pbr/test_sample/packed_mud.png" // temp
	)));
	item_textures.insert(make_pair("oak_log", make_shared<Texture>(
		device,
		L"./textures/pbr/oak_tree/oak_log.png")));
}

shared_ptr<Texture>& ItemResources::getTexture(string str)
{
	map<string, shared_ptr<Texture>>::iterator it;
	it = item_textures.find(str);
	if (it == item_textures.end()) {
		cout << "no item texture" << endl;
		assert(false);
	}
	return it->second;
}
