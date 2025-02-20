#include "pch.h"
#include "CollisionUtils.h"
#include "WorldUtils.h"
#include "Terrain.h"

CollisionUtils::CollisionUtils(
	float size_x, 
	float size_y, 
	float size_z
) : size_x(size_x), size_y(size_y), size_z(size_z)
{
}

CollisionUtils::CollisionUtils(vec3 const& size_xyz)
	: size_x(size_xyz.x), size_y(size_xyz.y), size_z(size_xyz.z)
{
}

static const vector<vec3> calc_move = {
	{-1, 0, 1}, {0, 0, 1}, {1, 0, 1},
	{-1, 0, 0}, {1, 0, 0},
	{-1, 0, -1}, {0, 0, -1}, {1, 0, -1}
};

vec3 CollisionUtils::calcCollisionY(vec3 const& now_pos_down, float move_y, bool* flag)
{
	vec3 ans = now_pos_down;
	ans.y += move_y;
	vec3 check_pos = now_pos_down;
	WorldIndex widx;
	vec3 min_v = ans + vec3(-this->size_x * 0.5f, 0.f, -this->size_z * 0.5f);
	vec3 max_v = ans + vec3(this->size_x * 0.5f, this->size_y, this->size_z * 0.5f);
	vec3 b_min;
	vec3 b_max;
	// y 축 충돌 체크
	if (move_y > 0)
		check_pos = check_pos + vec3(0, this->size_y, 0);
	else if (move_y < 0)
		check_pos = check_pos + vec3(0, -0.001, 0);
	if (move_y) {
		widx = p_terrain->getBlock(check_pos);
		if (widx.flag && widx.block_type && widx.block_type != BlockType::WATER) {
			b_min = widx.pos + vec3(0, 0, -1);
			b_max = widx.pos + vec3(1, 1, 0);
			if (this->detectAABB(min_v, max_v, b_min, b_max)) {
				if (flag == nullptr)
					ans.y = now_pos_down.y;
				else {
					*flag = false;
					ans = now_pos_down;
				}
				return ans;
			}
		}
		for (int i = 0; i < calc_move.size(); i++) {
			widx = p_terrain->getBlock(check_pos + calc_move[i]);
			if (widx.flag && widx.block_type &&
				widx.block_type != BlockType::WATER) {
				b_min = widx.pos + vec3(0, 0, -1);
				b_max = widx.pos + vec3(1, 1, 0);
				if (this->detectAABB(min_v, max_v, b_min, b_max)) {
					if (flag == nullptr)
						ans.y = now_pos_down.y;
					else {
						*flag = false;
						ans = now_pos_down;
					}
					return ans;
				}
			}
		}
	}
	if (flag)
		*flag = true;
	return ans;
}

