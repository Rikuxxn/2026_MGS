//===================================================
//
// プリコンパイル [pch.h]
// Author:YUTO YOSHIDA
//
//===================================================

//***************************************************
// 多重インクルード防止
//***************************************************
#ifndef _PCH_H_
#define _PCH_H_

//***************************************************
// インクルードファイル
//***************************************************
#define _CRT_SECURE_NO_WARNINGS		 // scanfの警告対処マクロ
#include <windows.h>
#include "d3dx9.h"
#include "d3d9.h"
#define DIRECTINPUT_VERSION (0x0800) // ビルド時の警告対処マクロ
#include "dinput.h"
#include "Xinput.h"
#include "xaudio2.h"

//***************************************************
// ライブラリのリンク
//***************************************************
#pragma comment(lib,"d3d9.lib")		// 描画処理に必要
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	// DirectXコンポネーション(部品)使用に必要
#pragma comment(lib,"winmm.lib")	// システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	// 入力処理に必要
#pragma comment(lib,"xinput.lib")	// ジョイパッド処理に必要

//***************************************************
// マクロ定義
//***************************************************
#define CLASS_NAME		"WindowClass"			// ウインドウクラスの名前
#define WINDOW_NAME		"Squash"				// ウインドウの名前(キャプションに表示)
#define SCREEN_WIDTH	(1280)					// ウインドウの幅
#define SCREEN_HEIGHT	(720)					// ウインドウの高さ

#define FVF_VERTEX_2D	(D3DFVF_XYZRHW	| D3DFVF_DIFFUSE	| D3DFVF_TEX1)									// 2Dの頂点情報
#define FVF_VERTEX_2DMT (D3DFVF_XYZRHW	| D3DFVF_DIFFUSE	| D3DFVF_TEX2)									// 2Dマルチテクスチャの頂点情報
#define FVF_VERTEX_3D	(D3DFVF_XYZ		| D3DFVF_NORMAL		| D3DFVF_DIFFUSE | D3DFVF_TEX1)					// 座標,法線,カラー,テクスチャ
#define FVF_VERTEX_3DMT (D3DFVF_XYZ		| D3DFVF_NORMAL		| D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEX2)   // 座標,法線,カラー,テクスチャ2
#define D3DFVF_VERTEX (	D3DFVF_XYZ		| D3DFVF_DIFFUSE)

//***************************************************
// インクルードファイル
//***************************************************
#include "vector"
#include "memory"
#include "string"
#include "unordered_map"
#include "array"
#include "fstream"
#include "sstream"

//***************************************************
// 定数の名前空間
//***************************************************
namespace Const
{
	const D3DXVECTOR3 CENTER_POS_2D				= { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };
	const D3DXCOLOR BACK_BUFFER_COLOR			= D3DCOLOR_RGBA(100, 100, 100, 255);	// バックバッファの色
	const D3DXQUATERNION QUATERNION_IDENTITY	= { 0.0f,0.0f,0.0f,1.0f };				// クォータニオンの初期化
	const D3DXVECTOR3 VEC3_NULL					= { 0.0f,0.0f,0.0f };					// VECTOR3の初期化
	const D3DXVECTOR2 VEC2_NULL					= { 0.0f,0.0f };						// VECTOR2の初期化
	const D3DXVECTOR3 INIT_SCAL					= { 1.0f,1.0f,1.0f };					// スケールの初期化
	const D3DXVECTOR3 AXIS_X					= { 1.0f,0.0f,0.0f };					// X軸
	const D3DXVECTOR3 AXIS_Y					= { 0.0f,1.0f,0.0f };					// Y軸
	const D3DXVECTOR3 AXIS_Z					= { 0.0f,0.0f,1.0f };					// Z軸
	const D3DXVECTOR3 VEC_U						= { 0.0f,1.0f,0.0f };					// 上方向ベクトル
	const D3DXCOLOR WHITE						= { 1.0f,1.0f,1.0f,1.0f };				// 色の初期カラー
	const D3DXMATRIX MTX_IDENTITY =														// マトリックスの初期化
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	constexpr float TWO_PI = D3DX_PI * 2.0f;				// 2π
	constexpr float MAX_GRABITY = 1.0f;						// 重力量
	constexpr int FRAME = 60;								// 一フレーム
	constexpr int MAX_WORD = 5024;							// 最大の文字数
}

//**********************
// 頂点情報[2D]の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;				// 頂点座標
	float rhw;						// 座標返還係数(1.0fで指定)
	D3DCOLOR col;					// 頂点カラー
	D3DXVECTOR2 tex;				// テクスチャ座標
}VERTEX_2D;

//**********************
// 頂点情報[3D]の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// 頂点座標
	float rhw;					// 座標返還係数(1.0fで指定)
	D3DCOLOR col;				// 頂点カラー
	D3DXVECTOR2 tex;			// テクスチャ座標
	D3DXVECTOR2 texMT;			// テクスチャ座標MT
}VERTEX_2D_MULT;

//**********************
// 頂点情報[3D]の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// 頂点座標
	D3DXVECTOR3 nor;			// 法線ベクトル
	D3DCOLOR col;				// 頂点カラー
	D3DXVECTOR2 tex;			// テクスチャ座標
}VERTEX_3D;

//**********************
// 頂点情報[3D]の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// 頂点座標
	D3DXVECTOR3 nor;			// 法線ベクトル
	D3DCOLOR col;				// 頂点カラー
	D3DXVECTOR2 tex;			// テクスチャ座標
	D3DXVECTOR2 texMT;			// テクスチャ座標MT
}VERTEX_3D_MULT;

//**********************
// 頂点情報線の構造体
//**********************
typedef struct
{
	D3DXVECTOR3 pos;			// 頂点座標
	D3DCOLOR col;				// 色
}LINE_LIST;

//**********************
// int型のvector2
//**********************
struct VECTOR2INT
{
	VECTOR2INT(int _x = 0, int _y = 0) :
		x(_x),y(_y)
	{}

	// 加算のオペレータ
	VECTOR2INT operator+(const VECTOR2INT& other) const
	{
		return { x + other.x, y + other.y };
	}
	// 減算のオペレータ
	VECTOR2INT operator-(const VECTOR2INT& other) const
	{
		return { x - other.x, y - other.y };
	}

	// 乗算のオペレータ
	VECTOR2INT operator*(int value) const
	{
		return { x * value, y * value };
	}

	VECTOR2INT& operator +=(const VECTOR2INT& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}
	VECTOR2INT& operator -=(const VECTOR2INT& other)
	{
		x -= other.x;
		y -= other.y;

		return *this;
	}
	bool operator ==(const VECTOR2INT& other) const
	{
		return x == other.x && y == other.y;
	}
	bool operator !=(const VECTOR2INT& other) const
	{
		return !(other == *this);
	}

	int x;	// x
	int y;	// y
};
#endif
