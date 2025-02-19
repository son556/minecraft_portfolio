#include "pch.h"
#include "Character.h"
#include "DeferredGraphics.h"
#include "Texture.h"
#include "LeftArm.h"
#include "LeftLeg.h"
#include "RightArm.h"
#include "RightLeg.h"
#include "Body.h"
#include "Head.h"
#include "Parts.h"
#include "Buffer.h"
#include "ConstantBuffer.h"
#include "TestCam.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "SamplerState.h"
#include "InputLayout.h"
#include "InputLayouts.h"
#include "Collision.h"

Character::Character(Mat pos, Mat rot) : pos(pos), rot(rot)
{
	ComPtr<ID3D11Device> device = d_graphic->getDevice();
	this->skin_tex = make_shared<Texture>(
		device,
		L"./textures/zombie/TEXTURE/zombie.png"
	);
	this->c_pos = vec3(pos._41, pos._42, pos._43);
	cout << "cpos: " << this->c_pos.x << ' ' << this->c_pos.y << ' ' << c_pos.z << endl;
	vec4 d = XMVector4Transform(vec4(0, 0, -1, 0), rot);
	this->dir = XMVector3Normalize(vec3(d.x, d.y, d.z));
	this->left_arm = make_shared<LeftArm>(pos, rot);
	this->right_arm = make_shared<RightArm>(pos, rot);
	this->left_leg = make_shared<LeftLeg>(pos, rot);
	this->right_leg = make_shared<RightLeg>(pos, rot);
	this->body = make_shared<Body>(pos, rot);
	this->head = make_shared<Head>(pos, rot);
	this->aabb_collision = make_shared<Collision>(this->c_pos + vec3(0, 1, 0), 
		vec3(0.5, 2, 0.5));
	this->constant_buffer = make_shared<ConstantBuffer>(
		device,
		d_graphic->getContext(),
		this->mvp
	);
	vec4 tmp;
	this->constant_buffer_water = make_shared<ConstantBuffer>(
		device,
		d_graphic->getContext(),
		tmp
	);
	vector<uint32> indices;
	Parts::makeIndices(indices);
	this->i_buffer = make_shared<Buffer<uint32>>(
		device,
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
	this->vertex_shader = make_shared<VertexShader>(
		device,
		L"CharacterVS.hlsl",
		"main",
		"vs_5_0"
	);
	this->input_layout = make_shared<InputLayout>(
		device,
		InputLayouts::layout_ptn.data(),
		InputLayouts::layout_ptn.size(),
		this->vertex_shader->getBlob()
	);
	this->pixel_shader = make_shared<PixelShader>(
		device,
		L"CharacterPS.hlsl",
		"main",
		"ps_5_0"
	);
	this->sampler_state = make_shared<SamplerState>(device);
}

void Character::render(
	CamType type,
	ComPtr<ID3D11ShaderResourceView> depth_srv,
	shared_ptr<SamplerState> sampler_tp,
	bool water_up_flag
)
{
	this->mvp.view = cam->getMVP(type).view.Transpose();
	this->mvp.proj = cam->getMVP(type).proj.Transpose();
	vec4 tmp = vec4(water_up_flag, WATER_HEIGHT, 0, 0);
	this->constant_buffer_water->update(tmp);
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	context->PSSetConstantBuffers(0, 1,
		this->constant_buffer_water->getComPtr().GetAddressOf());
	context->PSSetShaderResources(1, 1, depth_srv.GetAddressOf());
	context->PSSetSamplers(1, 1, sampler_tp->getComPtr().GetAddressOf());
	this->setPipe(context);
	this->renderHead(context);
	this->renderBody(context);
	this->renderLeftArm(context);
	this->renderRightArm(context);
	this->renderLeftLeg(context);
	this->renderRightLeg(context);
}

void Character::renderHead(ComPtr<ID3D11DeviceContext>& context)
{
	this->mvp.model = this->head->getWorld().Transpose();
	this->constant_buffer->update(this->mvp);
	context->VSSetConstantBuffers(0, 1,
		this->constant_buffer->getComPtr().GetAddressOf());
	uint32 stride;
	uint32 offset;
	stride = this->head->getVertexBuffer()->getStride();
	offset = this->head->getVertexBuffer()->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->head->getVertexBuffer()->getComPtr().GetAddressOf(), 
		&stride, &offset);
	context->DrawIndexed(
		this->i_buffer->getCount(),
		0, 0);
}

void Character::renderBody(ComPtr<ID3D11DeviceContext>& context)
{
	this->mvp.model = this->body->getWorld().Transpose();
	this->constant_buffer->update(this->mvp);
	context->VSSetConstantBuffers(0, 1,
		this->constant_buffer->getComPtr().GetAddressOf());
	uint32 stride;
	uint32 offset;
	stride = this->body->getVertexBuffer()->getStride();
	offset = this->body->getVertexBuffer()->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->body->getVertexBuffer()->getComPtr().GetAddressOf(), 
		&stride, &offset);
	context->DrawIndexed(
		this->i_buffer->getCount(),
		0, 0);
}

