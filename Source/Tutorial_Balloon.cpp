#include "Tutorial_Balloon.h"
#include "PanelObjectManager.h"
#include "cd_666s/TilebaseAI/Monster.h"


Tutorial_Balloon::Tutorial_Balloon()
{

}

Tutorial_Balloon::Tutorial_Balloon(std::string timingName)
{

	this->timingName = timingName;

	popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage1.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");
	popUpWindow.SetIsDisplay(false);
	isFinish = false;
	stepNum = 1;

	WaitCount = 0;
	isControlStop = false;
	ControlOrder = false;

	isSpot_Circle = false;

	auto backColor = Color4(0, 0, 0, 0.5f);

	SpotImage1.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Black.png");
	SpotImage1.SetRenderType(Texture2D::RenderType::UI);
	SpotImage1.SetPriority(static_cast<int>(Sprite::Priority::UI) - 10);
	SpotImage1.SetPosition(Vector2D(0, 0));
	SpotImage1.SetDisplayMode(false);
	SpotImage1.SetBaseColor(backColor);

	SpotImage2.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Black.png");
	SpotImage2.SetRenderType(Texture2D::RenderType::UI);
	SpotImage2.SetPriority(static_cast<int>(Sprite::Priority::UI) - 10);
	SpotImage2.SetPosition(Vector2D(0, 0));
	SpotImage2.SetDisplayMode(false);
	SpotImage2.SetBaseColor(backColor);

	SpotImage3.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Black.png");
	SpotImage3.SetRenderType(Texture2D::RenderType::UI);
	SpotImage3.SetPriority(static_cast<int>(Sprite::Priority::UI) - 10);
	SpotImage3.SetPosition(Vector2D(0, 0));
	SpotImage3.SetDisplayMode(false);
	SpotImage3.SetBaseColor(backColor);

	SpotImage4.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Black.png");
	SpotImage4.SetRenderType(Texture2D::RenderType::UI);
	SpotImage4.SetPriority(static_cast<int>(Sprite::Priority::UI) - 10);
	SpotImage4.SetPosition(Vector2D(0, 0));
	SpotImage4.SetDisplayMode(false);
	SpotImage4.SetBaseColor(backColor);

	BlackImage.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Black.png");
	BlackImage.SetRenderType(Texture2D::RenderType::UI);
	BlackImage.SetPriority(static_cast<int>(Sprite::Priority::UI) - 10);
	BlackImage.SetPosition(Vector2D(0, 0));
	BlackImage.SetDisplayMode(false);
	BlackImage.SetBaseColor(backColor);

	Spot_Circle.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Spot_Circle.png");
	Spot_Circle.SetRenderType(Texture2D::RenderType::UI);
	Spot_Circle.SetPriority(static_cast<int>(Sprite::Priority::UI) - 10);
	Spot_Circle.SetPosition(Vector2D(0, 0));
	Spot_Circle.SetDisplayMode(false);
	Spot_Circle.SetBaseColor(backColor);
	

	if (timingName == "Stage1-1_Edit") {
		tutorialType = TutorialType::Edit_1_1;
		Tutorial_Init_Edit_1_1(stepNum);
	}
	else if (timingName == "Stage1-1_Battle") {
		tutorialType = TutorialType::Battle_1_1;
		Tutorial_Init_Battle_1_1(stepNum);
	}
	else if (timingName == "Stage1-2_Edit") {
		tutorialType = TutorialType::Edit_1_2;
		Tutorial_Init_Edit_1_2(stepNum);
	}
	else if (timingName == "Stage1-4_Edit") {
		tutorialType = TutorialType::Edit_1_4;
		Tutorial_Init_Edit_1_4(stepNum);
	}
	else if (timingName == "Stage1-4_Battle") {
		tutorialType = TutorialType::Battle_1_4;
		Tutorial_Init_Battle_1_4(stepNum);
	}


}


Tutorial_Balloon::~Tutorial_Balloon()
{
}

bool Tutorial_Balloon::Update()
{

	bool isNeedUpdate = false;

	switch (tutorialType)
	{
	case Tutorial_Balloon::TutorialType::Edit_1_1:
		isNeedUpdate = Tutorial_Update_Edit_1_1(stepNum);
		break;
	case Tutorial_Balloon::TutorialType::Battle_1_1:
		isNeedUpdate = Tutorial_Update_Battle_1_1(stepNum);
		break;
	case Tutorial_Balloon::TutorialType::Edit_1_2:
		isNeedUpdate = Tutorial_Update_Edit_1_2(stepNum);
		break;
	case Tutorial_Balloon::TutorialType::Edit_1_4:
		isNeedUpdate = Tutorial_Update_Edit_1_4(stepNum);
		break;
	case Tutorial_Balloon::TutorialType::Battle_1_4:
		isNeedUpdate = Tutorial_Update_Battle_1_4(stepNum);
	default:
		break;
	}

	return isNeedUpdate;

}

