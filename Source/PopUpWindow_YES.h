#include "PopUpWindowClass.h"
#include "cd_666s/Render/Sprite.h"
#include "cd_666s/Sound/Sound.h"

#pragma once
class PopUpWindow_YES : public PopUpWindowClass
{
private:

	Sprite _yesButton;

	Sound _decide;

public:
	PopUpWindow_YES();
	virtual ~PopUpWindow_YES();

	virtual void Update() override;
	virtual void InitSetup(std::string BoardFilepath, std::string ButtonFilepath);
	virtual void Draw() override;

	void SetPosition(Vector2D pos) {
		_BoadImage.SetPosition(pos);
		_yesButton.SetPosition(Vector2D(_BoadImage.GetPosition()._x + _BoadImage.GetTexturePtr().lock()->GetBaseSize()._x / 2 - _yesButton.GetTexturePtr().lock()->GetBaseSize()._x / 2, _BoadImage.GetPosition()._y + _BoadImage.GetTexturePtr().lock()->GetBaseSize()._y / 2 - _yesButton.GetTexturePtr().lock()->GetBaseSize()._y / 2 + 50));
	}

};

