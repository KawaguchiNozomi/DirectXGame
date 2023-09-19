#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h" 
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <assert.h>
#include "PlayerBullet.h"
#include "Sprite.h"
#include <list>
#include "Collider.h"
class GameScene;

/// <summary>
/// 
/// </summary>
class Player :public Collider{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle,Vector3 position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

	void DrawUI();

	Vector3 GetWorldPosition() override;
	Vector3 GetPosition() { return worldTransform_.translation_; }

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	void OnCollision() override;
	int GetHP() { return HP_; }
	
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }
	void SetBulletModel(Model *model);

	static const int kFireInterval = 2;

private:
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	Model* bulletModel_ = nullptr;
	WorldTransform worldTransform_;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	WorldTransform worldTransform3DReticle_;
	Sprite* sprite2DRetecle_ = nullptr;
	ViewProjection viewProjection_;

	uint32_t mouseTexture_ = 0;
	Sprite* mouseSprite_ = nullptr;
	bool isPushed_ = false;

	uint32_t arrowTexture_ = 0;
	Sprite* arrowSprite_ = nullptr;
	bool isMovePushed_ = false;

	Sprite* lifeSprite_[3] = {nullptr,nullptr,nullptr};
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
	int32_t fireTimer_ = 0;
	Vector3 GetReticleWorldPos();
	
	unsigned int HP_ = 3;
	bool isArrive_ = true;
};
