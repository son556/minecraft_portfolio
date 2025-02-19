#include "pch.h"
#include "Collision.h"
#include "Terrain.h"

#define STDZERO 0.00000001

Collision::Collision(vec3 const& pos, vec3 size_xyz) 
	: c_utils(size_xyz), center_pos(pos), size_x(size_xyz.x), 
	size_y(size_xyz.y), size_z(size_xyz.z)
{
}

void Collision::update(vec3 const& new_pos)
{
	this->center_pos = new_pos;
}

vec3 Collision::checkCollision(vec3 const& dir, float speed)
{
	if (dir.x == 0 && dir.y == 0 && dir.z == 0)
		return dir;
	WorldIndex widx;
	vec3 future_pos = this->center_pos + speed * delta_time * dir;
	
	vec3 new_dir = this->rayMarching(this->center_pos, future_pos, dir);
	return new_dir;
}

vec3 Collision::intersectionRayAndPlane(
	vec3 const& r_pos, 
	vec3 const& r_dir, 
	vec3 const& p_pos, 
	vec3 const& p_dir
)
{
	vec3 res;
	float t;
	t = (p_pos.Dot(p_dir) - r_pos.Dot(p_dir)) / p_dir.Dot(r_dir);
	res = r_pos + t * r_dir;
	return res;
}

float Collision::fmin(float x, float y, float z) {
	float ans = min(x, y);
	ans = min(ans, z);
	return ans;
}

vec3 Collision::calcCollision(
	vec3 const& down_pos, 
	vec3 const& dir,
	float distance
)
{
	vec3 ans;
	ans = this->c_utils.calcCollisionY(down_pos, dir.y * distance);
	if (abs(dir.z) > abs(dir.x)) {
		ans = this->c_utils.calcCollisionZ(ans, dir.z * distance);
		ans = this->c_utils.calcCollisionX(ans, dir.x * distance);
	}
	else {
		ans = this->c_utils.calcCollisionX(ans, dir.x * distance);
		ans = this->c_utils.calcCollisionZ(ans, dir.z * distance);
	}
	return ans;
}

vec3 Collision::rayCheck(
	vec3 const& ray_start, 
	vector<float>& move, 
	vec3 const& dir
)
{
	bool res;
	bool prev_res;
	vec3 check_pos = ray_start + vec3(0, -this->size_y * 0.5, 0);
	vec3 prev_pos = check_pos;
	for (int i = 0; i < move.size(); i++) {
		check_pos += dir * move[i];
		if (this->detectCollison(check_pos)) {
			return this->calcCollision(prev_pos, dir, move[i]);
		}
		prev_pos = check_pos;
	}
	return dir;
}

bool Collision::detectCollison(vec3 const& pos_down)
{
	static vector<vec3> move_check = {
		// 내 주변
		{-1, 0, 1}, {0, 0, 1}, {1, 0, 1},
		{-1, 0, 0}, {0, 0, 0}, {1, 0, 0},
		{-1, 0, -1}, {0, 0, -1}, {1, 0, -1},

		{-1, 1, 1}, {0, 1, 1}, {1, 1, 1},
		{-1, 1, 0}, {0, 1, 0}, {1, 1, 0},
		{-1, 1, -1}, {0, 1, -1}, {1, 1, -1},

		{-1, -1, 1}, {0, -1, 1}, {1, -1, 1},
		{-1, -1, 0}, {0, -1, 0}, {1, -1, 0},
		{-1, -1, -1}, {0, -1, -1}, {1, -1, -1},

		{-1, 2, 1}, {0, 2, 1}, {1, 2, 1},
		{-1, 2, 0}, {0, 2, 0}, {1, 2, 0},
		{-1, 2, -1}, {0, 2, -1}, {1, 2, -1}
	};
	vec3 pos;
	WorldIndex widx;
	vec3 min_v = pos_down + vec3(-this->size_x * 0.5f, 0.f, -this->size_z * 0.5f);
	vec3 max_v = pos_down + vec3(this->size_x * 0.5f, this->size_y, this->size_z * 0.5f);

	for (int i = 0; i < move_check.size(); i++) {
		pos = pos_down + move_check[i];
		widx = p_terrain->getBlock(pos);
		if (widx.flag && widx.block_type && widx.block_type != BlockType::WATER) {
			vec3 b_min = widx.pos + vec3(0, 0, -1);
			vec3 b_max = widx.pos + vec3(1, 1, 0);
			bool flag = max_v.x > b_min.x &&
				min_v.x < b_max.x &&
				max_v.y > b_min.y &&
				min_v.y < b_max.y &&
				max_v.z > b_min.z &&
				min_v.z < b_max.z;
			if (flag)
				return true;
		}
	}
	return false;
}

