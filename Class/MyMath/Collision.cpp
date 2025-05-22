#include "Collision.h"
#include <math.h>

// 平面と球の衝突判定
bool IsCollision(const Sphere& sphere, const Plane& plane)
{

    // 球の中心から平面までの距離を計算
    float distance = Dot(plane.normal, sphere.center) - plane.distance;

    // 絶対距離
    float dist = fabsf(distance);

    // 球の半径と距離を比較
    return dist <= sphere.radius;
}

// 平面と線分の衝突判定
bool IsCollision(const Segment& segment, const Plane& plane)
{
    // 垂直判定を行うために、法線と線の内積を求める
    float dot = Dot(plane.normal, segment.diff);

    // 内積が0の場合、平面と線分は平行
    if (dot == 0.0f) {
        return false;
    }

    // tを求める
    float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

    return (0.0f <= t && t <= 1.0f);
}

bool IsCollision(const Triangle& triangle, const Segment& segment)
{
    // 法線を求める
    Vector3 edge1 = triangle.vertices[1] - triangle.vertices[0];
    Vector3 edge2 = triangle.vertices[2] - triangle.vertices[0];
    Vector3 normal = Normalize(Cross(edge1, edge2));
    float d = Dot(normal, triangle.vertices[0]);

    // 線分ベクトル
    Vector3 dir = segment.diff;

    float dot = Dot(normal, dir);
    if (dot == 0.0f)
        return false; // 平行

    float t = (d - Dot(normal, segment.origin)) / dot;
    if (t < 0.0f || t > 1.0f)
        return false; // 線分上にない

    // 交点
    Vector3 p = segment.origin + dir * t;

    // 向きチェック
    Vector3 v0p = p - triangle.vertices[0];
    Vector3 v1p = p - triangle.vertices[1];
    Vector3 v2p = p - triangle.vertices[2];

    Vector3 cross01 = Cross(triangle.vertices[1] - triangle.vertices[0], v0p);
    Vector3 cross12 = Cross(triangle.vertices[2] - triangle.vertices[1], v1p);
    Vector3 cross20 = Cross(triangle.vertices[0] - triangle.vertices[2], v2p);

    if (Dot(cross01, normal) >= 0.0f && Dot(cross12, normal) >= 0.0f && Dot(cross20, normal) >= 0.0f) {
        return true;
    }

    return false;
}