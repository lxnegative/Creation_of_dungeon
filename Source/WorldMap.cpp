#include "WorldMap.h"
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <typeinfo>
#include "DxLib.h"
#include "Game.h"
#include "Title.h"
#include "EditMap.h"
#include "ShopScene.h"
#include <regex>
#include "cd_666s\Utility\CSVReader.h"
#include "cd_666s/DebugDraw.h"
#include "TutorialPanel.h"
#include "MoneyManager.h"
#include "ShopAssortment.h"
#include "Main.h"
#include "Information_SaveData.h"


WorldMap::WorldMap()
	: class_name("world_map")
	, _bgm(Main::RESOURCEFILENAME + "resource/sound/Field.mp3", true)
{

	Init(1, 1);
	_bgm.Play();

}

WorldMap::WorldMap(int AreaNum, int PointNum)
	: class_name("world_map")
	, _bgm(Main::RESOURCEFILENAME + "resource/sound/Field.mp3", true)
{

	Init(AreaNum, PointNum);
	_bgm.Play();

}

WorldMap::WorldMap(int AreaNum, int PointNum, bool isClear)
	: class_name("world_map")
	, _bgm(Main::RESOURCEFILENAME + "resource/sound/Field.mp3", true)
{

	Init(AreaNum, PointNum);
	_bgm.Play();


	if (isClear) {
		Information_SaveData::GetInstance()->setIsClearActivation(AreaNum, PointNum);

		auto point = getPoint(PointNum);

		for (int i = 0; i < point.lock()->pointConect.size(); i++) {
			if (typeid(*(getPoint(point.lock()->pointConect[i]->connectPointNum)).lock()) == typeid(AreaConnectPoint)) {
				auto tempPoint = std::dynamic_pointer_cast<AreaConnectPoint>(getPoint(point.lock()->pointConect[i]->connectPointNum).lock());
				Information_SaveData::GetInstance()->setIsPointActivation(tempPoint->moveArea, tempPoint->firstMovePoint);
			}
			else {
				auto tempPoint = getPoint(point.lock()->pointConect[i]->connectPointNum).lock();
				Information_SaveData::GetInstance()->setIsPointActivation(tempPoint->AreaNum, tempPoint->pointNum);
			}
		}

	}

	SetPointInformation();

}


WorldMap::~WorldMap()
{

}

SceneBase* WorldMap::Update(UIManager _ui)
{
	//_ui.Update();
	return Update();
}


