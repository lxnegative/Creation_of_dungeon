#include "MakeDungeon.h"

#include "cd_666s\TilebaseAI\TiledObjectMnager.h"
#include "cd_666s/TilebaseAI/TileField.h"
#include "cd_666s\Resources\AllResourceManager.h"
#include "cd_666s/Render/RenderManager.h"
#include "cd_666s\Utility\CSVReader.h"

#include "cd_666s/TilebaseAI/TileField.h"
#include "cd_666s/TilebaseAI/River.h"
#include "cd_666s/TilebaseAI/Obstacle.h"
#include "cd_666s/TilebaseAI/WeakObstacle.h"
#include "cd_666s/TilebaseAI/EnemysItem.h"
#include "cd_666s/TilebaseAI/Monster.h"
#include "cd_666s/TilebaseAI/Bridge.h"
//#include "cd_666s/TilebaseAI/Goal.h"
//#include "cd_666s/TilebaseAI/StartPoint.h"

#include "cd_666s/TilebaseAI/Trap.h"

//#include "SettingBowgun.h"
#include "DungeonInfomation.h"

#include <assert.h>

MakeDungeon::MakeDungeon()
{
}

MakeDungeon::MakeDungeon(std::string areaNum, std::string stage_num)
    : _stage_num(stage_num)
	, _areaNumber(areaNum)
    , _mainsFrame(Main::RESOURCEFILENAME + "resource/graph/ui/main_window.png", Vector2D(20, 20))
    , _background(Main::RESOURCEFILENAME + "resource/graph/background/background.png", Vector2D(0, 7600))
    , _windowBackground(Main::RESOURCEFILENAME + "resource/graph/ui/main_window_background_cave.png", Vector2D(28, 28))
{

	auto b_pos = _stage_num.rfind('b');

	if (b_pos != std::string::npos) {
		_stage_num_a = _stage_num.substr(0, b_pos);
	}
	else {
		_stage_num_a = _stage_num;
	}

    _background.Load(Main::RESOURCEFILENAME + "resource/graph/background/background_cave.png");
    _background.SetPosition(Vector2D(0, 0));

    _windowBackground.Load(Main::RESOURCEFILENAME + "resource/graph/ui/main_window_background_cave.png");
    _windowBackground.SetPosition(Vector2D(28, 28));

    _mainsFrame.SetPriority(Sprite::Priority::UI);
    _background.SetPriority(Sprite::Priority::BACKGROUND);
    _windowBackground.SetPriority(static_cast<int>(Sprite::Priority::BACKGROUND) + 1);
}


MakeDungeon::~MakeDungeon()
{
    OBJECT_MGR->Clear();
    _start = nullptr;
    _goal = nullptr;
    FIELD->Clear();

    RESOURCE_TABLE->Refresh();

    OBJECT_MGR->Refresh();
}

void MakeDungeon::Update()
{
    //_monsters.Update();

    for (auto obj : OBJECT_MGR->_objects) {
        if (obj != nullptr) {
            //if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L)) {

            //obj->Update();
            //}
        }
    }

}

void MakeDungeon::Draw()
{
    FIELD->Draw();
	for (auto obj : OBJECT_MGR->_objects) {
		if (obj != nullptr)
			obj->Draw();
	}

}

