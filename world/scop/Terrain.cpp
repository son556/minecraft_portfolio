#include "pch.h"
#include "Terrain.h"
#include "Chunk.h"
#include "Graphics.h"
#include "Block.h"
#include "TestCam.h"
#include "DeferredGraphics.h"

Terrain::Terrain(
	int size_w,
	int size_h,
	HWND hwnd,
	UINT width,
	UINT height,
	int fov_chunk,
	int thread_cnt
)
{
	this->m_manager = make_shared<Map>(
		size_w,
		size_h,
		fov_chunk,
		thread_cnt,
		hwnd,
		width,
		height
	);
}

Terrain::~Terrain()
{
}

void Terrain::Render()
{
	this->m_manager->r_system.Render();
}

void Terrain::putBlock(
	vec3 const& ray_pos,
	vec3 const& ray_dir,
	int type
)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		WorldIndex add_idx;
		Index2& cidx = widx.c_idx;
		Index3& bidx = widx.b_idx;
		vec3 const& pos = widx.pos;
		if (widx.dir == 0) {
			if (ray_pos.y > pos.y && pos.y + 1 < 256) {
				add_idx = this->m_manager->
					m_info.getBlockIndex(pos.x, pos.y + 1, pos.z);
			}
			else if (ray_pos.y < pos.y && pos.y - 1 > -1) {
				add_idx = this->
					m_manager->m_info.getBlockIndex(pos.x, pos.y - 1, pos.z);
			}
		}
		else if (widx.dir == 1) {
			if (ray_pos.z < pos.z) {
				add_idx = this->m_manager->
					m_info.getBlockIndex(pos.x, pos.y, pos.z - 1);
			}
			else {
				add_idx = this->m_manager->
					m_info.getBlockIndex(pos.x, pos.y, pos.z + 1);
			}
		}
		else {
			if (ray_pos.x < pos.x) {
				add_idx = this->
					m_manager->m_info.getBlockIndex(pos.x - 1, pos.y, pos.z);
			}
			else {
				add_idx = this->
					m_manager->m_info.getBlockIndex(pos.x + 1, pos.y, pos.z);
			}
		}
		if (add_idx.flag) {
			vector<Index2> v_idx;
			cidx = add_idx.c_idx;
			bidx = add_idx.b_idx;
			Index2 adj_idx;
			Index2 const& cpos = 
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->chunk_pos;
			
			if (type < 0) {
				this->m_manager->m_info.addBlock(cidx, bidx, type);
				Chunk& chunk = *(this->m_manager->m_info.chunks[cidx.y][cidx.x]);
				chunk.tp_block_cnt += 1;
				vector<VertexColor>& u_vertices = chunk.tp_chunk.vertices_up;
				vector<uint32>& u_indices = chunk.tp_chunk.indices_up;
				uint32& u_idx = chunk.tp_chunk.vertices_idx_up;

				vector<VertexColor>& d_vertices = chunk.tp_chunk.vertices_down;
				vector<uint32>& d_indices = chunk.tp_chunk.indices_down;
				uint32& d_idx = chunk.tp_chunk.vertices_idx_down;
				int16& max_h = chunk.max_h;
				max_h = max(max_h, bidx.y + 1);
				vec4 col;
				if (type == -3)
					col = vec4(1, 0, 0, 0.3);
				if (bidx.y >= WATER_HEIGHT) {
					for (int i = 0; i < 6; i++) {
						Block::addBlockFacePosAndCol(
							chunk.start_pos, bidx.x, bidx.y, bidx.z, i,
							type, u_vertices);
						Block::addBlockFaceIndices(u_idx, u_indices);
						u_idx += 4;
					}
					chunk.tp_chunk.update(d_graphic->getDevice(), true);
				}
				else {
					for (int i = 0; i < 6; i++) {
						Block::addBlockFacePosAndCol(
							chunk.start_pos, bidx.x, bidx.y, bidx.z, i,
							type, d_vertices);
						Block::addBlockFaceIndices(d_idx, d_indices);
						d_idx += 4;
					}
					chunk.tp_chunk.update(d_graphic->getDevice(), false);
				}
				return;
			}
			
			if (bidx.x == 0) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x - 16, cpos.y);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, 15, bidx.y, bidx.z)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.x == 15) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x + 16, cpos.y);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, 0, bidx.y, bidx.z)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.z == 15) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x, cpos.y - 16);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, bidx.x, bidx.y, 0)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			if (bidx.z == 0) {
				adj_idx = this->m_manager->m_info.findChunkIndex(cpos.x, cpos.y + 16);
				if (adj_idx.flag && this->m_manager->m_info.findBlock(adj_idx, bidx.x, bidx.y, 15)) {
					this->m_manager->m_info.chunks[adj_idx.y][adj_idx.x]->vertices_idx = 0;
					v_idx.push_back(adj_idx);
				}
			}
			this->m_manager->m_info.addBlock(cidx, bidx, type);
			this->m_manager->m_info.setLight(cidx, bidx, 0);
			this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
			int16& max_h = this->m_manager->m_info.chunks[cidx.y][cidx.x]->max_h;
			max_h = max(max_h, bidx.y + 1);
			this->m_manager->l_system.chunkSetLight(cidx);
			v_idx.push_back(cidx);
			this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
		}
	}
}

