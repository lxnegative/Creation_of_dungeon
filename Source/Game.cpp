#include "Game.h"
#include "MouseManager.h"
#include "FunctionSyncParameter.h"
#include "cd_666s/InputManager/InputManager.h"
#include "cd_666s/DebugDraw.h"
#include "cd_666s/Resources/AllResourceManager.h"
#include "Main.h"
#include "WorldMap.h"
#include "Title.h"
#include "MoneyManager.h"
#include "TutorialPanel.h"
#include "DungeonInfomation.h"

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

Game::Game(std::string areaNum, std::string stageNumber, std::string pointNumber)
	:_stageNumber(stageNumber)
	, _areaNumber(areaNum)
	, _pointNumber(pointNumber)
	, _fadeoutCount(0)
	, _fadeinInterval(100)
	, _fadingInterval(200)
	, _fadeoutInterval(255)
	, _fadingout(true)
	, _state(GameState::READY)
	, isBoss(false)
	, _bgm(Main::RESOURCEFILENAME + "resource/sound/Stage_N_Noon.ogg")
{
	KEYBOARD->AddEvent(KeyInput::KeyType::KEY_LSHIHT
		, ButtonTypeInputEvent::State::Down
		, [&]()
	{
		Clear();
		Init();
	});

	this->PartNum = 0;

	_bgm.SetVolume(220);
	Init();

	_game_end.Load(Main::RESOURCEFILENAME + "resource/sound/game_end.wav");
	_game_end.SetVolume(200);


}

Game::Game(std::string areaNum, std::string stageNumber, std::string pointNumber, int PartNum)
	:_stageNumber(stageNumber)
	, _areaNumber(areaNum)
	, _pointNumber(pointNumber)
	, _fadeoutCount(0)
	, _fadeinInterval(100)
	, _fadingInterval(200)
	, _fadeoutInterval(255)
	, _fadingout(true)
	, _state(GameState::READY)
	, isBoss(false)
	, _bgm(Main::RESOURCEFILENAME + "resource/sound/Stage_N_Noon.ogg")
{
	/*
	KEYBOARD->AddEvent(KeyInput::KeyType::KEY_LSHIHT
		, ButtonTypeInputEvent::State::Down
		, [&]()
	{
		Clear();
		Init();
	});
	*/

	this->PartNum = PartNum;

	_bgm.SetVolume(220);
	Init();

	_game_end.Load(Main::RESOURCEFILENAME + "resource/sound/game_end.wav");
	_game_end.SetVolume(200);

}


Game::~Game()
{
	_functions.clear();


	if (_dungeon != nullptr)
	{
		delete _dungeon;
		_dungeon = nullptr;
	}
	INPUT_MGR->Clear();
	RESOURCE_TABLE->Refresh();

}

