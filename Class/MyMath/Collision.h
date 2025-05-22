#pragma once

#include "MyMath.h"

/// <summary>
/// 平面と球の衝突判定
/// </summary>
/// <param name="sphere"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere, const Plane& plane);

// 平面と線の当たり判定
bool IsCollision(const Segment& degment, const Plane& plane);

// 三角形と線の描画
bool IsCollision(const Triangle& triangle, const Segment& segment);