struct PS_INPUT
{
    float4 ndc_pos : SV_Position;
    float3 world_pos : POSITION;
    float2 uv;
};

Texture2D start_scene_start_button_tex; // 0
Texture2D start_scene_start_button_select_tex; // 1
Texture2D start_scene_exit_button_tex; // 2
Texture2D start_scene_exit_button_select_tex; //3
Texture2D start_scene_title_tex; // 4
TextureCube start_scene_sky_box; // 5

SamplerState sampler0 : register(s0);

cbuffer option : register(b0)
{
    int texture_idx;
    float2 uv;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color;
    if (texture_idx == 0)
        color = start_scene_start_button_tex.Sample(sampler0, input.uv);
    else if (texture_idx == 1)
        color = start_scene_start_button_select_tex.Sample(sampler0, input.uv);
    else if (texture_idx == 2)
        color = start_scene_exit_button_tex.Sample(sampler0, input.uv);
    else if (texture_idx == 3)
        color = start_scene_exit_button_select_tex.Sample(sampler0, input.uv);
    else if (texture_idx == 4)
    {
        color = start_scene_title_tex.Sample(sampler0, input.uv);
        if (color.r == 0 && color.g == 0 && color.b == 0)
            discard;
    }
    else
        color = start_scene_sky_box.Sample(sampler0, input.world_pos);
    color.w = 1;
	return color;
}