SceneBase * Game::Update(UIManager _ui)
{

	if (tutorialPanel != nullptr) {
		tutorialPanel->Update();
		if (!tutorialPanel->GetisTutorial()) {
			tutorial_Balloon = new Tutorial_Balloon(tutorialPanel->GetTimingName());
			delete(tutorialPanel);
			tutorialPanel = nullptr;
		}
		return this;
	}

	if (tutorial_Balloon != nullptr) {
		bool needUpdate = tutorial_Balloon->Update();
		_dungeon->SetIsTimeStop(true);
		if (tutorial_Balloon->GetIsControlStop()) {
			_dungeon->SetIsControlStop(true);
		}
		else {
			_dungeon->SetIsControlStop(false);
		}

		if (tutorial_Balloon->GetIsFinish()) {
			Tutorial::getInstance()->LookTutorial(tutorial_Balloon->GetTimingName());
			delete(tutorial_Balloon);
			tutorial_Balloon = nullptr;
			_dungeon->SetIsTimeStop(false);
			_dungeon->SetIsControlStop(false);
			return this;
		}

		if (!needUpdate) {
			return this;
		}

	}

	bool goTitle = false;
	bool goNext = false;
	switch (_state)
	{
	case Game::GameState::READY:
		GameReadyUpdate();
		break;

	case Game::GameState::GAMING:
		GamingUpdate();
		break;

	case Game::GameState::PAUSE:
		goTitle = PauseUpdate();
		break;

	case Game::GameState::GAME_OVER:
		goTitle = GameOverUpdate();
		break;

	case Game::GameState::WAVE_CLEAR:
		goNext = StageClearUpdate();
		break;
		/*
	case Game::GameState::GAME_CLEAR:
		goTitle = GameClearUpdate();
		break;
		*/
	default:
		break;
	}

	if (goTitle)
	{
		//return new Title();//WorldMap();
		/*
		if (_dungeon != nullptr)
		{
			delete _dungeon;
			_dungeon = nullptr;
		}
		INPUT_MGR->Clear();
		RESOURCE_TABLE->Refresh();
		*/
		if (isTutorial) {
			return new Title();
		}
		else {
			int tempareanum = std::stoi(_areaNumber);
			int temppointnum = std::stoi(_pointNumber);

			Information_SaveData::GetInstance()->TempLoadProcesses(&tempareanum, &temppointnum);
			return new WorldMap(tempareanum, temppointnum);
		}

	}

	if (goNext) {
		MoneyManager::getInstance()->increaseMoney(gameResult.GetMoney());
		//return new Title();//WorldMap();
		if (this->PartNum == 0 && !_dungeon->Get_Is_Boss()) {
			//朝パートだっだ場合

			if (_dungeon != nullptr)
			{
				delete _dungeon;
				_dungeon = nullptr;
			}
			INPUT_MGR->Clear();
			RESOURCE_TABLE->Refresh();

			return new EditMap(_areaNumber, _stageNumber, _pointNumber, false);
		}

		return new WorldMap(std::stoi(_areaNumber), std::stoi(_pointNumber), true);

	}

	
	//F4キーで強制クリア
	if (Main::DEBUGFLAG) {
		if (KEYBOARD->ButtonDown(KeyInput::KeyType::KEY_F4)) {
			_state = GameState::WAVE_CLEAR;
			_game_end.Play();
			gameResult.Init(_areaNumber, _stageNumber, isBoss, PartNum, _pointNumber);
			gameResult.Calculation();

			if (PartNum == 0) {
				DungeonInfomation::GetInstance()->SetDungeonRemainingPermition(_dungeon->GetPassedNum());

			}
		}
	}
	


	return this;
}

void Game::Draw()
{

	if (tutorialPanel != nullptr) {
		return;
	}

	if (tutorial_Balloon != nullptr) {
		tutorial_Balloon->Draw();
	}

	switch (_state)
	{
	case Game::GameState::READY:
		GameReadyDraw();
		break;

	case Game::GameState::GAMING:
		GamingDraw();
		break;

	case Game::GameState::PAUSE:
		//GamingDraw();_state
		PauseDraw();
		break;

	case Game::GameState::GAME_OVER:
		GameOverDraw();
		break;

	case Game::GameState::WAVE_CLEAR:
		StageClearDraw();
		break;
		/*
	case Game::GameState::GAME_CLEAR:
		GameClearDraw();
		break;
		*/
	default:
		break;
	}
}


