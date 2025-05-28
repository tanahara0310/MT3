#include "MyMath.h"
#include <Novice.h>
#include <assert.h>
#include <cmath>
#include <numbers>

// ベクトルの加算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
    return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

// ベクトルの減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
    return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

// ベクトルのスカラー倍
Vector3 Multiply(float scalar, const Vector3& v)
{
    return { scalar * v.x, scalar * v.y, scalar * v.z };
}

// ベクトルの内積
float Dot(const Vector3& v1, const Vector3& v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// ベクトルの長さ
float Length(const Vector3& v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// ベクトルの正規化
Vector3 Normalize(const Vector3& v)
{
    float length = Length(v);
    assert(length != 0.0f); // ゼロ除算を防ぐためのアサーション

    return { v.x / length, v.y / length, v.z / length };
}

Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
    return {
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    };
}

//================================================
// 4x4行列関数
//================================================

// 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 result;

    result.m[0][0] = m1.m[0][0] + m2.m[0][0];
    result.m[0][1] = m1.m[0][1] + m2.m[0][1];
    result.m[0][2] = m1.m[0][2] + m2.m[0][2];
    result.m[0][3] = m1.m[0][3] + m2.m[0][3];

    result.m[1][0] = m1.m[1][0] + m2.m[1][0];
    result.m[1][1] = m1.m[1][1] + m2.m[1][1];
    result.m[1][2] = m1.m[1][2] + m2.m[1][2];
    result.m[1][3] = m1.m[1][3] + m2.m[1][3];

    result.m[2][0] = m1.m[2][0] + m2.m[2][0];
    result.m[2][1] = m1.m[2][1] + m2.m[2][1];
    result.m[2][2] = m1.m[2][2] + m2.m[2][2];
    result.m[2][3] = m1.m[2][3] + m2.m[2][3];

    result.m[3][0] = m1.m[3][0] + m2.m[3][0];
    result.m[3][1] = m1.m[3][1] + m2.m[3][1];
    result.m[3][2] = m1.m[3][2] + m2.m[3][2];
    result.m[3][3] = m1.m[3][3] + m2.m[3][3];

    return result;
}

// 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 result;

    result.m[0][0] = m1.m[0][0] - m2.m[0][0];
    result.m[0][1] = m1.m[0][1] - m2.m[0][1];
    result.m[0][2] = m1.m[0][2] - m2.m[0][2];
    result.m[0][3] = m1.m[0][3] - m2.m[0][3];

    result.m[1][0] = m1.m[1][0] - m2.m[1][0];
    result.m[1][1] = m1.m[1][1] - m2.m[1][1];
    result.m[1][2] = m1.m[1][2] - m2.m[1][2];
    result.m[1][3] = m1.m[1][3] - m2.m[1][3];

    result.m[2][0] = m1.m[2][0] - m2.m[2][0];
    result.m[2][1] = m1.m[2][1] - m2.m[2][1];
    result.m[2][2] = m1.m[2][2] - m2.m[2][2];
    result.m[2][3] = m1.m[2][3] - m2.m[2][3];

    result.m[3][0] = m1.m[3][0] - m2.m[3][0];
    result.m[3][1] = m1.m[3][1] - m2.m[3][1];
    result.m[3][2] = m1.m[3][2] - m2.m[3][2];
    result.m[3][3] = m1.m[3][3] - m2.m[3][3];

    return result;
}

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
    Matrix4x4 result;

    for (int i = 0; i < 4; ++i) { // 行

        for (int j = 0; j < 4; ++j) { // 列

            // 初期化
            result.m[i][j] = 0;

            for (int k = 0; k < 4; ++k) { // 積の計算
                result.m[i][j] += m1.m[i][k] * m2.m[k][j];
            }
        }
    }

    return result;
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m)
{
    Matrix4x4 result;
    float det;

    // 各要素の余因子を直接計算
    float cof[4][4];

    cof[0][0] = m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]);

    cof[0][1] = -(m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[1][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]));

    cof[0][2] = m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]);

    cof[0][3] = -(m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[1][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

    cof[1][0] = -(m.m[0][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) + m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]));

    cof[1][1] = m.m[0][0] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) - m.m[0][2] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]);

    cof[1][2] = -(m.m[0][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) + m.m[0][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));

    cof[1][3] = m.m[0][0] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]) - m.m[0][1] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]) + m.m[0][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]);

    cof[2][0] = m.m[0][1] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) + m.m[0][3] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]);

    cof[2][1] = -(m.m[0][0] * (m.m[1][2] * m.m[3][3] - m.m[1][3] * m.m[3][2]) - m.m[0][2] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]));

    cof[2][2] = m.m[0][0] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) + m.m[0][3] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]);

    cof[2][3] = -(m.m[0][0] * (m.m[1][1] * m.m[3][2] - m.m[1][2] * m.m[3][1]) - m.m[0][1] * (m.m[1][0] * m.m[3][2] - m.m[1][2] * m.m[3][0]) + m.m[0][2] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]));

    cof[3][0] = -(m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) + m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]));

    cof[3][1] = m.m[0][0] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) - m.m[0][2] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][3] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]);

    cof[3][2] = -(m.m[0][0] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) + m.m[0][3] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));

    cof[3][3] = m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) - m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) + m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);

    // 行列式の計算
    det = m.m[0][0] * cof[0][0] + m.m[0][1] * cof[0][1] + m.m[0][2] * cof[0][2] + m.m[0][3] * cof[0][3];

    // 行列式が 0 の場合は逆行列なし
    assert(det != 0.0f);

    // 逆行列の計算
    float invDet = 1.0f / det;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.m[j][i] = cof[i][j] * invDet;
        }
    }

    return result;
}

// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m)
{
    Matrix4x4 result;

    result.m[0][0] = m.m[0][0];
    result.m[0][1] = m.m[1][0];
    result.m[0][2] = m.m[2][0];
    result.m[0][3] = m.m[3][0];

    result.m[1][0] = m.m[0][1];
    result.m[1][1] = m.m[1][1];
    result.m[1][2] = m.m[2][1];
    result.m[1][3] = m.m[3][1];

    result.m[2][0] = m.m[0][2];
    result.m[2][1] = m.m[1][2];
    result.m[2][2] = m.m[2][2];
    result.m[2][3] = m.m[3][2];

    result.m[3][0] = m.m[0][3];
    result.m[3][1] = m.m[1][3];
    result.m[3][2] = m.m[2][3];
    result.m[3][3] = m.m[3][3];

    return result;
}

// 単位行列
Matrix4x4 MakeIdentity4x4()
{
    Matrix4x4 result;

    result.m[0][0] = 1.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f;
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f;
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

// 平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{

    Matrix4x4 result;

    result.m[0][0] = 1.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f;
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f;
    result.m[2][3] = 0.0f;

    result.m[3][0] = translate.x;
    result.m[3][1] = translate.y;
    result.m[3][2] = translate.z;
    result.m[3][3] = 1.0f;

    return result;
}

// 拡縮行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
    Matrix4x4 result;

    result.m[0][0] = scale.x;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = scale.y;
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = scale.z;
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

// ベクトルの変換
Vector3 TransformCoord(const Vector3& vector, const Matrix4x4& matrix)
{
    Vector3 result;

    result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
    result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
    result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
    float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];

    /* assert(w != 0.0f);*/

    result.x /= w;
    result.y /= w;
    result.z /= w;

    return result;
}

