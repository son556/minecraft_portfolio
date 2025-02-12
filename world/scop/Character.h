#pragma once

#include "WorldUtils.h"

class Texture;
class LeftArm;
class RightArm;
class Head;
class Body;
class LeftLeg;
class RightLeg;
class ConstantBuffer;
template<typename T> class Buffer;

class Character
{
public:
	Character(Mat pos, Mat rot);
	~Character() = default;
	void render(
		CamType type, 
		bool shadow_flag = false,
		Mat const& view = Mat::Identity,
		Mat const& proj = Mat::Identity
	);

private:
	void renderHead(
		ComPtr<ID3D11DeviceContext>& context,
		bool shadow_flag = false);
	void renderBody(
		ComPtr<ID3D11DeviceContext>& context,
		bool shadow_flag = false);
	void renderLeftArm(
		ComPtr<ID3D11DeviceContext>& context,
		bool shadow_flag = false);
	void renderRightArm(ComPtr<ID3D11DeviceContext>& context,
		bool shadow_flag = false);
	void renderLeftLeg(ComPtr<ID3D11DeviceContext>& context,
		bool shadow_flag = false);
	void renderRightLeg(ComPtr<ID3D11DeviceContext>& context,
		bool shadow_flag = false);

private:
	Character() = delete;
	Character(Character const&) = delete;
	Character& operator=(Character const&) = delete;

private:
	Mat pos;
	Mat rot;
	shared_ptr<Texture> skin_tex;
	MVP mvp;
	MVP shadow_mvp;
	shared_ptr<ConstantBuffer> constant_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;

private:
	shared_ptr<LeftArm> left_arm;
	shared_ptr<RightArm> right_arm;
	shared_ptr<Head> head;
	shared_ptr<Body> body;
	shared_ptr<RightLeg> right_leg;
	shared_ptr<LeftLeg> left_leg;
};

