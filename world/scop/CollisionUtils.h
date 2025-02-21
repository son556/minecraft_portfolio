#pragma once
class CollisionUtils
{
public:
	CollisionUtils(float size_x, float size_y, float size_z);
	CollisionUtils(vec3 const& size_xyz);
	~CollisionUtils() = default;

	vec3 calcCollisionY(vec3 const& now_pos_down, float move_y, 
		bool* flag = nullptr);
	vec3 calcCollisionX(vec3 const& now_pos_down, float move_x);
	vec3 calcCollisionZ(vec3 const& now_pos_down, float move_z);
	bool detectAABB(
		vec3 const& min_a,
		vec3 const& max_a,
		vec3 const& min_b,
		vec3 const& max_b
	);

public:
	CollisionUtils() = delete;
	CollisionUtils(CollisionUtils const&) = delete;
	CollisionUtils& operator=(CollisionUtils const&) = delete;

private:
	float size_x;
	float size_y;
	float size_z;
};

