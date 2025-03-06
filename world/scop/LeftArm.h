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
	void updateAnimation(bool animation_type);
	void update(Mat const& o_pos, Mat const& o_rot);

private:
	void animation();
	LeftArm() = delete;
	LeftArm(LeftArm const&) = delete;
	LeftArm& operator=(LeftArm const&) = delete;

private:
	Mat world;
	Mat basic_mat;
	bool animation_flag;

private:
	vector<VertexPTN> vertices;
	vector<VertexPTN> ani_vertices;
	shared_ptr<Buffer<VertexPTN>> v_buffer;
};

