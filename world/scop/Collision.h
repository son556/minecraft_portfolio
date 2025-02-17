#pragma once

#include "WorldUtils.h"

/* aabb box index
   0 -- 1
   |	| // up
   3 -- 2

   0 -- 1
   |	| // down
   3 -- 2
*/

struct AABB_Box
{
	vector<vec3> up = { 
		{ -0.5, 0.5, 0.5 },
		{ 0.5, 0.5, 0.5 },
		{ 0.5, 0.5, -0.5 },
		{ -0.5, 0.5, -0.5 } 
	};

	vector<vec3> mid = {
		{ -0.5, 0, 0.5 },
		{ 0.5, 0, 0.5 },
		{ 0.5, 0, -0.5 },
		{ -0.5, 0, -0.5 }
	};

	vector<vec3> down = { 
		{ -0.5, -0.5, 0.5 },
		{ 0.5, -0.5, 0.5 },
		{ 0.5, -0.5, -0.5 },
		{ -0.5, -0.5, -0.5 } 
	};
};


class Collision
{
public:
	Collision(vec3 const& pos, vec3 size_xyz);
	~Collision() = default;
	void update(vec3 const& new_pos);
	vec3 checkCollision(vec3 const& dir, float speed);

public: // test
	Collision() = delete;
	Collision(Collision const&) = delete;
	Collision& operator=(Collision const&) = delete;
	vec3 intersectionRayAndPlane(
		vec3 const& r_pos,
		vec3 const& r_dir,
		vec3 const& p_pos,
		vec3 const& p_dir
	);

	/**
	 * @brief 물체의 이동방향과 처음위치를 계산해 최초 충돌 후 방향을 반환합니다.
	 * 
	 * \param start
	 * \param end
	 * \param dir
	 * \return 충돌 후 이동방향
	 */
	vec3 rayMarching(vec3 const& start, vec3 const& end, vec3 const& dir);
	float fmin(float x, float y, float z);

	/**
	 * @brief 충돌 후 방향 계산.
	 * 
	 * \param pos 블록의 아래 위치
	 * \param dir 이동 방향
	 * \return 
	 */
	vec3 calcCollision(vec3 const& down_pos, vec3 const& dir);

	
	vec3 rayCheck(
		vec3 const& ray_start,
		vector<float>& move,
		vec3 const& dir
	);

	/**
	 * @brief 현재 위치에서 주변 블록과의 충돌감지를 합니다.
	 * 
	 * \param pos_down 충돌 상자의 밑면 중앙 pos
	 * \return 충돌시 true를 반환
	 */
	bool detectCollison(vec3 const& pos_down);
private:
	AABB_Box aabb_box;
	vec3 center_pos;
	float size_x;
	float size_y;
	float size_z;
};

