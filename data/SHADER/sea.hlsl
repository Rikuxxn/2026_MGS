//=============================================================================
//
// 海の水面処理 [sea.hlsl]
// Author : SOICHIRO SASAKI
//
//=============================================================================

// 定数
static const float power = 0.2f;
static const float alpha = 0.6f;
static const float powPower = 2.0f;
static const float2 speed1 = float2(0.01f, 0.04f);
static const float2 speed2 = float2(-0.02f, 0.03f);
static const float3 plusColor = float3(0.7f, 0.8f, 1.0f);
static const float3 lightVec = float3(0.2f, -0.3f, 0.2f);
static const float4 lightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
static const float4 ambient = float4(0.8f, 0.8f, 0.8f, 1.0f);

// グローバル変数宣言
float g_fTime;
float4x4 g_mtxWld;
float4x4 g_mtxView;
float4x4 g_mtxProj;

// ロゴのテクスチャ
texture g_WaterMap;
sampler waterSampler = sampler_state
{
    texture = <g_WaterMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};
// 法線のテクスチャ
texture g_NoiseMap;
sampler noiseSampler = sampler_state
{
    texture = <g_NoiseMap>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//**********************************
// 頂点シェーダのインプット情報
//**********************************
struct VS_IN
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 nor : TEXCOORD1;
};

//***********************************
// 頂点シェーダのアウトプット情報
//***********************************
struct VS_OUT
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 nor : TEXCOORD1;
};

//***********************************
// ピクセルシェーダのインプット情報
//***********************************
struct PS_IN
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 nor : TEXCOORD1;
};

//********************
// 頂点シェーダ
//********************
VS_OUT main(VS_IN In)
{
    VS_OUT Out;
    
    float4 pos;
    pos = mul(In.pos, g_mtxWld);
    pos = mul(pos, g_mtxView);
    pos = mul(pos, g_mtxProj);
    
    float3 nor = normalize(mul(In.nor, (float3x3) g_mtxWld));
    
    Out.pos = pos;
    Out.nor = nor;
    Out.uv = In.uv;
    
    return Out;
}

//********************
// ピクセルシェーダ
//********************
float4 PS(PS_IN In) : COLOR0
{
    // ノイズUVをスクロール
    float2 noizeUV1 = In.uv + g_fTime * speed1;
    float2 noizeUV2 = In.uv + g_fTime * speed2;
    
    // ノイズ取得
    float2 noize1 = tex2D(noiseSampler, noizeUV1).rg;
    float2 noize2 = tex2D(noiseSampler, noizeUV2).rg;
    
    // 2方向ノイズ合成
    float2 noize = (noize1 + noize2) * 0.5f;
    
    // 歪みに使う(-0.5f～0.5fの範囲にするため-0.5f)
    float2 distort = (noize - 0.5f) * power;
    
    // 歪ませた水テクスチャ取得
    float4 base = tex2D(waterSampler, In.uv + distort);
    
    // 法線設定
    float3 N = normalize(In.nor);
    N.x += distort.x;
    N.z += distort.y;
    N = normalize(N);
    
    // ライトベクトル設定
    float3 L = normalize(-lightVec);
    
    // 光の当たり方を計算
    float diff = saturate(dot(N, L));
    
    // 累乗して影の濃さ調整
    diff = pow(diff, powPower);
   
    // 青を付け足す
    base.xyz *= plusColor;
    base.a = alpha;
    
    float4 color = base * (ambient + diff * lightColor);
    
    return base;
}

//********************
// テクニック宣言
//********************
technique tech
{
    pass P0
    {
        VertexShader = compile vs_3_0 main();
        PixelShader = compile ps_3_0 PS();
    }
}