void Tutorial_Balloon::Draw()
{

	SpotLightDraw();

	popUpWindow.Draw();

}

void Tutorial_Balloon::SpotLightDraw()
{
	if (spotLightPoint._x != -1 && spotLightPoint._y != -1 && spotLightSize._x != -1 && spotLightSize._y != -1) {

		BlackImage.SetDisplayMode(false);
		Spot_Circle.SetDisplayMode(false);

		SpotImage1.SetDisplayMode(true);
		SpotImage2.SetDisplayMode(true);
		SpotImage3.SetDisplayMode(true);
		SpotImage4.SetDisplayMode(true);

		SpotImage1.SetPosition(Vector2D(0, 0));
		Vector2D s = SpotImage1.GetSize();
		SpotImage1.SetScale(Vector2D(spotLightPoint._x / s._x, 720 / s._y));

		SpotImage2.SetPosition(Vector2D(spotLightPoint._x, 0));
		s = SpotImage2.GetSize();
		SpotImage2.SetScale(Vector2D(1280 / s._x, spotLightPoint._y / s._y));

		SpotImage3.SetPosition(Vector2D(spotLightPoint._x, spotLightPoint._y + spotLightSize._y));
		s = SpotImage3.GetSize();
		SpotImage3.SetScale(Vector2D(1280 / s._x, 720 / s._y));

		SpotImage4.SetPosition(Vector2D(spotLightPoint._x + spotLightSize._x, spotLightPoint._y));
		s = SpotImage4.GetSize();
		SpotImage4.SetScale(Vector2D(1280 / s._x, spotLightSize._y / s._y));
	}else if(isSpot_Circle){
		SpotImage1.SetDisplayMode(false);
		SpotImage2.SetDisplayMode(false);
		SpotImage3.SetDisplayMode(false);
		SpotImage4.SetDisplayMode(false);
		BlackImage.SetDisplayMode(false);

		Spot_Circle.SetDisplayMode(true);
	}
	else {
		SpotImage1.SetDisplayMode(false);
		SpotImage2.SetDisplayMode(false);
		SpotImage3.SetDisplayMode(false);
		SpotImage4.SetDisplayMode(false);
		Spot_Circle.SetDisplayMode(false);

		BlackImage.SetDisplayMode(true);
	}


}

void Tutorial_Balloon::Tutorial_Init_Edit_1_1(int step)
{
	switch (step)
	{
	case 1:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage2.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(540, 100));
		balloon.SetDisplayMode(false);

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage1.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);
		break;
	case 2:
		popUpWindow.SetIsDisplay(false);

		isSpot_Circle = true;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(1020 - 1280, 350 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage2.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(600, 110));
		balloon.SetDisplayMode(true);

		break;
	case 3:
		isSpot_Circle = false;
		spotLightPoint._x = 15;
		spotLightPoint._y = 515;
		spotLightSize._x = 896;
		spotLightSize._y = 194;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage3.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);

		break;
	case 4:
		popUpWindow.SetIsDisplay(false);
		isSpot_Circle = true;

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(530 - 1280, 230 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage4.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(600, 110));
		balloon.SetDisplayMode(true);

		break;
	case 5:
		isSpot_Circle = false;
		spotLightPoint._x = 960;
		spotLightPoint._y = 136;
		spotLightSize._x = 255;
		spotLightSize._y = 56;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage5.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	case 6:
		isSpot_Circle = false;
		spotLightPoint._x = 383;
		spotLightPoint._y = 533;
		spotLightSize._x = 157;
		spotLightSize._y = 36;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage6.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	case 7:
		isSpot_Circle = true;
		popUpWindow.SetIsDisplay(false);

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(1160 - 1280, 480 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage7.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(600, 110));
		balloon.SetDisplayMode(true);


		break;
	case 8:
		isSpot_Circle = true;
		popUpWindow.SetIsDisplay(false);

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(530 - 1280, 230 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage7.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(600, 110));
		balloon.SetDisplayMode(true);
		break;
	case 9:
		isSpot_Circle = false;
		spotLightPoint._x = 960;
		spotLightPoint._y = 136;
		spotLightSize._x = 255;
		spotLightSize._y = 56;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage8.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	case 10:
		isSpot_Circle = false;
		spotLightPoint._x = 960;
		spotLightPoint._y = 136;
		spotLightSize._x = 255;
		spotLightSize._y = 56;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage9.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	case 11:
		isSpot_Circle = false;
		spotLightPoint._x = 960;
		spotLightPoint._y = 136;
		spotLightSize._x = 255;
		spotLightSize._y = 56;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage10.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	case 12:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage11.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	case 13:
		isSpot_Circle = false;
		spotLightPoint._x = 950;
		spotLightPoint._y = 520;
		spotLightSize._x = 280;
		spotLightSize._y = 55;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/CreatMessage12.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	default:
		break;
	}

	return;
}