// X軸回転行列
Matrix4x4 MakeRotationXMatrix(float radian)
{

    Matrix4x4 result;
    result.m[0][0] = 1.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = std::cosf(radian);
    result.m[1][2] = std::sinf(radian);
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = -std::sinf(radian);
    result.m[2][2] = cosf(radian);
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

// Y軸回転行列
Matrix4x4 MakeRotationYMatrix(float radian)
{
    Matrix4x4 result;

    result.m[0][0] = std::cosf(radian);
    result.m[0][1] = 0.0f;
    result.m[0][2] = -std::sinf(radian);
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f;
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = std::sinf(radian);
    result.m[2][1] = 0.0f;
    result.m[2][2] = std::cosf(radian);
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

// Z軸回転行列
Matrix4x4 MakeRotationZMatrix(float radian)
{

    Matrix4x4 result;

    result.m[0][0] = std::cosf(radian);
    result.m[0][1] = std::sinf(radian);
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = -std::sinf(radian);
    result.m[1][1] = std::cosf(radian);
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f;
    result.m[2][3] = 0.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

// アフィン変換行列
Matrix4x4 makeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
    Matrix4x4 result;

    // 回転行列を個別に計算
    float cosX = std::cosf(rotate.x);
    float sinX = std::sinf(rotate.x);
    float cosY = std::cosf(rotate.y);
    float sinY = std::sinf(rotate.y);
    float cosZ = std::cosf(rotate.z);
    float sinZ = std::sinf(rotate.z);

    result.m[0][0] = scale.x * (cosY * cosZ);
    result.m[0][1] = scale.x * (cosY * sinZ);
    result.m[0][2] = scale.x * (-sinY);
    result.m[0][3] = 0.0f;

    result.m[1][0] = scale.y * (sinX * sinY * cosZ - cosX * sinZ);
    result.m[1][1] = scale.y * (sinX * sinY * sinZ + cosX * cosZ);
    result.m[1][2] = scale.y * (sinX * cosY);
    result.m[1][3] = 0.0f;

    result.m[2][0] = scale.z * (cosX * sinY * cosZ + sinX * sinZ);
    result.m[2][1] = scale.z * (cosX * sinY * sinZ - sinX * cosZ);
    result.m[2][2] = scale.z * (cosX * cosY);
    result.m[2][3] = 0.0f;

    result.m[3][0] = translate.x;
    result.m[3][1] = translate.y;
    result.m[3][2] = translate.z;
    result.m[3][3] = 1.0f;

    return result;
}

Matrix4x4 MakeLookAtMatrix(const Vector3& eye, const Vector3& target, const Vector3& up)
{
    // 視線方向（カメラのZ軸）
    Vector3 zAxis = Normalize(Subtract(target, eye));
    // 右方向（カメラのX軸）
    Vector3 xAxis = Normalize(Cross(up, zAxis));
    // 上方向（カメラのY軸）
    Vector3 yAxis = Cross(zAxis, xAxis);

    // 行優先（または列優先）に応じて要調整
    Matrix4x4 result = {
        xAxis.x, yAxis.x, zAxis.x, 0.0f,
        xAxis.y, yAxis.y, zAxis.y, 0.0f,
        xAxis.z, yAxis.z, zAxis.z, 0.0f,
        -Dot(xAxis, eye), -Dot(yAxis, eye), -Dot(zAxis, eye), 1.0f
    };

    return result;
}

//================================================
// 　レンダリングパイプライン用
//================================================

// 透視投影行列
Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
    Matrix4x4 result;

    result.m[0][0] = 1.0f / (aspectRatio * std::tanf(fovY / 2.0f));
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = 1.0f / std::tanf(fovY / 2.0f);
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = farClip / (farClip - nearClip);
    result.m[2][3] = 1.0f;

    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = -nearClip * farClip / (farClip - nearClip);
    result.m[3][3] = 0.0f;

    return result;
}

// 正射影行列
Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
    Matrix4x4 result;

    result.m[0][0] = 2.0f / (right - left);
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = 2.0f / (top - bottom);
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = 1.0f / (farClip - nearClip);
    result.m[2][3] = 0.0f;

    result.m[3][0] = -(right + left) / (right - left);
    result.m[3][1] = -(top + bottom) / (top - bottom);
    result.m[3][2] = -nearClip / (farClip - nearClip);
    result.m[3][3] = 1.0f;

    return result;
}

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{

    Matrix4x4 result;

    result.m[0][0] = width / 2.0f;
    result.m[0][1] = 0.0f;
    result.m[0][2] = 0.0f;
    result.m[0][3] = 0.0f;

    result.m[1][0] = 0.0f;
    result.m[1][1] = -height / 2.0f;
    result.m[1][2] = 0.0f;
    result.m[1][3] = 0.0f;

    result.m[2][0] = 0.0f;
    result.m[2][1] = 0.0f;
    result.m[2][2] = maxDepth - minDepth;
    result.m[2][3] = 0.0f;

    result.m[3][0] = left + width / 2.0f;
    result.m[3][1] = top + height / 2.0f;
    result.m[3][2] = minDepth;
    result.m[3][3] = 1.0f;

    return result;
}

