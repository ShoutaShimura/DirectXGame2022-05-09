#include "Enemy.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize(Model* model, const Vector3& position)
{
	//NULL�`�F�b�N
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

	//�e�X�V
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}


	//�s����X�V
	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Approach()
{
	/*velocity_ = { 0,0,-0.5f };*/
	//�ړ�(�x�N�g�������Z)
	worldTransform_.translation_ += velocity_;

	//���˃^�C�}�[�J�E���g�_�E��
	fireTimer--;
	//�w�莞�ԂɒB����
	if (fireTimer <= 0) {
		//�e�𔭎�
		Fire();
		//���˃^�C�}�[��������
		fireTimer = kFireInterval;
	}

}

void Enemy::Leave()
{
	//�ړ�(�x�N�g�������Z)
	velocity_ = { 0.5f,0.5f,0 };
	worldTransform_.translation_ += velocity_;
}

void Enemy::Fire()
{
	//�e�̑��x
	const float kBulletSpeed = 2.0f;
	Vector3 velocity(0, 0, -kBulletSpeed);

	Matrix4 mat;
	//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
	velocity = mat.VecMat(velocity, worldTransform_.matWorld_);

	//�e�𐶐����A������
	//PlayerBullet* newBullet = new PlayerBullet();
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	//�e��o�^����
	bullets_.push_back(std::move(newBullet));
}

void Enemy::ApproachReset()
{
	//���˃^�C�}�[��������
	fireTimer = kFireInterval;
}
