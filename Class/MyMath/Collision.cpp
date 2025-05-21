#include "Collision.h"
#include <math.h>

bool IsCollision(const Sphere& sphere, const Plane& plane)
{

    // 球の中心から平面までの距離を計算
    float distance = Dot(plane.normal, sphere.center) - plane.distance;

    // 絶対距離
    float dist = fabsf(distance);

    // 球の半径と距離を比較
    return dist <= sphere.radius;
}