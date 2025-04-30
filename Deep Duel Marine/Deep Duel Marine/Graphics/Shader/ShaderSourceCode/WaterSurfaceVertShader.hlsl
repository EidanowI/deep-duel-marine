cbuffer ConstBuf0 : register(b0)
{
    matrix BUF0_view;
    matrix BUF0_projection;
};

cbuffer ConstBuf1 : register(b1)
{
    float4 BUF1_global_time;
};

struct VSOut
{
    float3 VS_Normal : Normal;
    float2 VS_UV : UV;
    float4 VS_color : Colory;
    float3 VS_pixelPosWS : PixelPosWS;
    float3 VS_pixelPosVS : PixelPosVS;
    float3 VS_pixelPosSS : PixelPosSS;
    float4 VS_Pos : SV_Position;
};



float WaveSpeed(float wave_length)
{
    float PI = 3.141592f;
    float g = 9.8f;
    return sqrt(2 * PI / wave_length * g) * BUF1_global_time.x/3;
}
float3 Wave(float3 pos, float3 wave_direction, float wave_length, float wave_height, float wave_sharpness)
{
    float PI = 3.141592f;
    float g = 9.8f;
    
    float two_pi_div_by_length = 2 * PI / wave_length;
    
    float dt = dot(pos, normalize(wave_direction) * two_pi_div_by_length) - WaveSpeed(wave_length);
    
    float3 sharp = sin(dt) * (wave_height * wave_sharpness / (two_pi_div_by_length * wave_height)) * normalize(wave_direction);

    
    return (cos(dt) * float3(0.0f, wave_height, 0.0f)) - sharp;
}



VSOut main(float3 v_pos : Position, float3 v_normal : Normal, float2 v_UV : UV, float4 v_Color : Color)
{
    VSOut ret;

    float PI = 3.141592f;
    float g = 9.8f;
    
    float3 wave_direction = float3(1.0f, 0.0f, 0.0f);
    float wave_length = 4.5f;
    float wave_height = 0.1;
    float wave_sharpness = 0.15f;
    
    float3 wave_a = Wave(v_pos, float3(-0.2f, 0.0f, -0.7f), 2.5f, 0.02f, 0.3f);
    float3 wave_b = Wave(v_pos, float3(-1.0f, 0.0f, 0.0f), 3.0f, 0.02f, 0.3f);
    float3 wave_c = Wave(v_pos, float3(-0.5f, 0.0f, 0.5f), 1.8f, 0.01f, 0.4f);
    
    v_pos += wave_a + wave_b + wave_c;
    //v_pos.y += 
    //v_pos.y = 4;
    
    //v_pos.y = abs(cos((v_pos.x + BUF1_global_time.x) / 2)) * 2;
    ret.VS_Pos = mul(BUF0_projection, mul(BUF0_view, float4(v_pos.x, v_pos.y, v_pos.z, 1.0f)));
    ret.VS_Normal = v_normal;
    ret.VS_UV = v_UV;
    ret.VS_UV.x -= BUF1_global_time.x / 100;
    ret.VS_color.x = abs(cos((v_pos.x + BUF1_global_time.x) / 2));
    ret.VS_color = lerp(float4(0.0, 0.0, 0.3, 1.0f), float4(0.7f, 0.7, 0.8, 1.0f), abs(cos((v_pos.x + BUF1_global_time.x) / 2)));
   // ret.VS_color = float4(abs(cos(BUF1_global_time.x + v_pos.x * 30))*0.2, 0, 0, 0);
    //ret.VS_pixelPosWS = mul(BUF1_model, float4(v_pos.xyz, 1.0f));
    ret.VS_color = mul(BUF0_view, float4(v_pos.x, v_pos.y, v_pos.z, 1.0f));
    //ret.VS_pixelPosSS = mul(BUF0_projection, mul(BUF0_view, float4(ret.VS_pixelPosWS, 1.0f)));
    return ret;
}