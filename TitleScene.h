#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "DirectXCommon.h"
#include "Sprite.h"
class TitleScene {
public:
	TitleScene();
	~TitleScene();
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
	uint32_t startTexture_ = 0;
	Sprite* startSprite_ = nullptr;
	uint32_t titleTexture_ = 0;
	Sprite* titleSprite = nullptr;
};
