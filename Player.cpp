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

	//�L�����N�^�[�U������
	Attack();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf("Pos={%f,%f,%f}", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void
Player::Draw(ViewProjection& viewProjection)
{
	//3D���f����`��
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet:bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };

	const float speed = 0.5f;

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

	Rotate();

	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//�s��X�V
	worldTransform_.TransferMatrix();
}

void Player::Rotate()
{

	const float speed = 0.05f;

	//�ړ��x�N�g����ύX���鏈��
	if (input_->PushKey(DIK_U)) {
		worldTransform_.rotation_.y -= speed;
	}
	else if (input_->PushKey(DIK_I)) {
		worldTransform_.rotation_.y += speed;
	}


}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE)) {

		//�e�𐶐����A������
		//PlayerBullet* newBullet = new PlayerBullet();
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));
	}

}
