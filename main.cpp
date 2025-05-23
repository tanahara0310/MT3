#include "Class/MyMath/MyCollision.h"
#include "Class/MyMath/MyMath.h"
#include <Novice.h>
#include <imgui.h>

const char kWindowTitle[] = "LE2B_18_タナハラ_コア_タイトル";

//==============================
// 関数定義
//==============================

// 平面の描画
Vector3 perpendicular(const Vector3& vector);

// 平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionmatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// AABBの描画
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    //==============================
    // 変数初期化
    //==============================

    Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
    Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };

    // カメラのワールド行列
    Matrix4x4 cameraWorldMatrix = makeAffineMatrix({ 1.0, 1.0f, 1.0f }, { 0.26f, 0.0f, 0.0f }, { 0.0f, 1.9f, -6.49f });
    Matrix4x4 viewMatrix = Inverse(cameraWorldMatrix);
    Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
    Matrix4x4 viewPortMatrix = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);
    Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

    AABB aabb1 = {
        .min { -0.5f, -0.5f, -0.5f },
        .max { 0.0f, 0.0f, 0.0f }
    };

    AABB aabb2 = {
        .min { 0.2f, 0.2f, 0.2f },
        .max { 1.0f, 1.0f, 1.0f }
    };

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        // カメラや球の位置を更新
        ImGui::Begin("Object");
        ImGui::Separator();
        ImGui::Text("Camera");
        ImGui::SliderFloat3("Translate", &cameraTranslate.x, -5.0f, 5.0f, "%.2f");
        ImGui::SliderFloat3("Rotation Angle", &cameraRotate.x, -0.52f, 0.52f, "%.2f");

        // AABBの更新
        ImGui::Separator();
        ImGui::Text("AABB1");
        ImGui::DragFloat3("AABB1 Min", &aabb1.min.x, 0.01f, -5.0f, 5.0f, "%.2f");
        ImGui::DragFloat3("AABB1 Max", &aabb1.max.x, 0.01f, -5.0f, 5.0f, "%.2f");
        ImGui::Separator();
        ImGui::Text("AABB2");
        ImGui::DragFloat3("AABB2 Min", &aabb2.min.x, 0.01f, -5.0f, 5.0f, "%.2f");
        ImGui::DragFloat3("AABB2 Max", &aabb2.max.x, 0.01f, -5.0f, 5.0f, "%.2f");

        // リセットボタン
        if (ImGui::Button("Reset")) {
            cameraTranslate = { 0.0f, 1.9f, -6.49f };
            cameraRotate = { 0.26f, 0.0f, 0.0f };

            aabb1.min = { -0.5f, -0.5f, -0.5f };
            aabb1.max = { 0.0f, 0.0f, 0.0f };

            aabb2.min = { 0.2f, 0.2f, 0.2f };
            aabb2.max = { 1.0f, 1.0f, 1.0f };
        }
        ImGui::End();

        cameraWorldMatrix = makeAffineMatrix({ 1.0, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
        viewMatrix = Inverse(cameraWorldMatrix);
        viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        // AABB1の描画
        if (IsCollision(aabb1, aabb2)) {
            DrawAABB(aabb1, viewProjectionMatrix, viewPortMatrix, RED);
        } else {
            DrawAABB(aabb1, viewProjectionMatrix, viewPortMatrix, WHITE);
        }

        // AABB2の描画
        DrawAABB(aabb2, viewProjectionMatrix, viewPortMatrix, WHITE);

        // グリッド線
        DrawGrid(viewProjectionMatrix, viewPortMatrix);

        ///
        /// ↑描画処理ここまで
        ///

        // フレームの終了
        Novice::EndFrame();

        // ESCキーが押されたらループを抜ける
        if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
            break;
        }
    }

    // ライブラリの終了
    Novice::Finalize();
    return 0;
}

