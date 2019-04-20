#include "PopUpWindow_Click_Go_To_Next.h"
#include "cd_666s/InputManager/MouseInput.h"


PopUpWindow_Click_Go_To_Next::PopUpWindow_Click_Go_To_Next()
{

	isDisplay = false;

	_decideSE.Load(Main::RESOURCEFILENAME + "resource/sound/decide.wav");
	_decideSE.SetVolume(200);


}


PopUpWindow_Click_Go_To_Next::~PopUpWindow_Click_Go_To_Next()
{
}

void PopUpWindow_Click_Go_To_Next::Update()
{

	if (isDisplay) {
		if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L)) {
			_nextButton.SetDisplayMode(false);
			_BoadImage.SetDisplayMode(false);
			isDisplay = false;
			_decideSE.Play();
		}
	}
	else {
		return;
	}


}

void PopUpWindow_Click_Go_To_Next::InitSetup(std::string BoardFilepath, std::string ButtonFilepath)
{

	PopUpWindowClass::InitSetup(BoardFilepath);

	_nextButton.Load(ButtonFilepath);
	_nextButton.SetRenderType(Texture2D::RenderType::UI);
	_nextButton.SetPosition(Vector2D(860, 640));
	_nextButton.SetScale(Vector2D(2.0, 2.0));
	_nextButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 6);
	_nextButton.SetDisplayMode(false);


}

void PopUpWindow_Click_Go_To_Next::Draw()
{

	PopUpWindowClass::Draw();

	if (isDisplay) {
		_nextButton.SetDisplayMode(true);
	}
	else {
		_nextButton.SetDisplayMode(false);
	}
}










