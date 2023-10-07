#include "Common.hlsli"

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

float4 main(PSInput3D psIn) : SV_TARGET
{
    float3 diffuseLight = float3(0, 0, 0);
    float3 specularLight = float3(0, 0, 0);
    
    // Ambient
    const float ambientStrength = 0.1f;
    float3 ambientLight = g_DirectionalLight.m_Colour * ambientStrength * g_DirectionalLight.m_AmbientIntensity;
    
     // Diffuse
    float diffuseFactor = dot(normalize(psIn.m_Normal), -g_DirectionalLight.m_Direction);
    if (diffuseFactor > 0)
    {
        diffuseLight = g_DirectionalLight.m_Colour * diffuseFactor * g_DirectionalLight.m_DiffuseIntensity;

		// Specular
        float3 vertexToCamera = normalize(psIn.m_CameraWorldPosition - psIn.m_WorldPosition);
        float3 lightReflect = normalize(reflect(g_DirectionalLight.m_Direction, psIn.m_Normal));
        float specularFactor = dot(vertexToCamera, lightReflect);

        if (specularFactor > 0)
        {
            specularFactor = pow(specularFactor, g_DirectionalLight.m_SpecularAttenuation);
            specularLight = g_DirectionalLight.m_Colour * specularFactor * g_DirectionalLight.m_SpecularIntensity;
        }
    }
    
    // Final
    float4 lighting = float4(ambientLight + diffuseLight + specularLight, 1.0f) * g_DirectionalLight.m_Intensity;
    return Texture.Sample(Sampler, psIn.m_UV) * lighting;
}
