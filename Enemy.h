#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "EnemyBullet.h"
#include "Matrix4.h"
#include <memory>
#include <list>
#include <cassert>

/// <summary>
/// 敵
/// </summary>
class Enemy
{
public:
	Enemy();
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	void Approach();
	void Leave();

private:

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();
public:

	//発射間隔
	static const int kFireInterval = 60;

	//接近フェーズ初期化
	void ApproachReset();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//行動フェーズ
	enum class Phase {
		Approach,//接近する
		Leave,//離脱する
	};

	//フェーズ
	Phase phase_ = Phase::Approach;

	//弾
	std::list<std::unique_ptr<EnemyBullet>> bullets_;

	//発射タイマー
	int32_t fireTimer = 0;
};

