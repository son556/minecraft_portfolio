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
		return this->center_pos + vec3(0, -this->size_y * 0.5, 0);
	WorldIndex widx;
	vec3 start = this->center_pos + vec3(0, -0.5 * this->size_y, 0);
	vec3 future_pos = start + speed * delta_time * dir;
	
	vec3 new_pos = this->rayMarching(start, future_pos, dir);
	return new_pos;
}

vec3 Collision::intersectionRayAndPlane(
	vec3 const& start, 
	vec3 const& dir, 
	vec3 const& p_pos, 
	vec3 const& p_dir
)
{
	vec3 res;
	float t;
	t = (p_pos.Dot(p_dir) - start.Dot(p_dir)) / p_dir.Dot(dir);
	res = start + t * dir;
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
	vec3 const& check_pos, // 충돌 상자의 가운데 아래
	vec3 const& dir,
	vec3 const& pos
)
{
	// TODO swept aabb 이용하여 시간 + 충돌 위치 구하기
	// 결과 반환
	return pos;
}

bool Collision::detectCollison(vec3 const& pos_down, WorldIndex* w_idx)
{
	static vector<vec3> move_check = {
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

	float volume = 100000;
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
			if (flag) {
				if (w_idx) {
					float v = (widx.pos + vec3(0.5, 0.5, -0.5) - pos_down).Length();
					if (volume > v) {
						*w_idx = widx;
						volume = v;
					}
				}
				return true;
			}
		}
	}
	return false;
}

vec3 Collision::rayMarching(vec3 const& start, vec3 const& end, vec3 const& dir)
{
	// 1. x y z index 찾기
	vec3 ans = start;
	int x, y, z;
	vec3 tmp = ans;
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
	x = static_cast<int>(floor(tmp.x));
	y = static_cast<int>(floor(tmp.y));
	z = static_cast<int>(floor(tmp.z));
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

	vec3 pos = ans + fmin(max_x, max_y, max_z) * dir;
	float len = (end - start).Length();
	float p_len = (pos - start).Length();
	vector<vec3> check_move = {
		{-this->size_x * 0.5f, 0, this->size_z * 0.5f},
		{this->size_x * 0.5f, 0, this->size_z * 0.5f},
		{-this->size_x * 0.5f, 0, -this->size_z * 0.5f},
		{this->size_x * 0.5f, 0, this->size_z * 0.5f},

		{-this->size_x * 0.5f, 1, this->size_z * 0.5f},
		{this->size_x * 0.5f, 1, this->size_z * 0.5f},
		{-this->size_x * 0.5f, 1, -this->size_z * 0.5f},
		{this->size_x * 0.5f, 1, this->size_z * 0.5f}
	};
	p_len = 0;
	while (p_len < len) {
		if (max_x < max_y) {
			if (max_x < max_z) {
				x += step_x;
				max_x += delta_x;
				p_len += delta_x;
			}
			else {
				z += step_z;
				max_z += delta_z;
				p_len += delta_z;
			}
		}
		else {
			if (max_y < max_z) {
				y += step_y;
				max_y += delta_y;
				p_len += delta_y;
			}
			else {
				z += step_z;
				max_z += delta_z;
				p_len += delta_z;
			}
		}
		WorldIndex widx;
		vec3 xyz = vec3(x, y, z);
		widx = p_terrain->getBlock(xyz);
		for (int i = 0; i < check_move.size(); i++) {
			vec3 check = xyz + check_move[i];
			widx = p_terrain->getBlock(check);
			if (widx.flag && widx.block_type && widx.block_type != BlockType::WATER) {
				pos = check - check_move[i];
				return this->rayCheck(check, dir, pos);
			}
		}
	}

	ans = this->calcCollision(ans, dir, len);
	return ans;
}

