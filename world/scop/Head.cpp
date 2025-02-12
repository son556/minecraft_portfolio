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
	Parts::humanVertices(vertices, 0.5, 0.5, 0.5, HumanParts::HEAD);
	Parts::makeIndices(indices);
	this->v_buffer = make_shared<Buffer<VertexPTN>>(
		d_graphic->getDevice(),
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->basic_mat = Mat::CreateTranslation(vec3(0, 1.75, 0));
	this->world = o_rot * o_pos * this->basic_mat;
}

shared_ptr<Buffer<VertexPTN>>& Head::getVertexBuffer()
{
	return this->v_buffer;
}

Mat Head::getWorld()
{
	return this->world;
}

