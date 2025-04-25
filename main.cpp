#include "Collision.h"
#include "MyMath.h"
#include <Novice.h>
#include <imgui.h>

const char kWindowTitle[] = "LC1A_16_タナハラ_コア_タイトル";

struct Vector2 {
    float x, y;
};

//==============================
// 関数定義
//==============================

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

    Sphere sphere2;
    sphere2.center = { 1.5f, 0.0f, 0.0f };
    sphere2.radius = 0.5f;

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

        // 球2個目の描画
        ImGui::Separator();
        ImGui::Text("Sphere2");
        ImGui::SliderFloat3("Center2", &sphere2.center.x, -1.0f, 1.0f, "%.2f");
        ImGui::SliderFloat("Radius2", &sphere2.radius, 0.1f, 1.0f, "%.2f");

        // リセットボタン
        if (ImGui::Button("Reset")) {
            cameraTranslate = { 0.0f, 1.9f, -6.49f };
            cameraRotate = { 0.26f, 0.0f, 0.0f };

            sphere1.center = { 0.0f, 0.0f, 0.0f };
            sphere1.radius = 0.5f;

            sphere2.center = { 1.5f, 0.0f, 0.0f };
            sphere2.radius = 0.5f;
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
        // 球の衝突判定
        if (isCollision(sphere1, sphere2)) {
            DrawSphere(sphere1, (viewMatrix * projectionMatrix), viewPortMatrix, 0xFF0000FF);
        } else {
            DrawSphere(sphere1, (viewMatrix * projectionMatrix), viewPortMatrix, 0xFFFFFFFF);
        }

        // 球二個目の描画
        DrawSphere(sphere2, (viewMatrix * projectionMatrix), viewPortMatrix, 0xFFFFFFFF);

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
