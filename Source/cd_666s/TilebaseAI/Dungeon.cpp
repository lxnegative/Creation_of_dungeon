#include "Dungeon.h"
#include "../InputManager/InputManager.h"
#include "../Render/RenderManager.h"
#include "../Resources/AllResourceManager.h"
#include "../DebugDraw.h"
#include "../Utility/CSVReader.h"

#include "TileField.h"
#include "Enemy.h"
#include "Monster.h"
#include "../../IntrudedEnemy.h"
#include "Obstacle.h"
#include "WeakObstacle.h"
#include "River.h"
#include "EnemysItem.h"
#include "Goal.h"
#include "TiledObjectMnager.h"
#include "StartPoint.h"
#include "Trap.h"
#include "Bridge.h"

#include "../../DungeonInfomation.h"

#include <assert.h>
#include <iostream>
#include <regex>


Dungeon::Dungeon(std::string areaNum, std::string stageName, int PartNum)
	: _permitivePassedNum(2)
	, _stageName(stageName)
	, _areaNum(areaNum)
	, _goal(nullptr)
	, _start(nullptr)
	, _mainsFrame(Main::RESOURCEFILENAME + "resource/graph/ui/main_window.png", Vector2D(20, 20))
	, _background(Main::RESOURCEFILENAME + "resource/graph/background/background.png", Vector2D(0, 0))
	, _windowBackground(Main::RESOURCEFILENAME + "resource/graph/ui/main_window_background1.png", Vector2D(28, 28))
	, _waveInfomartionTimer(Main::RESOURCEFILENAME + "resource/graph/ui/enemytimer.png", Vector2D(754, 248))
	, _hartFrame(Main::RESOURCEFILENAME + "resource/graph/ui/LifeFrame.png", Vector2D(734, 248 - 200))
	, _hart(Main::RESOURCEFILENAME + "resource/graph/ui/HartFull.png", Vector2D(734 + 7, 78 + 28))
	, _infoDrawer(_dictionary)
	, _intruderInformation(_dictionary)
	, _intrudeLastCharacter(false)
	, _defeatedNum(0)
	, _bossFace(Main::RESOURCEFILENAME + "resource/graph/face_golem1.png", Vector2D(734 + 48, 248 - 200 + 40))
{

	auto b_pos = _stageName.rfind('b');

	if (b_pos != std::string::npos) {
		_stageNum = _stageName.substr(0, b_pos);
		_counter.Init();
	}
	else {
		_stageNum = _stageName;
	}

	_hart.SetPriority(Sprite::Priority::UI);
	_hart.SetDisplayMode(false);
	_hart.SetScale(Vector2D(5.0, 5.0));

	_hartFrame.SetPriority(Sprite::Priority::UI);
	_hartFrame.SetDisplayMode(true);

	hartFullImage = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/HartFull.png");
	hartHalfImage = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/HartHalf.png");
	hartNullImage = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/HartNull.png");

	_hartN.resize(6);

	for (int i = 0; i < _hartN.size(); i++) {

		_hartN[i].SetResource(hartFullImage);
		_hartN[i].SetPosition(Vector2D(773 + (i % 3) * 26, 103 + (i / 3) * 30));
		_hartN[i].SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
		_hartN[i].SetDisplayMode(false);
	}

	_mainsFrame.SetPriority(Sprite::Priority::UI);
	_background.SetPriority(Sprite::Priority::BACKGROUND);
	_windowBackground.SetPriority(static_cast<int>(Sprite::Priority::BACKGROUND) + 1);
	_waveInfomartionTimer.SetPriority(Sprite::Priority::UI);

	this->PartNum = PartNum;

	StageNamePlate.Load(Main::RESOURCEFILENAME + "resource/graph/ui/StageNamePlate.png");
	StageNamePlate.SetPosition(Vector2D(940, 0));
	StageNamePlate.SetPriority(Sprite::Priority::UI);
	StageNamePlate.SetDisplayMode(true);


	_bossFace.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_bossFace.SetDisplayMode(false);
}