bool Tutorial_Balloon::Tutorial_Update_Edit_1_1(int step)
{

	switch (step)
	{
	case 1:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_1(stepNum);
			return false;
		}
		break;
	case 2:
		for (auto p : PANEL_MGR->_objects) {

			if (p == nullptr)
				continue;

			//
			bool isClicked = p->IsClicked();

			//何かのパネルがクリックされる。
			//そのパネルの種類(クラス)によって
			//処理内容が変化する。
			//また、別のサブクラスのパネルの変数に
			//干渉する

			//パネルがクリックされたら
			if (isClicked) {
				std::shared_ptr<PanelSettingObject> ps = dynamic_pointer_cast<PanelSettingObject>(p);

				if (ps) {
					if (ps->getPanelObjectName().find("slime") != std::string::npos) {
						stepNum++;
						Tutorial_Init_Edit_1_1(stepNum);
						return true;
					}
				}
			}
		}

		break;
	case 3:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_1(stepNum);
			return false;
		}
		break;
	case 4:

		if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L)) {
			Vector2D cursorPos = MOUSE->GetCursorPos();
			TiledVector tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
			if (!FIELD->IsInside(tiledCursorPos))
				return false;

			if (tiledCursorPos._x == 7 && tiledCursorPos._y == 3) {
				if ((FIELD->GetTiledObjects(tiledCursorPos)).empty()) {
					stepNum++;
					Tutorial_Init_Edit_1_1(stepNum);
					return true;
				}
			}

		}

		break;
	case 5:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_1(stepNum);
			return false;
		}
		break;
	case 6:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_1(stepNum);
			return false;
		}
		break;
	case 7:
		for (auto p : PANEL_MGR->_objects) {

			if (p == nullptr)
				continue;

			//
			bool isClicked = p->IsClicked();

			//何かのパネルがクリックされる。
			//そのパネルの種類(クラス)によって
			//処理内容が変化する。
			//また、別のサブクラスのパネルの変数に
			//干渉する

			//パネルがクリックされたら
			if (isClicked) {
				std::shared_ptr<PanelTrashObject> ps = dynamic_pointer_cast<PanelTrashObject>(p);

				if (ps) {
					stepNum++;
					Tutorial_Init_Edit_1_1(stepNum);
					return true;

				}
			}
		}
		break;
	case 8:
		if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L)) {
			Vector2D cursorPos = MOUSE->GetCursorPos();
			TiledVector tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
			if (!FIELD->IsInside(tiledCursorPos))
				return false;

			if (tiledCursorPos._x == 7 && tiledCursorPos._y == 3) {
				stepNum++;
				Tutorial_Init_Edit_1_1(stepNum);
				return true;
			}

		}
		break;
	case 9:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_1(stepNum);
			return false;
		}
		break;
	case 10:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_1(stepNum);
			return false;
		}
		break;
	case 11:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_1(stepNum);
			return false;
		}
		break;
	case 12:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_1(stepNum);
			return false;
		}
		break;
	case 13:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			isFinish = true;
			return false;
		}
		break;
	default:
		break;
	}


	return false;
}

