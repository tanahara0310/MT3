#include "Class/MyMath/MyCollision.h"
#include "Class/MyMath/MyMath.h"
#include <Novice.h>
#include <imgui.h>

const char kWindowTitle[] = "LE2B_18_タナハラ_コア_タイトル";

/// <summary>
/// ばね構造体
/// </summary>
struct Spring {
    // アンカー。固定された端の位置
    Vector3 anchor;
    float naturalLength; // 自然長
    float stiffness; // ばね定数k
    float dampingCoefficient; // 減衰係数
};
/// <summary>
/// ボール構造体
/// </summary>
struct Ball {
    Vector3 position; // 位置
    Vector3 velocity; // 速度
    Vector3 acceleration; // 加速度
    float mass; // 質量
    float radius; // 半径
    unsigned int color; // 色
};

struct Pendulum {
    Vector3 anchor; // アンカーポイント。固定された端の位置
    float length; // ひもの長さ
    float angle; // 現在の角度
    float angularVelocity; // 角速度
    float angularAcceleration; // 角加速度
};

struct ConicalPendulum {
    Vector3 anchor; // アンカーポイント。固定された端の位置
    float length; // ひもの長さ
    float halfApexAngle; // 円錐の頂角の半分
    float angle; // 現在の角度
    float angularVelocity; // 角速度
};

//==============================
// 関数定義
//==============================

// デバッグカメラ
struct DebugCamera {
    float distance = 6.0f;
    float pitch = 0.0f; // 上下
    float yaw = std::numbers::pi_v<float>; // 左右
    Vector3 target = { 0.0f, 0.7f, 0.0f }; // 注視点

    bool draggingLeft = false;
    bool draggingMiddle = false;
    ImVec2 prevMousePos = { 0.0f, 0.0f };

    /// <summary>
    /// カメラリセット
    /// </summary>
    void Reset()
    {
        distance = 6.0f;
        pitch = 0.0f;
        yaw = std::numbers::pi_v<float>;
        target = { 0.0f, 0.7f, 0.0f };
    }

    Vector3 GetPosition() const
    {
        return {
            target.x + distance * cosf(pitch) * sinf(yaw),
            target.y + distance * sinf(pitch),
            target.z + distance * cosf(pitch) * cosf(yaw)
        };
    }

    // View行列を作成
    Matrix4x4 GetViewMatrix() const
    {
        return MakeLookAtMatrix(GetPosition(), target, { 0.0f, 1.0f, 0.0f });
    }

    /// <summary>
    /// デバッグカメラの更新
    /// </summary>
    void UpdateFromImGui()
    {
        ImGuiIO& io = ImGui::GetIO();

        // マウス入力対象外のUI上では無視
        if (ImGui::IsAnyItemActive() || io.WantCaptureMouse) {
            draggingLeft = false;
            draggingMiddle = false;
            return;
        }

        // ドラッグ状態更新
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
            draggingLeft = true;
        } else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            draggingLeft = false;
        }

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Middle)) {
            draggingMiddle = true;
        } else if (!ImGui::IsMouseDown(ImGuiMouseButton_Middle)) {
            draggingMiddle = false;
        }

        // ドラッグによる回転
        if (draggingLeft) {
            ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left);
            yaw -= delta.x * 0.01f;
            pitch -= delta.y * 0.01f;
            pitch = std::clamp(pitch, -1.5f, 1.5f);
            ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
        }

        // ドラッグによるパン（視点の移動）
        if (draggingMiddle) {
            ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Middle);
            Vector3 forward = {
                cosf(pitch) * sinf(yaw),
                sinf(pitch),
                cosf(pitch) * cosf(yaw)
            };
            Vector3 right = Normalize(Cross({ 0.0f, 1.0f, 0.0f }, forward));
            Vector3 up = Normalize(Cross(forward, right));

            target = Add(target, Multiply(delta.x * 0.01f, right));
            target = Add(target, Multiply(-delta.y * 0.01f, up));
            ImGui::ResetMouseDragDelta(ImGuiMouseButton_Middle);
        }

        // ホイールによるズーム
        distance -= io.MouseWheel * 0.5f;
        distance = std::clamp(distance, 1.0f, 50.0f);
    }
};

// 平面の描画
Vector3 perpendicular(const Vector3& vector);

// 平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionmatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// AABBの描画
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// 2次ベジェ曲線の描画
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPosint2,
    const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

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

#pragma region カメラ関連初期化
    Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
    Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };

    // カメラのワールド行列
    Matrix4x4 cameraWorldMatrix = makeAffineMatrix({ 1.0, 1.0f, 1.0f }, { 0.26f, 0.0f, 0.0f }, { 0.0f, 1.9f, -6.49f });
    Matrix4x4 viewMatrix = Inverse(cameraWorldMatrix);
    Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(1280) / float(720), 0.1f, 100.0f);
    Matrix4x4 viewPortMatrix = MakeViewportMatrix(0.0f, 0.0f, 1280.0f, 720.0f, 0.0f, 1.0f);
    Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

    // デバッグカメラの初期化
    DebugCamera debugCamera;

