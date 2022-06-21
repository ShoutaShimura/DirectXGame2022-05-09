#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cassert>
#include"DebugText.h"
#include "BaseEnemyState.h"
#include "EnemyStateApproach.h"
#include"EnemyStateLeave.h"

/// <summary>
/// �G
/// </summary>
class Enemy
{
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="position">�������W</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="viewProjection">�r���[�v���W�F�N�V����</param>
	void Draw(const ViewProjection& viewProjection);

	void Approach();
	void Leave();

	void changeState(BaseEnemyState* newState);


private:

	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//�s���t�F�[�Y
	enum class Phase {
		Approach,//�ڋ߂���
		Leave,//���E����
	};

	//�t�F�[�Y
	Phase phase_ = Phase::Approach;

	BaseEnemyState* state_;

	void Move(float x, float y, float z);
	Vector3 GetPos() { return worldTransform_.translation_; }


	//�f�o�b�N�e�L�X�g
	DebugText* debugText_ = nullptr;
};

