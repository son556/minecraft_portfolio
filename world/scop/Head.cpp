#include "pch.h"
#include "Head.h"
#include "ConstantBuffer.h"
#include "DeferredGraphics.h"
#include "Parts.h"
#include "Buffer.h"

Head::Head(Mat const& o_pos, Mat const& o_rot)
	: ori_pos(o_pos), ori_rot(o_rot)
{
	vector<VertexPTN> vertices;
	vector<uint32> indices;
	Parts::humanHead(vertices, 0.5, 0.5, 0.5);
	Parts::makeIndices(indices);
	this->v_buffer = make_shared<Buffer<VertexPTN>>(
		d_graphic->getDevice(),
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->i_buffer = make_shared<Buffer<uint32>>(
		d_graphic->getDevice(),
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	Mat move = Mat::CreateTranslation(vec3(0, 1.75, 0));
	this->world = o_pos * move * o_rot;
}

shared_ptr<Buffer<VertexPTN>>& Head::getVertexBuffer()
{
	return this->v_buffer;
}

shared_ptr<Buffer<uint32>>& Head::getIndexBuffer()
{
	return this->i_buffer;
}

Mat Head::getWorld()
{
	return this->world;
}

