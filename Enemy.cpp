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

	ApproachReset();
}

void Enemy::Update()
{
	switch (phase_)
	{
	case Enemy::Phase::Approach:
	default:
		Approach();
		break;
	case Enemy::Phase::Leave:
		Leave();
		break;
	}

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}


	//行列を更新
	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	/*velocity_ = { 0,0,-0.5f };*/
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
	const float kBulletSpeed = 2.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	Matrix4 mat;
	//速度ベクトルを自機の向きに合わせて回転させる
	velocity = mat.VecMat(velocity, worldTransform_.matWorld_);

	//弾を生成し、初期化
	//PlayerBullet* newBullet = new PlayerBullet();
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//弾を登録する
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachReset()
{
	//発射タイマーを初期化
	fireTimer = kFireInterval;
}
