#include "EnemyBullet.h"

EnemyBullet::EnemyBullet()
{
}

EnemyBullet::~EnemyBullet()
{
}

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	//�e�N�X�`���ǂݍ���
	textureHandle_ = TextureManager::Load("black.png");

	//���[���h�g�����X�t�H�[���̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W�����Z�b�g
	worldTransform_.translation_ = position;

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

void EnemyBullet::Update()
{
	//���W���ړ�������i�P�t���[�����̈ړ��𑫂����ށj
	worldTransform_.translation_ += velocity_;

	//���Ԍo�߂Ńf�X
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	//�s����X�V

	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
