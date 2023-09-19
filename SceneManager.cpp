#include "SceneManager.h"

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {
	delete gameScene_;
	delete titleScene_;
	delete clearScene_;
	delete failScene_;
}

void SceneManager::Initialize() {
	input_ = Input::GetInstance();
	gameScene_ = new GameScene;

	titleScene_ = new TitleScene;
	titleScene_->Initialize();

	clearScene_ = new ClearScene;
	clearScene_->Initialize();

	failScene_ = new FailScene;
	failScene_->Initialize();
}

void SceneManager::Update() {

	ScenesUpdate();

}

void SceneManager::Draw() {
	switch (scenes_) {
	case SceneManager::TITLE:
		titleScene_->Draw();
		break;
	case SceneManager::SELECT:
		break;
	case SceneManager::GAME:
		gameScene_->Draw();
		break;
	case SceneManager::CLEAR:
		clearScene_->Draw();
		break;
	case SceneManager::FAIL:
		failScene_->Draw();
		break;
	default:
		break;
	}
}

void SceneManager::ScenesUpdate() {
	switch (scenes_) {
	case SceneManager::TITLE:
		titleScene_->Update();
		if (input_->TriggerKey(DIK_SPACE)) {
			delete gameScene_;
			gameScene_ = new GameScene;
			gameScene_->Initialize();
			scenes_ = GAME;
		}
		break;
	case SceneManager::SELECT:
		break;
	case SceneManager::GAME:
		gameScene_->Update();
		if (gameScene_->GetClearFlag()) {
			scenes_ = CLEAR;
		}
		if (gameScene_->GetGameOverFlag()) {
			scenes_ = FAIL;
		}
		break;
	case SceneManager::CLEAR:
		clearScene_->Update();
		if (input_->TriggerKey(DIK_SPACE)) {
			scenes_ = TITLE;
		}
		break;
	case SceneManager::FAIL:
		failScene_->Update();
		if (input_->TriggerKey(DIK_SPACE)) {
			scenes_ = TITLE;
		}
		break;
	default:
		break;
	}
}