void Character::renderLeftArm(ComPtr<ID3D11DeviceContext>& context)
{
	this->mvp.model = this->left_arm->getWorld().Transpose();
	this->constant_buffer->update(this->mvp);
	context->VSSetConstantBuffers(0, 1,
		this->constant_buffer->getComPtr().GetAddressOf());
	uint32 stride;
	uint32 offset;
	stride = this->left_arm->getVertexBuffer()->getStride();
	offset = this->left_arm->getVertexBuffer()->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->left_arm->getVertexBuffer()->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->DrawIndexed(
		this->i_buffer->getCount(),
		0, 0);
}

void Character::renderRightArm(ComPtr<ID3D11DeviceContext>& context)
{
	this->mvp.model = this->right_arm->getWorld().Transpose();
	this->constant_buffer->update(this->mvp);
	context->VSSetConstantBuffers(0, 1,
		this->constant_buffer->getComPtr().GetAddressOf());
	uint32 stride;
	uint32 offset;
	stride = this->right_arm->getVertexBuffer()->getStride();
	offset = this->right_arm->getVertexBuffer()->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->right_arm->getVertexBuffer()->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->DrawIndexed(
		this->i_buffer->getCount(),
		0, 0);
}

void Character::renderLeftLeg(ComPtr<ID3D11DeviceContext>& context
)
{
	this->mvp.model = this->left_leg->getWorld().Transpose();
	this->constant_buffer->update(this->mvp);
	context->VSSetConstantBuffers(0, 1,
		this->constant_buffer->getComPtr().GetAddressOf());
	uint32 stride;
	uint32 offset;
	stride = this->left_leg->getVertexBuffer()->getStride();
	offset = this->left_leg->getVertexBuffer()->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->left_leg->getVertexBuffer()->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->DrawIndexed(
		this->i_buffer->getCount(),
		0, 0);
}

void Character::renderRightLeg(ComPtr<ID3D11DeviceContext>& context)
{
	this->mvp.model = this->right_leg->getWorld().Transpose();
	this->constant_buffer->update(this->mvp);
	context->VSSetConstantBuffers(0, 1,
		this->constant_buffer->getComPtr().GetAddressOf());
	uint32 stride;
	uint32 offset;
	stride = this->right_leg->getVertexBuffer()->getStride();
	offset = this->right_leg->getVertexBuffer()->getOffset();
	context->IASetVertexBuffers(0, 1,
		this->right_leg->getVertexBuffer()->getComPtr().GetAddressOf(),
		&stride, &offset);
	context->DrawIndexed(
		this->i_buffer->getCount(),
		0, 0);
}

void Character::setPipe(ComPtr<ID3D11DeviceContext>& context)
{
	context->IASetIndexBuffer(
		this->i_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->PSSetShaderResources(0, 1,
		this->skin_tex->getComPtr().GetAddressOf());
	context->IASetInputLayout(this->input_layout->getComPtr().Get());
	context->VSSetShader(this->vertex_shader->getComPtr().Get(),
		nullptr, 0);
	context->PSSetShader(this->pixel_shader->getComPtr().Get(), nullptr, 0);
	context->PSSetSamplers(0, 1, this->sampler_state->getComPtr().GetAddressOf());
}


vec3 const& Character::getPos()
{
	return this->c_pos;
}

vec3 const& Character::getDir()
{
	return this->dir;
}

void Character::update(vec3 const& dir)
{
	if (cam->getFreeCamFlag())
		return;

	vec3 new_dir = XMVector3Normalize(vec3(dir.x, 0, dir.z));
	this->dir = new_dir;
	float theta = atan2(-new_dir.x, -new_dir.z);
	this->rot = Mat::CreateRotationY(theta);

	vec3 right_dir = vec3(0, 1, 0).Cross(this->dir);
	vec3 up_dir = this->dir.Cross(right_dir);
	vec3 move_dir = vec3(0, 0, 0);
	if (GetAsyncKeyState('A') & 0x8000)
		move_dir -= XMVector3Normalize(vec3(right_dir.x, 0, right_dir.z));
	if (GetAsyncKeyState('D') & 0x8000)
		move_dir += XMVector3Normalize(vec3(right_dir.x, 0, right_dir.z));
	if (GetAsyncKeyState('W') & 0x8000)
		move_dir += XMVector3Normalize(vec3(this->dir.x, 0, this->dir.z));
	if (GetAsyncKeyState('S') & 0x8000)
		move_dir -= XMVector3Normalize(vec3(this->dir.x, 0, this->dir.z));
	if (GetAsyncKeyState('E') & 0x8000)
		move_dir.y -= 1;
	if (GetAsyncKeyState('Q') & 0x8000)
		move_dir.y += 1;
	move_dir = XMVector3Normalize(move_dir);
	
	this->c_pos = this->aabb_collision->calcCollision(
		this->c_pos, move_dir, 3 * delta_time);

	this->pos = Mat::CreateTranslation(this->c_pos);
	this->aabb_collision->update(this->c_pos + vec3(0, 1, 0));


	this->head->update(this->pos, this->rot);
	this->body->update(this->pos, this->rot);
	this->left_arm->update(this->pos, this->rot);
	this->right_arm->update(this->pos, this->rot);
	this->left_leg->update(this->pos, this->rot);
	this->right_leg->update(this->pos, this->rot);
}
