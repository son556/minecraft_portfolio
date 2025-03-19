#pragma once

#include "GUI.h"

class TabItems : public GUI
{
public:
	TabItems(float w, float h);
	virtual void setGUIBuffer(
		ComPtr<ID3D11DeviceContext> const& context) override;
	
	virtual void setOpacityItemBuffer(
		ComPtr<ID3D11DeviceContext> const& context,
		int idx
	) override;
	
	virtual void setTransParencyBuffer(
		ComPtr<ID3D11DeviceContext> const& context,
		int idx
	) override;

	/**
	 * opacity -> false, transparency -> true.
	 * 
	 * \param idx
	 * \return 
	 */
	virtual bool getItemOTFlag(int idx) override;
	virtual shared_ptr<BlockItem> const& getItem(int idx) override;
	virtual int getItemArraySize() override;
	virtual void moveGUIPos(vec3 const& new_pos) override;
	virtual void moveItem(int idx, vec3 const& new_pos) override;

private:
	TabItems() = delete;
	TabItems& operator=(TabItems const&) = delete;
	TabItems(TabItems const&) = delete;

private:
	shared_ptr<ConstantBuffer> constant_buffer;
	shared_ptr<Buffer<VertexDefer>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;

private:
	Mat gui_world = Mat::Identity;
	vector<shared_ptr<BlockItem>> items;
};

