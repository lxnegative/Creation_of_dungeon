#pragma once
#include "SceneBase.h"
#include "cd_666s/Render/Sprite.h"

class SceneManager 
{
private:
    SceneBase* now_scene;

	Graph test;

	Sprite _black;

	SceneBase* temp_scene;

	int interval;
	const int maxInterval = 20;

public:
    SceneManager();
    ~SceneManager();

	void Init() {
		_black.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Black.png");
		_black.SetRenderType(Texture2D::RenderType::UI);
		_black.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1000);
		_black.SetPosition(Vector2D(0, 0));
		_black.SetDisplayMode(false);
	}

    bool SceneManagement();


};