vec3 CollisionUtils::calcCollisionX(vec3 const& now_pos_down, float move_x)
{
	vec3 ans = now_pos_down;
	ans.x += move_x;
	vec3 check_pos = now_pos_down;
	WorldIndex widx;
	vec3 min_v = ans + vec3(-this->size_x * 0.5f, 0.f, -this->size_z * 0.5f);
	vec3 max_v = ans + vec3(this->size_x * 0.5f, this->size_y, this->size_z * 0.5f);
	vec3 b_min;
	vec3 b_max;
	for (int i = 0; i < calc_move.size(); i++) {
		widx = p_terrain->getBlock(check_pos + calc_move[i]);
		if (widx.flag && widx.block_type &&
			widx.block_type != BlockType::WATER) {
			b_min = widx.pos + vec3(0, 0, -1);
			b_max = widx.pos + vec3(1, 1, 0);
			if (this->detectAABB(min_v, max_v, b_min, b_max)) {
				if (b_max.y - ans.y < 0.4) {
					bool flag;
					ans = this->calcCollisionY(ans, abs(b_max.y - ans.y), &flag);
					if (flag == false)
						return now_pos_down;
					check_pos.y = ans.y;
					for (int j = 0; j < calc_move.size(); j++) {
						widx = p_terrain->getBlock(check_pos + calc_move[j]);
						if (widx.flag && widx.block_type &&
							widx.block_type != BlockType::WATER) {
							b_min = widx.pos + vec3(0, 0, -1);
							b_max = widx.pos + vec3(1, 1, 0);
							if (this->detectAABB(min_v, max_v, b_min, b_max))
								return now_pos_down;
						}
					}
				}
				else
					ans.x = now_pos_down.x;
				return ans;
			}
		}
	}

	check_pos.y = max_v.y - 0.1;
	for (int i = 0; i < calc_move.size(); i++) {
		widx = p_terrain->getBlock(check_pos + calc_move[i]);
		if (widx.flag && widx.block_type &&
			widx.block_type != BlockType::WATER) {
			b_min = widx.pos + vec3(0, 0, -1);
			b_max = widx.pos + vec3(1, 1, 0);
			if (this->detectAABB(min_v, max_v, b_min, b_max)) {
				ans.x = now_pos_down.x;
				break;
			}
		}
	}
	return ans;
}

vec3 CollisionUtils::calcCollisionZ(vec3 const& now_pos_down, float move_z)
{
	vec3 ans = now_pos_down;
	ans.z += move_z;
	vec3 check_pos = now_pos_down;
	WorldIndex widx;
	vec3 min_v = ans + vec3(-this->size_x * 0.5f, 0.f, -this->size_z * 0.5f);
	vec3 max_v = ans + vec3(this->size_x * 0.5f, this->size_y, this->size_z * 0.5f);
	vec3 b_min;
	vec3 b_max;
	for (int i = 0; i < calc_move.size(); i++) {
		widx = p_terrain->getBlock(check_pos + calc_move[i]);
		if (widx.flag && widx.block_type &&
			widx.block_type != BlockType::WATER) {
			b_min = widx.pos + vec3(0, 0, -1);
			b_max = widx.pos + vec3(1, 1, 0);
			if (this->detectAABB(min_v, max_v, b_min, b_max)) {
				if (b_max.y - ans.y < 0.4) {
					bool flag;
					ans = this->calcCollisionY(ans, b_max.y - ans.y, &flag);
					if (flag == false)
						return now_pos_down;
					check_pos.y = ans.y;
					for (int j = 0; j < calc_move.size(); j++) {
						widx = p_terrain->getBlock(check_pos + calc_move[j]);
						if (widx.flag && widx.block_type &&
							widx.block_type != BlockType::WATER) {
							b_min = widx.pos + vec3(0, 0, -1);
							b_max = widx.pos + vec3(1, 1, 0);
							if (this->detectAABB(min_v, max_v, b_min, b_max))
								return now_pos_down;
						}
					}
				}
				else
					ans.z = now_pos_down.z;
				return ans;
			}
		}
	}

	check_pos.y = max_v.y - 0.1;
	for (int i = 0; i < calc_move.size(); i++) {
		widx = p_terrain->getBlock(check_pos + calc_move[i]);
		if (widx.flag && widx.block_type &&
			widx.block_type != BlockType::WATER) {
			b_min = widx.pos + vec3(0, 0, -1);
			b_max = widx.pos + vec3(1, 1, 0);
			if (this->detectAABB(min_v, max_v, b_min, b_max)) {
				ans.z = now_pos_down.z;
				break;
			}
		}
	}
	return ans;
}

bool CollisionUtils::detectAABB(
	vec3 const& min_a,
	vec3 const& max_a,
	vec3 const& min_b,
	vec3 const& max_b
)
{
	return (max_a.x > min_b.x &&
		min_a.x < max_b.x &&
		max_a.y > min_b.y &&
		min_a.y < max_b.y &&
		max_a.z > min_b.z &&
		min_a.z < max_b.z);
}
