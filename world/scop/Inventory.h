#pragma once

#include "GUI.h"

class Inventory : public GUI
{
public:
	Inventory(float w, float h);
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
	virtual int getItemArraySize() override;
	virtual shared_ptr<BlockItem> const& getItem(int idx) override;
	virtual bool getItemOTFlag(int idx) override;

	/**
	 * 배열에 있는 반투명 아이템 위치를 이동합니다.
	 *
	 * \param idx 배열 인덱스
	 * \param new_pos 아이템의 새 위치
	 */
	virtual void moveItem(int idx, vec3 const& new_pos) override;
	virtual void moveGUIPos(vec3 const& new_pos) override;

	BlockType getBlockType(int idx);
private:
	Inventory() = delete;
	Inventory& operator=(Inventory const&) = delete;
	Inventory(Inventory const&) = delete;

private:
	shared_ptr<ConstantBuffer> constant_buffer;
	shared_ptr<Buffer<VertexDefer>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;

private:
	Mat gui_world = Mat::Identity;
	vector<shared_ptr<BlockItem>> items;
};

