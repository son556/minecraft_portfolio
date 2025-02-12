#pragma once

#include "WorldUtils.h"
/*
block idx
u : 윗면, d : 아랫면

 0 -- 1
 | u  |
 3 -- 2

 4 -- 5
 | d  |
 7 -- 6
*/

static const vector<vec3> box = {
	{-0.5, 0.5, 0.5},
	{0.5, 0.5, 0.5},
	{0.5, 0.5, -0.5},
	{-0.5, 0.5, -0.5},

	{-0.5, -0.5, 0.5},
	{0.5, -0.5, 0.5},
	{0.5, -0.5, -0.5},
	{-0.5, -0.5, -0.5},
};

static const vector<vec3> pos = {
	// top
	box[0], box[1], box[2], box[3],

	// bottom
	box[7], box[6], box[5], box[4],

	// right
	box[1], box[5], box[6], box[2],

	// left
	box[0], box[3], box[7], box[4],

	// back
	box[1], box[0], box[4], box[5],

	// front
	box[3], box[2], box[6], box[7]

};

static const vector<vec3> normal = {
	{0, 1, 0}, // 윗면
	{0, -1, 0}, // 아랫면
	{1, 0, 0}, // 오른쪽면
	{-1, 0, 0}, // 왼쪽면
	{0, 0, 1}, // 뒷면
	{0, 0, -1} // 앞면
};

static const float d = 1.f / 8.f;
static const float h_d = 1.f / 16.f;

static const vector<vec2> human_head_uv = { // 시계방향 정렬
	// 위 0 1 2 3
	{d, 0},
	{d * 2, 0},
	{d * 2, d},
	{d, d},

	// 아래 7 6 5 4
	{2 * d, 0},
	{3 * d, 0},
	{3 * d, d},
	{2 * d, d},

	// 오른쪽 1 5 6 2
	{3 * d, d},
	{3 * d, 2 * d},
	{2 * d, 2 * d},
	{2 * d, d},

	// 왼 0 3 7 4
	{0, d},
	{d, d},
	{d, 2 * d},
	{0, 2 * d},

	// 뒤 1 0 4 5
	{3 * d, d},
	{4 * d, d},
	{4 * d, 2 * d},
	{3 * d, 2 * d},

	// 앞 3 2 6 7
	{d, d},
	{2 * d, d},
	{2 * d, 2 * d},
	{d, 2 * d}
};

static const vector<uint32> arr_idx = {
	// 위 0 1 2 3
	0, 1, 2, 0, 2, 3,
	// 아래 7 6 5 4
	4, 5, 6, 4, 6, 7,
	// 오른
	8, 9, 10, 8, 10, 11,
	// 왼 0 3 7 4
	12, 13, 14, 12, 14, 15,
	// 뒤 1 0 4 5
	16, 17, 18, 16, 18, 19,
	// 앞 3 2 6 7
	20, 21, 22, 20, 22, 23
};

namespace Parts {
	inline void humanHead(
		vector<VertexPTN>& vertices, 
		float scale_x,
		float scale_y,
		float scale_z
	) {
		vertices.clear();
		VertexPTN ptn;
		int idx = 0;
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 4; j++) {
				ptn.pos = pos[i * 4 + j];
				ptn.pos.x *= scale_x;
				ptn.pos.y *= scale_y;
				ptn.pos.z *= scale_z;
				ptn.normal = normal[i];
				ptn.uv = human_head_uv[i * 4 + j];
				vertices.push_back(ptn);
			}
		}
	}
	inline void makeIndices(vector<uint32>& indices) {
		indices.clear();
		for (int i = 0; i < 36; i++) {
			indices.push_back(arr_idx[i]);
		}
	}
};