// ベクトルの射影
Vector3 Project(const Vector3& v1, const Vector3 v2)
{
    Vector3 result;

    float dot = Dot(v1, v2);
    float v2LengthSq = Dot(v2, v2);

    float scalar = dot / v2LengthSq;

    result.x = scalar * v2.x;
    result.y = scalar * v2.y;
    result.z = scalar * v2.z;

    return result;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
    Vector3 result;

    // ABのベクトル
    Vector3 ab = {
        segment.diff.x,
        segment.diff.y,
        segment.diff.z
    };

    // APのベクトル
    Vector3 ap = {
        point.x - segment.origin.x,
        point.y - segment.origin.y,
        point.z - segment.origin.z
    };

    // APベクトルの長さ
    Vector3 proj = Project(ap, ab);

    // スカラー値tを求める
    float abLengthSq = Dot(ab, ab);
    float t = Dot(ap, ab) / abLengthSq;

    // tを0~1にクランプ
    if (t < 0.0f) {
        t = 0.0f;
    }
    if (t > 1.0f) {
        t = 1.0f;
    }
    /*std::clamp
        #inc <algo*/

    // ABベクトルにスカラーtを掛けて、ABの始点に足す
    result.x = segment.origin.x + ab.x * t;
    result.y = segment.origin.y + ab.y * t;
    result.z = segment.origin.z + ab.z * t;

    return result;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
{
    Vector3 result;
    // v1とv2の間をtの割合で補間
    result.x = v1.x + (v2.x - v1.x) * t;
    result.y = v1.y + (v2.y - v1.y) * t;
    result.z = v1.z + (v2.z - v1.z) * t;
    return result;
}

// デバッグ用関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix)
{

    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {
            Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
        }
    }
}

void VectorScreenPrintf(int x, int y, const Vector3& vector)
{
    Novice::ScreenPrintf(x, y, "%.02f", vector.x);
    Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
    Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
}

