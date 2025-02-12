#pragma once

template<typename T> class Buffer;
class ConstantBuffer;
struct VertexPTN;

class RightLeg
{
public:
	RightLeg(Mat const& o_pos, Mat const& o_rot);
	~RightLeg() = default;
	shared_ptr<Buffer<VertexPTN>>& getVertexBuffer();
	Mat getWorld();

private:
	RightLeg() = delete;
	RightLeg(RightLeg const&) = delete;
	RightLeg& operator=(RightLeg const&) = delete;

private:
	Mat ori_pos;
	Mat ori_rot;
	Mat world;
	Mat basic_mat;

private:
	shared_ptr<Buffer<VertexPTN>> v_buffer;
};

