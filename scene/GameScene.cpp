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

	//自キャラの解放
	delete player_;

	delete debugCamera_;

	delete enemy_;
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

	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	enemy_ = new Enemy();

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	worldTransform_.translation_ = { 0,0,50 };
	EworldTransform_.translation_ = { 20,0,50 };
	enemy_->Initialize(model_, EworldTransform_.translation_);

};

void GameScene::Update() {
	//自キャラの更新
	player_->Update();

	if (enemy_ != nullptr) {
		enemy_->Update();
	}

	debugText_->SetPos(50, 80);
	debugText_->Printf("eye={%f,%f,%f}", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
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


	if (enemy_ != nullptr) {
		enemy_->Draw(viewProjection_);
	}

	//自キャラの描画
	player_->Draw(viewProjection_);


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


