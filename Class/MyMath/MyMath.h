#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <numbers>

#include "Matrix/Matrix4x4.h"
#include "Vector/Vector3.h"

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

/// <summary>
/// 線分
/// </summary>
struct Segment {
    Vector3 origin;
    Vector3 diff;
};

/// <summary>
/// 球
/// </summary>
struct Sphere {
    Vector3 center;
    float radius;
};

/// <summary>
/// 平面
/// </summary>
struct Plane {
    Vector3 normal; //!< 法線
    float distance; //!< 原点からの距離
};

/// <summary>
/// 三角形
/// </summary>
struct Triangle {
    Vector3 vertices[3];
};

struct AABB {
    Vector3 min; //!< 最小点
    Vector3 max; //!< 最大点
};

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v);

// 内積
float Dot(const Vector3& v1, const Vector3& v2);

// 長さ
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

//================================================
// 4x4行列関数
//================================================

// 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

// 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m);

// 単位行列
Matrix4x4 MakeIdentity4x4();

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 　座標変換
Vector3 TransformCoord(const Vector3& vector, const Matrix4x4& matrix);

// x軸回転行列
Matrix4x4 MakeRotationXMatrix(float radian);

// y軸回転行列
Matrix4x4 MakeRotationYMatrix(float radian);

// z軸回転行列
Matrix4x4 MakeRotationZMatrix(float radian);

// 3次元アフィン変換
Matrix4x4 makeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

Matrix4x4 MakeLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up);

//================================================
// 　レンダリングパイプライン用
//================================================

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip);

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip);

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth);

//================================================
// ベクトル
//================================================

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3 v2);
// 最接近点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

//================================================
// 2次ベジェ曲線
//================================================

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

//================================================
// 　値確認用
//================================================

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix);

void VectorScreenPrintf(int x, int y, const Vector3& vector);

// グリッド
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

// 球体の描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
