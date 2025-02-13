#pragma once

template<typename T> class Buffer;
class ConstantBuffer;
struct VertexPTN;

class LeftLeg
{
public:
	LeftLeg(Mat const& o_pos, Mat const& o_rot);
	~LeftLeg() = default;
	shared_ptr<Buffer<VertexPTN>>& getVertexBuffer();
	Mat getWorld();
	void update(Mat const& o_pos, Mat const& o_rot);

private:
	LeftLeg() = delete;
	LeftLeg(LeftLeg const&) = delete;
	LeftLeg& operator=(LeftLeg const&) = delete;

private:
	Mat world;
	Mat basic_mat;

private:
	shared_ptr<Buffer<VertexPTN>> v_buffer;
};

