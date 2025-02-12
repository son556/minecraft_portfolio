#pragma once

template<typename T> class Buffer;
class ConstantBuffer;
struct VertexPTN;

class LeftArm
{
public:
	LeftArm(Mat const& o_pos, Mat const& o_rot);
	~LeftArm() = default;
	shared_ptr<Buffer<VertexPTN>>& getVertexBuffer();
	Mat getWorld();

private:
	LeftArm() = delete;
	LeftArm(LeftArm const&) = delete;
	LeftArm& operator=(LeftArm const&) = delete;

private:
	Mat ori_pos;
	Mat ori_rot;
	Mat world;
	Mat basic_mat;

private:
	shared_ptr<Buffer<VertexPTN>> v_buffer;
};

