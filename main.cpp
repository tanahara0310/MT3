#include "Class/MyMath/MyMath.h"
#include "Collision.h"
#include <Novice.h>
#include <imgui.h>

const char kWindowTitle[] = "LC1A_16_タナハラ_コア_タイトル";

//==============================
// 関数定義
//==============================

// 平面と球の衝突判定
bool isCollision(const Sphere& sphere, const Plane& plane);

// 平面の描画
Vector3 perpendicular(const Vector3& vector);

// 平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionmatrix, const Matrix4x4& viewportMatrix, uint32_t color);

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

    Matrix4x4 cameraWorldMatrix = makeAffineMatrix({ 1.0, 1.0f, 1.0f }, { 0.26f, 0.0f, 0.0f }, { 0.0f, 1.9f, -6.49f });
    Matrix4x4 viewMatrix = Inverse(cameraWorldMatrix);
    Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
    Matrix4x4 viewPortMatrix = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);

    // 球の変数
    Sphere sphere1;
    sphere1.center = { 0.0f, 0.0f, 0.0f };
    sphere1.radius = 0.5f;

    Plane plane;
    plane.normal = { 0.0f, 1.0f, 0.0f };
    plane.distance = 1.0f;

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
        // 球の位置を更新
        ImGui::Separator();
        ImGui::Text("Sphere");
        ImGui::SliderFloat3("Center", &sphere1.center.x, -1.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Radius", &sphere1.radius, 0.1f, 1.0f, "%.2f");

        // リセットボタン
        if (ImGui::Button("Reset")) {
            cameraTranslate = { 0.0f, 1.9f, -6.49f };
            cameraRotate = { 0.26f, 0.0f, 0.0f };

            sphere1.center = { 0.0f, 0.0f, 0.0f };
            sphere1.radius = 0.5f;
        }
        ImGui::End();

        cameraWorldMatrix = makeAffineMatrix({ 1.0, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
        viewMatrix = Inverse(cameraWorldMatrix);

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        DrawGrid((viewMatrix * projectionMatrix), viewPortMatrix);

        // 球一個目の描画
        DrawSphere(sphere1, (viewMatrix * projectionMatrix), viewPortMatrix, WHITE);

        DrawPlane(plane, (viewMatrix * projectionMatrix), viewPortMatrix, WHITE);

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

    perpendiculars[0] = Normalize(perpendicular(plane.normal));  // 2
    perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };
    perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);  // 4
    perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z }; // 5

    // 6
    Vector3 points[4];
    for (int32_t index = 0; index < 4; ++index) {
        Vector3 extend = Multiply(2.0f, perpendiculars[index]);
        Vector3 point = Add(center, extend);
        points[index] = TransformCoord(TransformCoord(point, viewProjectionMatrix), viewportMatrix);
    }

    Novice::DrawLine(
        int(points[0].x), int(points[0].y), int(points[1].x), int(points[1].y), color);

    Novice::DrawLine(
        int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);

    Novice::DrawLine(
        int(points[3].x), int(points[3].y), int(points[2].x), int(points[2].y), color);

    Novice::DrawLine(
        int(points[2].x), int(points[2].y), int(points[0].x), int(points[0].y), color);
}
