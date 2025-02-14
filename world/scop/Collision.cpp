#include "pch.h"
#include "Collision.h"
#include "Terrain.h"

Collision::Collision(vec3 const& pos, vec3 size_xyz)
{
	Mat move = Mat::CreateTranslation(pos);
	center_pos = pos;
	for (int i = 0; i < 4; i++) {
		vec4 up_v = 
			vec4(this->aabb_box.up[i].x, this->aabb_box.up[i].y, 
				this->aabb_box.up[i].z, 1);
		vec4 mid_v = 
			vec4(this->aabb_box.mid[i].x, this->aabb_box.mid[i].y,
				this->aabb_box.mid[i].z, 1);
		vec4 down_v =
			vec4(this->aabb_box.down[i].x, this->aabb_box.down[i].y,
				this->aabb_box.down[i].z, 1);
		
		up_v.x *= size_xyz.x;
		up_v.y *= size_xyz.y;
		up_v.z *= size_xyz.z;

		mid_v.x *= size_xyz.x;
		mid_v.y *= size_xyz.y;
		mid_v.z *= size_xyz.z;
		
		down_v.x *= size_xyz.x;
		down_v.y *= size_xyz.y;
		down_v.z *= size_xyz.z;

		up_v = XMVector4Transform(up_v, move);
		mid_v = XMVector4Transform(mid_v, move);
		down_v = XMVector4Transform(down_v, move);
		this->aabb_box.up[i] = vec3(up_v.x, up_v.y, up_v.z);
		this->aabb_box.mid[i] = vec3(mid_v.x, mid_v.y, mid_v.z);
		this->aabb_box.down[i] = vec3(down_v.x, down_v.y, down_v.z);
	}
}

void Collision::update(vec3 const& new_pos)
{
	Mat move = Mat::CreateTranslation(new_pos - this->center_pos);
	this->center_pos = new_pos;
	for (int i = 0; i < 4; i++) {
		vec4 up_v =
			vec4(this->aabb_box.up[i].x, this->aabb_box.up[i].y,
				this->aabb_box.up[i].z, 1);
		vec4 mid_v =
			vec4(this->aabb_box.mid[i].x, this->aabb_box.mid[i].y,
				this->aabb_box.mid[i].z, 1);
		vec4 down_v =
			vec4(this->aabb_box.down[i].x, this->aabb_box.down[i].y,
				this->aabb_box.down[i].z, 1);

		up_v = XMVector4Transform(up_v, move);
		mid_v = XMVector4Transform(mid_v, move);
		down_v = XMVector4Transform(down_v, move);
		this->aabb_box.up[i] = vec3(up_v.x, up_v.y, up_v.z);
		this->aabb_box.mid[i] = vec3(mid_v.x, mid_v.y, mid_v.z);
		this->aabb_box.down[i] = vec3(down_v.x, down_v.y, down_v.z);
	}
}

vec3 Collision::checkCollision(vec3 const& dir, float speed)
{
	vec3 new_dir = dir;
	WorldIndex widx;
	vec3 future_pos = this->center_pos + speed * delta_time * dir;
	vec3 p = vec3(future_pos.x, this->aabb_box.down[0].y, future_pos.z);
	widx = p_terrain->getBlock(p); // 캐릭터가 있는 부분을 기준으로 아래 block 활성화

	// 미래의 충돌 박스 만들기
	Mat move = Mat::CreateTranslation(future_pos - this->center_pos);
	AABB_Box f_box = this->aabb_box;
	for (int i = 0; i < 4; i++) {
		vec4 up_v =
			vec4(f_box.up[i].x, f_box.up[i].y,
				f_box.up[i].z, 1);
		vec4 mid_v =
			vec4(f_box.mid[i].x, f_box.mid[i].y,
				f_box.mid[i].z, 1);
		vec4 down_v =
			vec4(f_box.down[i].x, f_box.down[i].y,
				f_box.down[i].z, 1);

		up_v = XMVector4Transform(up_v, move);
		mid_v = XMVector4Transform(mid_v, move);
		down_v = XMVector4Transform(down_v, move);
		f_box.up[i] = vec3(up_v.x, up_v.y, up_v.z);
		f_box.mid[i] = vec3(mid_v.x, mid_v.y, mid_v.z);
		f_box.down[i] = vec3(down_v.x, down_v.y, down_v.z);
	}

	// 검사해야 할 block들 찾기
	vec3 min_v = widx.pos - vec3(0, 0, 1);
	vec3 max_v = widx.pos + vec3(1, 0, 0);
	vec3 center = (min_v + max_v) * 0.5;
	vector<WorldIndex> colliable_block;
	if (future_pos.x > center.x) {
		colliable_block.push_back(p_terrain->getBlock(center + vec3(1, 0, 0)));
		if (future_pos.y > center.y) {
			colliable_block.push_back(p_terrain->getBlock(center + vec3(0, 0, 1)));
			colliable_block.push_back(p_terrain->getBlock(center + vec3(1, 0, 1)));
		}
	} 

	return new_dir;
}

