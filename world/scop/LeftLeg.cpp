#include "pch.h"
#include "LeftLeg.h"
#include "DeferredGraphics.h"
#include "ConstantBuffer.h"

LeftLeg::LeftLeg(Mat const& o_pos, Mat const& o_rot)
	: ori_pos(o_pos), ori_rot(o_rot)
{
	this->constant_buffer = make_shared<ConstantBuffer>(
		d_graphic->getDevice(),
		d_graphic->getContext(),
		this->mvp
	);
}

shared_ptr<Buffer<VertexPTN>>& LeftLeg::getVertexBuffer()
{
	return this->v_buffer;
}

shared_ptr<Buffer<uint32>>& LeftLeg::getIndexBuffer()
{
	return this->i_buffer;
}

shared_ptr<ConstantBuffer>& LeftLeg::getConstantBuffer()
{
	return this->constant_buffer;
}
