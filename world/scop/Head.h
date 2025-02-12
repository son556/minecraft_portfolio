#pragma once

template<typename T> class Buffer;
class ConstantBuffer;
struct VertexPTN;

class Head
{
public:
	Head(Mat const& o_pos, Mat const& o_rot);
	~Head() = default;
	shared_ptr<Buffer<VertexPTN>>& getVertexBuffer();
	Mat getWorld();

private:
	Head() = delete;
	Head(Head const&) = delete;
	Head& operator=(Head const&) = delete;

private:
	Mat ori_pos;
	Mat ori_rot;
	Mat world;
	Mat basic_mat;

private:
	shared_ptr<Buffer<VertexPTN>> v_buffer;
};

