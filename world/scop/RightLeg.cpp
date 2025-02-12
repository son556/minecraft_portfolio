#include "pch.h"
#include "RightLeg.h"
#include "ConstantBuffer.h"
#include "DeferredGraphics.h"
#include "Buffer.h"
#include "Parts.h"

RightLeg::RightLeg(Mat const& o_pos, Mat const& o_rot)
	: ori_pos(o_pos), ori_rot(o_rot)
{
	vector<VertexPTN> vertices;
	Parts::humanVertices(vertices, 0.25, 0.75, 0.25, HumanParts::RIGHTLEG);
	this->v_buffer = make_shared<Buffer<VertexPTN>>(
		d_graphic->getDevice(),
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->basic_mat = Mat::CreateTranslation(vec3(0.125, 0.375, 0));
	this->world = o_rot * o_pos * this->basic_mat;
}

shared_ptr<Buffer<VertexPTN>>& RightLeg::getVertexBuffer()
{
	return this->v_buffer;
}

Mat RightLeg::getWorld()
{
	return this->world;
}

