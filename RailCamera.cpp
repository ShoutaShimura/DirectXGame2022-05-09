#include "RailCamera.h"

void RailCamera::Initialize(const Vector3& position, const Vector3& rotation)
{

	//�����̃��[���h���W�����[���h�g�����X�t�H�[���ɐݒ�
	worldTransform_.translation_ = position;

	//�����̉�]�p�m���W�A���n�����[���h�g�����X�t�H�[���ɐݒ�
	worldTransform_.rotation_ = rotation;

	//�r���[�v���W�F�N�V����
	viewProjection_.farZ = 2000.0f;
	viewProjection_.Initialize();

}

void RailCamera::Update()
{
	//�ړ�
	worldTransform_.translation_+= Vector3(0, 0, 0.1f);

	//�s����X�V
	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.eye.x = worldTransform_.matWorld_.m[3][0];
	viewProjection_.eye.y = worldTransform_.matWorld_.m[3][1];
	viewProjection_.eye.z = worldTransform_.matWorld_.m[3][2];

	//���[���h�O���x�N�g��
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