void Tutorial_Balloon::Tutorial_Init_Battle_1_1(int step)
{

	switch (step)
	{
	case 1:
		isSpot_Circle = false;
		MOUSE->SetIsDoubleClicked(false);

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage2.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(540, 100));
		balloon.SetDisplayMode(false);

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage1.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);
		break;
	case 2:
		isSpot_Circle = true;
		popUpWindow.SetIsDisplay(false);

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(290 - 1280, 330 - 720));


		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage2.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(540, 100));
		balloon.SetDisplayMode(true);

		break;
	case 3:
		isSpot_Circle = true;
		popUpWindow.SetIsDisplay(false);

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(435 - 1280, 330 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage3.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(540, 100));
		balloon.SetDisplayMode(true);

		break;
	case 4:
		isSpot_Circle = true;
		MOUSE->SetIsDoubleClicked(true);
		popUpWindow.SetIsDisplay(false);

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(435 - 1280, 330 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage5.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(540, 100));
		balloon.SetDisplayMode(true);

		break;
	case 5:
		isSpot_Circle = true;
		popUpWindow.SetIsDisplay(false);

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(290 - 1280, 330 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage6.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(540, 100));
		balloon.SetDisplayMode(true);
		break;
	case 6:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage7.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	case 7:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage8.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);


		break;
	case 8:
		isSpot_Circle = true;
		popUpWindow.SetIsDisplay(false);

		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(625 - 1280, 140 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage9.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(140, 400));
		balloon.SetDisplayMode(true);
		break;
	case 9:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage10.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	case 10:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BattleMessage11.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	default:
		break;
	}

	return;

	return;
}

bool Tutorial_Balloon::Tutorial_Update_Battle_1_1(int step)
{
	switch (step)
	{
	case 1:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Battle_1_1(stepNum);
			return false;
		}
		break;
	case 2:
		if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L)) {
			Vector2D cursorPos = MOUSE->GetCursorPos();
			TiledVector tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
			if (!FIELD->IsInside(tiledCursorPos))
				return false;

			if (tiledCursorPos._x == 2 && tiledCursorPos._y == 5) {
				stepNum++;
				Tutorial_Init_Battle_1_1(stepNum);
				return true;
			}

		}

		break;
	case 3:
		if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L) && !ControlOrder) {
			Vector2D cursorPos = MOUSE->GetCursorPos();
			TiledVector tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
			if (!FIELD->IsInside(tiledCursorPos))
				return false;

			if (tiledCursorPos._x == 5 && tiledCursorPos._y == 5) {
				ControlOrder = true;
				WaitCount = 0;
				return true;
			}

		}

		if (ControlOrder) {
			isControlStop = true;
			WaitCount++;
			if (WaitCount > (30 * static_cast<double>(100 - 20 + 15) / 100) * 4) {
				WaitCount = 0;
				ControlOrder = false;
				isControlStop = false;
				stepNum++;
				Tutorial_Init_Battle_1_1(stepNum);
			}
			return true;
		}

		break;
	case 4:
		if (MOUSE->DoubleClicked() && !ControlOrder) {
			Vector2D cursorPos = MOUSE->GetCursorPos();
			TiledVector tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
			if (!FIELD->IsInside(tiledCursorPos))
				return false;

			if (tiledCursorPos._x == 5 && tiledCursorPos._y == 5) {
				ControlOrder = true;
				WaitCount = 0;
				return true;
			}

		}

		if (ControlOrder) {
			isControlStop = true;
			WaitCount++;
			if (WaitCount > (30 * static_cast<double>(100 - 20 + 15) / 100) * 4) {
				WaitCount = 0;
				ControlOrder = false;
				isControlStop = false;
				stepNum++;
				Tutorial_Init_Battle_1_1(stepNum);
			}
			return true;
		}
		break;
	case 5:
		if (MOUSE->DoubleClicked()) {
			Vector2D cursorPos = MOUSE->GetCursorPos();
			TiledVector tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
			if (!FIELD->IsInside(tiledCursorPos))
				return false;

			if (tiledCursorPos._x == 2 && tiledCursorPos._y == 5) {
				stepNum++;
				Tutorial_Init_Battle_1_1(stepNum);
				return true;
			}

		}
		break;
	case 6:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Battle_1_1(stepNum);
			return false;
		}

		break;
	case 7:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Battle_1_1(stepNum);
			return false;
		}
		break;
	case 8:
		if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L) && !ControlOrder) {
			Vector2D cursorPos = MOUSE->GetCursorPos();
			TiledVector tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
			if (!FIELD->IsInside(tiledCursorPos))
				return false;

			if (tiledCursorPos._x == 9 && tiledCursorPos._y == 1) {
				ControlOrder = true;
				WaitCount = 0;
				return true;
			}

		}

		if (ControlOrder) {
			isControlStop = true;
			WaitCount++;
			if (WaitCount > (30 * static_cast<double>(100 - 20 + 15) / 100) * 4) {
				WaitCount = 0;
				ControlOrder = false;
				isControlStop = false;
				stepNum++;
				Tutorial_Init_Battle_1_1(stepNum);
			}
			return true;
		}
		break;
	case 9:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Battle_1_1(stepNum);
			return false;
		}
		break;
	case 10:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			isFinish = true;
			return false;
		}
		break;
	default:
		break;
	}


	return false;
}

