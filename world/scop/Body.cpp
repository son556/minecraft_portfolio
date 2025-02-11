#include "pch.h"
#include "Body.h"
#include "ConstantBuffer.h"
#include "DeferredGraphics.h"

Body::Body(Mat const& o_pos, Mat const& o_rot) 
	: ori_pos(o_pos), ori_rot(o_rot)
{
	this->constant_buffer = make_shared<ConstantBuffer>(
		d_graphic->getDevice(),
		d_graphic->getContext(),
		this->mvp
	);
}

shared_ptr<Buffer<VertexPTN>>& Body::getVertexBuffer()
{
	return this->v_buffer;
}

shared_ptr<Buffer<uint32>>& Body::getIndexBuffer()
{
	return this->i_buffer;
}

shared_ptr<ConstantBuffer>& Body::getConstantBuffer()
{
	return this->constant_buffer;
}
