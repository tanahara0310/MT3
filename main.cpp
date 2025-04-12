#include "MyMath.h"
#include <Novice.h>
#include <imgui.h>

const char kWindowTitle[] = "LC1A_16_タナハラ_コア_タイトル";

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

    // カメラ用
    Matrix4x4 cameraWorldMatrix = makeAffineMatrix({ 1.0, 1.0f, 1.0f }, { 0.26f, 0.0f, 0.0f }, { 0.0f, 1.9f, -6.49f });
    Matrix4x4 viewMatrix = Inverse(cameraWorldMatrix);
    Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
    Matrix4x4 viewPortMatrix = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);

    //射影ベクトル
    Segment segment { { -2.0f, -1.0f, 0.0f }, { 3.0f, 2.0f, 2.0f } };
    Vector3 point { -1.5f, 0.6f, 0.6f };

    Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
    Vector3 closestPoint = ClosestPoint(point, segment);

    // 球の描画
    Sphere pointSphere { point, 0.01f };
    Sphere closestPointSphere { closestPoint, 0.01f };

    // 線の描画
    Vector3 start = TransformCoord(TransformCoord(segment.origin, (viewMatrix * projectionMatrix)), viewPortMatrix);
    Vector3 end = TransformCoord(TransformCoord(Add(segment.origin, segment.diff), (viewMatrix * projectionMatrix)), viewPortMatrix);

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

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        DrawGrid((viewMatrix * projectionMatrix), viewPortMatrix);

        DrawSphere(pointSphere, Multiply(viewMatrix, projectionMatrix), viewPortMatrix, RED);
        DrawSphere(closestPointSphere, Multiply(viewMatrix, projectionMatrix), viewPortMatrix, BLACK);

        Novice::DrawLine(
            int(start.x), int(start.y),
            int(end.x), int(end.y),
            WHITE);

        ImGui::InputFloat3("point", &point.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("segmentOrigin", &segment.origin.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
        ImGui::InputFloat3("segmentDiff", &segment.diff.x, "%.3f", ImGuiInputTextFlags_ReadOnly);

        ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);

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
