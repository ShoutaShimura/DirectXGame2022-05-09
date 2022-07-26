#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{

	//引数のワールド座標をワールドトランスフォームに設定
	worldTransform_.translation_ = position;

	//引数の回転角［ラジアン］をワールドトランスフォームに設定
	worldTransform_.rotation_ = rotation;

	//ビュープロジェクション
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

}

void RailCamera::Update()
{
	//移動
	worldTransform_.translation_+= Vector3(0, 0, 0.1f);

	//行列を更新
	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);

	forward = worldTransform_.matWorld_.VecMat(forward, worldTransform_.matWorld_);

	viewProjection_.target = viewProjection_.eye + forward;

	Vector3 up(0, 1, 0);

	viewProjection_.up = worldTransform_.matWorld_.VecMat(viewProjection_.up, worldTransform_.matWorld_);

	viewProjection_.UpdateMatrix();

	DebugText* debugText_ = DebugText::GetInstance();

	debugText_->SetPos(80, 100);
	debugText_->Printf("RailCamera Pos={%f,%f,%f}", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);

	
}
