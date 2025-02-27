#include "pch.h"
#include "LeftArm.h"
#include "ConstantBuffer.h"
#include "DeferredGraphics.h"
#include "Parts.h"
#include "Buffer.h"
#include "TestCam.h"

LeftArm::LeftArm(Mat const& o_pos, Mat const& o_rot)
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
	this->world = this->basic_mat * o_rot * o_pos;
}

shared_ptr<Buffer<VertexPTN>>& LeftArm::getVertexBuffer()
{
	return this->v_buffer;
}

Mat LeftArm::getWorld()
{
	return this->world;
}

void LeftArm::update(Mat const& o_pos, Mat const& o_rot)
{
	if (first_view) {
		vec3&& c_dir = cam->getDir();
		float d = c_dir.Dot(XMVector3Normalize(vec3(c_dir.x, 0, c_dir.z)));
		float theta = atan2(c_dir.y, d);
		Mat rotX = Mat::CreateRotationX(theta);
		this->basic_mat = Mat::CreateTranslation(vec3(0, -0.25, 0)) *
			Mat::CreateRotationX(XMConvertToRadians(90)) * rotX * 
			Mat::CreateTranslation(vec3(-0.375, 1.375, 0));
	}
	else
		this->basic_mat = Mat::CreateTranslation(vec3(-0.375, 1.125, 0));

	/*vec3&& c_dir = cam->getDir();
	float d = c_dir.Dot(XMVector3Normalize(vec3(c_dir.x, 0, c_dir.z)));
	float theta = atan2(c_dir.y, d);
	Mat rotX = Mat::CreateRotationX(theta);
	this->basic_mat = Mat::CreateTranslation(vec3(0, -0.25, 0)) *
		Mat::CreateRotationX(XMConvertToRadians(90)) *
		Mat::CreateRotationZ(XMConvertToRadians(45)) *
		Mat::CreateTranslation(vec3(-0.375, 1.375, 0));*/

	this->world = this->basic_mat * o_rot * o_pos;
}

