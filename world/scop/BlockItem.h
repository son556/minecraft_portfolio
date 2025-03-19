#pragma once

template<typename T> class Buffer;

class BlockItem
{
public:
	BlockItem() = default;
	virtual ~BlockItem() = default;

	/**
	 * 블록이 반투명인지 불투명인지 반환합니다.
	 * 
	 * \return 반투명 true, 불투명 false 
	 */
	bool getBlockFlag();
	BlockItem& operator=(BlockItem const& other);
	virtual BlockType getBlockType() abstract;

protected:
	void setInfo(bool tp_flag);
	virtual void copyFrom(BlockItem const& other);

private:
	BlockItem(BlockItem const&) = delete;

private:
	bool tp_flag = false;
};

