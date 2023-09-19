#pragma once
#include "Input.h"
#include "DirectXCommon.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
class FailScene {
public:
	FailScene();
	~FailScene();
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
	uint32_t skyTexture_ = 0;
	Sprite* clearSprite_ = nullptr;
};