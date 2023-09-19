#include "TitleScene.h"
#include "ImGuiManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() { delete modelSkydome_; 
delete startSprite_;
	delete titleSprite;
}

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	startTexture_ = TextureManager::Load("start.png");
	startSprite_ = Sprite::Create(startTexture_, {640, 360}, {1, 1, 1, 1}, {0.5, 0.5});
	titleTexture_ = TextureManager::Load("title_.png");
	titleSprite = Sprite::Create(titleTexture_, {640, 100}, {1, 1, 1, 1}, {0.5, 0.5});
}

void TitleScene::Update() { 
	#ifdef _DEBUG
	ImGui::Text("title");
#endif // DEBUG
	viewProjection_.TransferMatrix();
	
}

void TitleScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreDraw(commandList);
	modelSkydome_->Draw(worldTransform_, viewProjection_);
	Model::PostDraw();

	Sprite::PreDraw(commandList);
	startSprite_->Draw();
	titleSprite->Draw();
	Sprite::PostDraw();
}