void Game::Init()
{

	int _stage_num = 0;
	std::string _stageNumber_after = { _stageNumber.front() };

	try {
		_stage_num = std::stoi(_stageNumber);
	}
	catch (const std::invalid_argument& e) {
		std::cout << e.what() << std::endl;
		return;
	}
	catch (const std::out_of_range& e) {
		std::cout << e.what() << std::endl;
		return;
	}

	switch (_stage_num)
	{
	case 1:
		TILE_SIZE = 48;
		break;
	case 2:
		TILE_SIZE = 40;
		break;
	case 3:
		TILE_SIZE = 32;
		break;
	};

	_state = GameState::READY;
	_fadeoutCount = 0;
	_fadingout = true;
	_bgm.Stop();


	if (_dungeon != nullptr)
	{
		delete _dungeon;
		_dungeon = nullptr;
	}

	_dungeon = new Dungeon(_areaNumber, _stageNumber, PartNum);
	_dungeon->Init();

	_black.Load(Main::RESOURCEFILENAME + "resource/graph/ui/result/result_Black.png");
	_black.SetRenderType(Texture2D::RenderType::UI);
	_black.SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);
	_black.SetPosition(Vector2D(0, 0));
	_black.SetDisplayMode(false);

	isTutorial = false;


	isBoss = _dungeon->Get_Is_Boss();

	_gameOverUI.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Game_Over.png");
	_gameOverUI.SetRenderType(Texture2D::RenderType::UI);
	_gameOverUI.SetPriority(static_cast<int>(Sprite::Priority::UI));
	_gameOverUI.SetPosition(Vector2D(1280 / 2 - _gameOverUI.GetTexturePtr().lock()->GetBaseSize()._x / 2, 720 / 2 - _gameOverUI.GetTexturePtr().lock()->GetBaseSize()._y / 2));
	_gameOverUI.SetDisplayMode(false);

	Click_go_to_Next.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Click_go_to_Next.png");
	Click_go_to_Next.SetRenderType(Texture2D::RenderType::UI);
	Click_go_to_Next.SetPriority(static_cast<int>(Sprite::Priority::UI));
	Click_go_to_Next.SetPosition(Vector2D(320, 350));
	Click_go_to_Next.SetScale(Vector2D(2.0, 2.0));
	Click_go_to_Next.SetDisplayMode(false);

	Click_Click_and_Start.Load(Main::RESOURCEFILENAME + "resource/graph/ui/Click_and_Start.png");
	Click_Click_and_Start.SetRenderType(Texture2D::RenderType::UI);
	Click_Click_and_Start.SetPriority(static_cast<int>(Sprite::Priority::UI));
	Click_Click_and_Start.SetPosition(Vector2D(1280 / 2 - Click_Click_and_Start.GetTexturePtr().lock()->GetBaseSize()._x / 2, 720 / 2 - Click_Click_and_Start.GetTexturePtr().lock()->GetBaseSize()._y / 2));
	Click_Click_and_Start.SetDisplayMode(false);

	PauseUI.Load(Main::RESOURCEFILENAME + "resource/graph/ui/PAUSE.png");
	PauseUI.SetRenderType(Texture2D::RenderType::UI);
	PauseUI.SetPriority(static_cast<int>(Sprite::Priority::UI));
	PauseUI.SetPosition(Vector2D(540, 180));
	PauseUI.SetDisplayMode(false);

	GameEndAlarmUI.Load(Main::RESOURCEFILENAME + "resource/graph/ui/FINISH.png");
	GameEndAlarmUI.SetRenderType(Texture2D::RenderType::UI);
	GameEndAlarmUI.SetPriority(static_cast<int>(Sprite::Priority::UI));
	GameEndAlarmUI.SetPosition(Vector2D(540, 200));
	GameEndAlarmUI.SetScale(Vector2D(1.5, 1.5));
	GameEndAlarmUI.SetDisplayMode(false);

	TimeUpAlarmUI = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/Time_Up.png");

	GameEndCnt = 0;

	std::string fieldType = _dungeon->GetFieldType();

	if (fieldType == "#CAV" || fieldType == "#STN") {
		_bgm.Load(Main::RESOURCEFILENAME + "resource/sound/Dungeon1.mp3");
	}

	if (fieldType == "#FST" || fieldType == "#TIM") {
		_bgm.Load(Main::RESOURCEFILENAME + "resource/sound/Dungeon2.mp3");
	}

	


}


void Game::Clear()
{
	_dungeon->Clear();
}


