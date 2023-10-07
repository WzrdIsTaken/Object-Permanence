// --- 3D ---

struct VSInput3D
{
    float3 m_Position : POSITION;
    float2 m_UV : TEXCOORD;
    float3 m_Normal : NORMAL;
};

struct PSInput3D
{
    float4 m_Position : SV_POSITION;
    float2 m_UV : TEXCOORD;
    float3 m_Normal : NORMAL;
    
    float3 m_CameraWorldPosition : POSITION0;
    float3 m_WorldPosition : POSITION1;
};
typedef PSInput3D VSOutput3D;

// --- 2D ---

struct VSInput2D
{
    float3 m_Position : POSITION;
    float2 m_UV : TEXCOORD;
};

struct PSInput2D
{
    float4 m_Position : SV_POSITION;
    float2 m_UV : TEXCOORD;
};
typedef PSInput2D VSOutput2D;

// --- Light ---

struct Light
{
    float3 m_Position;
    float3 m_Direction;
    float3 m_Colour;
    
    float m_Intensity;
    float m_SpotAngle;
    float m_AttenuationRadius;
    
    float m_AmbientIntensity;
    float m_DiffuseIntensity;
    float m_SpecularAttenuation;
    float m_SpecularIntensity;
};

// --- Buffers ---

cbuffer PerFrame : register(b0)
{
    float4x4 g_ViewMatrix;
    float4x4 g_ProjectionMatrix;
    float3 g_CameraPosition;
};

cbuffer PerMesh : register(b1)
{
    float4x4 g_ModelMatrix;
}

cbuffer PerLightChange : register(b0)
{
    Light g_DirectionalLight;
};