void MakeDungeon::Init(std::string file_name)
{

    _messageReciever.Init();

	_messageReciever.DrawFalse();

	InUI.Load(Main::RESOURCEFILENAME + "resource/graph/ui/IN.png");
	InUI.SetPriority(static_cast<int>(Sprite::Priority::UI) + 50);
	InUI.SetPosition(Vector2D(0, 0));
	InUI.SetDisplayMode(false);

	OutUI.Load(Main::RESOURCEFILENAME + "resource/graph/ui/OUT.png");
	OutUI.SetPriority(static_cast<int>(Sprite::Priority::UI) + 50);
	OutUI.SetPosition(Vector2D(0, 0));
	OutUI.SetDisplayMode(false);

	for (int i = 0; i < 2; i++) {
		_NotPutObjectPlace[i].Load(Main::RESOURCEFILENAME + "resource/graph/ui/batu.png");
		_NotPutObjectPlace[i].SetRenderType(Texture2D::RenderType::UI);
		_NotPutObjectPlace[i].SetPriority(static_cast<int>(Sprite::Priority::UI) - 1);
		_NotPutObjectPlace[i].SetPosition(Vector2D(0, 0));
		_NotPutObjectPlace[i].SetDisplayMode(false);
	}

    //タイルの大きさを読み込む
    std::string fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNumber + "/tilesize.csv";
    std::vector<std::string> tileInfoArray;
    CSVReader reader;
    reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, tileInfoArray);
    LoadTileSize(_stage_num, tileInfoArray);

    std::vector<std::string> _stageArray;
    //フィールドの大本となるデータを読み込む
    std::string filename = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNumber  + "/";
    filename += (file_name + ".csv");

    reader.Read(RESOURCE_TABLE->GetFolderPath() + filename, _stageArray);



    auto fieldSizeH = reader.GetLineSize(filename, 0);
    auto fieldSizeV = reader.GetLineNum(filename);

    FIELD->Init(fieldSizeH, fieldSizeV);

    int countX = 0;
    int countY = 0;
    auto& _objs = OBJECT_MGR->_objects;

    filename = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNumber + "/monsters";
    filename += (_stage_num + ".csv");
    Monster::LoadMonsters(_objs, _monsters, filename);

    //FIELD->Init(fieldSizeH, fieldSizeV);

	SetFieldType();

    for (auto data : _stageArray) {
        GenerateObject(data, countX, countY);

        //ftが地形情報なので渡す
        FIELD->SetFieldType(TiledVector(countX, countY), fieldTypeStr);

        countX++;

        if (countX == fieldSizeH) {
            countX = 0;
            countY++;
            if (countY == fieldSizeV) {
                break;
            }
        }
    }

    /*設置オブジェクトをロードする処理が必要*/

    _monsters.Update();

    FIELD->Setup();

    OBJECT_MGR->Refresh();

    for (auto obj : _objs)
    {
        if (obj.get() != nullptr)
            obj->Init();
    }

	for (int i = 0; i < 2; i++) {
		_NotPutObjectPlace[i].SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));
	}

	InUI.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));
	OutUI.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));

    _selectingObj = "NONE";

}

void  MakeDungeon::Init(std::string file_name, bool isLoadNewMap) {

	_messageReciever.Init();

	_messageReciever.DrawFalse();

	InUI.Load(Main::RESOURCEFILENAME + "resource/graph/ui/IN.png");
	InUI.SetPriority(static_cast<int>(Sprite::Priority::UI) + 50);
	InUI.SetPosition(Vector2D(0, 0));
	InUI.SetDisplayMode(false);

	OutUI.Load(Main::RESOURCEFILENAME + "resource/graph/ui/OUT.png");
	OutUI.SetPriority(static_cast<int>(Sprite::Priority::UI) + 50);
	OutUI.SetPosition(Vector2D(0, 0));
	OutUI.SetDisplayMode(false);

	for (int i = 0; i < 2; i++) {
		_NotPutObjectPlace[i].Load(Main::RESOURCEFILENAME + "resource/graph/ui/batu.png");
		_NotPutObjectPlace[i].SetPriority(static_cast<int>(Sprite::Priority::UI) + 50);
		_NotPutObjectPlace[i].SetPosition(Vector2D(0, 0));
		_NotPutObjectPlace[i].SetDisplayMode(false);
	}

	//タイルの大きさを読み込む
	std::string fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNumber + "/tilesize.csv";
	std::vector<std::string> tileInfoArray;
	CSVReader reader;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, tileInfoArray);
	LoadTileSize(_stage_num, tileInfoArray);

	std::vector<std::string> _stageArray;

	std::string filename;

	size_t fieldSizeH;
	size_t fieldSizeV;

	//フィールドの大本となるデータを読み込む
	if (isLoadNewMap) {
		filename = Main::RESOURCEFILENAME + "csv/StageData/";
		filename += (file_name + ".csv");

		reader.Read(RESOURCE_TABLE->GetFolderPath() + filename, _stageArray);

		fieldSizeH = reader.GetLineSize(filename, 0);
		fieldSizeV = reader.GetLineNum(filename);
	}
	else {
		_stageArray = DungeonInfomation::GetInstance()->GetEditMapDataList();

		fieldSizeH = DungeonInfomation::GetInstance()->GetLineSize();
		fieldSizeV = DungeonInfomation::GetInstance()->GetLineNum();
	}

	

	FIELD->Init(fieldSizeH, fieldSizeV);

	int countX = 0;
	int countY = 0;
	auto& _objs = OBJECT_MGR->_objects;

	if (isLoadNewMap) {
		filename = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNumber + "/monsters";
		filename += (_stage_num + ".csv");
		Monster::LoadMonsters(_objs, _monsters, filename);
	}
	else {
		//filename = Main::RESOURCEFILENAME + "csv/StageData/EditMap_MonsterData.csv";
		Monster::LoadMonsters_DungeonInfomation(_objs, _monsters);
	}
	
	

	//FIELD->Init(fieldSizeH, fieldSizeV);

	SetFieldType();

	for (auto data : _stageArray) {
		GenerateObject(data, countX, countY);

		//ftが地形情報なので渡す
		FIELD->SetFieldType(TiledVector(countX, countY), fieldTypeStr);

		countX++;

		if (countX == fieldSizeH) {
			countX = 0;
			countY++;
			if (countY == fieldSizeV) {
				break;
			}
		}

	}

	/*設置オブジェクトをロードする処理が必要*/

	_monsters.Update();

	FIELD->Setup();

	OBJECT_MGR->Refresh();

	for (auto obj : _objs)
	{
		if (obj.get() != nullptr)
			obj->Init();
	}

	for (int i = 0; i < 2; i++) {
		_NotPutObjectPlace[i].SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));
	}

	InUI.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));
	OutUI.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));

	_selectingObj = "NONE";

	

}