bool Game::GameOverUpdate() {

	_fadeoutCount++;

	if (_fadeoutCount > 200
		&& MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
	{
		return true;
	}

	return false;
}


void Game::GameOverDraw()
{
	double blend = double(_fadeoutCount) * 17 / 10;

	if (blend > 150) {
		blend = 150;
	}

	auto color = Color4(0, 0, 0, 1.0 * blend / 255);
	_black.SetDisplayMode(true);
	_black.SetBaseColor(color);
	//Debug::DrawRectWithSize(Vector2D(0, 0), Vector2D(WINDOW_WIDTH, WINDOW_HEIGHT), color, true);

	if (_fadeoutCount > 150) {
		//Debug::DrawString(Vector2D(400, 200), "GAME OVER", ColorPalette::RED4, 40);
		_gameOverUI.SetDisplayMode(true);
	}
	if (_fadeoutCount > 200) {
		//Debug::DrawString(Vector2D(410, 400), "左クリックで戻る", ColorPalette::WHITE4, 20);
		Click_go_to_Next.SetPosition(Vector2D(860, 640));
		Click_go_to_Next.SetDisplayMode(true);
	}
}


bool Game::StageClearUpdate() {

	if (_fadeoutCount < _fadingInterval)
	{
		_fadeoutCount++;
		return false;
	}

	if (_fadeoutCount == _fadingInterval) {
		gameResult.Update();
		//_fadingout = MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L);
		if (gameResult.GetIsStop()) {
			Click_go_to_Next.SetPosition(Vector2D(860, 640));
			Click_go_to_Next.SetDisplayMode(true);

			_fadingout = MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L);
		}
	}



	if (!_fadingout)
		return false;

	_fadeoutCount++;
	if (_fadeoutCount == _fadeoutInterval)
	{

		//TODO : game^3が終わったらコメントアウトを外す
		//Clear();
		//Init();
		return true;
	}

	return false;
}

void Game::StageClearDraw() {

	double blend = double(_fadeoutCount) * 17 / 10;

	if (!_fadingout) {
		if (blend > 150) {
			blend = 150;
		}
	}
	else {
		blend = 150 + 105 * (static_cast<double>(_fadeoutCount - _fadingInterval) / static_cast<double>(_fadeoutInterval - _fadingInterval));
	}

	auto color = Color4(0, 0, 0, 1.0 * blend / 255);
	_black.SetDisplayMode(true);
	_black.SetBaseColor(color);
	if (_fadingout) {
		Debug::DrawRectWithSize_TopPriority(Vector2D(0, 0), Vector2D(WINDOW_WIDTH, WINDOW_HEIGHT), color, true);
	}

	if (_fadeoutCount >= 150
		&& _fadeoutCount <= _fadingInterval) {
		//Debug::DrawString(Vector2D(200, 200), "ステージクリア", ColorPalette::WHITE4);
	}
	if (_fadeoutCount == _fadingInterval) {
		gameResult.Draw();
		//Debug::DrawString(Vector2D(200, 400), "左クリックで戻る", ColorPalette::WHITE4);
	}
}

/*
bool Game::GameClearUpdate() {

	_fadeoutCount++;

	if (_fadeoutCount > 200 && MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
	{
		return true;
	}

	return false;

}

void Game::GameClearDraw() {

	double blend = double(_fadeoutCount) * 17 / 10;

	if (blend > 150) {
		blend = 150;
	}

	auto color = Color4(0, 0, 0, 1.0 * blend / 255);
	Debug::DrawRectWithSize(Vector2D(0, 0), Vector2D(WINDOW_WIDTH, WINDOW_HEIGHT), color, true);

	if (_fadeoutCount > 150) {
		Debug::DrawString(Vector2D(200, 200), "ゲームクリア", ColorPalette::WHITE4);
	}
	if (_fadeoutCount > _fadingInterval) {
		Debug::DrawString(Vector2D(200, 400), "左クリックで戻る", ColorPalette::WHITE4);
	}
}
*/