SceneBase * WorldMap::Update()
{

	if (tutorialPanel == nullptr) {
		if (!Tutorial::getInstance()->isLookingTutorial("WorldMap")) {
			tutorialPanel = new TutorialPanel("WorldMap");
		}
	}

	if (tutorialPanel == nullptr) {
		if (Information_SaveData::GetInstance()->getIsClear(4, 16)) {
			if (!Tutorial::getInstance()->isLookingTutorial("Stage4-4_WorldMap")) {
				tutorialPanel = new TutorialPanel("Stage4-4_WorldMap");
			}
		}
	}

	if (tutorialPanel != nullptr) {
		tutorialPanel->Update();
		if (!tutorialPanel->GetisTutorial()) {
			if (tutorialPanel->GetTimingName() != "List/page") {
				Tutorial::getInstance()->LookTutorial(tutorialPanel->GetTimingName());
			}
			delete(tutorialPanel);
			tutorialPanel = nullptr;
		}
		return this;
	}

	popUpWindow.Update();

	if (popUpWindow.GetIsDisplay()) {
		return this;
	}

	char Buf[256];

	GetHitKeyStateAll(Buf);

	if (Buf[KEY_INPUT_ESCAPE] == 1)
	{
		if (keyFlag == false) {
			if (isPause) {
				PauseUI.SetDisplayMode(false);
				_black.SetDisplayMode(false);
			}
			isPause = !isPause;
			keyFlag = true;
		}

	}
	else {
		keyFlag = false;
	}

	int mx, my;

	GetMousePoint(&mx, &my);

	if (isPause == false) {
		/*
		if (blendFlag == 0) {
			blend += 10;
		}
		else {
			blend -= 10;
		}

		if (blend > 255) {
			blend = 255;
			blendFlag = 1;
		}
		if (blend < 0) {
			blend = 0;
			blendFlag = 0;
		}
		*/

		if (movePointList.empty() == true) {

			if (!worldMap_Menu->GetIsActive()) {

				for (int i = 0; i < nowAreaPointList.size(); i++) {

					if (nowAreaPointList[i]->isPoint == 1 && nowAreaPointList[i]->isStayPoint == 1) {

						int hit = 0;

						if (nowAreaPointList[i]->hitType == 0) {

							hit += ClickCheckCircle(nowAreaPointList[i]->x, nowAreaPointList[i]->y, pointR);
							hit += ClickCheckCircle(nowAreaPointList[i]->x - 5, nowAreaPointList[i]->y, pointR - 1);
							hit += ClickCheckCircle(nowAreaPointList[i]->x - 5 - 4, nowAreaPointList[i]->y, pointR - 2);
							hit += ClickCheckCircle(nowAreaPointList[i]->x - 5 - 4 - 3, nowAreaPointList[i]->y, pointR - 3);
							hit += ClickCheckCircle(nowAreaPointList[i]->x + 5, nowAreaPointList[i]->y, pointR - 1);
							hit += ClickCheckCircle(nowAreaPointList[i]->x + 5 + 4, nowAreaPointList[i]->y, pointR - 2);
							hit += ClickCheckCircle(nowAreaPointList[i]->x + 5 + 4 + 3, nowAreaPointList[i]->y, pointR - 3);


							hit += ClickCheckCircle(nowAreaPointList[i]->_stageIcon->GetPosition()._x + nowAreaPointList[i]->_stageIcon->GetTexturePtr().lock()->GetBaseSize()._x / 2, nowAreaPointList[i]->_stageIcon->GetPosition()._y + nowAreaPointList[i]->_stageIcon->GetTexturePtr().lock()->GetBaseSize()._y / 2, nowAreaPointList[i]->_stageIcon->GetTexturePtr().lock()->GetBaseSize()._x / 2);

							if (hit >= 1) {
								_decide.Play();
							}

						}
						else if (nowAreaPointList[i]->hitType == 100 && CanAreaMove(*(std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])))) {
							if (typeid(*nowAreaPointList[i]) == typeid(AreaConnectPoint)) {
								hit += ClickCheck(std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])->_signboard->GetPosition(), std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])->_signboard->GetTexturePtr().lock()->GetBaseSize());

								if (hit >= 1) {
									_flame.Play();
								}
							}
						}


						if (hit >= 1) {

							if (nowMyPointNum == nowAreaPointList[i]->pointNum) {
								if (nowAreaPointList[i]->stageNum.find("shop") != std::string::npos) {

									//std::regex regex("shop");
									//std::string shopNum = std::regex_replace(nowAreaPointList[i]->stageNum, regex, "");
									stageNum = nowAreaPointList[i]->stageNum;
									return new ShopScene(stageNum, nowMyPointNum);
								}
								else {

									stageNum = nowAreaPointList[i]->stageNum;
									//return new Game(stageNum);

									worldMap_Menu.reset();

									Information_SaveData::GetInstance()->TempSaveProcesses(nowMyAreaNum, nowMyPointNum);

									return new EditMap(std::to_string(nowMyAreaNum), stageNum, std::to_string(nowMyPointNum));
								}
							}
							else {
								searchPath(nowAreaPointList[i]->pointNum);
								//nowMyPointNum = nowAreaPointList[i]->pointNum;
							}
						}

					}
				}

				if (ClickCheck(_shopIcon.GetPosition(), _shopIcon.GetTexturePtr().lock()->GetBaseSize())) {
					_decide.Play();
					return new ShopScene("shop" + std::to_string(nowMyAreaNum), nowMyPointNum);
				}


			}



			if (ClickCheck(_MenuButton.GetPosition(), _MenuButton.GetTexturePtr().lock()->GetBaseSize())) {

				_decide.Play();

				worldMap_Menu->SetActive(!worldMap_Menu->GetIsActive());

				
				if (worldMap_Menu->GetIsActive()) {
					_StageSelectMenuButton.SetDisplayMode(false);
					_StageSelectMenuButton_Open.SetDisplayMode(false);
					_StageSelectButton1.SetDisplayMode(false);
					_StageSelectButton2.SetDisplayMode(false);
					_StageSelectButton3.SetDisplayMode(false);
					_SaveButton.SetDisplayMode(false);
				}
				else {
					_StageSelectMenuButton.SetDisplayMode(!isStageSelectButtonOpen);
					_StageSelectMenuButton_Open.SetDisplayMode(isStageSelectButtonOpen);
					if (canStageSelectButton1) {
						_StageSelectButton1.SetDisplayMode(isStageSelectButtonOpen);
					}
					else {
						_StageSelectButton1.SetDisplayMode(false);
					}
					if (canStageSelectButton2) {
						_StageSelectButton2.SetDisplayMode(isStageSelectButtonOpen);
					}
					else {
						_StageSelectButton2.SetDisplayMode(false);
					}
					if (canStageSelectButton3) {
						_StageSelectButton3.SetDisplayMode(isStageSelectButtonOpen);
					}
					else {
						_StageSelectButton3.SetDisplayMode(false);
					}
					_SaveButton.SetDisplayMode(true);
				}
				

			}
			
			if (ClickCheck(_StageSelectMenuButton.GetPosition(), _StageSelectMenuButton.GetTexturePtr().lock()->GetBaseSize()) && !worldMap_Menu->GetIsActive()) {
				if (isStageSelectButtonOpen) {
					_StageSelectMenuButton.SetDisplayMode(true);
					_StageSelectMenuButton_Open.SetDisplayMode(false);
					_StageSelectButton1.SetDisplayMode(false);
					_StageSelectButton2.SetDisplayMode(false);
					_StageSelectButton3.SetDisplayMode(false);
					isStageSelectButtonOpen = false;
				}
				else {
					_StageSelectMenuButton.SetDisplayMode(false);
					_StageSelectMenuButton_Open.SetDisplayMode(true);
					if (canStageSelectButton1) {
						_StageSelectButton1.SetDisplayMode(true);
					}
					else {
						_StageSelectButton1.SetDisplayMode(false);
					}
					if (canStageSelectButton2) {
						_StageSelectButton2.SetDisplayMode(true);
					}
					else {
						_StageSelectButton2.SetDisplayMode(false);
					}
					if (canStageSelectButton3) {
						_StageSelectButton3.SetDisplayMode(true);
					}
					else {
						_StageSelectButton3.SetDisplayMode(false);
					}
					isStageSelectButtonOpen = true;
				}

				_decide.Play();

			}
			
			if (isStageSelectButtonOpen && !worldMap_Menu->GetIsActive() && canStageSelectButton1) {
				if (ClickCheck(_StageSelectButton1.GetPosition(), _StageSelectButton1.GetTexturePtr().lock()->GetBaseSize())) {
					_flame.Play();
					nowMyAreaNum = stageSelectNumber[0];
					setNowAreaPointList();
					myPointSet_StageNum("1");
					worldMap_Menu.reset();
					_bgm.~Sound();
					_decide.~Sound();
					_flame.~Sound();
					_openTreasure.~Sound();
					popUpWindow.~PopUpWindow_YES();
					return new  WorldMap(nowMyAreaNum, nowMyPointNum);
				}
			}

			if (isStageSelectButtonOpen && !worldMap_Menu->GetIsActive() && canStageSelectButton2) {
				if (ClickCheck(_StageSelectButton2.GetPosition(), _StageSelectButton2.GetTexturePtr().lock()->GetBaseSize())) {
					_flame.Play();
					nowMyAreaNum = stageSelectNumber[1];
					setNowAreaPointList();
					myPointSet_StageNum("1");
					worldMap_Menu.reset();
					_bgm.~Sound();
					_decide.~Sound();
					_flame.~Sound();
					_openTreasure.~Sound();
					popUpWindow.~PopUpWindow_YES();
					return new  WorldMap(nowMyAreaNum, nowMyPointNum);
				}
			}

			if (isStageSelectButtonOpen && !worldMap_Menu->GetIsActive() && canStageSelectButton3) {
				if (ClickCheck(_StageSelectButton3.GetPosition(), _StageSelectButton3.GetTexturePtr().lock()->GetBaseSize())) {
					_flame.Play();
					nowMyAreaNum = stageSelectNumber[2];
					setNowAreaPointList();
					myPointSet_StageNum("1");
					worldMap_Menu.reset();
					_bgm.~Sound();
					_decide.~Sound();
					_flame.~Sound();
					_openTreasure.~Sound();
					popUpWindow.~PopUpWindow_YES();
					return new  WorldMap(nowMyAreaNum, nowMyPointNum);
				}
			}

			if (ClickCheck(_SaveButton.GetPosition(), _SaveButton.GetTexturePtr().lock()->GetBaseSize()) && !worldMap_Menu->GetIsActive()) {
				_openTreasure.Play();
				Information_SaveData::GetInstance()->SaveProcesses(nowMyAreaNum,nowMyPointNum);
				popUpWindow.SetIsDisplay(true);
			}

		}
		else {
			movePoint();
		}

		if (Main::DEBUGFLAG) {
			//F2でお金増加
			if (KEYBOARD->ButtonDown(KeyInput::KeyType::KEY_F2)) {
				MoneyManager::getInstance()->increaseMoney(10000);
			}

			//F3で全モンスター、トラップ、ブロックの購入と初期化（レベル1に）
			if (KEYBOARD->ButtonDown(KeyInput::KeyType::KEY_F3)) {
				ShopAssortment::getInstance()->AllAssortmentGet();
			}
		}
		


	}
	else {

		PauseUI.SetDisplayMode(true);

		auto backColor = Color4(0, 0, 0, 0.9f);
		_black.SetDisplayMode(true);
		_black.SetBaseColor(backColor);


		//Debug::DrawRectWithSize(Vector2D(0, 0), Vector2D(WINDOW_WIDTH, WINDOW_HEIGHT), backColor, true);

		Vector2D center(WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0);
		Vector2D frameSize(WINDOW_WIDTH / 4.0, WINDOW_HEIGHT / 8.0);

		auto frameColor = ColorPalette::WHITE4;
		Vector2D upperPannelPos(center._x - frameSize._x / 2.0, center._y - frameSize._y / 0.7);
		Vector2D centerPannelPos(center._x - frameSize._x / 2.0, center._y);
		Vector2D lowerPannelPos(center._x - frameSize._x / 2.0, center._y + frameSize._y / 0.7);

		int fontSize = 32;
		Debug::DrawString_TopPriority(upperPannelPos + frameSize * 0.5 - Vector2D(fontSize * 2.5 - 3.0, fontSize / 2 - 4.0), "ゲームに戻る", ColorPalette::WHITE4, 24);

		Debug::DrawString_TopPriority(centerPannelPos + frameSize * 0.5 - Vector2D(fontSize * 2.25 - 12.0, fontSize / 2 - 4.0), "初心者の館", ColorPalette::WHITE4, 24);

		Debug::DrawString_TopPriority(lowerPannelPos + frameSize * 0.5 - Vector2D(fontSize * 3.0 - 6.0, fontSize / 2 - 4.0), "タイトルに戻る", ColorPalette::WHITE4, 24);


		Debug::DrawRectWithSize_TopPriority(upperPannelPos, frameSize, frameColor, false);

		Debug::DrawRectWithSize_TopPriority(centerPannelPos, frameSize, frameColor, false);

		Debug::DrawRectWithSize_TopPriority(lowerPannelPos, frameSize, frameColor, false);


		auto mousePos = MOUSE->GetCursorPos();
		bool mouseOnUpperPannel =
			(upperPannelPos._x < mousePos._x
				&&  upperPannelPos._y < mousePos._y
				&&  mousePos._x < upperPannelPos._x + frameSize._x
				&&  mousePos._y < upperPannelPos._y + frameSize._y);

		bool mouseOnCenterPannel =
			(centerPannelPos._x < mousePos._x
				&&  centerPannelPos._y < mousePos._y
				&&  mousePos._x < centerPannelPos._x + frameSize._x
				&&  mousePos._y < centerPannelPos._y + frameSize._y);

		bool mouseOnLowerPannel =
			(lowerPannelPos._x < mousePos._x
				&&  lowerPannelPos._y < mousePos._y
				&&  mousePos._x < lowerPannelPos._x + frameSize._x
				&&  mousePos._y < lowerPannelPos._y + frameSize._y);

		auto fillColor = Color4(1.0f, 1.0f, 1.0f, 0.5f);
		Debug::DrawRectWithSize_TopPriority(upperPannelPos, frameSize, fillColor, mouseOnUpperPannel);

		Debug::DrawRectWithSize_TopPriority(centerPannelPos, frameSize, fillColor, mouseOnCenterPannel);

		Debug::DrawRectWithSize_TopPriority(lowerPannelPos, frameSize, fillColor, mouseOnLowerPannel);


		if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
		{
			if (mouseOnUpperPannel)
			{
				isPause = false;
				_black.SetDisplayMode(false);
				PauseUI.SetDisplayMode(false);
			}
			else if (mouseOnCenterPannel) {
				if (tutorialPanel == nullptr) {
					tutorialPanel = new TutorialPanel("List/page");
				}
			}
			else if (mouseOnLowerPannel)
			{
				_black.SetDisplayMode(false);
				PauseUI.SetDisplayMode(false);
				return new Title();
			}
		}

	}

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{

		if (clickFlag == false) {
			clickFlag = true;
		}

	}
	else {
		clickFlag = false;
	}


	worldMap_Menu->Update();


	printfDx("x:%d y:%d", mx, my);

	return this;
}

