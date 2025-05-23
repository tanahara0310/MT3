#pragma once

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <numbers>

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

/// <summary>
/// ベクトル構造体
/// </summary>
struct Vector3 {
    float x, y, z;

    Vector3 operator+(const Vector3& v) const
    {
        return { x + v.x, y + v.y, z + v.z };
    }

    Vector3 operator-(const Vector3& v) const
    {
        return { x - v.x, y - v.y, z - v.z };
    }

    Vector3 operator*(float scalar) const
    {
        return { x * scalar, y * scalar, z * scalar };
    }
};

/// <summary>
/// 行列構造体
/// </summary>
struct Matrix4x4 {

    float m[4][4];

    Matrix4x4 operator+(const Matrix4x4& matrix) const
    {
        Matrix4x4 result;
        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 4; ++column) {
                result.m[row][column] = m[row][column] + matrix.m[row][column];
            }
        }
        return result;
    }

    Matrix4x4 operator-(const Matrix4x4& matrix) const
    {
        Matrix4x4 result;
        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 4; ++column) {
                result.m[row][column] = m[row][column] - matrix.m[row][column];
            }
        }
        return result;
    }

    Matrix4x4 operator*(const Matrix4x4& matrix) const
    {
        Matrix4x4 result;
        for (int row = 0; row < 4; ++row) {
            for (int column = 0; column < 4; ++column) {
                result.m[row][column] = 0.0f;
                for (int i = 0; i < 4; ++i) {
                    result.m[row][column] += m[row][i] * matrix.m[i][column];
                }
            }
        }
        return result;
    }
};

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
// 　値確認用
//================================================

void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix);

void VectorScreenPrintf(int x, int y, const Vector3& vector);

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);
