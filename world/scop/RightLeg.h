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
	shared_ptr<Buffer<uint32>>& getIndexBuffer();
	shared_ptr<ConstantBuffer>& getConstantBuffer();

private:
	RightLeg() = delete;
	RightLeg(RightLeg const&) = delete;
	RightLeg& operator=(RightLeg const&) = delete;

private:
	Mat ori_pos;
	Mat ori_rot;
	MVP mvp;

private:
	shared_ptr<Buffer<VertexPTN>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;
	shared_ptr<ConstantBuffer> constant_buffer;
};