// 球描画用(後で分ける)
//   グリッドを描画する
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
    // グリッド半分の幅
    const float kGridHalfWidth = 2.0f;
    // 分割数
    const uint32_t kSubDivision = 10;
    // １つ分の長さ
    const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubDivision);

    // 奥から手前への線を順番に描画
    for (uint32_t i = 0; i <= kSubDivision; ++i) {
        // 上の情報を使ってワールド座標系上の始点と終点を計算

        Vector3 start = { -kGridHalfWidth + kGridEvery * i, 0.0f, -kGridHalfWidth };
        Vector3 end = { -kGridHalfWidth + kGridEvery * i, 0.0f, kGridHalfWidth };

        Vector3 ndcStart = TransformCoord(start, viewProjectionMatrix);
        Vector3 ndcEnd = TransformCoord(end, viewProjectionMatrix);

        // スクリーン座標系まで掛ける
        Vector3 startScreen = TransformCoord(ndcStart, viewportMatrix);
        Vector3 endScreen = TransformCoord(ndcEnd, viewportMatrix);

        // 変換した座標を使い、線を描画
        Novice::DrawLine(
            int(startScreen.x), int(startScreen.y),
            int(endScreen.x), int(endScreen.y),
            0xAAAAAAFF);

        if (i == 5) {
            Novice::DrawLine(
                int(startScreen.x), int(startScreen.y),
                int(endScreen.x), int(endScreen.y),
                0x000000FF);
        }
    }

    for (uint32_t i = 0; i <= kSubDivision; ++i) {
        // 上の情報を使ってワールド座標系上の始点と終点を計算

        Vector3 start = { -kGridHalfWidth, 0.0f, -kGridHalfWidth + kGridEvery * i };
        Vector3 end = { kGridHalfWidth, 0.0f, -kGridHalfWidth + kGridEvery * i };

        Vector3 ndcStart = TransformCoord(start, viewProjectionMatrix);
        Vector3 ndcEnd = TransformCoord(end, viewProjectionMatrix);

        // スクリーン座標系まで掛ける
        Vector3 startScreen = TransformCoord(ndcStart, viewportMatrix);
        Vector3 endScreen = TransformCoord(ndcEnd, viewportMatrix);

        // 変換した座標を使い、線を描画
        Novice::DrawLine(
            int(startScreen.x), int(startScreen.y),
            int(endScreen.x), int(endScreen.y),
            0xAAAAAAFF);

        if (i == 5) {
            Novice::DrawLine(
                int(startScreen.x), int(startScreen.y),
                int(endScreen.x), int(endScreen.y),
                0x000000FF);
        }
    }
}

// スフィアを描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    // 球の分割数
    const uint32_t kSubDivision = 20;

    // 経度分割１つ分の角度
    const float kLongEvery = 2.0f * std::numbers::pi_v<float> / static_cast<float>(kSubDivision);

    // 緯度分割１つ分の角度
    const float kLatEvery = std::numbers::pi_v<float> / static_cast<float>(kSubDivision);

    // 緯度の方向に分割 -π/2 ~ π/2
    for (uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex) {

        // 現在の緯度
        float lat = -std::numbers::pi_v<float> / 2.0f + kLatEvery * latIndex;

        // 経度の方向に分割 0 ~ 2π
        for (uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {

            // 現在の経度
            float lon = kLongEvery * lonIndex;

            // 各頂点の計算
            Vector3 a, b, c;

            a.x = std::cos(lat) * std::cos(lon) * sphere.radius + sphere.center.x;
            a.y = std::sin(lat) * sphere.radius + sphere.center.y;
            a.z = std::cos(lat) * std::sin(lon) * sphere.radius + sphere.center.z;

            b.x = std::cos(lat + kLatEvery) * std::cos(lon) * sphere.radius + sphere.center.x;
            b.y = std::sin(lat + kLatEvery) * sphere.radius + sphere.center.y;
            b.z = std::cos(lat + kLatEvery) * std::sin(lon) * sphere.radius + sphere.center.z;

            c.x = sphere.radius * std::cos(lat) * std::cos(lon + kLongEvery) + sphere.center.x;
            c.y = sphere.radius * std::sin(lat) + sphere.center.y;
            c.z = sphere.radius * std::cos(lat) * std::sin(lon + kLongEvery) + sphere.center.z;

            // ビュー座標系に変換
            Vector3 aView = TransformCoord(a, viewProjectionMatrix);
            Vector3 bView = TransformCoord(b, viewProjectionMatrix);
            Vector3 cView = TransformCoord(c, viewProjectionMatrix);

            // スクリーン座標系に変換
            Vector3 aScreen = TransformCoord(aView, viewportMatrix);
            Vector3 bScreen = TransformCoord(bView, viewportMatrix);
            Vector3 cScreen = TransformCoord(cView, viewportMatrix);

            // 線を描画
            Novice::DrawLine(int(aScreen.x), int(aScreen.y), int(bScreen.x), int(bScreen.y), color);
            Novice::DrawLine(int(aScreen.x), int(aScreen.y), int(cScreen.x), int(cScreen.y), color);
        }
    }
}
