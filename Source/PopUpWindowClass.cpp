#include "PopUpWindowClass.h"



PopUpWindowClass::PopUpWindowClass()
{



}


PopUpWindowClass::~PopUpWindowClass()
{



}

void PopUpWindowClass::Update()
{
}

void PopUpWindowClass::InitSetup(std::string filepath)
{

	_BoadImage.Load(filepath);
	_BoadImage.SetRenderType(Texture2D::RenderType::UI);
	_BoadImage.SetPosition(Vector2D(1280 / 2 - _BoadImage.GetTexturePtr().lock()->GetBaseSize()._x / 2, 720 / 2 - _BoadImage.GetTexturePtr().lock()->GetBaseSize()._y / 2));
	_BoadImage.SetPriority(static_cast<int>(Sprite::Priority::UI) + 5);
	_BoadImage.SetDisplayMode(false);

}

void PopUpWindowClass::Draw()
{
	if (isDisplay) {
		_BoadImage.SetDisplayMode(true);
	}
	else {
		_BoadImage.SetDisplayMode(false);
	}
}




bool PopUpWindowClass::ClickCheck(Vector2D pos, Vector2D size) {

	if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
		return false;

	//クリック位置を取得
	auto cursorPos = MOUSE->GetCursorPos();

	if (cursorPos._x < pos._x)
		return false;
	if (cursorPos._y < pos._y)
		return false;
	if (pos._x + size._x < cursorPos._x)
		return false;
	if (pos._y + size._y < cursorPos._y)
		return false;

	return true;

}



