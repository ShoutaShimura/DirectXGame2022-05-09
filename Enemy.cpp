#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Model* model, const Vector3& position)
{
	//NULLチェック
	assert(model);

	model_ = model;

	textureHandle_ = TextureManager::Load("enemy.png");

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

	debugText_ = DebugText::GetInstance();


	//接近フェーズ初期化
	ApproachReset();
}

void(Enemy::* Enemy::spFuncTable[])() = {
	&Enemy::Approach,
	&Enemy::Leave
};

void Enemy::Update()
{
	for (std::unique_ptr<EnemyBullet>& ebullet:ebullets_) {
		ebullet->Update();
	}

	(this->*spFuncTable[static_cast<size_t>(phase_)])();

	//行列を更新
	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	//デバック用表示
	debugText_->SetPos(50, 100);
	debugText_->Printf("enemy={%f,%f,%f}", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (std::unique_ptr<EnemyBullet>&ebullet:ebullets_) {
		ebullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	//velocity_ = { 0,0,-0.1f };
	//移動(ベクトルを加算)
	worldTransform_.translation_ += velocity_;

	//発射タイマーカウントダウン
	fireTimer--;
	//指定時間に達した
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = kFireInterval;
	}

}

void Enemy::Leave()
{
	//移動(ベクトルを加算)
	velocity_ = { 0.5f,0.5f,0 };
	worldTransform_.translation_ += velocity_;
}

void Enemy::Fire()
{
	//弾の速度
	const float kBulletSpeed = -2.0f;
	Vector3 bVelocity(0, 0, kBulletSpeed);

	Vector3 position = worldTransform_.translation_;

	//弾を生成し、初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_,position, bVelocity);

	//弾を登録する
	ebullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachReset()
{
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}