Vector3 perpendicular(const Vector3& vector)
{
    if (vector.x != 0.0f || vector.y != 0.0f) {
        return { -vector.y, vector.x, 0.0f };
    }

    return { 0.0f, -vector.z, vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    Vector3 center = Multiply(plane.distance, plane.normal); // 1
    Vector3 perpendiculars[4];

    perpendiculars[0] = Normalize(perpendicular(plane.normal)); // 2
    perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
    perpendiculars[2] = Cross(plane.normal, perpendiculars[0]); // 4
    perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z }; // 5

    // 6
    Vector3 points[4];
    for (int32_t index = 0; index < 4; ++index) {
        Vector3 extend = Multiply(2.0f, perpendiculars[index]);
        Vector3 point = Add(center, extend);
        points[index] = TransformCoord(TransformCoord(point, viewProjectionMatrix), viewportMatrix);
    }

    Novice::DrawLine(
        int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);

    Novice::DrawLine(
        int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);

    Novice::DrawLine(
        int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);

    Novice::DrawLine(
        int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    Vector3 points[3];

    for (int i = 0; i < 3; ++i) {
        points[i] = TransformCoord(TransformCoord(triangle.vertices[i], viewProjectionMatrix), viewportMatrix);
    }

    Novice::DrawLine((int)points[0].x, (int)points[0].y, (int)points[1].x, (int)points[1].y, color);
    Novice::DrawLine((int)points[1].x, (int)points[1].y, (int)points[2].x, (int)points[2].y, color);
    Novice::DrawLine((int)points[2].x, (int)points[2].y, (int)points[0].x, (int)points[0].y, color);
}

// AABBの描画
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    // 1) ８頂点を world 空間で用意
    Vector3 corners[8] = {
        { aabb.min.x, aabb.min.y, aabb.min.z }, // 0
        { aabb.max.x, aabb.min.y, aabb.min.z }, // 1
        { aabb.max.x, aabb.max.y, aabb.min.z }, // 2
        { aabb.min.x, aabb.max.y, aabb.min.z }, // 3
        { aabb.min.x, aabb.min.y, aabb.max.z }, // 4
        { aabb.max.x, aabb.min.y, aabb.max.z }, // 5
        { aabb.max.x, aabb.max.y, aabb.max.z }, // 6
        { aabb.min.x, aabb.max.y, aabb.max.z } // 7
    };

    // 2) 各頂点をクリップ→スクリーン座標に変換
    Vector3 pts[8];
    for (int i = 0; i < 8; ++i) {
        // ビュー→プロジェクション→ビューポート の順で呼び出す
        pts[i] = TransformCoord(
            TransformCoord(corners[i], viewProjectionMatrix),
            viewportMatrix);
    }

    // 3) 底面（0-1-2-3）
    Novice::DrawLine((int)pts[0].x, (int)pts[0].y, (int)pts[1].x, (int)pts[1].y, color);
    Novice::DrawLine((int)pts[1].x, (int)pts[1].y, (int)pts[2].x, (int)pts[2].y, color);
    Novice::DrawLine((int)pts[2].x, (int)pts[2].y, (int)pts[3].x, (int)pts[3].y, color);
    Novice::DrawLine((int)pts[3].x, (int)pts[3].y, (int)pts[0].x, (int)pts[0].y, color);

    // 4) 上面（4-5-6-7）
    Novice::DrawLine((int)pts[4].x, (int)pts[4].y, (int)pts[5].x, (int)pts[5].y, color);
    Novice::DrawLine((int)pts[5].x, (int)pts[5].y, (int)pts[6].x, (int)pts[6].y, color);
    Novice::DrawLine((int)pts[6].x, (int)pts[6].y, (int)pts[7].x, (int)pts[7].y, color);
    Novice::DrawLine((int)pts[7].x, (int)pts[7].y, (int)pts[4].x, (int)pts[4].y, color);

    // 5) 側面のエッジ（0-4, 1-5, 2-6, 3-7）
    Novice::DrawLine((int)pts[0].x, (int)pts[0].y, (int)pts[4].x, (int)pts[4].y, color);
    Novice::DrawLine((int)pts[1].x, (int)pts[1].y, (int)pts[5].x, (int)pts[5].y, color);
    Novice::DrawLine((int)pts[2].x, (int)pts[2].y, (int)pts[6].x, (int)pts[6].y, color);
    Novice::DrawLine((int)pts[3].x, (int)pts[3].y, (int)pts[7].x, (int)pts[7].y, color);
}