Dungeon::~Dungeon()
{
	Clear();
	OBJECT_MGR->Refresh();
}


void Dungeon::Init()
{
	_controller.Init();
	_infoDrawer.Init();

	_messageReciever.Init();
	LoadMessage(_stageName);

	//ステージ生成
	std::vector<std::string> dataArray;
	CSVReader reader;

	//ボスステージかどうかを判別する
	std::string fileName = Main::RESOURCEFILENAME + "csv/boss/list.csv";
	std::vector<std::string> bossStageListArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, bossStageListArray, 1);
	_is_boss = GetIsBossStage(bossStageListArray);

	//ハートフレームの表示フラグの再設定
	_hartFrame.SetDisplayMode(true);

	//ウェーブの情報を読み込む
	fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNum + "/wave.csv";
	std::vector<std::string> waveInfoArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, waveInfoArray, 1);
	auto waveInterval = std::stoi(waveInfoArray[std::stoi(_stageName) * 3 - (2 - PartNum)]);

	_counter.InitWithSetup(waveInterval);
	isBossMessageDisplay = true;

	_timer.InitWithSetup(waveInterval);

	if (_is_boss) {
		_timer.SetDisplayMode(false);
	}
	else {
		_counter.SetDisplayMode(false);

		if (PartNum == 1) {
			_timer.NightSetting();
		}

	}

	

	fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNum + "/permition.csv";
	std::vector<std::string> permitionInfoArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, permitionInfoArray, 1);
	_permitivePassedNum = std::stoi(permitionInfoArray[std::stoi(_stageName) * 2 - 1]);

	beforePartPassedNum = 0;


	if (PartNum == 0) {

		DungeonInfomation::GetInstance()->SetDungeonRemainingPermition(0);

	}else if (PartNum == 1) {
		beforePartPassedNum = DungeonInfomation::GetInstance()->GetDungeonRemainingPermition();
	}

	//タイルの大きさを読み込む
	fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNum + "/tilesize.csv";
	std::vector<std::string> tileInfoArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, tileInfoArray);
	LoadTileSize(_stageName, tileInfoArray);

	//フィールドのサイズを読み込む
	//fileName = Main::RESOURCEFILENAME + "csv/StageData/map";
	//fileName += (_stageName + ".csv");
	//fileName = Main::RESOURCEFILENAME + "csv/StageData/EditMapData.csv";
	//auto fieldSizeH = reader.GetLineSize(fileName, 0);
	//auto fieldSizeV = reader.GetLineNum(fileName);

	//フィールドのデータを読み込む
	dataArray = DungeonInfomation::GetInstance()->GetEditMapDataList();

	//フィールドのサイズを読み込む
	auto fieldSizeH = DungeonInfomation::GetInstance()->GetLineSize();
	auto fieldSizeV = DungeonInfomation::GetInstance()->GetLineNum();

	int countX = 0;
	int countY = 0;
	FIELD->Init(fieldSizeH, fieldSizeV);

	SetFieldType();

	//オブジェクトを読み込む
	auto& _objs = OBJECT_MGR->_objects;
	for (auto data : dataArray)
	{
		//受け取ったデータを変換表をもとに変換
		GenerateObject(data, countX, countY);

		FIELD->SetFieldType(TiledVector(countX, countY), fieldTypeStr);

		//次のマップ番号まで
		countX++;
		if (countX == fieldSizeH)
		{
			countX = 0;
			countY++;

			if (countY == fieldSizeV)
				break;
		}
	}
	assert((_goal != nullptr) && (_start != nullptr) && "Cannot Read Start and Goal");

	//侵入位置を設定
	_intruderInformation.InitWithSetup(_start);

	//キャラたちをロード
	fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNum + "/enemys" + _stageName;
	fileName += (PartNum == 0 ? "_Morning" : "_Night");
	fileName += ".csv";

	Enemy::LoadEnemys(_objs, *_start, *_goal, _enemys, fileName);
	
	//fileName = Main::RESOURCEFILENAME + "csv/StageData/monsters";
	//fileName += (_stageName + ".csv");
	//fileName = Main::RESOURCEFILENAME + "csv/StageData/EditMap_MonsterData.csv";
	Monster::LoadMonsters_DungeonInfomation(_objs, _monsters);

	FIELD->Setup();

	//ボス情報の読み込み
	if (_is_boss) {
		_bossBattle.Init(_areaNum, _stageNum, _objs, _boss);


		fileName = Main::RESOURCEFILENAME + "csv/boss/boss";
		fileName += _areaNum + "-" + _stageNum + ".csv";

		std::vector<std::string> bossArray;
		CSVReader reader;
		reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, bossArray, 1);

		std::string name = bossArray[8];

		fileName = Main::RESOURCEFILENAME + "csv/boss/boss_face.csv";
		reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, bossArray, 1);

		for (int i = 0; i < bossArray.size(); i += 2) {

			if (bossArray[i] == name) {
				_bossFace.Load(Main::RESOURCEFILENAME + "resource/graph/" + bossArray[i + 1] + ".png");
				_bossFace.SetDisplayMode(true);
			}

		}

	}
	//ここまで

	_monsters.Update();
	_enemys.Update();
	if (_is_boss) {
		_boss.Update();
		_intruders.Update();
	}
	OBJECT_MGR->Refresh();

	for (auto obj : _objs)
	{
		if (obj.get() != nullptr)
			obj->Init();
	}

	IsTimeStop = false;
	IsControlStop = false;
}


