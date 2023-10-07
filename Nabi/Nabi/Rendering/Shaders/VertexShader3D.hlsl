#include "Common.hlsli"

VSOutput3D main(VSInput3D vsIn)
{
    VSOutput3D vsOut = (VSOutput3D) 0;
    float4 vertexPosition = float4(vsIn.m_Position, 1.0f);
    
    // Transform the vertex position into projection space
    vertexPosition = mul(vertexPosition, g_ModelMatrix);
    vertexPosition = mul(vertexPosition, g_ViewMatrix);
    vertexPosition = mul(vertexPosition, g_ProjectionMatrix);
    vsOut.m_Position = vertexPosition;
    
    // Set the texture
    vsOut.m_UV = vsIn.m_UV;
    
    // Work out stuff for lighting magic
    float4 vertexNormal = float4(vsIn.m_Normal, 0.0f);
    vsOut.m_Normal = mul(vertexNormal, g_ModelMatrix).xyz;
   
    float4 worldPosition = float4(vsIn.m_Position, 1.0f);
    vsOut.m_WorldPosition = mul(worldPosition, g_ModelMatrix).xyz;
    vsOut.m_CameraWorldPosition = g_CameraPosition;
    
    return vsOut;
}
