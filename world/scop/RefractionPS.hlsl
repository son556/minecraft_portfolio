Texture2D world_pos_tex_refraction : register(t0);
Texture2D color_tex_refraction : register(t1);
Texture2D plane_pos_tex_refraction : register(t2);

SamplerState sampler0 : register(s0);

cbuffer cam : register(b0)
{
    float4 cam_pos;
}

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

static const float air = 1.00029f; // 공기의 굴절률
static const float water = 1.33f; // 물의 굴절률

float4 main(PS_INPUT input) : SV_TARGET
{
    
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}