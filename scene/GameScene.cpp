#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include <math.h>
#include "Matrix4.h"
#include <random> 

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	//キャラクターの大本
	worldTransforms_[PartId::kRoot].Initialize();


	//脊椎
	worldTransforms_[PartId::kSpine].Initialize();
	worldTransforms_[PartId::kSpine].parent_ = &worldTransforms_[PartId::kRoot];
	worldTransforms_[PartId::kSpine].translation_ = { 0,4.5f,0 };

	//上半身
	worldTransforms_[PartId::kChest].Initialize();
	worldTransforms_[PartId::kChest].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kChest].translation_ = { 0,0,0 };

	worldTransforms_[PartId::kHead].Initialize();
	worldTransforms_[PartId::kHead].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kHead].translation_ = { 0,4.5f,0 };

	worldTransforms_[PartId::kArmL].Initialize();
	worldTransforms_[PartId::kArmL].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmL].translation_ = { -4.5f,0,0 };

	worldTransforms_[PartId::kArmR].Initialize();
	worldTransforms_[PartId::kArmR].parent_ = &worldTransforms_[PartId::kChest];
	worldTransforms_[PartId::kArmR].translation_ = { 4.5f,0,0 };

	//下半身
	worldTransforms_[PartId::kHip].Initialize();
	worldTransforms_[PartId::kHip].parent_ = &worldTransforms_[PartId::kSpine];
	worldTransforms_[PartId::kHip].translation_ = { 0,-4.50,0 };

	worldTransforms_[PartId::kLegL].Initialize();
	worldTransforms_[PartId::kLegL].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegL].translation_ = { -4.5f,-4.5f,0 };

	worldTransforms_[PartId::kLegR].Initialize();
	worldTransforms_[PartId::kLegR].parent_ = &worldTransforms_[PartId::kHip];
	worldTransforms_[PartId::kLegR].translation_ = { 4.5f,-4.5f,0 };

};

void GameScene::Update() {

	//キャラクター移動処理

	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float kCharaSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharaSpeed,0,0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharaSpeed,0,0 };
	}

	//上半身回転処理
	{
		const float kChestRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransforms_[PartId::kChest].rotation_.y -= kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransforms_[PartId::kChest].rotation_.y += kChestRotSpeed;
		}
	}

	//下半身回転処理
	{
		const float kHipRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_J)) {
			worldTransforms_[PartId::kHip].rotation_.y -= kHipRotSpeed;
		}
		else if (input_->PushKey(DIK_K)) {
			worldTransforms_[PartId::kHip].rotation_.y += kHipRotSpeed;
		}
	}

	worldTransforms_[PartId::kRoot].translation_ += move;
	Matrix4 unit;
	unit.MatIdentity();
	worldTransforms_[PartId::kRoot].matWorld_ = unit;
	Matrix4 matScale;
	matScale.MatScale(worldTransforms_->scale_);
	Matrix4 matRotZ;
	matRotZ.MatRotZ(worldTransforms_->rotation_.z);
	Matrix4 matRotX;
	matRotX.MatRotX(worldTransforms_->rotation_.x);
	Matrix4 matRotY;
	matRotY.MatRotY(worldTransforms_->rotation_.y);
	Matrix4 matTrans;
	matTrans.MatTrans(worldTransforms_->translation_);

	worldTransforms_[PartId::kRoot].matWorld_ *= matScale;
	worldTransforms_[PartId::kRoot].matWorld_ *= matRotZ;
	worldTransforms_[PartId::kRoot].matWorld_ *= matRotX;
	worldTransforms_[PartId::kRoot].matWorld_ *= matRotY;
	worldTransforms_[PartId::kRoot].matWorld_ *= matTrans;

	worldTransforms_[PartId::kRoot].TransferMatrix();

	debugText_->SetPos(50, 50);
	debugText_->Printf("Root=(%f,%f,%f)", worldTransforms_[PartId::kRoot].translation_.x, worldTransforms_[PartId::kRoot].translation_.y, worldTransforms_[PartId::kRoot].translation_.z);

	//大本から順に更新していく
	for (int i = 0; i < kNumPartId; i++) {
		Matrix4 unit;
		unit.MatIdentity();
		worldTransforms_[i].matWorld_ = unit;
		Matrix4 matScale;
		matScale.MatScale(worldTransforms_[i].scale_);
		Matrix4 matRotZ;
		matRotZ.MatRotZ(worldTransforms_[i].rotation_.z);
		Matrix4 matRotX;
		matRotX.MatRotX(worldTransforms_[i].rotation_.x);
		Matrix4 matRotY;
		matRotY.MatRotY(worldTransforms_[i].rotation_.y);
		Matrix4 matTrans;
		matTrans.MatTrans(worldTransforms_[i].translation_);

		worldTransforms_[i].matWorld_ *= matScale;
		worldTransforms_[i].matWorld_ *= matRotZ;
		worldTransforms_[i].matWorld_ *= matRotX;
		worldTransforms_[i].matWorld_ *= matRotY;
		worldTransforms_[i].matWorld_ *= matTrans;

		if (worldTransforms_[i].parent_ != nullptr) {
			worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;
		}

		worldTransforms_[i].TransferMatrix();
	}
}
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();


#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	for (int i = 0; i < kNumPartId; i++) {
		if (i == kSpine || i == kRoot) {
			continue;
		}
		model_->Draw(worldTransforms_[i], viewProjection_, textureHandle_);
	}

	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


