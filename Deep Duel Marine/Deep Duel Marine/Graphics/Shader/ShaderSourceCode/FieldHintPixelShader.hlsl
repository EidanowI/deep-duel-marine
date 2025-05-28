float4 main(float3 p_normal : Normal, float2 p_UV : UV, float4 p_color : Colory, float3 VS_pixelPosWS : PixelPosWS, float3 VS_pixelPosVS : PixelPosVS, float VS_pixelPosSS : PixelPosSS) : SV_Target //uint FaceID : SV_PrimitiveID
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}