void Tutorial_Balloon::Tutorial_Init_Edit_1_2(int step)
{
	switch (step)
	{
	case 1:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/BoxMessage2.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(540, 100));
		balloon.SetDisplayMode(false);

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BoxMessage1.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);
		break;
	case 2:
		isSpot_Circle = true;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(680 - 1280, 140 - 720));

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BoxMessage2.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);

		break;
	case 3:
		isSpot_Circle = true;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(240 - 1280, 380 - 720));

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BoxMessage3.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);

		break;
	case 4:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BoxMessage4.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);

		break;
	case 5:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BoxMessage5.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	default:
		break;
	}

	return;
}

bool Tutorial_Balloon::Tutorial_Update_Edit_1_2(int step)
{

	switch (step)
	{
	case 1:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_2(stepNum);
			return false;
		}
		break;
	case 2:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_2(stepNum);
			return false;
		}

		break;
	case 3:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_2(stepNum);
			return false;
		}
		break;
	case 4:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_2(stepNum);
			return false;
		}
		break;
	case 5:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			isFinish = true;
			return false;
		}
		break;
	default:
		break;
	}


	return false;
}

void Tutorial_Balloon::Tutorial_Init_Edit_1_4(int step)
{
	switch (step)
	{
	case 1:
		isSpot_Circle = true;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(675 - 1280, 450 - 720));

		balloon.Load(Main::RESOURCEFILENAME + "resource/graph/ui/BossMessage2.png");
		balloon.SetRenderType(Texture2D::RenderType::UI);
		balloon.SetPriority(static_cast<int>(Sprite::Priority::UI));
		balloon.SetPosition(Vector2D(540, 100));
		balloon.SetDisplayMode(false);

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BossMessage1.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");
		popUpWindow.SetPosition(Vector2D(200, 300));

		popUpWindow.SetIsDisplay(true);
		break;
	case 2:
		isSpot_Circle = false;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BossMessage2.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);

		break;
	default:
		break;
	}

	return;
}

bool Tutorial_Balloon::Tutorial_Update_Edit_1_4(int step)
{

	switch (step)
	{
	case 1:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Edit_1_4(stepNum);
			return false;
		}
		break;
	case 2:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			isFinish = true;
			return false;
		}

		break;
	default:
		break;
	}

	return false;
}

void Tutorial_Balloon::Tutorial_Init_Battle_1_4(int step)
{
	switch (step)
	{
	case 1:
		isSpot_Circle = true;
		spotLightPoint._x = -1;
		spotLightPoint._y = -1;
		spotLightSize._x = -1;
		spotLightSize._y = -1;

		Spot_Circle.SetPosition(Vector2D(675 - 1280, 450 - 720));

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BossMessage3.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");
		popUpWindow.SetPosition(Vector2D(200, 300));
		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);

		break;
	case 2:
		isSpot_Circle = false;
		spotLightPoint._x = 726;
		spotLightPoint._y = 36;
		spotLightSize._x = 173;
		spotLightSize._y = 181;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BossMessage4.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);

		break;
	case 3:
		isSpot_Circle = false;
		spotLightPoint._x = 15;
		spotLightPoint._y = 42;
		spotLightSize._x = 187;
		spotLightSize._y = 123;

		popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/BossMessage5.png", Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");

		popUpWindow.SetIsDisplay(true);

		balloon.SetDisplayMode(false);
		break;
	default:
		break;
	}

	return;
}

bool Tutorial_Balloon::Tutorial_Update_Battle_1_4(int step)
{
	switch (step)
	{
	case 1:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Battle_1_4(stepNum);
			return false;
		}
		break;
	case 2:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			stepNum++;
			Tutorial_Init_Battle_1_4(stepNum);
			return false;
		}
		break;
	case 3:
		popUpWindow.Update();

		if (popUpWindow.GetIsDisplay()) {
			return false;
		}
		else {
			isFinish = true;
			return false;
		}
		break;
	default:
		break;
	}

	return false;
}




