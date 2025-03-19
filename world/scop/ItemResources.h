#pragma once

class Texture;
class ItemResources
{
public:
	static void initializeItemResources();
	static shared_ptr<Texture>& getTexture(BlockType block_type);

private:
	ItemResources() = delete;
	~ItemResources() = delete;
	ItemResources(ItemResources const&) = delete;
	ItemResources& operator=(ItemResources const&) = delete;

private:
	static vector<shared_ptr<Texture>> item_textures;
};

