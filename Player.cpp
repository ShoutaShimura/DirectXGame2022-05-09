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


	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();
}

void Player::Update()
{
}

void Player::Draw()
{
}
