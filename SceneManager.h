#pragma once
#include"GameScene.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "Input.h"
#include "FailScene.h"
class SceneManager {

public:
	SceneManager();
	~SceneManager();
	void Initialize();
	void Update();
	void Draw();

private:
	void ScenesUpdate();
	enum Scenes {
		TITLE,
		SELECT,
		GAME,
		CLEAR,
		FAIL

	};
	Scenes scenes_;
	GameScene* gameScene_ = nullptr;
	TitleScene* titleScene_ = nullptr;
	ClearScene* clearScene_ = nullptr;
	FailScene* failScene_ = nullptr;
	Input* input_;



};
