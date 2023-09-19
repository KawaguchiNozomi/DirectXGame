#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <list>
#include "Collider.h"

class Player;
class GameScene;

class BigEnemy : public Collider {
public:
	~BigEnemy();
	/// <summary>
	/// 敵の初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position);
	/// <summary>
	/// 敵の更新
	/// </summary>
	void Update();
	/// <summary>
	/// 敵の描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	void SetPlayer(Player* player) { player_ = player; }
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vector3 GetWorldPosition() override;

	const float GetRadius() { return radius_; }
	bool GetIsDead() { return isDead; }
	const float radius_ = 2.0f;

	void OnCollision() override;
	bool GetIsArrive() { return isArrive_; }
	void SetIsArrive(bool flag);
	void SetPos(const Vector3& pos);

	// const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	// 発車時間
	static const int kFireInterval = 50;

private:
	enum class Phase {
		
	};
	Phase phase_ ;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	// 発射タイマー
	int32_t fireTimer_ = 0;
	Player* player_ = nullptr;
	GameScene* gameScene_ = nullptr;
	bool isArrive_ = false;
	bool isDead = false;
	int deathTimer = 300;
	int HP_ = 20;
	bool hitting_ = false;

	void Fire();
};
