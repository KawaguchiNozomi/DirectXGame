#pragma once
#include "Input.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class ClearScene {
public:
	ClearScene();
	~ClearScene();
	void Update();
	void Draw();
	void Initialize();

private:
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	bool sceneChange = false;
	Model* modelSkydome_ = nullptr;
	uint32_t clearTexture_ = 0;
	Sprite* clearSprite_ = nullptr;
};