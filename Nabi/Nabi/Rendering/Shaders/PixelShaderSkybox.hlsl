#include "Common.hlsli"

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

float4 main(PSInput3D psIn) : SV_TARGET
{
    float4 pixelColour = Texture.Sample(Sampler, psIn.m_UV);
    clip(pixelColour.a < 0.1F ? -1 : 1); 
    
    return pixelColour;
}
