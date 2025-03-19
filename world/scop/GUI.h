#pragma once

#include "OBlockItem.h"
#include "TBlockItem.h"
#include "GUIResources.h"

class ConstantBuffer;

class GUI
{
public:
	GUI() = default;
	virtual ~GUI() = default;
	virtual void setGUIBuffer(
		ComPtr<ID3D11DeviceContext> const& context) abstract;
	virtual void setOpacityItemBuffer(
		ComPtr<ID3D11DeviceContext> const& context,
		int idx
	) abstract;
	virtual void setTransParencyBuffer(
		ComPtr<ID3D11DeviceContext> const& context,
		int idx
	) abstract;
	virtual int getItemArraySize() abstract;
	virtual shared_ptr<BlockItem> const& getItem(int idx) abstract;
	virtual bool getItemOTFlag(int idx) abstract;

	/**
	 * 배열에 있는 반투명 아이템 위치를 이동합니다.
	 *
	 * \param idx 배열 인덱스
	 * \param new_pos 아이템의 새 위치
	 */
	virtual void moveItem(int idx, vec3 const& new_pos) abstract;
	virtual void moveGUIPos(vec3 const& new_pos) abstract;
};

