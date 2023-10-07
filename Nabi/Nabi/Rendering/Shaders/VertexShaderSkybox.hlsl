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
    
    // Normals are not needed for the skybox
    
    return vsOut;
}
