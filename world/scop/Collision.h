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

private:
	Collision() = delete;
	Collision(Collision const&) = delete;
	Collision& operator=(Collision const&) = delete;
	vec3 intersectionRayAndPlane(
		vec3 const& r_pos,
		vec3 const& r_dir,
		vec3 const& p_pos,
		vec3 const& p_dir
	);
	vec3 rayMarching(vec3 const& start, vec3 const& end, vec3 const& dir);

private:


private:
	AABB_Box aabb_box;
	vec3 center_pos;
};