#pragma endregion

#pragma region 円錐振り子初期化

    ConicalPendulum conicalPendulum;
    conicalPendulum.anchor = { 0.0f, 1.0f, 0.0f }; // アンカーポイント
    conicalPendulum.length = 0.8f; // ひもの長さ
    conicalPendulum.halfApexAngle = 0.7f; // 円錐の頂角の半分
    conicalPendulum.angle = 0.0f; // 現在の角度
    conicalPendulum.angularVelocity = 0.0f; // 角速度

    Sphere bob;
    bob.center.x = conicalPendulum.anchor.x + std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
    bob.center.y = conicalPendulum.anchor.y - std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length; // 上方向が負のY軸
    bob.center.z = conicalPendulum.anchor.z;
    bob.radius = 0.08f; // 振り子の玉の半径

    bool isStarted = false; // シミュレーション開始フラグ

#pragma endregion

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

        float deltaTime = 1.0f / 60.0f;

#pragma region 振り子更新

        if (isStarted) {

            conicalPendulum.angularVelocity = std::sqrt(9.8f / (conicalPendulum.length * std::cos(conicalPendulum.halfApexAngle)));
            conicalPendulum.angle += conicalPendulum.angularVelocity * deltaTime;

            float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
            float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;

            // 振り子の玉の位置を更新
            bob.center.x = conicalPendulum.anchor.x + radius * std::sin(conicalPendulum.angle);
            bob.center.y = conicalPendulum.anchor.y - height;
            bob.center.z = conicalPendulum.anchor.z - radius * std::cos(conicalPendulum.angle);
        }

#pragma endregion

#pragma region カメラ更新

        // デバッグカメラの更新
        debugCamera.UpdateFromImGui();

        viewMatrix = debugCamera.GetViewMatrix();
        viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);

#pragma endregion

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

#pragma region imgui
        // デバッグカメラの位置と注視点を表示
        ImGui::Begin("Debug Camera");
        ImGui::Text("Position: (%.2f, %.2f, %.2f)", debugCamera.GetPosition().x, debugCamera.GetPosition().y, debugCamera.GetPosition().z);
        ImGui::Text("Target: (%.2f, %.2f, %.2f)", debugCamera.target.x, debugCamera.target.y, debugCamera.target.z);
        ImGui::Text("Distance: %.2f", debugCamera.distance);
        ImGui::Text("Pitch: %.2f", debugCamera.pitch);
        ImGui::Text("Yaw: %.2f", debugCamera.yaw);

        // カメラのリセットボタン
        if (ImGui::Button("Reset Camera")) {
            debugCamera.Reset();
        }

        // シミュレーション開始ボタン
        if (ImGui::Button("Start Simulation")) {
            isStarted = true;
        }

        ImGui::End();

#pragma endregion

        // 振り子の線の描画
        Vector3 screenAnchor = TransformCoord(TransformCoord(conicalPendulum.anchor, viewProjectionMatrix), viewPortMatrix);
        Vector3 screenBob = TransformCoord(TransformCoord(bob.center, viewProjectionMatrix), viewPortMatrix);

        Novice::DrawLine(
            int(screenAnchor.x), int(screenAnchor.y),
            int(screenBob.x), int(screenBob.y), WHITE);

        // 振り子の玉の描画
        DrawSphere(bob, viewProjectionMatrix, viewPortMatrix, WHITE);

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

// 2次ベジェ曲線の描画
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPosint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
    const int kSegmentCount = 20; // 分割数

    for (int i = 0; i < kSegmentCount; ++i) {
        float t0 = static_cast<float>(i) / static_cast<float>(kSegmentCount);
        float t1 = static_cast<float>(i + 1) / static_cast<float>(kSegmentCount);

        // 線形補間を使ってベジェ曲線の2点を計算
        Vector3 p0p1 = Lerp(controlPoint0, controlPoint1, t0);
        Vector3 p1p2 = Lerp(controlPoint1, controlPosint2, t0);
        Vector3 p = Lerp(p0p1, p1p2, t0);

        Vector3 p0p1Next = Lerp(controlPoint0, controlPoint1, t1);
        Vector3 p1p2Next = Lerp(controlPoint1, controlPosint2, t1);
        Vector3 pNext = Lerp(p0p1Next, p1p2Next, t1);

        // ベジェ曲線の2点をスクリーン座標に変換
        Vector3 screeenP0 = TransformCoord(TransformCoord(p, viewProjectionMatrix), viewportMatrix);
        Vector3 screeenP1 = TransformCoord(TransformCoord(pNext, viewProjectionMatrix), viewportMatrix);

        // 線を描画
        Novice::DrawLine(
            static_cast<int>(screeenP0.x), static_cast<int>(screeenP0.y),
            static_cast<int>(screeenP1.x), static_cast<int>(screeenP1.y),
            color);
    }
}
