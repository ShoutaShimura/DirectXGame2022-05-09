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

	//カメラ視点座標を設定
	//viewProjection_.eye={ 0,0,-10 };
	
	//カメラ注視点座標を設定
	viewProjection_.target = { 10,0,0 };

	//カメラ上方向ベクトルを設定(右上45度指定)
	viewProjection_.up = { cosf(3.1415f / 4.0f),sinf(3.1415f / 4.0f),0.0f };

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	std::random_device seed_gen;

	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> rotDist(0, 3.1415f);

	std::uniform_real_distribution<float> posDist(-10.0f, 10.0f);

	for (WorldTransform& worldTransform : worldTransforms_) {

		worldTransform.Initialize();

		//X,Y,Z方向のスケーリングを設定
		worldTransform.scale_ = { 1.0f,1.0f,1.0f };
		//X,Y,Z軸周りの回転角を設定
		worldTransform.rotation_ = { rotDist(engine),rotDist(engine),rotDist(engine) };

		//X,Y,Z軸周りの平行移動を設定
		worldTransform.translation_ = { posDist(engine),posDist(engine),posDist(engine) };

		//単位行列
		Matrix4 unitMatrix;
		unitMatrix.MatIdentity();

		//スケーリング行列を宣言
		Matrix4 matScale;
		matScale.Scaliing(worldTransform.scale_.x,worldTransform.scale_.y, worldTransform.scale_.z);

		//Z軸回転行列を設定
		Matrix4 matRotZ;
		matRotZ.MatRotZ(worldTransform.rotation_.z);

		//X軸回転行列を設定
		Matrix4 matRotX;
		matRotX.MatRotX(worldTransform.rotation_.x);

		//Y軸回転行列を設定
		Matrix4 matRotY;
		matRotY.MatRotY(worldTransform.rotation_.y);

		//平行移動行列を宣言
		Matrix4 matTrans;
		matTrans.MatTrans(worldTransform.translation_.x, worldTransform.translation_.y, worldTransform.translation_.z);

		//単位行列を代入
		worldTransform.matWorld_ = unitMatrix;

		//行列にそれぞれ掛け算して代入
		//スケール
		worldTransform.matWorld_ *= matScale;

		//回転
		worldTransform.matWorld_ *= matRotZ;
		worldTransform.matWorld_ *= matRotX;
		worldTransform.matWorld_ *= matRotY;

		//平行移動
		worldTransform.matWorld_ *= matTrans;

		//行列の転送
		worldTransform.TransferMatrix();

	}

};

void GameScene::Update() {

	//視点移動処理
	{
		//視点の移動ベクトル
		Vector3 move;

		//視点の移動速さ
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move.z += kEyeSpeed;
		}
		else if (input_->PushKey(DIK_S)) {
			move.z -= kEyeSpeed;
		}

		//視点移動(ベクトルの加算)
		viewProjection_.eye += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf("eye(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);
	}

	//注視点移動処理
	{
		//注視点の移動ベクトル
		Vector3 move;

		//注視点の移動速さ
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_LEFT)) {
			move.x += kEyeSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move.x -= kEyeSpeed;
		}

		//視点移動(ベクトルの加算)
		viewProjection_.target += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 70);
		debugText_->Printf("target(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);

	}

	//上方向回転処理
	{
		//上方向の回転速さ[ラジアン/frame]
		const float kUpRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpRotSpeed;
	
			//2πを超えたら0に戻す
			viewAngle = fmodf(viewAngle, 3.1415f * 2.0f);
		}

		//上方向ベクトルを計算
		viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバック用表示
		debugText_->SetPos(50, 90);
		debugText_->Printf("up(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);

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
	for (WorldTransform& worldTransform : worldTransforms_) {
		model_->Draw(worldTransform, viewProjection_, textureHandle_);
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



