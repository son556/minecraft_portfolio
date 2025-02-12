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
	void renderHead(bool shadow_flag = false);
	void renderBody();
	void renderLeftArm();
	void renderRightArm();
	void renderLeftLeg();
	void renderRightLeg();

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

private:
	shared_ptr<LeftArm> left_arm;
	shared_ptr<RightArm> right_arm;
	shared_ptr<Head> head;
	shared_ptr<Body> body;
	shared_ptr<RightLeg> right_leg;
	shared_ptr<LeftLeg> left_leg;
};

