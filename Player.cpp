#include "Player.h"


Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULL�|�C���^�`�F�b�N
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	//�ړ�
	Move();
	
	
	
	debugText_->SetPos(50, 50);
	debugText_->Printf("Pos={%f,%f,%f}", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void
Player::Draw(ViewProjection& viewProjection)
{
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}

void Player::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	const float speed = 0.2f;

	//�ړ��x�N�g����ύX���鏈��
	if (input_->PushKey(DIK_LEFT)) {
		move = { -speed,0,0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { speed,0,0 };
	}

	if (input_->PushKey(DIK_UP)) {
		move = { 0,speed,0 };
	}
	else if (input_->PushKey(DIK_DOWN)) {
		move = { 0,-speed,0 };
	}

	//���W�ړ��i�x�N�g���̉��Z�j
	worldTransform_.translation_ += move;

	//�ړ����E���W
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	//�͈͂𒴂��Ȃ�����
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	Matrix4 matTrans;
	matTrans.MatTrans(worldTransform_.translation_);

	worldTransform_.matWorld_ *= matTrans;

	//�s��X�V
	worldTransform_.TransferMatrix();
}
