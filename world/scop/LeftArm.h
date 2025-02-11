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
	shared_ptr<Buffer<uint32>>& getIndexBuffer();
	shared_ptr<ConstantBuffer>& getConstantBuffer();

private:
	LeftArm() = delete;
	LeftArm(LeftArm const&) = delete;
	LeftArm& operator=(LeftArm const&) = delete;

private:
	Mat ori_pos;
	Mat ori_rot;
	MVP mvp;

private:
	shared_ptr<Buffer<VertexPTN>> v_buffer;
	shared_ptr<Buffer<uint32>> i_buffer;
	shared_ptr<ConstantBuffer> constant_buffer;
};

