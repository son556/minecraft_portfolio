#pragma once

template<typename T> class Buffer;
class ConstantBuffer;
struct VertexPTN;

class Body
{
public:
	Body(Mat const& o_pos, Mat const& o_rot);
	~Body() = default;
	shared_ptr<Buffer<VertexPTN>>& getVertexBuffer();
	Mat getWorld();

private:
	Body() = delete;
	Body(Body const&) = delete;
	Body& operator=(Body const&) = delete;

private:
	Mat ori_pos;
	Mat ori_rot;
	Mat world;
	Mat basic_mat;

private:
	shared_ptr<Buffer<VertexPTN>> v_buffer;
};

