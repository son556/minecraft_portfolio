#pragma once

class InputLayout;
class VertexShader;
class RasterizerState;
class PixelShader;
class SamplerState;
class Character;
class DeferredBuffer;

class Entity
{
public:
	Entity();
	~Entity() = default;
	void setRTV(ComPtr<ID3D11DepthStencilView> dsv);
	void setCharacter(vec3 pos, vec3 radian_xyz);
	void render(CamType type);
	void shadowRender(
		CamType type,
		Mat const& view,
		Mat const& proj,
		ComPtr<ID3D11DepthStencilView> const& dsv
	);
	ComPtr<ID3D11ShaderResourceView> getSRV();
	ComPtr<ID3D11ShaderResourceView> getSRVPos();
	ComPtr<ID3D11ShaderResourceView> getSRVNormal();

private:
	Entity(Entity const&) = delete;
	Entity& operator=(Entity const&) = delete;

private:
	void setPipe();

private:
	shared_ptr<DeferredBuffer> d_buffer;
	shared_ptr<InputLayout> input_layout;
	shared_ptr<VertexShader> vertex_shader;
	shared_ptr<RasterizerState> rasterizer_state;
	shared_ptr<PixelShader> pixel_shader;
	shared_ptr<SamplerState> sampler_state;

private:
	ComPtr<ID3D11DepthStencilView> dsv;

private:
	shared_ptr<Character> character;
};

