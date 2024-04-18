
cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
};

matrix World;

//-----------------------------------------------------------------------------
//Structures
//-----------------------------------------------------------------------------
struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
};

struct VertexOutput
{
    float4 Position : SV_POSITION0;
    float2 Uv : UV0;
};

//-----------------------------------------------------------------------------
//Vertex Shader
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);
    
    output.Uv = input.Uv;


    return output;
}

//-----------------------------------------------------------------------------
//Pixel Shader
//-----------------------------------------------------------------------------

Texture2D Map;
SamplerState Sampler;

float4 PS(VertexOutput input) : SV_Target0
{
    float4 color = Map.Sample(Sampler, input.Uv);

    return color;
}

//-----------------------------------------------------------------------------
// Blend State
//-----------------------------------------------------------------------------

/* 
    A를 누르면 뒤집혀, 그려내지 않는 것을 방지하기위해 설정 
            BackSPACE 컬링을 설정 해제합니다.
                                                        */
RasterizerState Cull
{
    CullMode = None;
    DepthClipEnable = false;
};

BlendState AlphaBlend
{
    AlphaToCoverageEnable = true;

    BlendEnable[0] = true;
    DestBlend[0] = INV_SRC_ALPHA;
    SrcBlend[0] = SRC_ALPHA;
    BlendOp[0] = Add;

    SrcBlendAlpha[0] = One;
    DestBlendAlpha[0] = One;
    RenderTargetWriteMask[0] = 0x0F;
};


//-----------------------------------------------------------------------------
//Techniques
//-----------------------------------------------------------------------------
technique11 T0
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS()));
        SetPixelShader(CompileShader(ps_5_0, PS()));
        
        /*  
           Culling mode 해제   
                            */
        SetRasterizerState(Cull);
        SetBlendState(AlphaBlend, float4(0, 0, 0, 0), 0xFF);
    }


}
