#pragma once

class Texture;

class GUIResources
{
public:
	static void initialize();
	static shared_ptr<Texture> const& getTexture(string str);

private:
	~GUIResources() = delete;
	GUIResources() = delete;
	GUIResources(GUIResources const&) = delete;
	GUIResources& operator=(GUIResources const&) = delete;

private:
	static map<string, shared_ptr<Texture>> gui_resources;
};

