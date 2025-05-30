struct VS_INPUT
{
    float3 pos : POSITION;
    float4 color : COLOR;
};


struct PS_INPUT
{
    float4 pos : SV_Position;
    float4 color : COLOR;
};

cbuffer world : register(b0)
{
    matrix world;
}

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output;
    output.pos = mul(float4(input.pos, 1), world);
    output.color = input.color;
    
    return output;
}