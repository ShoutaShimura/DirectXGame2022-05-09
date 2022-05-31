#include "PlayerBullet.h"

PlayerBullet::PlayerBullet()
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(Model* model, const Vector3& position)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("black.png");

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をリセット
	worldTransform_.translation_ = position;

}

void PlayerBullet::Update()
{
	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	Matrix4 matScale;
	matScale.MatScale(worldTransform_.scale_);
	Matrix4 matRotZ;
	matRotZ.MatRotZ(worldTransform_.rotation_.z);
	Matrix4 matRotX;
	matRotX.MatRotX(worldTransform_.rotation_.x);
	Matrix4 matRotY;
	matRotY.MatRotY(worldTransform_.rotation_.y);
	Matrix4 matTrans;
	matTrans.MatTrans(worldTransform_.translation_);

	worldTransform_.matWorld_ *= matScale;
	worldTransform_.matWorld_ *= matRotZ;
	worldTransform_.matWorld_ *= matRotX;
	worldTransform_.matWorld_ *= matRotY;
	worldTransform_.matWorld_ *= matTrans;

	worldTransform_.TransferMatrix();
}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