void WorldMap::Draw()
{

	clsDx();


	if (isPause == false) {

		popUpWindow.Draw();

		DrawMap();

		worldMap_Menu->Draw();

	}
	else {

		

		DrawMap();
		//worldMap_Menu->Draw();

		if (tutorialPanel != nullptr) {
			return;
		}



	}



}



void WorldMap::Init(int AreaNum, int PointNum) {

	Information_SaveData::GetInstance()->TempInit();
	/*

	finishPointNum = -1;

	isMyMove = false;

	myMoveDirection = -1;

	myVx = 0;

	myVy = 0;


	myPointSet(1);

	mySpeed = 3;

	*/

	stageSelectNumber.clear();

	for (int i = 1; i <= 4; i++) {
		if (AreaNum != i) {
			stageSelectNumber.push_back(i);
		}
	}

	nowMyAreaNum = stageSelectNumber[0];
	setNowAreaPointList();
	myPointSet_StageNum("1");
	canStageSelectButton1 = Information_SaveData::GetInstance()->getIsPoint(nowMyAreaNum, nowMyPointNum);

	nowMyAreaNum = stageSelectNumber[1];
	setNowAreaPointList();
	myPointSet_StageNum("1");
	canStageSelectButton2 = Information_SaveData::GetInstance()->getIsPoint(nowMyAreaNum, nowMyPointNum);

	nowMyAreaNum = stageSelectNumber[2];
	setNowAreaPointList();
	myPointSet_StageNum("1");
	canStageSelectButton3 = Information_SaveData::GetInstance()->getIsPoint(nowMyAreaNum, nowMyPointNum);


	AreaGrBuffer[0] = LoadGraph((Main::RESOURCEFILENAME + "resource/graph/worldMap/Area1Icon.png").c_str());
	AreaGrBuffer[1] = LoadGraph((Main::RESOURCEFILENAME + "resource/graph/worldMap/Area2Icon.png").c_str());
	AreaGrBuffer[2] = LoadGraph((Main::RESOURCEFILENAME + "resource/graph/worldMap/Area3Icon.png").c_str());
	AreaGrBuffer[3] = LoadGraph((Main::RESOURCEFILENAME + "resource/graph/worldMap/Area4Icon.png").c_str());
	AreaGrBuffer[4] = LoadGraph((Main::RESOURCEFILENAME + "resource/graph/worldMap/Area5Icon.png").c_str());
	AreaGrBuffer[5] = LoadGraph((Main::RESOURCEFILENAME + "resource/graph/worldMap/Area6Icon.png").c_str());
	AreaGrBuffer[6] = LoadGraph((Main::RESOURCEFILENAME + "resource/graph/worldMap/Area7Icon.png").c_str());
	AreaGrBuffer[7] = LoadGraph((Main::RESOURCEFILENAME + "resource/graph/worldMap/Area8Icon.png").c_str());

	AreaGr = &AreaGrBuffer[0];

	std::string fileName = Main::RESOURCEFILENAME + "resource/graph/worldMap//DevilGirlMini/DevilGirl";

	_animator.AddAnimation("front", std::make_shared<GraphArray>(fileName + "Front.png", 32, 48));
	_animator.AddAnimation("right", std::make_shared<GraphArray>(fileName + "Right.png", 32, 48));
	_animator.AddAnimation("left", std::make_shared<GraphArray>(fileName + "Left.png", 32, 48));
	_animator.AddAnimation("back", std::make_shared<GraphArray>(fileName + "Back.png", 32, 48));

	auto currentGraph = _animator.GetCurrentGraph();
	currentGraph->SetDisplayMode(true);
	_animator.Transform([&](GraphArray* animation)
	{
		animation->GetGraphPtr()->SetPosition(Vector2D(0, 0));
		animation->GetGraphPtr()->SetPriority(Sprite::Priority::UI);
	});


	blend = 0;
	blendFlag = 0;

	isPause = false;
	keyFlag = false;

	clickFlag = false;

	nowMyAreaNum = AreaNum;
	nowMyPointNum = PointNum;



	mySpeed = 0.0;
	mySpeedFrameNum = 90.0;

	myVx = 0.0;
	myVy = 0.0;

	debugFlag = false;

	SetAreaAndPoint(nowMyAreaNum, nowMyPointNum);


	CSVReader reader;

	fileName = Main::RESOURCEFILENAME + "csv/WorldMap/ShopPosition.csv";
	std::vector<std::string> Array;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, Array, 1);

	_shopIcon.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/IconShop" + std::to_string(AreaNum) + ".png");
	_shopIcon.SetPosition(Vector2D(std::stoi(Array[(nowMyAreaNum - 1) * 3 + 1]), std::stoi(Array[(nowMyAreaNum - 1) * 3 + 2])));
	_shopIcon.SetPriority(Sprite::Priority::UI);


	_MenuButton.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/menuButton.png");
	_MenuButton.SetPosition(Vector2D(1100, 10));
	_MenuButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);

	_StageSelectMenuButton.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stageClose.png");
	_StageSelectMenuButton.SetPosition(Vector2D(900, 10));
	_StageSelectMenuButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_StageSelectMenuButton.SetDisplayMode(true);

	_StageSelectMenuButton_Open.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stageOpen.png");
	_StageSelectMenuButton_Open.SetPosition(Vector2D(480, 10));
	_StageSelectMenuButton_Open.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_StageSelectMenuButton_Open.SetDisplayMode(false);

	

	_StageSelectButton1.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stage" + std::to_string(stageSelectNumber[0]) + ".png");
	_StageSelectButton1.SetPosition(Vector2D(500, 20));
	_StageSelectButton1.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_StageSelectButton1.SetScale(Vector2D(1.0 / 3.0, 1.0 / 3.0));
	_StageSelectButton1.SetDisplayMode(false);

	_StageSelectButton2.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stage" + std::to_string(stageSelectNumber[1]) + ".png");
	_StageSelectButton2.SetPosition(Vector2D(640, 20));
	_StageSelectButton2.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_StageSelectButton2.SetScale(Vector2D(1.0 / 3.0, 1.0 / 3.0));
	_StageSelectButton2.SetDisplayMode(false);

	_StageSelectButton3.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stage" + std::to_string(stageSelectNumber[2]) + ".png");
	_StageSelectButton3.SetPosition(Vector2D(780, 20));
	_StageSelectButton3.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_StageSelectButton3.SetScale(Vector2D(1.0 / 3.0, 1.0 / 3.0));
	_StageSelectButton3.SetDisplayMode(false);




	_SaveButton.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/saveButton.png");
	_SaveButton.SetPosition(Vector2D(1000, 10));
	_SaveButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_SaveButton.SetDisplayMode(true);

	worldMap_Menu = std::make_shared<WorldMap_Menu>();

	worldMap_Menu->SetActive(false);

	isStageSelectButtonOpen = false;

	PauseUI.Load(Main::RESOURCEFILENAME + "resource/graph/ui/PAUSE.png");
	PauseUI.SetRenderType(Texture2D::RenderType::UI);
	PauseUI.SetPriority(static_cast<int>(Sprite::Priority::UI));
	PauseUI.SetPosition(Vector2D(540, 100));
	PauseUI.SetDisplayMode(false);

	_black.Load(Main::RESOURCEFILENAME + "resource/graph/ui/result/result_Black.png");
	_black.SetRenderType(Texture2D::RenderType::UI);
	_black.SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);
	_black.SetPosition(Vector2D(0, 0));
	_black.SetDisplayMode(false);

	_AreaIcon.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stage" + std::to_string(nowMyAreaNum) + ".png");
	_AreaIcon.SetRenderType(Texture2D::RenderType::UI);
	_AreaIcon.SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);
	_AreaIcon.SetPosition(Vector2D(10, 10));
	_AreaIcon.SetScale(Vector2D(1.0 / 3.0, 1.0 / 3.0));
	_AreaIcon.SetDisplayMode(true);

	popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/SystemMessage1.png", Main::RESOURCEFILENAME + "resource/graph/ui/OK.png");
	
	popUpWindow.SetIsDisplay(false);


	_decide.Load(Main::RESOURCEFILENAME + "resource/sound/decide.wav");
	_decide.SetVolume(200);

	_openTreasure.Load(Main::RESOURCEFILENAME + "resource/sound/openTreasure.wav");
	_openTreasure.SetVolume(200);

	_flame.Load(Main::RESOURCEFILENAME + "resource/sound/flame.wav");
	_flame.SetVolume(200);


}


