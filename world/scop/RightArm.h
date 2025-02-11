#pragma once

template<typename T> class Buffer;
class ConstantBuffer;
struct VertexPTN;

class RightArm
{
public:
	RightArm(Mat const& o_pos, Mat const& o_rot);
	~RightArm() = default;
	shared_ptr<Buffer<VertexPTN>>& getVertexBuffer();
	shared_ptr<Buffer<uint32>>& getIndexBuffer();
	shared_ptr<ConstantBuffer>& getConstantBuffer();

private:
	RightArm() = delete;
	RightArm(RightArm const&) = delete;
	RightArm& operator=(RightArm const&) = delete;

private:
	Mat ori_pos;
	Mat ori_rot;
	MVP mvp;

private:
	shared_ptr<Buffer<VertexPTN>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;
	shared_ptr<ConstantBuffer> constant_buffer;
};

