Texture2D halohram_tex : register(t0);

SamplerState splr;

float4 main(float3 p_normal : Normal, float2 p_UV : UV, float4 p_color : Colory, float3 mpo : PixelPosWS, float3 vpo : PixelPosVS, float3 spo : PixelPosSS) : SV_Target //uint FaceID : SV_PrimitiveID
{
    return lerp(halohram_tex.Sample(splr, float2(spo.x * 10, spo.y * 10 * 1.78)), float4(0.07, 0.84, 0.84, 1.0f), 0.7);
}