void WorldMap::DrawMap() {

	DrawGraph(0, 0, *AreaGr, TRUE);


	for (int i = 0; i < nowRoadConnect.size(); i++) {

		int road1 = PointSearch(nowRoadConnect[i]->road[0]);
		int road2 = PointSearch(nowRoadConnect[i]->road[1]);

		DrawLine(nowAreaPointList[road1]->x, nowAreaPointList[road1]->y, nowAreaPointList[road2]->x, nowAreaPointList[road2]->y, GetColor(243, 237, 226), 15);

	}


	for (int i = 0; i < nowAreaPointList.size(); i++) {

		if (nowAreaPointList[i]->isPoint == 1) {

			if (nowAreaPointList[i]->isStayPoint == 1) {
				if (typeid(*nowAreaPointList[i]) == typeid(AreaConnectPoint)) {
					//DrawCircle(std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])->buttonX, std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])->buttonY, 15, GetColor(255, 0, 0), TRUE);
				}
				else {

					/*
					DrawCircle(nowAreaPointList[i]->x, nowAreaPointList[i]->y, pointR, GetColor(255, 0, 0), TRUE);
					DrawCircle(nowAreaPointList[i]->x - 5, nowAreaPointList[i]->y, pointR - 1, GetColor(255, 0, 0), TRUE);
					DrawCircle(nowAreaPointList[i]->x - 5 - 4, nowAreaPointList[i]->y, pointR - 2, GetColor(255, 0, 0), TRUE);
					DrawCircle(nowAreaPointList[i]->x - 5 - 4 - 3, nowAreaPointList[i]->y, pointR - 3, GetColor(255, 0, 0), TRUE);
					DrawCircle(nowAreaPointList[i]->x + 5, nowAreaPointList[i]->y, pointR - 1, GetColor(255, 0, 0), TRUE);
					DrawCircle(nowAreaPointList[i]->x + 5 + 4, nowAreaPointList[i]->y, pointR - 2, GetColor(255, 0, 0), TRUE);
					DrawCircle(nowAreaPointList[i]->x + 5 + 4 + 3, nowAreaPointList[i]->y, pointR - 3, GetColor(255, 0, 0), TRUE);
					*/
				}
			}
			else {
				DrawCircle(nowAreaPointList[i]->x, nowAreaPointList[i]->y, pointR, GetColor(255, 0, 0), TRUE);
			}

		}

		if (nowAreaPointList[i]->pointNum == nowMyPointNum) {
			myX = nowAreaPointList[i]->x;
			myY = nowAreaPointList[i]->y;
		}

	}


	if (nowMyPointNum != -1 || (myVx == 0 && myVy == 0)) {
		_animator.SwitchWithReset("front");
	}
	else if (fabs(myVx) > fabs(myVy)) {
		if (myVx > 0) {
			_animator.SwitchWithReset("right");
		}
		else {
			_animator.SwitchWithReset("left");
		}
	}
	else {
		if (myVy > 0) {
			_animator.SwitchWithReset("front");
		}
		else {
			_animator.SwitchWithReset("back");
		}
	}

	_animator.Transform([&](GraphArray* animation)
	{
		animation->GetGraphPtr()->SetPosition(Vector2D(myX - animation->GetSingleSize()._x / 2, myY - animation->GetSingleSize()._y / 2 - 12));
	});

	_animator.Update();


}


