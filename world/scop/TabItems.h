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
	virtual void optRender() override;

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
	const float gap_width = 0.148f;
	const float gap_height = 0.215;
	const float obh_size = w_width * 0.0001f - 0.01;
	const float obw_size = w_height * 0.0001f - 0.01;
	const float tbh_size = 2 * obh_size;
	const float tbw_size = 2 * obw_size;
	const float items_x_0 = -0.66f;
	const float items_y_0 = 0.7f;
	const float items_y_45 = -0.43f;
};

