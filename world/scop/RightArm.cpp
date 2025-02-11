#include "pch.h"
#include "RightArm.h"
#include "ConstantBuffer.h"
#include "DeferredGraphics.h"

RightArm::RightArm(Mat const& o_pos, Mat const& o_rot)
	: ori_pos(o_pos), ori_rot(o_rot)
{
	this->constant_buffer = make_shared<ConstantBuffer>(
		d_graphic->getDevice(),
		d_graphic->getContext(),
		this->mvp
	);
}

shared_ptr<Buffer<VertexPTN>>& RightArm::getVertexBuffer()
{
	return this->v_buffer;
}

shared_ptr<Buffer<uint32>>& RightArm::getIndexBuffer()
{
	return this->i_buffer;
}

shared_ptr<ConstantBuffer>& RightArm::getConstantBuffer()
{
	return this->constant_buffer;
}
