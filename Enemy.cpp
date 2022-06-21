#include "Enemy.h"

Enemy::Enemy()
{
	state_ = new EnemyStateApproach();
}

Enemy::~Enemy()
{
	delete state_;
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

}


void Enemy::Update()
{
	
	//行列を更新
	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	//デバック用表示
	debugText_->SetPos(50, 100);
	debugText_->Printf("enemy={%f,%f,%f}",worldTransform_.translation_.x,worldTransform_.translation_.y,worldTransform_.translation_.z);

}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void Enemy::Approach()
{
	velocity_ = { 0,0,-0.5f };
	//移動(ベクトルを加算)
	worldTransform_.translation_ += velocity_;
	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave()
{
	//移動(ベクトルを加算)
	velocity_ = { 0.5f,0.5f,0 };
	worldTransform_.translation_ += velocity_;
}

void Enemy::changeState(BaseEnemyState* newState)
{
}

void Enemy::Move(float x, float y, float z)
{
	Vector3 vel = { x,y,z };

	worldTransform_.translation_ += vel;
}