void Dungeon::GenerateObject(std::string typeName, int countX, int countY)
{
	FIELD->SetRawNumber(TiledVector(countX, countY), stoi(typeName));
	FIELD->SetFieldType(TiledVector(countX, countY), typeName);

	auto& _objs = OBJECT_MGR->_objects;

	if (typeName.find("300") != std::string::npos)
	{
		_objs.push_back(std::make_shared<WeakObstacle>(TiledVector(countX, countY), typeName));
		return;
	}

	if (typeName.find("301") != std::string::npos)
	{
		_objs.push_back(std::make_shared<Bridge>(typeName, TiledVector(countX, countY)));
		return;
	}

	if (typeName.find("9#") != std::string::npos)
	{
		Trap::CreateTrap(typeName, countX, countY, _objs);
		return;
	}

	if (typeName.find("2&") != std::string::npos)
	{
		LoadItem(typeName, countX, countY, _objs);
		return;
	}

	if (typeName.find("200") != std::string::npos)
	{
		if (_start == nullptr)
		{
			_start = std::make_shared<StartPoint>(TiledVector(countX, countY), _messageReciever);
			_objs.push_back(_start);
		}
		return;
	}

	if (typeName.find("100") != std::string::npos)
	{
		if (_goal == nullptr)
		{
			_goal = std::make_shared<Goal>(TiledVector(countX, countY), _monsters, _messageReciever, _permitivePassedNum);
			_objs.push_back(_goal);
		}
		return;
	}

	if (typeName.find("6") != std::string::npos)
	{
		_objs.push_back(std::make_shared<River>(TiledVector(countX, countY), typeName));
		return;
	}

	if (typeName.find("1") != std::string::npos)
	{
		_objs.push_back(std::make_shared<Obstacle>(TiledVector(countX, countY)));
		return;
	}

	if (typeName == "0")
		return;

	
}

bool Dungeon::GetIsBossStage(const std::vector<std::string>& _bossList)
{
	for (int i = 0; i < _bossList.size(); i+=2){
		if (_bossList[i] == _areaNum && _bossList[i + 1] == _stageName) {
			return true;
		}
	}
	return false;
}


