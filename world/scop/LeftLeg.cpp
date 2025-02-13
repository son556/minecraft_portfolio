#include "pch.h"
#include "LeftLeg.h"
#include "DeferredGraphics.h"
#include "ConstantBuffer.h"
#include "Parts.h"
#include "Buffer.h"

LeftLeg::LeftLeg(Mat const& o_pos, Mat const& o_rot)
{
	vector<VertexPTN> vertices;
	Parts::humanVertices(vertices, 0.25, 0.75, 0.25, HumanParts::LEFTLEG);
	this->v_buffer = make_shared<Buffer<VertexPTN>>(
		d_graphic->getDevice(),
		vertices.data(),
		vertices.size(),
		D3D11_BIND_VERTEX_BUFFER
	);
	this->basic_mat = Mat::CreateTranslation(vec3(-0.125, 0.375, 0));
	this->world = this->basic_mat * o_rot * o_pos;
}

shared_ptr<Buffer<VertexPTN>>& LeftLeg::getVertexBuffer()
{
	return this->v_buffer;
}

Mat LeftLeg::getWorld()
{
	return this->world;
}

void LeftLeg::update(Mat const& o_pos, Mat const& o_rot)
{
	this->world = this->basic_mat * o_rot * o_pos;
}
