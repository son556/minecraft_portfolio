Texture2D reflection_terrain : register(t0);
Texture2D water_distortion_tex_reflection : register(t1);
SamplerState sampler0 : register(s0);


struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv = input.uv;
    uv.y *= -1;
    float offset = water_distortion_tex_reflection.Sample(sampler0, uv).rg;
    offset = offset * 2 - float2(1, 1);
    input.uv += offset * 0.0132;
    input.uv.x = clamp(input.uv.x, 0, 1);
    input.uv.y = clamp(input.uv.y, 0.01, 1);
    float3 color = reflection_terrain.Sample(sampler0, input.uv).rgb;
    return float4(color, 1);
}