void Dungeon::Clear()
{
	_infoDrawer.Clear();

	OBJECT_MGR->Clear();
	_start = nullptr;
	_goal = nullptr;
	FIELD->Clear();

	RESOURCE_TABLE->Refresh();
}


bool Dungeon::HasClear()
{
	//WAVEを耐えきったらクリア(通常ステージのみ)
	if (_timer.HasTimeUp() && !_is_boss)
		return true;

	//WAVE中の敵を全滅させたらクリア
	if (Enemy::HasWipeOuted())
		return true;

	//最後の敵を倒したらクリア
	if (_start->GetTimeUnitlNext() == StartPoint::NobodyIntruder()
		&& _enemys.GetColleagues() == 0
		&& !HasGameOver()) {
		if (!_is_boss) {
			return true;
		}
		else if (_bossBattle.GetIntruderObject() == nullptr) {
			return true;
		}
		else {
			return false;
		}
	}
		

	if (_is_boss && _counter.GetAllEnemy() <= _counter.GetCount()) {
		return true;
	}

	/*
	auto b = _start->GetTimeUnitlNext();
	auto a = _enemys.GetColleagues();
	*/

	return false;
}

bool Dungeon::HasTimeUp() {

	if (_timer.HasTimeUp() && !_is_boss)
		return true;

	return false;

}


bool Dungeon::HasGameOver()
{
	//ボスステージのゲームオーバー条件 : ボスの体力
	if (_is_boss) {
		//MEMO:暫定的に敵をさせていい上限をpermitedNumとしている。
		/*
		auto passedNum = _goal->GetPassedNum();
		if (_permitivePassedNum < passedNum)
			return true;
		*/
		return _bossBattle.WasBossKilled();
	}
	//通常ステージのゲームオーバー条件 : 通過させた敵の数
	else {
		auto passedNum = _goal->GetPassedNum();
		if (_permitivePassedNum < passedNum + beforePartPassedNum)
			return true;
	}
	return false;
}




void Dungeon::Update()
{
	if (!IsTimeStop) {
		//メッセージ更新
		UpdateSecretary();
	}
	

#if 1
	if (!IsTimeStop) {
		if (_is_boss) {
			_counter.Update(_defeatedNum);
		}
		else {
			_timer.Update();
		}
	}
#endif

	//_timer.Update();

	//情報網更新
	_monsters.Update();
	_enemys.Update();

	if (_is_boss) {
		_boss.Update();
		_intruders.Update();
	}

	//捜査情報更新
	if (!IsControlStop) {
		_controller.Update();
	}

	auto& _objs = OBJECT_MGR->_objects;


	for (auto& obj : _objs)
	{
		if (obj != nullptr) {
			if (obj != _start || !IsTimeStop) {
				obj->Update();

				if (!_is_boss)
					continue;

				if (obj->GetType() == TiledObject::Type::ENEMY) {
					_defeatedNum = obj->GetDefeatedNum();
				}

				//敵がゴールに到着したらボスとの戦闘開始
				if (obj->HasArrived())
				{
					_bossBattle.SetBattleObject(obj, _intruders);
					OBJECT_MGR->Add(_bossBattle.GetIntruderObject());
				}
			}
		}
	}


	if (_is_boss) {
		/*
		if(_bossBattle.GetIntruderObject() != nullptr){
		}
		else
		}*/

		_bossBattle.Update();
	}
}


