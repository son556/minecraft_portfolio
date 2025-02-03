Texture2D reflection_terrain : register(t0);

SamplerState sampler0 : register(s0);


struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float3 color = reflection_terrain.Sample(sampler0, input.uv).rgb;
    return float4(color, 1);
}