#include "PopUpWindow_YES.h"



PopUpWindow_YES::PopUpWindow_YES()
{

	isDisplay = false;


	_decide.Load(Main::RESOURCEFILENAME + "resource/sound/enemy_fall2.wav");
	_decide.SetVolume(200);

}


PopUpWindow_YES::~PopUpWindow_YES()
{
	_decide.~Sound();
}

void PopUpWindow_YES::Update()
{

	if (isDisplay) {
		if (ClickCheck(_yesButton.GetPosition(), _yesButton.GetTexturePtr().lock()->GetBaseSize())) {
			_decide.Play();
			_yesButton.SetDisplayMode(false);
			_BoadImage.SetDisplayMode(false);
			isDisplay = false;
		}
	}
	else {
		return;
	}


}

void PopUpWindow_YES::InitSetup(std::string BoardFilepath, std::string ButtonFilepath)
{

	PopUpWindowClass::InitSetup(BoardFilepath);

	_yesButton.Load(ButtonFilepath);
	_yesButton.SetRenderType(Texture2D::RenderType::UI);
	_yesButton.SetPosition(Vector2D(_BoadImage.GetPosition()._x + _BoadImage.GetTexturePtr().lock()->GetBaseSize()._x / 2 - _yesButton.GetTexturePtr().lock()->GetBaseSize()._x / 2, _BoadImage.GetPosition()._y + _BoadImage.GetTexturePtr().lock()->GetBaseSize()._y / 2 - _yesButton.GetTexturePtr().lock()->GetBaseSize()._y / 2 + 65));
	_yesButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 6);
	_yesButton.SetDisplayMode(false);


}

void PopUpWindow_YES::Draw()
{

	PopUpWindowClass::Draw();

	if (isDisplay) {
		_yesButton.SetDisplayMode(true);
	}
	else {
		_yesButton.SetDisplayMode(false);
	}
}