bool WorldMap::myPointSet_PointNum(int PointNum) {

	std::vector<Point>::iterator itr;


	for (auto itr = nowAreaPointList.begin(); itr != nowAreaPointList.end(); itr++) {

		if ((*itr)->pointNum == PointNum) {
			nowMyPointNum = (*itr)->pointNum;
			myX = (*itr)->x;
			myY = (*itr)->y;
			return true;
		}

	}

	return false;


}

bool WorldMap::myPointSet_StageNum(std::string StageNum) {

	std::vector<Point>::iterator itr;


	for (auto itr = nowAreaPointList.begin(); itr != nowAreaPointList.end(); itr++) {

		if ((*itr)->stageNum == StageNum) {
			nowMyPointNum = (*itr)->pointNum;
			myX = (*itr)->x;
			myY = (*itr)->y;
			return true;
		}

	}

	return false;


}


int WorldMap::PointSearch(int pointNum) {


	for (int vec = 0; vec < nowAreaPointList.size(); vec++) {

		if (nowAreaPointList[vec]->pointNum == pointNum) {
			return vec;
		}

	}



	return -1;



}






bool WorldMap::checkMoveReach(std::weak_ptr<WorldMap::Point> point) {



	double margin = 5.0;

	if (point.lock()->x - margin <= myX && point.lock()->x + margin >= myX && point.lock()->y - margin <= myY && point.lock()->y + margin >= myY) {

		myX = point.lock()->x;
		myY = point.lock()->y;

		return true;
	}



	return false;



}

