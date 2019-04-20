#include "PopUpWindowClass.h"
#include "cd_666s/Render/Sprite.h"

#include "cd_666s/Sound/Sound.h"

#pragma once
class PopUpWindow_Click_Go_To_Next :public PopUpWindowClass
{
private:

	Sprite _nextButton;

	Sound _decideSE;


public:
	PopUpWindow_Click_Go_To_Next();
	virtual ~PopUpWindow_Click_Go_To_Next();

	virtual void Update() override;
	virtual void InitSetup(std::string BoardFilepath, std::string ButtonFilepath);
	virtual void Draw() override;

	void SetPosition(Vector2D pos) {
		_BoadImage.SetPosition(pos);
	}
};

