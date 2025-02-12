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
	this->renderHead(shadow_flag);
}

void Character::renderHead(bool shadow_flag)
{
	ComPtr<ID3D11DeviceContext> context = d_graphic->getContext();
	if (shadow_flag == false) {
		context->PSSetShaderResources(0, 1,
			this->skin_tex->getComPtr().GetAddressOf());
	}
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
	context->IASetIndexBuffer(
		this->head->getIndexBuffer()->getComPtr().Get(),
		DXGI_FORMAT_R32_UINT,
		0
	);
	context->DrawIndexed(
		this->head->getIndexBuffer()->getCount(),
		0, 0);
}

void Character::renderBody()
{
}

void Character::renderLeftArm()
{
}

void Character::renderRightArm()
{
}

void Character::renderLeftLeg()
{
}

void Character::renderRightLeg()
{
}
