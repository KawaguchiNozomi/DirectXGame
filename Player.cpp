﻿#include "Player.h"
#include "MathUtility.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DRetecle_;
	delete mouseSprite_;
	delete arrowSprite_;
	for (int i = 0; i < 3; i++) {
		delete lifeSprite_[i];
	}
}

void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 position) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DRetecle_ = Sprite::Create(
	    textureReticle, {640, 360}, {1,1,1,1},
	    {0.5, 0.5});
	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer_);
	SetCollisionMask(CollisionConfig::kCollisionAttributeEnemy_);

	mouseTexture_ = TextureManager::Load("mouse.png");
	mouseSprite_ = Sprite::Create(mouseTexture_, {700, 460}, {1, 1, 1, 1}, {0.5, 0.5});

	arrowTexture_ = TextureManager::Load("asdw.png");
	arrowSprite_ = Sprite::Create(arrowTexture_, {580, 460}, {1, 1, 1, 1}, {0.5, 0.5});

	uint32_t life = TextureManager::Load("life.png");
	for (int i = 0; i < 3; i++) {
		lifeSprite_[i] = Sprite::Create(life, {float(100+(64*i)), 600}, {1, 1, 1, 1}, {0.5, 0.5});
	}
}

void Player::Update(ViewProjection& viewProjection) {  

	if (HP_ <= 0) {
		isArrive_ = false;
	}
	viewProjection_ = viewProjection;
		bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0,0,0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
		isMovePushed_ = true;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
		isMovePushed_ = true;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
		isMovePushed_ = true;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
		isMovePushed_ = true;
	}

	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	//範囲制限
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.UpdateMatrix();

	worldTransform_.TransferMatrix();

	POINT mousePosition;
	//マウス座標(画面全体のスクリーン座標)を取得する
	/*画面左上から右下までの中のどこにカーソルがあるか*/
	GetCursorPos(&mousePosition);
	//クライアントエリア座標(ウィンドウ内のスクリーン座標)に変換する
	/*今開いているウィンドウの中のどこにカーソルがあるか*/
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	//2Dレティクルに
	sprite2DRetecle_->SetPosition(Vector2{float(mousePosition.x),float(mousePosition.y)});
	//ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = viewProjection_.matView * viewProjection_.matProjection *
	                   MakeViewportMatrix(0, 0, 1280, 720, 0, 1);
	//合成行列の逆行列を計算する
	Matrix4x4 matInveersVPV = Inverse(matVPV);
	//スクリーン座標
	Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0);
	Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);
	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInveersVPV);
	posFar = Transform(posFar, matInveersVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar-posNear;
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject =  50;
	worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();

	//キャラクター攻撃処理
	fireTimer_--;
	Attack();
	//弾を更新する
	for (PlayerBullet*bullet : bullets_) {
		bullet->Update();
	}


	
	
#ifdef _DEBUG
	// ImGui
	int a = 0;
	ImGui::SetNextWindowSize({250,250});
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("pos", sliderValue, -20.0f, 20.0f);
	ImGui::Text(
	    "pos::(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	float reticle[3] = {worldTransform3DReticle_.translation_.x,worldTransform3DReticle_.translation_.y,worldTransform3DReticle_.translation_.z};
	ImGui::InputFloat3("reticle", reticle);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::Text(
	    "2DReticle:(%f,%f)", sprite2DRetecle_->GetPosition().x, sprite2DRetecle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,&+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,&+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DRetice:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	if (a == 0) {
		ImGui::Begin("Player");
		ImGui::Text("Move:WASD fire:SPACE");
	}
	ImGui::End();
#endif // DEBUG
	
}

void Player::Draw(ViewProjection& viewProjection) { 
	if (isArrive_) {

		model_->Draw(worldTransform_, viewProjection);
		for (PlayerBullet* bullet : bullets_) {
			bullet->Draw(viewProjection);
		}
	}

}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Vector3 Player::GetReticleWorldPos() {

	Vector3 worldPos;
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() { HP_--; }

void Player::SetBulletModel(Model *model) { bulletModel_ = model; }

void Player::Attack() {
	if (fireTimer_ <= 0) {
		if (input_->IsPressMouse(0)) {
			
			isPushed_ = true;
			// 弾の速度
			const float kBulletSpeed = 1.0f;
			// 3Dレティクルと自機の差分ベクトル
			Vector3 velocity = GetReticleWorldPos() - GetWorldPosition();
			velocity = Normalize(velocity) * kBulletSpeed;
			// 弾を生成し初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);
			// 弾を登録する
			bullets_.push_back(newBullet);
			fireTimer_ = kFireInterval;
		}
	}
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::DrawUI() { sprite2DRetecle_->Draw();
	if (isPushed_ == false) {
		mouseSprite_->Draw();
	}
	if (isMovePushed_ == false) {
		arrowSprite_->Draw();
	}
	for (unsigned int i = 0; i < HP_; i++) {
		lifeSprite_[i]->Draw();
	}
}
