Texture2D water_surface_tex : register(t0);

SamplerState splr;

float ILerp(float4 a, float4 b, float4 t)
{
    return  (t- a) / (b - a);
}

float4 main(float3 p_normal : Normal, float2 p_UV : UV, float4 p_color : Colory, float3 VS_pixelPosWS : PixelPosWS, float3 VS_pixelPosVS : PixelPosVS, float VS_pixelPosSS : PixelPosSS) : SV_Target //uint FaceID : SV_PrimitiveID
{
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //return float4(BUF0_PointLights[0].position.x, 0.0f, 0.0f, 1.0f);
    //return facesColors[FaceID / 2];
    //return p_color;
    //return lightColorBUF0;
    float4 a = water_surface_tex.Sample(splr, p_UV * 2);
    //a =- float4(0.02f, 0.01f, -0.05f, 0.0f);
    
    float n = clamp(ILerp(6, 35,length(p_color.xyz)), 0.0f, 1.0f);
    
    return lerp(a, float4(0.66f, 0.67f, 0.73f, 1.0f), n);
}