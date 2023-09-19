#include "FailScene.h"

FailScene::FailScene() {}

FailScene::~FailScene() { delete clearSprite_;
	delete modelSkydome_;
}

void FailScene::Update() { viewProjection_.TransferMatrix(); }

void FailScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreDraw(commandList);
	modelSkydome_->Draw(worldTransform_, viewProjection_,skyTexture_);
	Model::PostDraw();

	Sprite::PreDraw(commandList);
	clearSprite_->Draw();
	Sprite::PostDraw();
}

void FailScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	clearTexture_ = TextureManager::Load("gameover_.png");
	clearSprite_ = Sprite::Create(clearTexture_, {640, 100}, {1, 1, 1, 1}, {0.5, 0.5});
	skyTexture_ = TextureManager::Load("sky2.png");
}
