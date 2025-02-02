Texture2D reflection_pos : register(t0); // world space
Texture2D reflection_color : register(t1);

SamplerState sampler0 : register(s0);

cbuffer constant_buffer : register(b0)
{
    float water_h;
}

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 r_pos = reflection_pos.Sample(sampler0, input.uv);
    if (r_pos.y > water_h)
        discard;
    float3 color = reflection_color.Sample(sampler0, input.uv).rgb;
    return float4(color, 1);
}