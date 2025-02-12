#include "pch.h"
#include "LeftArm.h"
#include "ConstantBuffer.h"
#include "DeferredGraphics.h"
#include "Parts.h"
#include "Buffer.h"

LeftArm::LeftArm(Mat const& o_pos, Mat const& o_rot)
	: ori_pos(o_pos), ori_rot(o_rot)
{
	vector<VertexPTN> vertices;
	Parts::humanVertices(vertices, 0.25, 0.75, 0.25, HumanParts::LEFTARM);
	this->v_buffer = make_shared<Buffer<VertexPTN>>(
		d_graphic->getDevice(),
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->basic_mat = Mat::CreateTranslation(vec3(-0.375, 1.125, 0));
	this->world = o_rot * o_pos * this->basic_mat;
}

shared_ptr<Buffer<VertexPTN>>& LeftArm::getVertexBuffer()
{
	return this->v_buffer;
}

Mat LeftArm::getWorld()
{
	return this->world;
}