bool Game::GameReadyUpdate()
{
	if (_fadingout)
	{
		if (_fadeoutCount < _fadeinInterval)
			_fadeoutCount++;
		else
			_fadingout = false;

		return false;
	}

	if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
	{
		_black.SetDisplayMode(false);
		Click_Click_and_Start.SetDisplayMode(false);
		_state = GameState::GAMING;
		_fadeoutCount = 0;
		_bgm.Play();

		if (tutorialPanel == nullptr) {
			if (_areaNumber == "1" && _stageNumber.find("1") != std::string::npos && PartNum == 0) {
				if (!Tutorial::getInstance()->isLookingTutorial("Stage1-1_Battle")) {
					tutorialPanel = new TutorialPanel("Stage1-1_Battle");
					isTutorial = true;
				}
			}
		}

		if (tutorial_Balloon == nullptr) {
			if (_areaNumber == "1" && _stageNumber.find("4") != std::string::npos && PartNum == 0) {
				if (!Tutorial::getInstance()->isLookingTutorial("Stage1-4_Battle")) {
					tutorial_Balloon = new Tutorial_Balloon("Stage1-4_Battle");
				}
			}
		}

		

		return true;
	}

	return false;

}

void Game::GameReadyDraw() {

	double blend = 0.3 * (_fadeinInterval - _fadeoutCount) / _fadeinInterval + 0.7;
	auto color = Color4(0, 0, 0, blend);
	_black.SetDisplayMode(true);
	_black.SetBaseColor(color);
	//Debug::DrawRectWithSize(Vector2D(0, 0), Vector2D(WINDOW_WIDTH, WINDOW_HEIGHT), color, true);

	if (_fadeinInterval <= _fadeoutCount) {
		//Debug::DrawString(Vector2D(500, 280), "左クリックで開始", ColorPalette::WHITE4, 32);
		Click_Click_and_Start.SetPosition(Vector2D(1280 / 2 - Click_Click_and_Start.GetTexturePtr().lock()->GetBaseSize()._x / 2, 720 / 2 - Click_Click_and_Start.GetTexturePtr().lock()->GetBaseSize()._y / 2));
		Click_Click_and_Start.SetDisplayMode(true);
	}

}

void Game::GamingUpdate()
{

	if (_dungeon->HasClear()) {
		if (GameEndCnt == 0) {
			_game_end.Play();
		}
		GameEndCnt++;
		if (_dungeon->HasTimeUp()) {
			GameEndAlarmUI.SetResource(TimeUpAlarmUI);
		}
		GameEndAlarmUI.SetPosition(Vector2D(1280 / 2 - Click_Click_and_Start.GetTexturePtr().lock()->GetBaseSize()._x / 2, 720 / 2 - Click_Click_and_Start.GetTexturePtr().lock()->GetBaseSize()._y / 2));

		GameEndAlarmUI.SetDisplayMode(true);

		if (GameEndCnt <= GameEndCntMax) {
			return;
		}
		
	}

	if (_dungeon->HasClear() && GameEndCnt > GameEndCntMax)
	{
		//if (_stageNumber < 3)
		//{
		//    _stageNumber++;
		GameEndCnt = 0;
		GameEndAlarmUI.SetDisplayMode(false);
		_state = GameState::WAVE_CLEAR;
		gameResult.Init(_areaNumber, _stageNumber, isBoss, PartNum, _pointNumber);
		gameResult.Calculation();

		if (PartNum == 0) {
			DungeonInfomation::GetInstance()->SetDungeonRemainingPermition(_dungeon->GetPassedNum());

		}

		return;
		//}
		//else
		//{
		//    _state = GameState::GAME_CLEAR;
		//    _bgm.Stop();
		//    return;
		//}
	}

	if (!_bgm.IsPlaying())
		_bgm.Play();

	if (_dungeon->HasGameOver())
	{
		_state = GameState::GAME_OVER;
		_bgm.Stop();
	}
	_dungeon->Update();

	if (KEYBOARD->ButtonDown(KeyInput::KeyType::KEY_ESCAPE))
		_state = GameState::PAUSE;
}

