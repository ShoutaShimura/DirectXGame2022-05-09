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

	delete modelSkydome_;

	delete skydome_;

	//自キャラの解放
	delete player_;

	delete debugCamera_;

	delete enemy_;

	delete railCamera_;
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

	modelSkydome_ = Model::CreateFromOBJ("tenkyu", true);

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラ
	debugCamera_ = new DebugCamera(1280, 720);

	//レールカメラの生成
	railCamera_ = new RailCamera();
	railCamera_->Initialize(Vector3(0, 0, -50), Vector3(0, 0, 0));

	railWorldTransform_ = railCamera_->GetWorldMatrix();

	//天球の生成
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, SkyworldTransform_.translation_);

	//自キャラの生成
	player_ = new Player();

	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);
	
	player_->SetParentWorldTransform(railCamera_->GetWorldMatrix());





	//敵キャラの生成
	enemy_ = new Enemy();

	//敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	EworldTransform_.translation_ = { 20,0,50 };
	enemy_->Initialize(model_, EworldTransform_.translation_);

};

void GameScene::Update() {
	railCamera_->Update();

	viewProjection_.matView = railCamera_->GetViewProjection().matView;
	viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;

	viewProjection_.TransferMatrix();

	skydome_->Update();

	//自キャラの更新
	player_->Update();

	if (enemy_ != nullptr) {
		enemy_->Update();
	}

	CheckAllCollisions();

	debugText_->SetPos(50, 80);
	debugText_->Printf("eye={%f,%f,%f}", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);


	debugText_->SetPos(50, 180);
	debugText_->Printf("%p", &railWorldTransform_);

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
		enemy_->Draw(railCamera_->GetViewProjection());
	}

	//自キャラの描画
	player_->Draw(railCamera_->GetViewProjection());

	//天球の描画
	skydome_->Draw(railCamera_->GetViewProjection());

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

void GameScene::CheckAllCollisions()
{
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();

	//敵弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();


#pragma region
	//自キャラと敵弾の当たり判定

		//自キャラの座標
	posA = player_->GetWorldPotision();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {

		//敵弾の座標
		posB = bullet->GetWorldPotision();

		//debugText_->SetPos(100, 500);
		//debugText_->Printf("Pos={%2f,%2f,%2f}",posB.x,posB.y,posB.z );

		float dx = (posB.x - posA.x);
		float dy = (posB.y - posA.y);
		float dz = (posB.z - posA.z);

		float L = (enemySize + playerSize) * (enemySize + playerSize);

		float distance = (dx * dx) + (dy * dy) + (dz * dz);

		//球と球の交差判定
		if (distance <= L) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();;
			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}

#pragma endregion

#pragma region
	//自弾と敵キャラの当たり判定

		//敵キャラの座標
	posA = enemy_->GetWorldPotision();

	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {

		//自弾の座標
		posB = bullet->GetWorldPotision();


		float dx = (posB.x - posA.x);
		float dy = (posB.y - posA.y);
		float dz = (posB.z - posA.z);

		float L = (enemySize + playerSize) * (enemySize + playerSize);

		float distance = (dx * dx) + (dy * dy) + (dz * dz);

		//球と球の交差判定
		if (distance <= L) {
			//敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();;
			//自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}

	}


#pragma endregion

#pragma region
	//自弾と敵弾の当たり判定

		//自弾と敵弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& pbullet : playerBullets) {
		for (const std::unique_ptr<EnemyBullet>& ebullet : enemyBullets) {
			//自弾の座標
			posB = pbullet->GetWorldPotision();
			//敵弾の座標
			posA = ebullet->GetWorldPotision();

			float dx = (posB.x - posA.x);
			float dy = (posB.y - posA.y);
			float dz = (posB.z - posA.z);

			float L = (enemySize + playerSize) * (enemySize + playerSize);

			float distance = (dx * dx) + (dy * dy) + (dz * dz);

			//球と球の交差判定
			if (distance <= L) {
				//敵キャラの衝突時コールバックを呼び出す
				pbullet->OnCollision();;
				//自弾の衝突時コールバックを呼び出す
				ebullet->OnCollision();
			}
		}
	}

#pragma endregion

}


