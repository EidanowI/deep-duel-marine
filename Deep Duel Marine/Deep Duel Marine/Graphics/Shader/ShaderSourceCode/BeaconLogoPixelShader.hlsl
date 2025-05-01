Texture2D beaconLogo_color_tex : register(t0);
Texture2D beaconLogo_blinker_mask_tex : register(t1);

SamplerState splr;

float ILerp(float4 a, float4 b, float4 t)
{
    return (t - a) / (b - a);
}

float SquareWave(float inp)
{
    return 1.0f - 2.0f * round(frac(inp));

}

float4 main(float3 p_normal : Normal, float2 p_UV : UV, float4 p_color : Colory, float3 VS_pixelPosWS : PixelPosWS, float3 VS_pixelPosVS : PixelPosVS, float VS_pixelPosSS : PixelPosSS) : SV_Target //uint FaceID : SV_PrimitiveID
{
    float4 base_color = beaconLogo_color_tex.Sample(splr, p_UV);

    float n = clamp(ILerp(6, 35, length(p_color.xyz)), 0.0f, 1.0f);
    
    float4 color_with_fog = base_color * 0.8;

    //clamp(sin(p_color.x * 10), 0.0f, 1.0f)
    float a = lerp(clamp(sin(p_color.x * 15), 0.0f, 1.0f), 0.0f, SquareWave(p_color.x / 5));
    return lerp(color_with_fog, lerp(color_with_fog, float4(1.0f, 0.0f, 0.0f, 1.0f), a), beaconLogo_blinker_mask_tex.Sample(splr, p_UV).x);
    //return beaconLogo_color_tex.Sample(splr, p_UV);
}