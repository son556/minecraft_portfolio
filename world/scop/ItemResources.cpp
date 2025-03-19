#include "pch.h"
#include "ItemResources.h"
#include "Texture.h"
#include "DeferredGraphics.h"

vector<shared_ptr<Texture>> ItemResources::item_textures;

void ItemResources::initializeItemResources()
{
	ComPtr<ID3D11Device> const& device = d_graphic->getDevice();

	item_textures.push_back(make_shared<Texture>(device,
		L"./textures/pbr/test_sample/packed_mud.png"));
}

shared_ptr<Texture>& ItemResources::getTexture(BlockType block_type)
{
	int idx = static_cast<int>(block_type) - 1;
	return item_textures[idx];
}
