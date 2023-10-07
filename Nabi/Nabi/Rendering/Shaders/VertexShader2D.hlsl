#include "Common.hlsli"

VSOutput2D main(VSInput2D vsIn)
{
    VSOutput2D vsOut = (VSOutput2D) 0;
    float4 vertexPosition = float4(vsIn.m_Position.x, vsIn.m_Position.y, 0.0f, 1.0f);
    
    // Transform the vertex position into projection space
    vertexPosition = mul(vertexPosition, g_ModelMatrix);
    vertexPosition = mul(vertexPosition, g_ViewMatrix);
    vertexPosition = mul(vertexPosition, g_ProjectionMatrix);
    vsOut.m_Position = vertexPosition;
    
    // Set the texture
    vsOut.m_UV = vsIn.m_UV;
    
    return vsOut;
}
