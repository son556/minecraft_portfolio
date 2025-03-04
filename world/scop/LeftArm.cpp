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
	this->animation_flag = 0;
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
		float d = sqrt(c_dir.x * c_dir.x + c_dir.z * c_dir.z);
		float theta = atan2(c_dir.y, d);
		Mat rotX = Mat::CreateRotationX(theta);
		this->basic_mat = Mat::CreateTranslation(vec3(0, -0.25, 0)) *
			Mat::CreateRotationX(XMConvertToRadians(90)) * rotX * 
			Mat::CreateTranslation(vec3(-0.375, 1.375, 0));
	}
	else
		this->basic_mat = Mat::CreateTranslation(vec3(-0.375, 1.125, 0));

	if (animation_flag)
		this->animation();
	this->world = this->basic_mat * o_rot * o_pos;
}

void LeftArm::updateAnimation(int animation_type)
{
	this->animation_flag = animation_type;
}

void LeftArm::animation()
{
	if (animation_flag == 0)
		return;
	// 쿼터니언 사용법 + 구면 보간
	/*SimpleMath::Quaternion quaternion;
	quaternion.
	Mat::CreateFromQuaternion()*/
	vec3 s = XMVector3Normalize(vec3(1, -1, -1));
	static float w = 0;
	static float dt = 0;
	static int tick = 0;
	SimpleMath::Quaternion q = vec4(s.x, s.y, s.z, w);
	dt += delta_time;
	w = cosf(dt * XMConvertToRadians(3) * 0.5);
	tick += 1;
	Mat rot = SimpleMath::Matrix::CreateFromQuaternion(q);
	this->basic_mat *= rot;
	if (tick == 120) {
		w = 0;
		dt = 0;
		tick = 0;
		this->animation_flag = 0;
	}
}

