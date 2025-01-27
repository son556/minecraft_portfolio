Texture2D reflection_tex : register(t0);
Texture2D refrection_tex : register(t1);
SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	// test
    float4 color = reflection_tex.Sample(sampler0, input.uv);
    if (color.r == 0 && color.g == 0 && color.b == 0)
        discard;
    return color;
}