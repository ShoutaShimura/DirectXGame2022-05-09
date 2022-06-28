#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cassert>
#include "DebugText.h"
#include "EnemyBullet.h"
#include <memory> 
#include <list>

//自機クラスの前方宣言
class Player;

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

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	void SetPlayer(Player* player) { player_ = player; }
	
	//ワールド座標を取得
	Vector3 GetWorldPotision();

//接近フェーズ初期化
	void ApproachReset();

public:

	//発射間隔
	static const int kFireInterval = 60;

private:

	//弾
	std::list<std::unique_ptr<EnemyBullet>> ebullets_;

	//発射タイマー
	int32_t fireTimer = 0;

	//メンバ関数ポインタ
	static void(Enemy::* spFuncTable[])();

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

	//自キャラ
	Player* player_ = nullptr;

	//デバックテキスト
	DebugText* debugText_ = nullptr;

	
};

