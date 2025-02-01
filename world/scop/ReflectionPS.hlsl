TextureCube reflection_cube : register(t0);
Texture2D w_pos : register(t1);

SamplerState sampler0 : register(s0);

cbuffer cam : register(b0)
{
    float4 cam_pos;
}

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

float collisionPlane(float3 o, float3 d, float3 n)
{
    float d_dot_n = dot(d, n);
    if (d_dot_n < 0.00000001)
        return -1;
    
    float3 x;
    if (n.x != 0)
    {
        if (n.x == 1)
            x = cam_pos.xyz - float3(3000, 0, 0);
        else
            x = cam_pos.xyz + float3(3000, 0, 0);
        if (o.y == 0 && o.z == 0)
            x = float3(x.x, 1, 1);
        else
            x = float3(x.x, 0, 0);
    }
    else if (n.y != 0)
    {
        if (n.y == 1)
            x = cam_pos.xyz - float3(0, cam_pos.y, 0);
        else
            x = float3(0, 6000, 0);
        if (o.x == 0 && o.z == 0)
            x = float3(1, x.y, 1);
        else
            x = float3(0, x.y, 0);
    }
    else
    {
        if (n.z == 1)
            x = cam_pos.xyz - float3(0, 0, 3000);
        else
            x = cam_pos.xyz + float3(0, 0, 3000);
        if (o.x == 0 && o.y == 0)
            x = float3(1, 1, x.z);
        else
            x = float3(0, 0, x.z);
    }
    
    return dot(n, x - o) / d_dot_n;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 o = w_pos.Sample(sampler0, input.uv).xyz;
    if (o.x == 0 && o.y == 0 && o.z == 0)
        discard;
    float3 a = o - cam_pos.xyz;
    float3 d = reflect(normalize(a), float3(0, 1, 0));
    d = normalize(d);
    //float3 color = reflection_cube.Sample(sampler0, d).rgb;
    //return float4(color, 1.0f);
    
    float t = 1000000;
    t = min(t, collisionPlane(o, d, float3(1, 0, 0)));
    t = min(t, collisionPlane(o, d, float3(-1, 0, 0)));
    t = min(t, collisionPlane(o, d, float3(0, 1, 0)));
    t = min(t, collisionPlane(o, d, float3(0, -1, 0)));
    t = min(t, collisionPlane(o, d, float3(0, 0, 1)));
    t = min(t, collisionPlane(o, d, float3(0, 0, -1)));
    float3 b = d * t;
    
    float3 c = a + b;
    c = normalize(c);
    float3 color = reflection_cube.SampleLevel(sampler0, d, 0).rgb;
    return float4(color, 1.0f);
}