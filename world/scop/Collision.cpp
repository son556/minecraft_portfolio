#include "pch.h"
#include "Collision.h"
#include "Terrain.h"
#include "SweptAABB.h"

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
	
	vec3 new_pos = this->rayMarching(start, future_pos, dir, speed);
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

bool Collision::detectCollison(
	vec3 const& pos_down,
	vec3 const& dir,
	vector<WorldIndex>* blocks)
{
	static const vector<vec3> move_check = {
		{-1, 0, 1}, {0, 0, 1}, {1, 0, 1},
		{-1, 0, 0}, {0, 0, 0}, {1, 0, 0},
		{-1, 0, -1}, {0, 0, -1}, {1, 0, -1},

		{-1, 1, 1}, {0, 1, 1}, {1, 1, 1},
		{-1, 1, 0}, {0, 1, 0}, {1, 1, 0},
		{-1, 1, -1}, {0, 1, -1}, {1, 1, -1}
	};
	static const vector<vec3> move_check_up = {
		{-1, 2, 1}, {0, 2, 1}, {1, 2, 1},
		{-1, 2, 0}, {0, 2, 0}, {1, 2, 0},
		{-1, 2, -1}, {0, 2, -1}, {1, 2, -1}
	};
	static const vector<vec3> move_check_down = {
		{-1, -1, 1}, {0, -1, 1}, {1, -1, 1},
		{-1, -1, 0}, {0, -1, 0}, {1, -1, 0},
		{-1, -1, -1}, {0, -1, -1}, {1, -1, -1}
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
				if (blocks)
					blocks->push_back(widx);
				return true;
			}
		}
	}
	if (dir.y > 0) {
		for (int i = 0; i < move_check_up.size(); i++) {
			pos = pos_down + move_check_up[i];
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
					if (blocks)
						blocks->push_back(widx);
					return true;
				}
			}
		}
	}
	if (dir.y < 0) {
		for (int i = 0; i < move_check_down.size(); i++) {
			pos = pos_down + move_check_down[i];
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
					if (blocks)
						blocks->push_back(widx);
					return true;
				}
			}
		}
	}
	return false;
}

vec3 Collision::findFirstCollision(
	vec3 const& start,
	vec3 const& end_pos, 
	vector<WorldIndex>& blocks, 
	vec3 const& velocity,
	vec3 const& dir
)
{
	Box obj, other;
	obj.top_left_up = start + vec3(-this->size_x * 0.5, this->size_y, this->size_z * 0.5);
	obj.size_x = this->size_x;
	obj.size_y = this->size_y;
	obj.size_z = this->size_z;
	obj.velocity = velocity;

	other.size_x = 1;
	other.size_y = 1;
	other.size_z = 1;

	float m_time = 1;
	float tmp_time = 0;
	int idx = 0;
	for (int i = 0; i < blocks.size(); i++) {
		other.top_left_up = blocks[i].pos + vec3(0, 1, 0);
		tmp_time = SweptAABB::sweptAABB(obj, other);
		if (m_time > tmp_time) {
			idx = i;
			m_time = tmp_time;
		}
	}

	vec3 np = start + velocity * m_time;
	blocks.clear();
	if (this->detectCollison(np, dir, &blocks)) {
		while (this->detectCollison(np, dir, &blocks)) {
			for (int i = 0; i < blocks.size(); i++) {
				other.top_left_up = blocks[i].pos + vec3(0, 1, 0);
				tmp_time = SweptAABB::sweptAABB(obj, other);
				if (m_time > tmp_time) {
					idx = i;
					m_time = tmp_time;
				}
			}
			np = start + velocity * m_time;
			blocks.erase(blocks.begin() + idx);
			if (blocks.size() == 0)
				break;
		}
		return np;
	}

	float len = (end_pos - np).Length();
	vec3 res = this->calcCollision(np, dir, len);
	return res;
}


vec3 selectPos(vec3 const& r_start, vec3 const& x, vec3 const& y, vec3 const& z) {
	float l_x = (x - r_start).Length();
	float l_y = (y - r_start).Length();
	float l_z = (z - r_start).Length();
	vec3 res;
	if (l_x < l_y) {
		if (l_x < l_z)
			res = x;
		else
			res = z;
	}
	else {
		if (l_y < l_z)
			res = y;
		else
			res = z;
	}
	return res;
}

vec3 Collision::rayMarching(
	vec3 const& start, 
	vec3 const& end, 
	vec3 const& dir,
	float speed
)
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
	vector<WorldIndex> blocks;
	vec3 v = dir * speed;
	vec3 pos_x = ans + dir * max_x;
	vec3 pos_y = ans + dir * max_y;
	vec3 pos_z = ans + dir * max_z;
	while (p_len < len && pos.y < 256 && pos.y >= 0) {
		if (max_x < max_y) {
			if (max_x < max_z) {
				x += step_x;
				max_x += delta_x;
				pos_x += delta_x * dir;
			}
			else {
				z += step_z;
				max_z += delta_z;
				pos_z += delta_z * dir;
			}
		}
		else {
			if (max_y < max_z) {
				y += step_y;
				max_y += delta_y;
				pos_y += delta_y * dir;
			}
			else {
				z += step_z;
				max_z += delta_z;
				pos_z += delta_z * dir;
			}
		}

		p_len = (pos - start).Length();
		blocks.clear();
		if (p_len < len && this->detectCollison(pos, dir, &blocks)) {
			return this->findFirstCollision(start, pos, blocks, v, dir);
		}
		pos = selectPos(ans, pos_x, pos_y, pos_z);
	}

	blocks.clear();
	if (this->detectCollison(end, dir, &blocks)) {
		return this->findFirstCollision(start, end, blocks, v, dir);
	}
	ans = this->calcCollision(start, dir, len);
	return ans;
}