void Dungeon::Draw()
{
	//時間になったら初期化
	if (_timer.HasTimeUp()/* && !_is_boss*/)
		return;

	FIELD->Draw();
	for (auto obj : OBJECT_MGR->_objects)
	{
		if (obj != nullptr)
			obj->Draw();
	}

	_infoDrawer.Update();
	_infoDrawer.Draw();

	if (_start->GetTimeUnitlNext() == 0) {
		_infoDrawer.SetCharacter(_start->GetNextEnemy());
	}

	_controller.Draw();

	OBJECT_MGR->Refresh();


	//メッセージウィンドウ更新
	if (_bossBattle.GetIntruderObject() == nullptr) {
		_messageReciever.DrawTrue();
		_messageReciever.Update();
		_messageReciever.Draw();
	}
	else {
		_messageReciever.DrawFalse();
	}

	//ステージ名表示
	//Debug::DrawRectWithSize(Vector2D(970, 40), Vector2D(250, 60), Color4(0.5, 0.65, 0.85, 1.0), true);
	//Debug::DrawRectWithSize(Vector2D(970, 40), Vector2D(250, 60), ColorPalette::WHITE4, false);
	Debug::DrawString(Vector2D(1010, 80), "洞窟ダンジョン その" + _stageName);

#if 1
	if (_is_boss) {
		_counter.Draw();
	}
	else {
		//残り時間表示
		_timer.Draw();
	}
#endif

	//_timer.Draw();

	DrawEnemyTimer();
}

void Dungeon::DrawEnemyTimer() {
	//侵入者情報表示
	_intruderInformation.Draw();

	if (_is_boss) {

		std::weak_ptr<BossBattleObject> bossobj = _bossBattle.GetBossObject();


		const int height = 15;
		auto _pos = Vector2D(_hartFrame.GetPosition()._x + 43, _hartFrame.GetPosition()._y + 103);
		auto _wh1 = Vector2D(((double)(bossobj.lock()->_param._hp) / bossobj.lock()->_param._maxHP) * 73.0, height);
		auto _wh2 = Vector2D(((double)(bossobj.lock()->_param._maxHP) / bossobj.lock()->_param._maxHP) * 73.0 , height);

		if (bossobj.lock()->_param._hp >= 0) {
			Debug::DrawRectWithSize(_pos, _wh1, ColorPalette::RED4, true);
		}
		Debug::DrawRectWithSize(_pos, _wh2, ColorPalette::RED4, false);

	}
	else {

		//ノルマ表示
		//std::string passed = "×";
		//Debug::DrawString(_waveInfomartionTimer.GetPosition() + Vector2D(30, 170), passed);
		//std::string passed = std::to_string(_permitivePassedNum - _goal->GetPassedNum());
		//auto color = (_permitivePassedNum - _goal->GetPassedNum() <= 0) ? ColorPalette::RED4 : ColorPalette::BLACK4;
		//Debug::DrawString(_hartFrame.GetPosition() + Vector2D(64, 58), passed, color, 48);

		for (int i = 0; i < _permitivePassedNum + 1; i++) {
			_hartN[i].SetDisplayMode(true);
			if (i < (_permitivePassedNum + 1) - _goal->GetPassedNum() - beforePartPassedNum) {
				_hartN[i].SetResource(hartFullImage);
			}
			else {
				_hartN[i].SetResource(hartNullImage);
			}
		}



		/*
		passed += "/";
		passed += std::to_string(_permitivePassedNum);
		Debug::DrawString(_waveInfomartionTimer.GetPosition() + Vector2D(60, 190), passed);
		*/

	}
		

	
}


