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

Character::Character(Mat pos, Mat rot) : pos(pos), rot(rot)
{
	this->skin_tex = make_shared<Texture>(
		d_graphic->getDevice(),
		L"./textures/zombie/TEXTURE/zombie.png"
	);
	this->left_arm = make_shared<LeftArm>(pos, rot);
	this->right_arm = make_shared<RightArm>(pos, rot);
	this->left_leg = make_shared<LeftLeg>(pos, rot);
	this->right_leg = make_shared<RightLeg>(pos, rot);
	this->body = make_shared<Body>(pos, rot);
	this->head = make_shared<Head>(pos, rot);
	this->constant_buffer = make_shared<ConstantBuffer>(
		d_graphic->getDevice(),
		d_graphic->getContext(),
		this->mvp
	);
	vector<uint32> indices;
	Parts::makeIndices(indices);
	this->i_buffer = make_shared<Buffer<uint32>>(
		d_graphic->getDevice(),
		indices.data(),
		indices.size(),
		D3D11_BIND_INDEX_BUFFER
	);
}

void Character::render(
	CamType type, 
	bool shadow_flag,
	Mat const& view,
	Mat const& proj
)
{
	if (shadow_flag == false) {
		this->mvp.view = cam->getMVP(type).view.Transpose();
		this->mvp.proj = cam->getMVP(type).proj.Transpose();
	}
	else {
		this->mvp.view = view;
		this->mvp.proj = proj;
	}
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	context->IASetIndexBuffer(
		this->i_buffer->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	if (shadow_flag == false) {
		context->PSSetShaderResources(0, 1,
			this->skin_tex->getComPtr().GetAddressOf());
	}
	this->renderHead(context, shadow_flag);
	this->renderBody(context, shadow_flag);
	this->renderLeftArm(context, shadow_flag);
	this->renderRightArm(context, shadow_flag);
	this->renderLeftLeg(context, shadow_flag);
	this->renderRightLeg(context, shadow_flag);
}

void Character::renderHead(
	ComPtr<ID3D11DeviceContext>& context,
	bool shadow_flag
)
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

void Character::renderBody(
	ComPtr<ID3D11DeviceContext>& context,
	bool shadow_flag
)
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

void Character::renderLeftArm(
	ComPtr<ID3D11DeviceContext>& context,
	bool shadow_flag
)
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

void Character::renderRightArm(
	ComPtr<ID3D11DeviceContext>& context,
	bool shadow_flag
)
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

void Character::renderLeftLeg(
	ComPtr<ID3D11DeviceContext>& context,
	bool shadow_flag
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

void Character::renderRightLeg(
	ComPtr<ID3D11DeviceContext>& context,
	bool shadow_flag
)
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
