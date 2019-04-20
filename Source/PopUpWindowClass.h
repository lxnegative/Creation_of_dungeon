#include "cd_666s/Render/Sprite.h"
#include <string>
#include "cd_666s\InputManager\InputManager.h"

#pragma once
class PopUpWindowClass
{
protected:

	Sprite _BoadImage;

	bool isDisplay;



public:
	PopUpWindowClass();
	virtual ~PopUpWindowClass();

	virtual void Update();
	virtual void InitSetup(std::string filepath);
	virtual void Draw();

	bool ClickCheck(Vector2D pos, Vector2D size);


	bool GetIsDisplay() {
		return isDisplay;
	}

	void SetIsDisplay(bool a) {
		isDisplay = a;
	}



};

