#pragma once

#include "OBlockItem.h"
#include "TBlockItem.h"
#include "GUIResources.h"

template<typename T> class Buffer;
struct VertexDefer;

class GUI
{
public:
	GUI(float w, float h, string gui_name);
	void setGUIBuffer(ComPtr<ID3D11DeviceContext> const& context);
	void setOpacityItemBuffer(
		ComPtr<ID3D11DeviceContext> const& context,
		int idx
	);
	void setTransParencyBuffer(
		ComPtr<ID3D11DeviceContext> const& context,
		int idx
	);
	~GUI() = default;

private:
	GUI() = delete;
	GUI(GUI const&) = delete;
	GUI& operator=(GUI const&) = delete;

private:
	shared_ptr<Buffer<VertexDefer>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;
	string gui_name;

private:
	vector<OBlockItem> opacity_items;
	vector<TBlockItem> tp_items;
};

