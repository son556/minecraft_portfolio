Texture2D water_reflection : register(t0);
Texture2D water_refraction : register(t1);
Texture2D terrain_tex : register(t2);
SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = water_reflection.Sample(sampler0, input.uv);
    if (color.r == 0 && color.g == 0 && color.b == 0)
        return terrain_tex.Sample(sampler0, input.uv);
    float4 ref_col = water_refraction.Sample(sampler0, input.uv);
    
    color = lerp(color, ref_col, 0.5);
    return color;
}