vec3 Collision::rayMarching(vec3 const& start, vec3 const& end, vec3 const& dir)
{
	vec3 ans = dir;
	float x, y, z;
	vec3 tmp = start;
	vector<float> moves;
	if (start.y >= 256) {
		if (dir.y >= 0)
			return ans;
		tmp = this->intersectionRayAndPlane(
			start,
			dir,
			vec3(start.x, 256, start.z),
			vec3(0, 1, 0)
		);
	}
	else if (start.y < 0) {
		if (dir.y <= 0)
			return ans;
		tmp = this->intersectionRayAndPlane(
			start,
			dir,
			vec3(start.x, 0, start.z),
			vec3(0, 1, 0)
		);
	}
	x = floor(tmp.x);
	y = floor(tmp.y);
	z = floor(tmp.z);
	if (z < tmp.z)
		z += 1;
	int step_x = 1;
	int step_y = 1;
	int step_z = 1;
	if (dir.x < 0)
		step_x = -1;
	if (dir.y < 0)
		step_y = -1;
	if (dir.z < 0)
		step_z = -1;

	// 2 max x y z, delta x y z 구하기
	float max_x, max_y, max_z;
	float delta_x = 0, delta_y = 0, delta_z = 0;
	bool flag_x = true, flag_y = true, flag_z = true;
	int sx, sy, sz;
	vec3 first, second;
	sx = x;
	if (dir.x > 0)
		sx += 1;
	max_x = dir.Dot(vec3(1, 0, 0));
	if (max_x <= STDZERO && max_x >= -STDZERO)
		flag_x = false;
	if (flag_x) {
		first = this->intersectionRayAndPlane(
			start,
			dir,
			vec3(sx, 0, 0),
			vec3(1, 0, 0)
		);
		second = this->intersectionRayAndPlane(
			start,
			dir,
			vec3(sx + step_x, 0, 0),
			vec3(1, 0, 0)
		);
		max_x = (start - first).Length();
		delta_x = (first - second).Length();
	}
	sy = y;
	if (dir.y > 0)
		sy += 1;
	max_y = dir.Dot(vec3(0, 1, 0));
	if (max_y <= STDZERO && max_y >= -STDZERO)
		flag_y = false;
	if (flag_y) {
		first = this->intersectionRayAndPlane(
			start,
			dir,
			vec3(0, sy, 0),
			vec3(0, 1, 0)
		);
		second = this->intersectionRayAndPlane(
			start,
			dir,
			vec3(0, sy + step_y, 0),
			vec3(0, 1, 0)
		);
		max_y = (start - first).Length();
		delta_y = (first - second).Length();
	}
	sz = z;
	if (dir.z < 0)
		sz -= 1;
	max_z = dir.Dot(vec3(0, 0, 1));
	if (max_z <= STDZERO && max_z >= -STDZERO)
		flag_z = false;
	if (flag_z) {
		first = this->intersectionRayAndPlane(
			start,
			dir,
			vec3(0, 0, sz),
			vec3(0, 0, 1)
		);
		second = this->intersectionRayAndPlane(
			start,
			dir,
			vec3(0, 0, sz + step_z),
			vec3(0, 0, 1)
		);
		max_z = (start - first).Length();
		delta_z = (first - second).Length();
	}
	if (flag_x == false)
		max_x = INT_MAX;
	if (flag_y == false)
		max_y = INT_MAX;
	if (flag_z == false)
		max_z = INT_MAX;

	float f_len = (end - start).Length();
	vec3 pos = start + fmin(max_x, max_y, max_z) * dir;
	vec3 ray_start = pos;
	float len = (pos - start).Length();
	WorldIndex widx = p_terrain->getBlock(pos);
	if (widx.flag && len < f_len) {
		if (widx.block_type && widx.block_type != BlockType::WATER)
			return this->calcCollision(start + vec3(0, -this->size_y * 0.5, 0), dir, 
				fmin(max_x, max_y, max_z));
	}
	while (len < f_len) {
		if (max_x < max_y) {
			if (max_x < max_z) {
				x += step_x;
				max_x += delta_x;
				pos += delta_x * dir;
				moves.push_back(delta_x);
			}
			else {
				z += step_z;
				max_z += delta_z;
				pos += delta_z * dir;
				moves.push_back(delta_z);
			}
		}
		else {
			if (max_y < max_z) {
				y += step_y;
				max_y += delta_y;
				pos += delta_y * dir;
				moves.push_back(delta_y);
			}
			else {
				z += step_z;
				max_z += delta_z;
				pos += delta_z * dir;
				moves.push_back(delta_z);
			}
		}
		len = (pos - start).Length();
		WorldIndex widx = p_terrain->getBlock(x, y, z);
		if (widx.flag && len < f_len) {
			if (widx.block_type && widx.block_type != BlockType::WATER) {
				return this->rayCheck(ray_start, moves, dir);
			}
		}
	}
	vec3 ep = end + vec3(0, -this->size_y * 0.5, 0);
	bool res = this->detectCollison(ep);
	if (res) {
		return this->calcCollision(start + vec3(0, -this->size_y * 0.5, 0), dir, 
			(end - start).Length());
	}
	return ans;
}

