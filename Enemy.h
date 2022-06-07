#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <cassert>
#include"DebugText.h"

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


	//デバックテキスト
	DebugText* debugText_ = nullptr;
};