void Dungeon::LoadMessage(std::string stageName)
{
	/*
	int messageNum = 0;
	if (stageName == "1")
	messageNum = 1;
	else if (stageName == "2")
	messageNum = 2;
	else if (stageName == "3")
	messageNum = 3;
	*/
	int messageNum = 0;
	
	

	std::string filePath = Main::RESOURCEFILENAME + "csv/talkData/Area" + _areaNum + "/stage" + stageName + "/";
	filePath += (PartNum == 0 ? "Morning" : "Night");
	filePath += "/";

	std::vector<string> tempFileList = GetFileList(filePath);

	std::vector<string> fileList;

	for (int i = 0; i < tempFileList.size(); i++) {
		if (std::regex_match(tempFileList[i], std::regex("TalkData_Time_\\d+\.csv"))) {
			fileList.push_back(tempFileList[i]);
		}
		else {
			tempFileList[i] = "";
		}
	}

	std::vector<string> talkBeginTime;

	talkBeginTime.resize(fileList.size());

	for (int i = 0; i < fileList.size(); i++) {
		std::string tempStr = std::regex_replace(fileList[i], std::regex("TalkData_Time_"), "");
		tempStr = std::regex_replace(tempStr, std::regex(".csv"), "");
		talkBeginTime[i] = tempStr;
	}


	for (int i = 0; i < fileList.size(); i++) {
		_dungeonMessage.insert(std::make_pair(talkBeginTime[i], _messageReciever._processer.CreateTalkData(filePath + fileList[i], Talk_Type::nomal)));
	}
	

	//_dungeonMessage.insert(std::make_pair("missed", _messageReciever._processer.CreateTalkData(Main::RESOURCEFILENAME + "csv/talkData/missed.csv", Talk_Type::nomal)));
	//_dungeonMessage.insert(std::make_pair("start", _messageReciever._processer.CreateTalkData(filePath + "0_secretary_start.csv", Talk_Type::nomal)));
	//_dungeonMessage.insert(std::make_pair("middle", _messageReciever._processer.CreateTalkData(filePath + "1_secretary_middle.csv", Talk_Type::nomal)));

	/*
	if (messageNum != 3)
	{
		_dungeonMessage.insert(std::make_pair("last", _messageReciever._processer.CreateTalkData(filePath + "2_secretary_last.csv", Talk_Type::nomal)));
	}
	else
	{
		_dungeonMessage.insert(std::make_pair("blaver", _messageReciever._processer.CreateTalkData(filePath + "3_blaver_appeared.csv", Talk_Type::nomal)));
		_dungeonMessage.insert(std::make_pair("last", _messageReciever._processer.CreateTalkData(filePath + "2_secretary_blaverAppeared.csv", Talk_Type::nomal)));
	}
	*/



}


void Dungeon::UpdateSecretary()
{
	if (!_is_boss) {
		auto timeCnt = _start->GetTimeFromDungeonStart();

		std::string t = "";

		int zeroCnt = 3 - std::to_string(timeCnt).length();

		if (zeroCnt > 3 && zeroCnt < 0) {
			return;
		}
		else {
			for (int i = 0; i < zeroCnt; i++) {
				t += "0";
			}
			t += std::to_string(timeCnt);
		}
		

		auto timeIte = _dungeonMessage.find(t);


		if (timeIte != _dungeonMessage.end()) {
			_messageReciever.Recieve((*(timeIte)).second);
			_dungeonMessage.erase(timeIte);
		}


		/*
		auto timeRatio = _timer.GetTimeRatio();

		if (timeRatio == 0)
		{
			_messageReciever.Recieve(_dungeonMessage.at("start"));
			return;
		}

		if (timeRatio == 0.5)
		{
			_messageReciever.Recieve(_dungeonMessage.at("middle"));
			return;
		}

		if (_intrudeLastCharacter)
			return;

		if (_stageName == "1")
		{
			if ((_timer.GetCount() / 60) == 40)
			{
				_messageReciever.Recieve(_dungeonMessage.at("last"));
				_intrudeLastCharacter = true;
				return;
			}
		}
		else if (_stageName == "2")
		{
			if ((_timer.GetCount() / 60) == 50)
			{
				_messageReciever.Recieve(_dungeonMessage.at("last"));
				_intrudeLastCharacter = true;
				return;
			}
		}
		else
		{
			if (_start->GetTimeUnitlNext() == -1)
			{
				_messageReciever.Recieve(_dungeonMessage.at("last"));
				_messageReciever.Recieve(_dungeonMessage.at("blaver"));
				_intrudeLastCharacter = true;
			}
		}
		*/

	}
	else {

		auto timeCnt = _start->GetTimeFromDungeonStart();

		std::string t = "";

		int zeroCnt = 3 - std::to_string(timeCnt).length();

		if (zeroCnt > 3 && zeroCnt < 0) {
			return;
		}
		else {
			for (int i = 0; i < zeroCnt; i++) {
				t += "0";
			}
			t += std::to_string(timeCnt);
		}


		auto timeIte = _dungeonMessage.find(t);


		if (timeIte != _dungeonMessage.end()) {
			_messageReciever.Recieve((*(timeIte)).second);
			_dungeonMessage.erase(timeIte);
		}

		/*
		if (isBossMessageDisplay) {
			_messageReciever.Recieve(_dungeonMessage.at("start"));
			isBossMessageDisplay = false;
			return;
		}
		*/
	}
}


