#include "Player.h"


Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(Model* model, uint32_t textureHandle)
{
	//NULLポインタチェック
	assert(model);

	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
}

void Player::Update()
{
	//移動
	Move();

	//キャラクター攻撃処理
	Attack();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	debugText_->SetPos(50, 50);
	debugText_->Printf("Pos={%f,%f,%f}", worldTransform_.translation_.x, worldTransform_.translation_.y, worldTransform_.translation_.z);
}

void
Player::Draw(ViewProjection& viewProjection)
{
	//3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet:bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };

	const float speed = 0.5f;

	//移動ベクトルを変更する処理
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

	//座標移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	//移動限界座標
	const float kMoveLimitX = 35;
	const float kMoveLimitY = 19;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	Rotate();

	Matrix4 unit;
	unit.MatIdentity();
	worldTransform_.matWorld_ = unit;
	worldTransform_.matWorld_ = unit.MatCal(worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	//行列更新
	worldTransform_.TransferMatrix();
}

void Player::Rotate()
{

	const float speed = 0.05f;

	//移動ベクトルを変更する処理
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

		//弾を生成し、初期化
		//PlayerBullet* newBullet = new PlayerBullet();
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_, worldTransform_.translation_);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));
	}

}
