#pragma once

class Texture;

enum class GUITexture {
	TAB_ITEMS,
	INVENTORY,
	SELECT_ITEM
};

class GUIResources
{
public:
	static void initialize();
	static shared_ptr<Texture> const& getTexture(GUITexture tex_id);

private:
	~GUIResources() = delete;
	GUIResources() = delete;
	GUIResources(GUIResources const&) = delete;
	GUIResources& operator=(GUIResources const&) = delete;

private:
	static shared_ptr<Texture> gui_resources[3];
};

