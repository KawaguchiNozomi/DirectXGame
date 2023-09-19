#include "BigEnemy.h"
#include <assert.h>
#include "Player.h"
#include "GameScene.h"
#include <cmath>

BigEnemy::~BigEnemy() {}

void BigEnemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("big2.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy_);
	SetCollisionMask(CollisionConfig::kCollisionAttributePlayer_);
}


void BigEnemy::Update() { 
	hitting_ = false;
	if (isArrive_) {
		//worldTransform_.translation_.z = +0.005f;
	fireTimer_--;
	if (fireTimer_ == 0) {
		Fire();
		fireTimer_ = kFireInterval;
	}
	}
worldTransform_.UpdateMatrix(); 
	if (HP_ <= 0) {
		isDead = true;
	}

	
}

void BigEnemy::Draw(const ViewProjection& viewProjection) {
	if (isArrive_ == true) {
		if (isDead == false) {
		if (hitting_) {
			model_->Draw(worldTransform_, viewProjection,textureHandle_);
		} else {

			model_->Draw(worldTransform_, viewProjection);
		}
		}
	}
}

Vector3 BigEnemy::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void BigEnemy::OnCollision() { HP_--; 
hitting_ = true;
}

void BigEnemy::SetIsArrive(bool flag) { isArrive_ = flag; }

void BigEnemy::SetPos(const Vector3& pos) { worldTransform_.translation_ = pos; }

void BigEnemy::Fire() {
	assert(player_);

	// 弾の速度
	const float kBulletSpeed = 0.75f;

	// 座標の取得
	Vector3 player = player_->GetWorldPosition();
	Vector3 enemy = GetWorldPosition();

	// 敵とプレイヤーの差分ベクトル
	Vector3 vector = {(player.x - enemy.x), (player.y - enemy.y), (player.z - enemy.z)};

	// ベクトルの正規化
	float length = std::sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	Vector3 dir = {vector.x / length, vector.y / length, vector.z / length};

	// 長さを合わせた弾の速度
	Vector3 velocity = {dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed};
	EnemyBullet* bullet = new EnemyBullet();
	bullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を生成し初期化
	gameScene_->AddEnemyBullet(bullet);
}
