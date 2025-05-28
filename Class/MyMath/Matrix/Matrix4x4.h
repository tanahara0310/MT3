#pragma once

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