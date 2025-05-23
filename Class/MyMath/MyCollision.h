#pragma once

#include "MyMath.h"

/// <summary>
/// 平面と球の衝突判定
/// </summary>
/// <param name="sphere"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Sphere& sphere, const Plane& plane);

/// <summary>
/// 平面と線分の衝突判定
/// </summary>
/// <param name="degment"></param>
/// <param name="plane"></param>
/// <returns></returns>
bool IsCollision(const Segment& degment, const Plane& plane);

/// <summary>
/// 三角形と線分の衝突判定
/// </summary>
/// <param name="triangle"></param>
/// <param name="segment"></param>
/// <returns></returns>
bool IsCollision(const Triangle& triangle, const Segment& segment);

/// <summary>
/// AABBとAABBの衝突判定
/// </summary>
/// <param name="aabb1"></param>
/// <param name="aabb2"></param>
/// <returns></returns>
bool IsCollision(const AABB& aabb1, const AABB& aabb2);

// 球とAABBの衝突
bool IsCollision(const Sphere& sphere, const AABB& aabb);

// AABBと線分の衝突
bool IsCollision(const AABB& aabb, const Segment& segment);