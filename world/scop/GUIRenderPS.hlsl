Texture2D gui_texture : register(t0);

SamplerState sampler0 : register(s0);


struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

cbuffer world : register(b0)
{
    int gui_flag;
}

float4 main(PS_INPUT input) : SV_Target
{
    float4 color;
    color = gui_texture.Sample(sampler0, input.uv);
    if (gui_flag)
        color.rgb = pow(color.rgb, 1.0/2.2);
    color.w = 1;
    return color;
}