void Terrain::deleteBlock(vec3 const& ray_pos, vec3 const& ray_dir)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		int type = this->m_manager->m_info.findBlock(widx.c_idx, widx.b_idx);
		this->m_manager->m_info.addBlock(widx.c_idx, widx.b_idx, 0);
		int16& max_h = 
			this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->max_h;
		max_h = max(max_h, widx.b_idx.y - 1);
		
		if (type < 0) { // 반투명인 경우
			Chunk& chunk = 
				*(this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]);
			chunk.tp_block_cnt -= 1;
			chunk.tp_chunk.reset();
			this->m_manager->vertexAndIndexGeneratorTP(widx.c_idx);
			return;
		}
		this->m_manager->l_system.chunkSetLight(widx.c_idx);
		this->m_manager->
			m_info.chunks[widx.c_idx.y][widx.c_idx.x]->vertices_idx = 0;
		vector<Index2> v_idx;
		v_idx.push_back(widx.c_idx);
		Index2 cidx;
		Index2 const& pos = this->m_manager->m_info.chunks[widx.c_idx.y][widx.c_idx.x]->chunk_pos;
		if (widx.b_idx.x == 0) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x - 16, pos.y);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.x == 15) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x + 16, pos.y);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.z == 0) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x, pos.y + 16);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		if (widx.b_idx.z == 15) {
			cidx = this->m_manager->m_info.findChunkIndex(pos.x, pos.y - 16);
			if (cidx.flag) {
				this->m_manager->m_info.chunks[cidx.y][cidx.x]->vertices_idx = 0;
				v_idx.push_back(cidx);
			}
		}
		this->m_manager->chunksSetVerticesAndIndices(v_idx, 0, v_idx.size());
	}
}

WorldIndex Terrain::getBlock(float x, float y, float z)
{
	return this->m_manager->m_info.getBlockIndex(x, y, z);
}

WorldIndex Terrain::getBlock(vec3 world_pos)
{
	return this->m_manager->m_info.getBlockIndex(world_pos.x, world_pos.y, world_pos.z);
}

void Terrain::setSightChunk(int cnt)
{
	int max_fov = min(this->m_manager->m_info.size_h - 1, 
		this->m_manager->m_info.size_w - 1);
	this->m_manager->setSightChunk(min(max_fov, cnt));
}

void Terrain::userPositionCheck(float x, float z)
{
	this->m_manager->userPositionCheck(x, z);
}

int16 Terrain::getHeight(float x, float z) const
{
	WorldIndex w_idx = m_manager->m_info.getBlockIndex(x, 0, z);
	return m_manager->m_info.findHeight(w_idx.c_idx, w_idx.b_idx);
}

void Terrain::testClickLightBlock(
	vec3 const& ray_pos,
	vec3 const& ray_dir)
{
	WorldIndex widx = this->m_manager->m_info.pickBlock(ray_pos, ray_dir);
	if (widx.flag) {
		Index2& cidx = widx.c_idx;
		Index3& bidx = widx.b_idx;
		cout << "block type: " << widx.block_type << endl;
		cout << "chunk idx: " << cidx.y << ' ' << cidx.x << endl;
		cout << "block idx: " << bidx.x << ' ' << bidx.y << ' ' << bidx.z << endl << endl;
	}
}
