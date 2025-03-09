#pragma once

class Texture;
class ItemResources
{
public:
	static void initializeItemResources();
	static shared_ptr<Texture>& getTexture(string str);

private:
	ItemResources() = delete;
	~ItemResources() = delete;
	ItemResources(ItemResources const&) = delete;
	ItemResources& operator=(ItemResources const&) = delete;

private:
	static map<string, shared_ptr<Texture>> item_textures;
};

