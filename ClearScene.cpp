#include "ClearScene.h"

ClearScene::ClearScene() {}

ClearScene::~ClearScene() { delete clearSprite_;
	delete modelSkydome_;
}

void ClearScene::Update() { viewProjection_.TransferMatrix(); }

void ClearScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreDraw(commandList);
	modelSkydome_->Draw(worldTransform_, viewProjection_);
	Model::PostDraw();

	Sprite::PreDraw(commandList);
	clearSprite_->Draw();
	Sprite::PostDraw();
}

void ClearScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	clearTexture_ = TextureManager::Load("clear_.png");
	clearSprite_ = Sprite::Create(clearTexture_, {640, 100}, {1, 1, 1, 1}, {0.5, 0.5});
}