void WorldMap::setNowAreaPointList() {

	//nowAreaPointList.reserve(100);

	CSVReader reader;

	//ここからポイント情報の読み込み
	//ファイルの読み込み

	nowAreaPointList.clear();

	std::string path = Main::RESOURCEFILENAME + "csv/WorldMap/Area" + std::to_string(nowMyAreaNum) + ".csv";

	if (debugFlag == true) {
		path = Main::RESOURCEFILENAME + "csv/WorldMap/Area_debug.csv";
	}

	int FileHandle;
	char str[512];

	FileHandle = FileRead_open(path.c_str());

	int dataType = 0;   //読み込むデータ　ポイント情報か、エリア移動か

	//csvファイルを1行ずつ読み込む
	// ファイルの終端が来るまで表示する
	while (FileRead_eof(FileHandle) == 0)
	{

		// 一行読み込み
		FileRead_gets(str, 512, FileHandle);

		std::string s = str;

		std::string::size_type index = s.find("#");  // "#"を検索
													 //"#"が入っていた行は飛ばす
		if (index != std::string::npos) {
			continue;
		}

		index = s.find("end");  // "end"を検索

		if (index != std::string::npos) {
			dataType++;
			continue;
		}

		std::shared_ptr<Point> point = std::make_shared<Point>();

		std::shared_ptr<AreaConnectPoint> areaConnectPoint = std::make_shared<AreaConnectPoint>();

		std::vector<std::string> strVec = reader.split(s, ',');

		for (int i = 0; i < strVec.size(); i++) {

			if (dataType == 0) {

				switch (i) {
				case 0:
					point->pointNum = stoi(strVec[i]);
					break;
				case 1:
					point->stageNum = strVec[i];
					break;
				case 2:
					point->x = stoi(strVec[i]);
					break;
				case 3:
					point->y = stoi(strVec[i]);
					break;
				case 4:
					point->hitType = stoi(strVec[i]);
					break;
				case 5:
					point->isStayPoint = stoi(strVec[i]);
					break;

				default:

					if (i > 5) {
						PointConnect* pc = new PointConnect();
						pc->connectPointNum = stoi(strVec[i]);
						pc->PointCost = -1;
						point->pointConect.push_back(pc);
					}

					break;
				}
			}
			else {

				switch (i) {
				case 0:
					areaConnectPoint->pointNum = stoi(strVec[i]);
					break;
				case 1:
					areaConnectPoint->moveArea = stoi(strVec[i]);
					break;
				case 2:
					areaConnectPoint->buttonX = stoi(strVec[i]);
					break;
				case 3:
					areaConnectPoint->buttonY = stoi(strVec[i]);
					break;
				case 4:
					areaConnectPoint->x = stoi(strVec[i]);
					break;
				case 5:
					areaConnectPoint->y = stoi(strVec[i]);
					break;
				case 6:
					areaConnectPoint->firstMovePoint = stoi(strVec[i]);
					break;
				case 7:
					areaConnectPoint->firstX = stoi(strVec[i]);
					break;
				case 8:
					areaConnectPoint->firstY = stoi(strVec[i]);
					break;

				default:

					if (i > 8) {
						PointConnect* pc = new PointConnect();
						pc->connectPointNum = stoi(strVec[i]);
						pc->PointCost = -1;
						areaConnectPoint->pointConect.push_back(pc);
					}

					break;

				}
			}

		}

		if (dataType == 0) {
			point->AreaNum = nowMyAreaNum;
			point->isPoint = Information_SaveData::GetInstance()->getIsPoint(point->AreaNum, point->pointNum);
			point->isClear = Information_SaveData::GetInstance()->getIsClear(point->AreaNum, point->pointNum);
			point->_stageIcon = std::make_shared<Sprite>();
			if (point->AreaNum == 4 && std::stoi(point->stageNum) == 4) {
				point->_stageIcon->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/IconBossVer2.png");
			}
			else {
				point->_stageIcon->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/" + GetFieldType(point->stageNum));
			}
			point->_stageIcon->SetPosition(Vector2D(point->x - point->_stageIcon->GetTexturePtr().lock()->GetBaseSize()._x / 2, point->y - point->_stageIcon->GetTexturePtr().lock()->GetBaseSize()._y / 2 - 80));
			point->_stageIcon->SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);
			point->_stagePoint = std::make_shared<Sprite>();
			if (point->isPoint == 0) {
				point->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_3.png");
			}
			else {
				if (point->isClear == 0) {
					point->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_1.png");
				}
				else {
					point->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_2.png");
				}
			}

			point->_stagePoint->SetPosition(Vector2D(point->x - point->_stagePoint->GetTexturePtr().lock()->GetBaseSize()._x / 2, point->y - point->_stagePoint->GetTexturePtr().lock()->GetBaseSize()._y / 2));
			point->_stagePoint->SetPriority(Sprite::Priority::FIELD_BASE);
			nowAreaPointList.push_back(point);
		}
		else if (dataType == 1) {
			areaConnectPoint->AreaNum = nowMyAreaNum;
			areaConnectPoint->hitType = 100;

			if (CanAreaMove(*areaConnectPoint)) {
				Information_SaveData::GetInstance()->setIsPointActivation(areaConnectPoint->AreaNum, areaConnectPoint->pointNum);
			}

			areaConnectPoint->isPoint = Information_SaveData::GetInstance()->getIsPoint(areaConnectPoint->AreaNum, areaConnectPoint->pointNum);
			areaConnectPoint->isClear = Information_SaveData::GetInstance()->getIsClear(areaConnectPoint->AreaNum, areaConnectPoint->pointNum);

			areaConnectPoint->isStayPoint = 1;
			areaConnectPoint->stageNum = "";
			areaConnectPoint->_stageIcon = nullptr;
			areaConnectPoint->_stagePoint = std::make_shared<Sprite>();
			areaConnectPoint->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_1.png");
			areaConnectPoint->_stagePoint->SetPosition(Vector2D(areaConnectPoint->x - areaConnectPoint->_stagePoint->GetTexturePtr().lock()->GetBaseSize()._x / 2, areaConnectPoint->y - areaConnectPoint->_stagePoint->GetTexturePtr().lock()->GetBaseSize()._y / 2));
			areaConnectPoint->_stagePoint->SetPriority(Sprite::Priority::FIELD_BASE);
			areaConnectPoint->_signboard = std::make_shared<Sprite>();
			std::string dir = "Left";
			if (areaConnectPoint->buttonX > 1280 / 2) {
				dir = "Right";
			}
			areaConnectPoint->_signboard->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Signboard_" + dir + ".png");
			areaConnectPoint->_signboard->SetPosition(Vector2D(areaConnectPoint->buttonX, areaConnectPoint->buttonY));
			areaConnectPoint->_signboard->SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);
			if (CanAreaMove(*areaConnectPoint)) {
				areaConnectPoint->_signboard->SetDisplayMode(true);
			}
			else {
				areaConnectPoint->_signboard->SetDisplayMode(false);
			}
			nowAreaPointList.push_back(areaConnectPoint);
		}

	}

	/*
	std::ifstream ifs(path);
	if (ifs.fail()) {
		printfDx("fail\n");
		return;
	}

	int dataType = 0;   //読み込むデータ　ポイント情報か、エリア移動か

	//csvファイルを1行ずつ読み込む
	std::string str;
	while (getline(ifs, str)) {
		std::string::size_type index = str.find("#");  // "#"を検索
													   //"#"が入っていた行は飛ばす
		if (index != std::string::npos) {
			continue;
		}

		index = str.find("end");  // "end"を検索

		if (index != std::string::npos) {
			dataType++;
			continue;
		}

		std::string token;
		std::istringstream stream(str);

		std::shared_ptr<Point> point = std::make_shared<Point>();

		std::shared_ptr<AreaConnectPoint> areaConnectPoint = std::make_shared<AreaConnectPoint>();

		int num = 0;
		//1行のうち、文字列とコンマを分割する
		while (getline(stream, token, ',')) {

			if (dataType == 0) {

				switch (num) {
				case 0:
					point->pointNum = stoi(token);
					break;
				case 1:
					point->stageNum = token;
					break;
				case 2:
					point->x = stoi(token);
					break;
				case 3:
					point->y = stoi(token);
					break;
				case 4:
					point->hitType = stoi(token);
					break;
				case 5:
					point->isStayPoint = stoi(token);
					break;

				default:

					if (num > 5) {
						PointConnect* pc = new PointConnect();
						pc->connectPointNum = stoi(token);
						pc->PointCost = -1;
						point->pointConect.push_back(pc);
					}

					break;
				}
			}
			else {

				switch (num) {
				case 0:
					areaConnectPoint->pointNum = stoi(token);
					break;
				case 1:
					areaConnectPoint->moveArea = stoi(token);
					break;
				case 2:
					areaConnectPoint->buttonX = stoi(token);
					break;
				case 3:
					areaConnectPoint->buttonY = stoi(token);
					break;
				case 4:
					areaConnectPoint->x = stoi(token);
					break;
				case 5:
					areaConnectPoint->y = stoi(token);
					break;
				case 6:
					areaConnectPoint->firstMovePoint = stoi(token);
					break;
				case 7:
					areaConnectPoint->firstX = stoi(token);
					break;
				case 8:
					areaConnectPoint->firstY = stoi(token);
					break;

				default:

					if (num > 8) {
						PointConnect* pc = new PointConnect();
						pc->connectPointNum = stoi(token);
						pc->PointCost = -1;
						areaConnectPoint->pointConect.push_back(pc);
					}

					break;

				}
			}
			num++;
		}

		if (num != -1 && dataType == 0) {
			point->AreaNum = nowMyAreaNum;
			point->isPoint = Information_SaveData::GetInstance()->getIsPoint(point->AreaNum, point->pointNum);
			point->isClear = Information_SaveData::GetInstance()->getIsClear(point->AreaNum, point->pointNum);
			point->_stageIcon = std::make_shared<Sprite>();
			if (point->AreaNum == 4 && std::stoi(point->stageNum) == 4) {
				point->_stageIcon->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/IconBossVer2.png");
			}
			else {
				point->_stageIcon->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/" + GetFieldType(point->stageNum));
			}
			point->_stageIcon->SetPosition(Vector2D(point->x - point->_stageIcon->GetTexturePtr().lock()->GetBaseSize()._x / 2, point->y - point->_stageIcon->GetTexturePtr().lock()->GetBaseSize()._y / 2 - 80));
			point->_stageIcon->SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);
			point->_stagePoint = std::make_shared<Sprite>();
			if (point->isPoint == 0) {
				point->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_3.png");
			}
			else {
				if (point->isClear == 0) {
					point->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_1.png");
				}
				else {
					point->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_2.png");
				}
			}
			
			point->_stagePoint->SetPosition(Vector2D(point->x - point->_stagePoint->GetTexturePtr().lock()->GetBaseSize()._x / 2, point->y - point->_stagePoint->GetTexturePtr().lock()->GetBaseSize()._y / 2));
			point->_stagePoint->SetPriority(Sprite::Priority::FIELD_BASE);
			nowAreaPointList.push_back(point);
		}
		else if (num != -1 && dataType == 1) {
			areaConnectPoint->AreaNum = nowMyAreaNum;
			areaConnectPoint->hitType = 100;
			
			if (CanAreaMove(*areaConnectPoint)) {
				Information_SaveData::GetInstance()->setIsPointActivation(areaConnectPoint->AreaNum, areaConnectPoint->pointNum);
			}

			areaConnectPoint->isPoint = Information_SaveData::GetInstance()->getIsPoint(areaConnectPoint->AreaNum, areaConnectPoint->pointNum);
			areaConnectPoint->isClear = Information_SaveData::GetInstance()->getIsClear(areaConnectPoint->AreaNum, areaConnectPoint->pointNum);

			areaConnectPoint->isStayPoint = 1;
			areaConnectPoint->stageNum = "";
			areaConnectPoint->_stageIcon = nullptr;
			areaConnectPoint->_stagePoint = std::make_shared<Sprite>();
			areaConnectPoint->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_1.png");
			areaConnectPoint->_stagePoint->SetPosition(Vector2D(areaConnectPoint->x - areaConnectPoint->_stagePoint->GetTexturePtr().lock()->GetBaseSize()._x / 2, areaConnectPoint->y - areaConnectPoint->_stagePoint->GetTexturePtr().lock()->GetBaseSize()._y / 2));
			areaConnectPoint->_stagePoint->SetPriority(Sprite::Priority::FIELD_BASE);
			areaConnectPoint->_signboard = std::make_shared<Sprite>();
			std::string dir = "Left";
			if (areaConnectPoint->buttonX > 1280 / 2) {
				dir = "Right";
			}
			areaConnectPoint->_signboard->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Signboard_" + dir + ".png");
			areaConnectPoint->_signboard->SetPosition(Vector2D(areaConnectPoint->buttonX, areaConnectPoint->buttonY));
			areaConnectPoint->_signboard->SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);
			if (CanAreaMove(*areaConnectPoint)) {
				areaConnectPoint->_signboard->SetDisplayMode(true);
			}
			else {
				areaConnectPoint->_signboard->SetDisplayMode(false);
			}
			nowAreaPointList.push_back(areaConnectPoint);
		}

	}
	*/

	setPointCost();


}

