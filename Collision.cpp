#include "Collision.h"

bool isCollision(const Sphere& s1, const Sphere& s2)
{

    // 2つの球の中心間のベクトルを求める
    float distance = Length(s2.center - s1.center);
    // 2つの球の半径の合計より短ければ衝突
    if (distance <= s1.radius + s2.radius) {
        return true;
    }

    return false;
}