void MakeDungeon::PickupObject()
{

}

void MakeDungeon::GenerateObject(std::string typeName, int countX, int countY)
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
			InUI.SetPosition(TiledVector(countX - 1, countY).GetWorldPos());
			InUI.SetDisplayMode(true);
			_NotPutObjectPlace[0].SetPosition(GetStartFrontPosition().GetWorldPos());
			_NotPutObjectPlace[0].SetDisplayMode(true);
            _objs.push_back(_start);
        }
        return;
    }

    if (typeName.find("100") != std::string::npos)
    {
        if (_goal == nullptr)
        {
            _goal = std::make_shared<Goal>(TiledVector(countX, countY), _monsters, _messageReciever, 99);
			OutUI.SetPosition(TiledVector(countX + 1, countY).GetWorldPos());
			OutUI.SetDisplayMode(true);
			_NotPutObjectPlace[1].SetPosition(GetGoalFrontPosition().GetWorldPos());
			_NotPutObjectPlace[1].SetDisplayMode(true);
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

TiledVector MakeDungeon::GetStartFrontPosition()
{
	if (_start->GetTilePos()._y == 0) {
		return (_start->GetTilePos() + TiledVector(0, 1));
	}
	else {
		return (_start->GetTilePos() + TiledVector(0, -1));
	}


}

TiledVector MakeDungeon::GetGoalFrontPosition()
{

	if (_goal->GetTilePos()._y == 0) {
		return (_goal->GetTilePos() + TiledVector(0, 1));
	}
	else {
		return (_goal->GetTilePos() + TiledVector(0, -1));
	}

}



TiledObject* MakeDungeon::GenerateAddObject(std::string typeName, int countX, int countY, Vector2D mousePos)
{
    FIELD->SetRawNumber(TiledVector(countX, countY), stoi(typeName));
    FIELD->SetFieldType(TiledVector(countX, countY), typeName);


    auto& _objs = OBJECT_MGR->_objects;

	if (typeName.find("300") != std::string::npos)
	{
		_objs.push_back(std::make_shared<WeakObstacle>(TiledVector(countX, countY), typeName));
		//一つのマスに複数のオブジェクトはないのでこれで生成したオブジェクトのポインタをとれるはず
		std::vector<TiledObject*> toTemp = FIELD->GetTiledObjects(TiledVector(countX, countY));
		TiledObject* to;
		if (!toTemp.empty()) {
			to = toTemp[0];
		}
		else {
			to = nullptr;
		}
		return to;
	}

	if (typeName.find("301") != std::string::npos)
	{
		_objs.push_back(std::make_shared<Bridge>(typeName, TiledVector(countX, countY)));
		//一つのマスに複数のオブジェクトはないのでこれで生成したオブジェクトのポインタをとれるはず
		std::vector<TiledObject*> toTemp = FIELD->GetTiledObjects(TiledVector(countX, countY));
		TiledObject* to;
		if (!toTemp.empty()) {
			to = toTemp[0];
		}
		else {
			to = nullptr;
		}
		return to;
	}

    if (typeName.find("9#") != std::string::npos)
    {
        Trap::CreateTrap(typeName, countX, countY, _objs);
        //一つのマスに複数のオブジェクトはないのでこれで生成したオブジェクトのポインタをとれるはず
        std::vector<TiledObject*> toTemp = FIELD->GetTiledObjects(TiledVector(countX, countY));
        TiledObject* to;
        if (!toTemp.empty()) {
            to = toTemp[0];
        }
        else {
            to = nullptr;
        }
        return to;
    }

    if (typeName.find("2&") != std::string::npos)
    {
        LoadItem(typeName, countX, countY, _objs);
        //一つのマスに複数のオブジェクトはないのでこれで生成したオブジェクトのポインタをとれるはず
        std::vector<TiledObject*> toTemp = FIELD->GetTiledObjects(TiledVector(countX, countY));
        TiledObject* to;
        if (!toTemp.empty()) {
            to = toTemp[0];
        }
        else {
            to = nullptr;
        }
        return to;
    }
    /*
    if (typeName.find("200") != std::string::npos)
    {
        if (_start == nullptr)
        {
            _start = std::make_shared<StartPoint>(TiledVector(countX, countY));
            _objs.push_back(_start);
        }
        return;
    }

    if (typeName.find("100") != std::string::npos)
    {
        if (_goal == nullptr)
        {
            _goal = std::make_shared<Goal>(TiledVector(countX, countY), _monsters);
            _objs.push_back(_goal);
        }
        return;
    }
    */
    if (typeName.find("6") != std::string::npos)
    {
		_objs.push_back(std::make_shared<River>(TiledVector(countX, countY), typeName));
        //一つのマスに複数のオブジェクトはないのでこれで生成したオブジェクトのポインタをとれるはず
        std::vector<TiledObject*> toTemp = FIELD->GetTiledObjects(TiledVector(countX, countY));
        TiledObject* to;
        if (!toTemp.empty()) {
            to = toTemp[0];
        }
        else {
            to = nullptr;
        }
        return to;
    }

    if (typeName.find("1") != std::string::npos)
    {
        _objs.push_back(std::make_shared<Obstacle>(TiledVector(countX, countY)));
        //一つのマスに複数のオブジェクトはないのでこれで生成したオブジェクトのポインタをとれるはず
        std::vector<TiledObject*> toTemp = FIELD->GetTiledObjects(TiledVector(countX, countY));
        TiledObject* to;
        if (!toTemp.empty()) {
            to = toTemp[0];
        }
        else {
            to = nullptr;
        }
        return to;
    }

    if (typeName == "0")
        return nullptr;

    

    return nullptr;

}

std::vector<TiledObject*> MakeDungeon::GenerateMonster(std::string fileName, TiledVector startPos, std::string* GenerateText, int level) {

	std::vector<TiledObject*> temp;
	temp.clear();
	
	if (level <= 0) {
		return temp;
	}

    auto& _objs = OBJECT_MGR->_objects;

	temp = Monster::GenerateMonster(_objs, _monsters, fileName, startPos, GenerateText, level);

    _monsters.Update();

    return temp;

}



void MakeDungeon::LoadTileSize(std::string stageName, std::vector<std::string>& rawData)
{
    auto stageNum = std::stoi(stageName);
    auto tileSize = rawData[stageNum * 2 - 1];

    if (tileSize == "large")
        TILE_SIZE = 48;
    else if (tileSize == "midium")
        TILE_SIZE = 40;
    else
        TILE_SIZE = 32;
}


std::shared_ptr<Goal> MakeDungeon::getGoal(){

    return _goal;

}

std::shared_ptr<StartPoint> MakeDungeon::getStart(){

    return _start;

}


void MakeDungeon::SetFieldType() {

	//ダンジョンの地形の設定

	CSVReader reader;

	std::vector<std::string> FieldTypeArray;
	std::string fileName = Main::RESOURCEFILENAME + "csv/StageData/MapData/Area" + _areaNumber + "/DungeonType.csv";
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, FieldTypeArray, 2);

	int FieldTypeNum = stoi(FieldTypeArray[stoi(_stage_num_a) * 2 - 1]);

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


std::string MakeDungeon::GetFieldType() {
	return fieldTypeStr;
}