void Dungeon::SetFieldType() {

	//ダンジョンの地形の設定

	CSVReader reader;

	std::vector<std::string> FieldTypeArray;
	std::string fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNum + "/DungeonType.csv";
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, FieldTypeArray, 2);

	int FieldTypeNum = stoi(FieldTypeArray[stoi(_stageNum) * 2 - 1]);

	switch (FieldTypeNum) {
	case 0:
		fieldTypeStr = "#CAV";
		_windowBackground.Load(Main::RESOURCEFILENAME + "resource/graph/ui/main_window_background_cave.png");
		_background.Load(Main::RESOURCEFILENAME + "resource/graph/background/background_cave.png");
		break;
	case 1:
		fieldTypeStr = "#FST";
		_windowBackground.Load(Main::RESOURCEFILENAME + "resource/graph/ui/main_window_background_forest.png");
		_background.Load(Main::RESOURCEFILENAME + "resource/graph/background/background_forest.jpg");
		break;
	case 2:
		fieldTypeStr = "#STN";
		_windowBackground.Load(Main::RESOURCEFILENAME + "resource/graph/ui/main_window_background_stone.png");
		_background.Load(Main::RESOURCEFILENAME + "resource/graph/background/background_stone.jpg");
		break;
	case 3:
		fieldTypeStr = "#TIM";
		_windowBackground.Load(Main::RESOURCEFILENAME + "resource/graph/ui/main_window_background_wood.png");
		_background.Load(Main::RESOURCEFILENAME + "resource/graph/background/background_wood.png");
		break;
	default:
		fieldTypeStr = "#CAV";
		_windowBackground.Load(Main::RESOURCEFILENAME + "resource/graph/ui/main_window_background_cave.png");
		_background.Load(Main::RESOURCEFILENAME + "resource/graph/background/background_cave.png");
		break;
	}
	_windowBackground.SetPosition(Vector2D(28, 28));
	_background.SetPosition(Vector2D(0, 0));

}


int Dungeon::GetPassedNum() {
	return _goal->GetPassedNum(); 
}


std::vector<std::string> Dungeon::GetFileList(const std::string& path)
{

	FILEINFO FileInfo;
	DWORD_PTR FindHandle;

	std::vector<std::string> fileList;
	fileList.clear();

	// 検索開始、最初のファイルの情報を取得
	// ( *.* や ???.* などのシンプルなワイルドカード表現に対応しています )
	FindHandle = FileRead_findFirst((path + "*.csv").c_str(), &FileInfo);

	// ファイルが見つかった場合のみ分岐の中を実行
	if (FindHandle != (DWORD_PTR)-1)
	{
		do
		{
			//もしファイルだったら（ディレクトリではなかったら）
			if (FileInfo.DirFlag == FALSE) {
				//ファイルリストにファイルを追加
				fileList.push_back(FileInfo.Name);
			}
			// 検索に引っかかる次のファイルの情報を取得、無かったらループを抜ける
		} while (FileRead_findNext(FindHandle, &FileInfo) >= 0);
	}

	// 検索ハンドルの後始末
	FileRead_findClose(FindHandle);

	return fileList;
}