void WorldMap::setNowRoadConnect() {

	nowRoadConnect.clear();

	for (int i = 0; i < nowAreaPointList.size(); i++) {

		if (nowAreaPointList[i]->isPoint == 1) {

			for (int j = 0; j < nowAreaPointList[i]->pointConect.size(); j++) {

				bool isConnect = true;

				for (int n = 0; n < nowRoadConnect.size(); n++) {

					for (int b = 0; b < 2; b++) {
						if (nowRoadConnect[n]->road[(b + 2) % 2] == nowAreaPointList[i]->pointNum && nowRoadConnect[n]->road[(b + 1 + 2) % 2] == nowAreaPointList[i]->pointConect[j]->connectPointNum) {
							isConnect = false;
							break;
						}
					}
					if (isConnect == false) {
						break;
					}

				}

				if (isConnect == true && getPoint(nowAreaPointList[i]->pointConect[j]->connectPointNum).lock()->isPoint == 1) {
					RoadConnect* rc = new RoadConnect();
					rc->road[0] = nowAreaPointList[i]->pointNum;
					rc->road[1] = nowAreaPointList[i]->pointConect[j]->connectPointNum;
					nowRoadConnect.push_back(rc);
				}

			}



		}


	}
}

void WorldMap::setPointCost() {

	for (int i = 0; i < nowAreaPointList.size(); i++) {
		for (int j = 0; j < nowAreaPointList[i]->pointConect.size(); j++) {

			if (nowAreaPointList[i]->pointConect[j]->PointCost == -1) {

				std::weak_ptr<Point> tempPoint = getPoint(nowAreaPointList[i]->pointConect[j]->connectPointNum);
				nowAreaPointList[i]->pointConect[j]->PointCost = std::hypot(nowAreaPointList[i]->x - tempPoint.lock()->x, nowAreaPointList[i]->y - tempPoint.lock()->y);

				for (int n = 0; n < tempPoint.lock()->pointConect.size(); n++) {
					if (tempPoint.lock()->pointConect[n]->connectPointNum == nowAreaPointList[i]->pointNum) {
						tempPoint.lock()->pointConect[n]->PointCost = nowAreaPointList[i]->pointConect[j]->PointCost;
					}
				}

			}

		}
	}

}



int WorldMap::ClickCheckBox(int x, int y, int rx, int ry) {

	int mx, my;

	GetMousePoint(&mx, &my);

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{

		if (clickFlag == false) {
			if (mx > x && mx < x + rx && my > y && my < y + ry) {
				return 1;
			}
		}

	}

	return 0;

}


int WorldMap::ClickCheckCircle(int x, int y, int r) {

	int mx, my;

	GetMousePoint(&mx, &my);

	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{

		if (clickFlag == false) {
			if (pow((x - mx)*(x - mx) + (y - my)*(y - my), 0.5) < r) {
				return 1;
			}
		}

	}

	return 0;

}




std::weak_ptr<WorldMap::Point> WorldMap::getPoint(int pointNum) {

	for (int i = 0; i < nowAreaPointList.size(); i++) {
		if (nowAreaPointList[i]->pointNum == pointNum) {
			return nowAreaPointList[i];
		}
	}

	return shared_ptr<WorldMap::Point>();

}



void WorldMap::movePoint() {

	std::weak_ptr<WorldMap::Point> point;

	point = getPoint(movePointList[0]);

	bool isArrive = false;

	isArrive = checkMoveReach(point);

	if (isArrive == true) {


		if (typeid(*point.lock()) == typeid(AreaConnectPoint)) {

			std::weak_ptr<WorldMap::AreaConnectPoint> areaPoint = std::dynamic_pointer_cast<AreaConnectPoint>(point.lock());

			myX = areaPoint.lock()->firstX;
			myY = areaPoint.lock()->firstY;

			int movePoint = areaPoint.lock()->firstMovePoint;

			setAreaNum(areaPoint.lock()->moveArea);

			movePointList.push_back(movePoint);

			int moveCost = std::hypot(getPoint(movePoint).lock()->x - myX, getPoint(movePoint).lock()->y - myY);

			mySpeed = moveCost / mySpeedFrameNum;

		}
		else {
			movePointList.erase(movePointList.begin());

			if (movePointList.empty()) {
				nowMyPointNum = point.lock()->pointNum;
				mySpeed = 0.0;
				myVx = 0.0;
				myVy = 0.0;
				_animator.GetCurrentAnimation()->SetCurrentTime(0);
			}
		}


	}
	else {

		//mySpeed = mySpeedMax;

		double radian = 0.0;

		radian = atan2(point.lock()->y - myY, point.lock()->x - myX);

		myVx = cos(radian) * mySpeed;
		myVy = sin(radian) * mySpeed;

		if (CheckLineOfPoint(Vector2D(point.lock()->x, point.lock()->y), Vector2D(myX, myY), Vector2D(myX + myVx, myY + myVy))) {
			myX = point.lock()->x;
			myY = point.lock()->y;
		}
		else {

			myX += myVx;
			myY += myVy;

		}

	}


}


void WorldMap::searchPath(int pointNum) {

	movePointList.clear();

	std::vector<SearchPointConnect*> spc;
	spc.clear();

	SearchPointConnect* sss = new SearchPointConnect();
	sss->connectPointNum = -1;
	sss->isSearch = false;
	sss->PointCost = 0;
	sss->pointNum = pointNum;

	spc.push_back(sss);

	setSearchPointConnect(pointNum, &spc);


	setMovePath(nowMyPointNum, &spc);


	/*
	Point* point;

	point = getPoint(nowMyPointNum);

	for (PointConnect* connectPoint : point->pointConect) {
		if (pointNum == connectPoint->connectPointNum) {
			movePointList.push_back(connectPoint->connectPointNum);
		}
	}
	*/



	int moveCost = 0;
	for (SearchPointConnect* temp : spc) {
		if (nowMyPointNum == temp->pointNum) {
			moveCost = temp->PointCost;
			break;
		}
	}

	mySpeed = moveCost / mySpeedFrameNum;



	if (movePointList.empty()) {

	}
	else {
		nowMyPointNum = -1;
		_animator.GetCurrentAnimation()->SetCurrentTime(0);
	}





}




void WorldMap::setSearchPointConnect(int pointNum, std::vector<SearchPointConnect*>* spc) {

	double cost = 0;

	for (SearchPointConnect* ss : *spc) {
		if (ss->pointNum == pointNum && ss->isSearch == true) {
			return;
		}
		else if (ss->pointNum == pointNum && ss->isSearch == false) {
			ss->isSearch = true;
			cost = ss->PointCost;
		}
	}



	std::weak_ptr<Point> point;

	point = getPoint(pointNum);

	for (PointConnect* connectPoint : point.lock()->pointConect) {

		bool is = false;

		for (SearchPointConnect* ss : *spc) {
			if (connectPoint->connectPointNum == ss->pointNum) {
				is = true;
				if (cost + connectPoint->PointCost < ss->PointCost) {
					ss->connectPointNum = pointNum;
					ss->PointCost = cost + connectPoint->PointCost;
				}
			}
		}

		if (is == false) {
			SearchPointConnect* ssq = new SearchPointConnect();
			ssq->connectPointNum = pointNum;
			ssq->isSearch = false;
			ssq->PointCost = cost + connectPoint->PointCost;
			ssq->pointNum = connectPoint->connectPointNum;
			spc->push_back(ssq);
		}

	}

	int pn = -1;
	int minCost = -1;

	for (SearchPointConnect* ss : *spc) {

		if (minCost == -1 && ss->isSearch == false) {
			pn = ss->pointNum;
			minCost = ss->PointCost;
		}
		else if (minCost > ss->PointCost && ss->isSearch == false) {
			pn = ss->pointNum;
			minCost = ss->PointCost;
		}

	}

	if (minCost == -1 && pn == -1) {
		return;
	}
	else {
		setSearchPointConnect(pn, spc);
	}


}



