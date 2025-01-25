Texture2D depth_tex : register(t0);

SamplerState sampler0 : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 c_pos : CLIP_SPACE;
    float4 color : COLOR;
    float2 uv : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float2 uv;
    float3 ndc = input.c_pos.xyz / input.c_pos.w;
    uv.x = (ndc.x + 1.0f) * 0.5f;
    uv.y = (-ndc.y + 1.0f) * 0.5f;
    float d = depth_tex.Sample(sampler0, uv).r;
    if (ndc.z >= d)
        discard;
    return float4(0, 0, 1, 1);
}