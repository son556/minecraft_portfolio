struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    int shadow_flag : SHADOW;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 ndc_pos : NDC;
};

cbuffer mvp : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};


PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    output.pos = float4(input.pos, 1);
    output.pos = mul(output.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, proj);
    output.ndc_pos = output.pos;
    return output;
}