void Game::GamingDraw()
{
	if (_dungeon->HasClear()) {
		if (GameEndCnt <= GameEndCntMax) {
			return;
		}
	}
	
	_dungeon->Draw();
}

bool Game::PauseUpdate()
{

	PauseUI.SetDisplayMode(true);

	auto backColor = Color4(0, 0, 0, 0.9f);
	_black.SetDisplayMode(true);
	_black.SetBaseColor(backColor);

	if (KEYBOARD->ButtonDown(KeyInput::KeyType::KEY_ESCAPE)) {
		_state = GameState::GAMING;
		PauseUI.SetDisplayMode(false);
		_black.SetDisplayMode(false);
	}


	//Debug::DrawRectWithSize(Vector2D(0, 0), Vector2D(WINDOW_WIDTH, WINDOW_HEIGHT), backColor, true);

	Vector2D center(WINDOW_WIDTH / 2.0, WINDOW_HEIGHT / 2.0);
	Vector2D frameSize(WINDOW_WIDTH / 4.0, WINDOW_HEIGHT / 8.0);

	auto frameColor = ColorPalette::WHITE4;
	Vector2D upperPannelPos(center._x - frameSize._x / 2.0, center._y - frameSize._y / 1.5);
	Vector2D lowerPannelPos(center._x - frameSize._x / 2.0, center._y + frameSize._y / 1.5);

	int fontSize = 32;
	Debug::DrawString_TopPriority(upperPannelPos + frameSize * 0.5 - Vector2D(fontSize * 2.5 - 3.0, fontSize / 2 - 4.0), "ゲームに戻る", ColorPalette::WHITE4, 24);

	if (!isTutorial) {
		Debug::DrawString_TopPriority(lowerPannelPos + frameSize * 0.5 - Vector2D(fontSize * 4.0 - 4.0, fontSize / 2 - 4.0), "ワールドマップに戻る", ColorPalette::WHITE4, 24);
	}

	Debug::DrawRectWithSize_TopPriority(upperPannelPos, frameSize, frameColor, false);

	if (!isTutorial) {
		Debug::DrawRectWithSize_TopPriority(lowerPannelPos, frameSize, frameColor, false);
	}

	auto mousePos = MOUSE->GetCursorPos();
	bool mouseOnUpperPannel =
		(upperPannelPos._x < mousePos._x
			&&  upperPannelPos._y < mousePos._y
			&&  mousePos._x < upperPannelPos._x + frameSize._x
			&&  mousePos._y < upperPannelPos._y + frameSize._y);

	bool mouseOnLowerPannel =
		(lowerPannelPos._x < mousePos._x
			&&  lowerPannelPos._y < mousePos._y
			&&  mousePos._x < lowerPannelPos._x + frameSize._x
			&&  mousePos._y < lowerPannelPos._y + frameSize._y);

	auto fillColor = Color4(1.0f, 1.0f, 1.0f, 0.5f);
	Debug::DrawRectWithSize_TopPriority(upperPannelPos, frameSize, fillColor, mouseOnUpperPannel);

	if (!isTutorial) {
		Debug::DrawRectWithSize_TopPriority(lowerPannelPos, frameSize, fillColor, mouseOnLowerPannel);
	}

	if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
	{
		if (mouseOnUpperPannel)
		{
			_state = GameState::GAMING;
			_black.SetDisplayMode(false);
			PauseUI.SetDisplayMode(false);
		}
		else if (mouseOnLowerPannel)
		{
			if (!isTutorial) {
				_black.SetDisplayMode(false);
				PauseUI.SetDisplayMode(false);
				return true;
			}
		}
	}

	return false;
}

void Game::PauseDraw()
{
}