void WorldMap::setMovePath(int pointNum, std::vector<SearchPointConnect*>* spc) {

	for (SearchPointConnect* ss : *spc) {
		if (ss->pointNum == pointNum) {
			if (ss->connectPointNum == -1) {
				return;
			}
			else {
				movePointList.push_back(ss->connectPointNum);
				setMovePath(ss->connectPointNum, spc);
			}
		}
	}

}



void WorldMap::setAreaNum(int areaNum) {

	nowMyAreaNum = areaNum;
	nowMyPointNum = -1;

	stageSelectNumber.clear();

	for (int i = 1; i <= 4; i++) {
		if (nowMyAreaNum != i) {
			stageSelectNumber.push_back(i);
		}
	}

	_StageSelectButton1.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stage" + std::to_string(stageSelectNumber[0]) + ".png");
	_StageSelectButton2.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stage" + std::to_string(stageSelectNumber[1]) + ".png");
	_StageSelectButton3.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stage" + std::to_string(stageSelectNumber[2]) + ".png");

	_AreaIcon.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/stage" + std::to_string(nowMyAreaNum) + ".png");

	_shopIcon.Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/IconShop" + std::to_string(nowMyAreaNum) + ".png");

	mySpeed = 0.0;

	myVx = 0.0;
	myVy = 0.0;

	_animator.GetCurrentAnimation()->SetCurrentTime(0);

	setNowAreaPointList();

	setNowRoadConnect();

	movePointList.clear();

	AreaGr = &AreaGrBuffer[areaNum - 1];

	std::string fileName = Main::RESOURCEFILENAME + "csv/WorldMap/ShopPosition.csv";
	std::vector<std::string> Array;
	CSVReader reader;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, Array, 1);

	_shopIcon.SetPosition(Vector2D(std::stoi(Array[(nowMyAreaNum - 1) * 3 + 1]), std::stoi(Array[(nowMyAreaNum - 1) * 3 + 2])));
	_shopIcon.SetPriority(Sprite::Priority::UI);


}

//現在読み込んでいるポイントにのみ移動する（ない場合はエラー）
void WorldMap::setPointNum(int pointNum) {

	nowMyPointNum = pointNum;

	mySpeed = 0.0;

	myVx = 0.0;
	myVy = 0.0;

	_animator.GetCurrentAnimation()->SetCurrentTime(0);

	movePointList.clear();

	std::weak_ptr<WorldMap::Point> p = getPoint(pointNum);

	myX = p.lock()->x;
	myY = p.lock()->y;

}

void WorldMap::SetAreaAndPoint(int areaNum, int pointNum) {

	setAreaNum(areaNum);
	setPointNum(pointNum);

}


bool WorldMap::ClickCheck(Vector2D pos, Vector2D size) {

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



std::string WorldMap::GetFieldType(std::string stageNum) {

	//ダンジョンの地形の設定

	CSVReader reader;

	std::vector<std::string> FieldTypeArray;
	std::string fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + std::to_string(nowMyAreaNum) + "/DungeonType.csv";
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, FieldTypeArray, 2);

	int s = 0;
	std::smatch results;
	if (std::regex_search(stageNum, results, std::regex("\\d+"))) {
		s = stoi(results.str());
	}

	int FieldTypeNum = stoi(FieldTypeArray[s * 2 - 1]);

	switch (FieldTypeNum) {
	case 0:
		return "IconCave.png";
		break;
	case 1:
		return "IconForest.png";
		break;
	case 2:
		return "IconStone.png";
		break;
	case 3:
		return "IconWood.png";
		break;
	default:
		return "IconCave.png";
		break;
	}

}

//点Pと線分(AB)の距離
bool WorldMap::CheckLineOfPoint(Vector2D P, Vector2D A, Vector2D B)
{
	Vector2D temp = B - A;

	if (temp._x >= 0 && temp._y >= 0) {
		if (P._x >= A._x && P._y >= A._y && P._x <= B._x && P._y <= B._y) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (temp._x >= 0 && temp._y < 0) {
		if (P._x >= A._x && P._y <= A._y && P._x <= B._x && P._y >= B._y) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (temp._x < 0 && temp._y >= 0) {
		if (P._x <= A._x && P._y >= A._y && P._x >= B._x && P._y <= B._y) {
			return true;
		}
		else {
			return false;
		}
	}
	else if (temp._x < 0 && temp._y < 0) {
		if (P._x <= A._x && P._y <= A._y && P._x >= B._x && P._y >= B._y) {
			return true;
		}
		else {
			return false;
		}
	}

	return false;

}

void WorldMap::SetPointInformation()
{
	for (int i = 0; i < nowAreaPointList.size(); i++) {
		nowAreaPointList[i]->isPoint = Information_SaveData::GetInstance()->getIsPoint(nowAreaPointList[i]->AreaNum, nowAreaPointList[i]->pointNum);
		nowAreaPointList[i]->isClear = Information_SaveData::GetInstance()->getIsClear(nowAreaPointList[i]->AreaNum, nowAreaPointList[i]->pointNum);
		
	}

	for (int i = 0; i < nowAreaPointList.size(); i++) {
		if (typeid(*nowAreaPointList[i]) == typeid(AreaConnectPoint)) {
			if (CanAreaMove(*(std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])))) {
				std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])->_signboard->SetDisplayMode(true);
				Information_SaveData::GetInstance()->setIsPointActivation(nowAreaPointList[i]->AreaNum, nowAreaPointList[i]->pointNum);
				std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])->isPoint = Information_SaveData::GetInstance()->getIsPoint(nowAreaPointList[i]->AreaNum, nowAreaPointList[i]->pointNum);
			}
			else {
				std::dynamic_pointer_cast<AreaConnectPoint>(nowAreaPointList[i])->_signboard->SetDisplayMode(false);
			}
		}
		else {

			if (nowAreaPointList[i]->isPoint == 0) {
				nowAreaPointList[i]->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_3.png");
			}
			else {
				if (nowAreaPointList[i]->isClear == 0) {
					nowAreaPointList[i]->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_1.png");
				}
				else {
					nowAreaPointList[i]->_stagePoint->Load(Main::RESOURCEFILENAME + "resource/graph/worldMap/Mark_2.png");
				}
			}

		}
	}

	setNowRoadConnect();

	return;
}

bool WorldMap::CanAreaMove(AreaConnectPoint a)
{

	bool isLook = false;

	for (int i = 0; i < a.pointConect.size(); i++) {
		if (Information_SaveData::GetInstance()->getIsPoint(a.AreaNum, a.pointConect[i]->connectPointNum)) {
			isLook = true;
			break;
		}
	}

	if (!isLook) {
		return false;
	}

	isLook = false;


	if (Information_SaveData::GetInstance()->getIsPoint(a.moveArea, a.firstMovePoint)) {
		isLook = true;
	}

	if (!isLook) {
		return false;
	}

	return true;
}







