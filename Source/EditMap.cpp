#include "EditMap.h"
#include "FunctionTransScene.h"
#include "PanelBase.h"

#include "PanelAffectObjects.h"
#include "PanelDisplayer.h"
#include "PanelSceneTransition.h"
#include "PanelSettingObject.h"
#include "PanelPageChangeObject.h"

#include "cd_666s/Utility/CSVReader.h"
#include "cd_666s/Resources/AllResourceManager.h"

#include "PanelObjectManager.h"

#include "cd_666s/TilebaseAI/TileField.h"
#include "cd_666s/TilebaseAI/TiledObjectMnager.h"

#include "cd_666s/DebugDraw.h"

#include "Game.h"

#include "cd_666s\TilebaseAI\Enemy.h"
#include "cd_666s\TilebaseAI\AI\AstarPathFinder.h"

#include <typeinfo>
#include <sstream>

#include <iostream>

#include <assert.h>

#include "ShopAssortment.h"
#include "MoneyManager.h"
#include "PanelTrashObject.h"

#include "Main.h"
#include "WorldMap.h"
#include "cd_666s/TilebaseAI/Trap.h"
#include "cd_666s/TilebaseAI/Emplacement.h"
#include "DungeonInfomation.h"

#include <regex>

#define STR(var) #var


EditMap::EditMap(std::string _area_num, string _stage_num, std::string _point_num)
	: stage_num(_stage_num)
	, area_num(_area_num)
	, point_num(_point_num)
	, NPOS(std::string::npos)
	, stage_num_temp({ _stage_num.front() })
	, _bgm(Main::RESOURCEFILENAME + "resource/sound/PutThings.mp3", true)
{

	auto b_pos = stage_num.rfind('b');

	if (b_pos != std::string::npos) {
		stage_num_temp = stage_num.substr(0, b_pos);
	}
	else {
		stage_num_temp = stage_num;
	}

	_functions.reserve(20);
	//panels.reserve(30);
	class_name = "editmap";

    _selectCategoryGr.Load(Main::RESOURCEFILENAME + "resource/graph/ui/SelectTypeFrame.png");
    _selectCategoryGr.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);

    _selectCategoryGr.SetDisplayMode(false);
    
    _selectObjectGr.Load(Main::RESOURCEFILENAME + "resource/graph/ui/SelectObjectFrame.png");
    _selectObjectGr.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
    
    _selectObjectGr.SetDisplayMode(false);

	isLoadNewMap = true;

	_moneyPouch.Load(Main::RESOURCEFILENAME + "resource/graph/ui/MoneyPouch.png");
	_moneyPouch.SetPriority(static_cast<int>(Sprite::Priority::UI) + 10);
	_moneyPouch.SetScale(Vector2D(0.7, 0.7));
	_moneyPouch.SetPosition(Vector2D(980, 140));

    Init();

	_bgm.Play();
}

EditMap::EditMap(std::string _area_num, string _stage_num, std::string _point_num, bool isLoadNewMap)
	: stage_num(_stage_num)
	, area_num(_area_num)
	, point_num(_point_num)
	, NPOS(std::string::npos)
	, stage_num_temp({ _stage_num.front() })
	,_bgm(Main::RESOURCEFILENAME + "resource/sound/PutThings.mp3", true)
{

	auto b_pos = stage_num.rfind('b');

	if (b_pos != std::string::npos) {
		stage_num_temp = stage_num.substr(0, b_pos);
	}
	else {
		stage_num_temp = stage_num;
	}

	_functions.reserve(20);
	//panels.reserve(30);
	class_name = "editmap";

	_selectCategoryGr.Load(Main::RESOURCEFILENAME + "resource/graph/ui/SelectTypeFrame.png");
	_selectCategoryGr.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);

	_selectCategoryGr.SetDisplayMode(false);

	_selectObjectGr.Load(Main::RESOURCEFILENAME + "resource/graph/ui/SelectObjectFrame.png");
	_selectObjectGr.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);

	_selectObjectGr.SetDisplayMode(false);

	this->isLoadNewMap = isLoadNewMap;

	_moneyPouch.Load(Main::RESOURCEFILENAME + "resource/graph/ui/MoneyPouch.png");
	_moneyPouch.SetPriority(static_cast<int>(Sprite::Priority::UI) + 10);
	_moneyPouch.SetScale(Vector2D(0.7, 0.7));
	_moneyPouch.SetPosition(Vector2D(980, 140));

	Init();

	_bgm.Play();
}


EditMap::~EditMap()
{
    _functions.clear();


#if 1
    std::string pass = RESOURCE_TABLE->GetFolderPath();
    std::string str = Main::RESOURCEFILENAME + "csv/Stagedata/template.csv";
    pass += str;
    std::remove(pass.c_str());
#endif
}

SceneBase * EditMap::Update(UIManager _ui)
{
	
	if (tutorialPanel == nullptr && tutorial_Balloon == nullptr) {
		if (area_num == "1" && stage_num.find("1") != std::string::npos) {
			if (!Tutorial::getInstance()->isLookingTutorial("Stage1-1_Edit")) {
				tutorialPanel = new TutorialPanel("Stage1-1_Edit");
				isTutorial = true;
			}
		}
	}

	if (tutorialPanel == nullptr && tutorial_Balloon == nullptr) {
		if (area_num == "1" && stage_num.find("2") != std::string::npos) {
			if (!Tutorial::getInstance()->isLookingTutorial("Stage1-2_Edit")) {
				tutorial_Balloon = new Tutorial_Balloon("Stage1-2_Edit");
			}
		}
	}

	if (tutorialPanel == nullptr && tutorial_Balloon == nullptr) {
		if (area_num == "1" && stage_num.find("4") != std::string::npos) {
			if (!Tutorial::getInstance()->isLookingTutorial("Stage1-4_Edit")) {
				tutorial_Balloon = new Tutorial_Balloon("Stage1-4_Edit");
			}
		}
	}

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
		if (tutorial_Balloon->GetIsFinish()) {
			Tutorial::getInstance()->LookTutorial(tutorial_Balloon->GetTimingName());
			delete(tutorial_Balloon);
			tutorial_Balloon = nullptr;
			return this;
		}

		if (!needUpdate) {
			return this;
		}
		
	}





	if (isPause) {
		bool goTitle = PauseUpdate();

		if (goTitle)
		{
			PANEL_MGR->Clear();
			PANEL_MGR->Refresh();

			int tempareanum = std::stoi(area_num);
			int temppointnum = std::stoi(point_num);

			Information_SaveData::GetInstance()->TempLoadProcesses(&tempareanum, &temppointnum);


			return new WorldMap(tempareanum, temppointnum);
		}

		return this;
	}

	if (tutorial_Balloon == nullptr && tutorialPanel == nullptr) {
		if (KEYBOARD->ButtonDown(KeyInput::KeyType::KEY_ESCAPE))
			isPause = true;
	}


	popUpWindow.Update();

	if (popUpWindow.GetIsDisplay()) {
		return this;
	}



    //std function に変更する？
    _functions.resize(0);

    //NOTE:
    //各個別UIに種類別の情報を格納し、
    //またほかのUIからの戻り値や情報の変化によってその値を返したり変更したりする
    //仕組みを考案する必要がある

    std::string _ui_name;

#if 1
    /*試しコード*/
    /*NOTE*/
    /*
        PanelBaseには、そのパネルの名前、座標、変数が格納されている
        変数は子クラスごとに異なるので注意
    */
    //std::vector<PanelBase> panels; //試験コード用

    /*
        別のオブジェクトへのアクセスが必要になった際に
        対象となるオブジェクトを格納する
        配列 ならびに 簡易関数
    */
    /*multimap キー:カテゴリ名, 値:対象のクラス型オブジェクト*/
    std::multimap<std::string, PanelBase> _targets;
    auto SetClassType = [&] {
        _targets.clear();
    };

    auto scene = PanelFunction();



#endif

#if 0
    _ui.Update(_functions, _ui_name);

    for (auto f : _functions) {
        if (f.find("CHANGE_LIST") != std::string::npos) {

            //_se.Play();
        }
        else if (f.find("SHOW") != std::string::npos) {
            //_se.Play();
        }
        else if (f.find("MOVE") != std::string::npos) {
            //_se.Play();
        }
        else if (f.find("SELECT_OBJECT") != std::string::npos) {
            _dungeon->PickupObject();
            //_se.Play();
        }

    }
#endif

    for(auto p : PANEL_MGR->_objects){
        if (p != nullptr)
            p->Update();
    }

    _dungeon->Update();

    SetObject();

	RotateObject();

	if (objectTextPanel->EvolCheck()) {
		std::shared_ptr<PanelSettingObject> temp = selectedObject;
		ShopAssortment::getInstance()->EvolMonster(selectedObject);
		editObject.ResetLevel();

		_buybycoinD.Play();

		for (auto p : PANEL_MGR->_objects) {

			if (p == nullptr)
				continue;

			auto str = std::string(typeid(*p).name());

			//クリックされたパネルの名前が"AffectObjects"だった場合
			if (str.find("AffectObjects") != NPOS) {
				if (selectPanelCategory == p->GetCategoryName()) {
					PanelAffectObjectsFunction(p);
				}
			}

		}
		selectedObject = temp;
		_selectCategoryGr.SetDisplayMode(true);

		payOffCost = CheckPayOff();

		if (selectedObject != nullptr) {
			objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
		}

	}

    //DeleteAddedObject();

    return scene;
}


void EditMap::Draw()
{
	if (tutorialPanel != nullptr) {
		return;
	}

	if (tutorial_Balloon != nullptr) {
		tutorial_Balloon->Draw();
	}

	if (isPause) {

		PauseDraw();

		return;
	}

	popUpWindow.Draw();

    _dungeon->Draw();

    for (auto& p : PANEL_MGR->_objects) {
        if (p != nullptr)
            p->Draw();
    }

    PANEL_MGR->Refresh();


	if (selectedObject) {

		//カーソル位置がフィールド内かチェック
		auto cursorPos = MOUSE->GetCursorPos();
		auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);

		for (int i = 0; i < NotPutObjectPos.size(); i++) {
			Debug::DrawRectWithSize(NotPutObjectPos[i], Vector2D(TILE_SIZE, TILE_SIZE), Color4(1.0, 0.0, 0.0, 0.3), true);
		}

		if(!popUpWindow.GetIsDisplay() && payOffCost <= MoneyManager::getInstance()->getMoney()) {
			if (SetObjectCheck(tiledCursorPos) && FIELD->IsInside(tiledCursorPos) && _dungeon->GetStartFrontPosition() != tiledCursorPos && _dungeon->GetGoalFrontPosition() != tiledCursorPos) {
				_PutPlanObjectImage.SetPosition(tiledCursorPos.GetWorldPos());
				auto imageColor = new Color4(0, 0, 0, 0.5f);
				_PutPlanObjectImage.SetBaseColor(*imageColor);
				_PutPlanObjectImage.SetDisplayMode(true);
			}
			else {
				_PutPlanObjectImage.SetDisplayMode(false);
			}
		}



	}

	//ゴミ箱が選択されているか
	if (trashPanel->GetSelect()) {
		for (int i = 0; i < addTiledObjectList_Trap.size(); i++) {
			Debug::DrawRectWithSize(addTiledObjectList_Trap[i].tiledObject->GetPosition(), Vector2D(TILE_SIZE, TILE_SIZE), Color4(0.0, 1.0, 0.0, 0.5), true);
		}
		for (int i = 0; i < addTiledObjectList_Block.size(); i++) {
			Debug::DrawRectWithSize(addTiledObjectList_Block[i].tiledObject->GetPosition(), Vector2D(TILE_SIZE, TILE_SIZE), Color4(0.0, 1.0, 0.0, 0.5), true);
		}
		for (int i = 0; i < addTiledObjectList_Monster.size(); i++) {
			Debug::DrawRectWithSize(addTiledObjectList_Monster[i].MagicSquareObject->GetPosition(), Vector2D(TILE_SIZE, TILE_SIZE), Color4(0.0, 1.0, 0.0, 0.5), true);
		}

		for (int i = 0; i < RemainingObjectList_Trap.size(); i++) {
			Debug::DrawRectWithSize(RemainingObjectList_Trap[i].tiledObject->GetPosition(), Vector2D(TILE_SIZE, TILE_SIZE), Color4(1.0, 0.6, 0.0, 0.5), true);
		}
		for (int i = 0; i < RemainingObjectList_Block.size(); i++) {
			Debug::DrawRectWithSize(RemainingObjectList_Block[i].tiledObject->GetPosition(), Vector2D(TILE_SIZE, TILE_SIZE), Color4(1.0, 0.6, 0.0, 0.5), true);
		}
		for (int i = 0; i < RemainingObjectList_Monster.size(); i++) {
			Debug::DrawRectWithSize(RemainingObjectList_Monster[i].MagicSquareObject->GetPosition(), Vector2D(TILE_SIZE, TILE_SIZE), Color4(1.0, 0.6, 0.0, 0.5), true);
		}
	}


	/*
    std::string debugStr = "設置上限\n";
    debugStr += "モンスター:" + std::to_string(set_count["MONSTER"]) + "/" + std::to_string(LIMIT_MONSTER)+ " ";
    debugStr += "トラップ:" + std::to_string(set_count["TRAP"]) + "/" + std::to_string(LIMIT_TRAP) + "\n";
    debugStr += "ブロック:" + std::to_string(set_count["BLOCK"]) + "/" + std::to_string(LIMIT_BLOCK) + "\n";
	*/

	payOffCost = CheckPayOff();

	//std::string debugStr = "所持金：" + std::to_string(MoneyManager::getInstance()->getMoney()) + "M\n";
	//debugStr += "清算予定：" + std::to_string(payOffCost) + "M\n";

	std::string debugStr = std::to_string(MoneyManager::getInstance()->getMoney()- payOffCost) + "Ｍ\n";

	double payMoneyRate = (double)(MoneyManager::getInstance()->getMoney() - payOffCost) / MoneyManager::getInstance()->getMoney();

	int greenColor[] = { 60,179,113 };
	int redColor[] = { 211,28,48 };

	double StringColor[] = { 0,0,0 };

	for (int i = 0; i < 3; i++) {
		StringColor[i] = (double)redColor[i] - (double)(redColor[i] - greenColor[i]) * payMoneyRate;

		StringColor[i] = StringColor[i] / 255;

		if (payMoneyRate < 0.0) {
			StringColor[i] = (double)redColor[i] / 255;
		}
	}
	

    Debug::DrawString(Vector2D(1070, 155), debugStr,Color4(StringColor[0], StringColor[1], StringColor[2], 1.0f),28);



    std::string pageStr = "";

    if (selectPanelCategory.find("MONSTER") != std::string::npos) {
        pageStr += std::to_string(pageNum) + "/" + std::to_string(pageMaxNum);
    }
    else if (selectPanelCategory.find("TRAP") != std::string::npos) {
        pageStr += std::to_string(pageNum) + "/" + std::to_string(pageMaxNum);
    }
    else if (selectPanelCategory.find("BLOCK") != std::string::npos) {
        pageStr += std::to_string(pageNum) + "/" + std::to_string(pageMaxNum);
    }
    else {
        pageStr += std::to_string(pageNum) + "/" + std::to_string(pageMaxNum);
    }

    Debug::DrawString(Vector2D(1055, 270), pageStr, Color4(0, 0, 0, 1.0f), 40);

	objectTextPanel->Draw();

	/*
	clsDx();

	printfDx("所持金：%d",MoneyManager::getInstance()->getMoney());
	*/

	



}

void EditMap::Init()
{

	isInit = true;

	if (MoneyManager::getInstance()->getMoney() < 400) {
		MoneyManager::getInstance()->increaseMoney(400 - MoneyManager::getInstance()->getMoney());
	}

	_blockSelect.Load(Main::RESOURCEFILENAME + "resource/sound/blockSelect.wav");
	_blockSelect.SetVolume(200);

	_setbloksD.Load(Main::RESOURCEFILENAME + "resource/sound/set bloksD.wav");
	_setbloksD.SetVolume(200);

	_decideSE.Load(Main::RESOURCEFILENAME + "resource/sound/decide.wav");
	_decideSE.SetVolume(200);

	_buybycoinD.Load(Main::RESOURCEFILENAME + "resource/sound/buy by coinD.wav");
	_buybycoinD.SetVolume(200);

    //パネルの読み込み
    CSVReader reader;
    std::vector<std::string> panels_str;
    std::string filename = Main::RESOURCEFILENAME + "csv/Panel/";
    filename += class_name + ".csv";
    reader.Read(RESOURCE_TABLE->GetFolderPath() + filename, panels_str, 1);

    /*stringを各要素ごとに代入*/
    /*
    std::map<std::string, std::shared_ptr<PanelBase>> panelTypes;
    panelTypes.insert(std::make_pair("CHANGE_LIST",new PanelAffectObjects()));
    panelTypes.insert(std::make_pair("MOVE", new PanelSceneTransition()));
    panelTypes.insert(std::make_pair("SELECT_OBJ", new PanelSettingObject()));
    panelTypes.insert(std::make_pair("SHOW", new PanelDisplayer()));
    auto b = panelTypes.begin();
    */

    //DebugOutputFile();

    //ここにPANEL_MGR追加処理を
    int elem_count = 0;
    std::vector<std::string> panel_temp;
    std::vector<PanelContent> panel_cont_temp(1);
    panel_cont_temp.reserve(30);

    auto& panel_obj = PANEL_MGR->_objects;

    for (std::string p : panels_str) {
        if (p != "") {
            panel_temp.push_back(p);
            if (elem_count >= 4) {

                try {
                    std::shared_ptr<PanelBase> temp;

                    auto temp2 = std::make_shared<PanelContent>(Vector2D(std::stoi(panel_temp[0]), std::stoi(panel_temp[1])), panel_temp[2], panel_temp[4]);

                    SetPanelInstance(panel_temp[3], temp, *temp2);

					auto str = std::string(typeid(*temp).name());

					if (str.find("SceneTransition") != NPOS) {
						gameStartButton = std::dynamic_pointer_cast<PanelSceneTransition>(temp);
					}

                    panel_obj.push_back(temp);
                    
                    //panel_cont_temp.push_back(PanelContent(
                        //Vector2D(std::stoi(panel_temp[0]), std::stoi(panel_temp[1])), panel_temp[2], panel_temp[4]));
                }
                catch (std::out_of_range&) {
                    assert("Cannot push_back panel elem");
                }
                panel_temp.clear();
                elem_count = 0;
            }
            else {
                elem_count++;
            }
        }
    }

    std::string file_name = (this->isLoadNewMap ? "MapData/Area" + area_num + "/map" + stage_num : "EditMapData");

    
    PANEL_MGR->Refresh();
 
    /*
    int elem = 0;
    for (auto obj : panel_obj) {
        if (obj.get() != nullptr) {
            obj->Init(panel_cont_temp[elem]);
            obj->DrawDebugPrint();
        }
        elem++;
    }
    */
    
    _dungeon = std::make_shared<MakeDungeon>(area_num, stage_num);
	_dungeon->Init(file_name, this->isLoadNewMap);

    panel_cont_temp.clear();
    
    selectedObject.reset();

    selectPanelCategory = "";

    addTiledObjectList_Trap.clear();
    addTiledObjectList_Block.clear();

    /***各オブジェクト設置数カウンタ初期化***/
    //set_count.insert({ "BLOCK", 0 });
    //set_count.insert({ "MONSTER", 0 });
    //set_count.insert({ "TRAP", 0 });
    //set_count[0] = 0;
    //set_count[1] = 0;
    //set_count[2] = 0;
    /***各オブジェクト設置数カウンタ初期化 ここまで***/

	/*
	std::vector<std::string> limit_str;
	filename = Main::RESOURCEFILENAME + "csv/StageData/EditLimit.csv";
	reader.Read(RESOURCE_TABLE->GetFolderPath() + filename, limit_str, 1);

    if (TILE_SIZE == 48) {
		LIMIT_MONSTER = std::stoi(limit_str[4 * 0 + 1]);
        LIMIT_TRAP = std::stoi(limit_str[4 * 0 + 2]);
        LIMIT_BLOCK = std::stoi(limit_str[4 * 0 + 3]);
    }
    else if (TILE_SIZE == 40) {
		LIMIT_MONSTER = std::stoi(limit_str[4 * 1 + 1]);
		LIMIT_TRAP = std::stoi(limit_str[4 * 1 + 2]);
		LIMIT_BLOCK = std::stoi(limit_str[4 * 1 + 3]);
    }
    else if(TILE_SIZE == 32){
		LIMIT_MONSTER = std::stoi(limit_str[4 * 2 + 1]);
		LIMIT_TRAP = std::stoi(limit_str[4 * 2 + 2]);
		LIMIT_BLOCK = std::stoi(limit_str[4 * 2 + 3]);
    }
	else {
		LIMIT_MONSTER = std::stoi(limit_str[4 * 0 + 1]);
		LIMIT_TRAP = std::stoi(limit_str[4 * 0 + 2]);
		LIMIT_BLOCK = std::stoi(limit_str[4 * 0 + 3]);
	}
	*/

    _cancelSE.Load(Main::RESOURCEFILENAME + "resource/sound/cancelA.wav");
    _cancelSE.SetVolume(200);

	objectTextPanel = std::make_shared<ObjectTextPanel>(class_name);

	payOffCost = 0;
	pageNum = 0;
	pageMaxNum = 0;


	for (auto p : PANEL_MGR->_objects) {

		std::shared_ptr<PanelAffectObjects> ps = dynamic_pointer_cast<PanelAffectObjects>(p);

		if (ps) {

			if (ps->GetPanelContent()._name == "AffectObject_Monster") {
				PanelAffectObjectsFunction(p);
			}

		}

	}

	if (!isLoadNewMap) {
		std::vector<std::string> RemainingObjectPosArray = DungeonInfomation::GetInstance()->GetEditMap_AddObjectDataList();

		int parameterNum = 5;

		for (int i = 0; i < RemainingObjectPosArray.size(); i += parameterNum) {

			if (RemainingObjectPosArray[i] == "MONSTER") {

				addTileObject_Monster temp_m;

				std::vector<TiledObject*> temp = FIELD->GetTiledObjects(TiledVector(std::stoi(RemainingObjectPosArray[i + 2]), std::stoi(RemainingObjectPosArray[i + 3])));

				for (auto to : temp) {

					auto aff_str = std::string(typeid(*to).name());
					if (aff_str.find("Monster") != NPOS) {
						temp_m.MonsterObject = to;
					}
					else {
						temp_m.MagicSquareObject = to;
					}
				}

				std::regex regex("\\|");

				temp_m.ObjectName = RemainingObjectPosArray[i + 1];
				temp_m.GenerateText = std::regex_replace(RemainingObjectPosArray[i + 4], regex, ",");

				temp_m.Cost = 0;

				RemainingObjectList_Monster.push_back(temp_m);


			}
			else if (RemainingObjectPosArray[i] == "TRAP") {

				addTileObject atemp;

				TiledObject* temp = FIELD->GetTiledObject(TiledVector(std::stoi(RemainingObjectPosArray[i + 2]), std::stoi(RemainingObjectPosArray[i + 3])));

				atemp.tiledObject = temp;

				atemp.ObjectName = RemainingObjectPosArray[i + 1];
				atemp.GenerateText = RemainingObjectPosArray[i + 4];

				atemp.Cost = 0;

				RemainingObjectList_Trap.push_back(atemp);

			}
			else if (RemainingObjectPosArray[i] == "BLOCK") {

				addTileObject atemp;

				TiledObject* temp = FIELD->GetTiledObject(TiledVector(std::stoi(RemainingObjectPosArray[i + 2]), std::stoi(RemainingObjectPosArray[i + 3])));

				atemp.tiledObject = temp;

				atemp.ObjectName = RemainingObjectPosArray[i + 1];
				atemp.GenerateText = RemainingObjectPosArray[i + 4];

				atemp.Cost = 0;

				RemainingObjectList_Block.push_back(atemp);

			}
		}
	}

	tutorialPanel = nullptr;

	canGameStartImage = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/SceneTransition.png");
	cannotGameStartImage = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/GameStartButtonDoNot.png");

	isPause = false;

	isTutorial = false;

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

	NotPutObjectPos.clear();

	for (int x = 1; x < FIELD->GetFieldSize()._x - 1; x++) {
		for (int y = 1; y < FIELD->GetFieldSize()._y - 1; y++) {
			TiledVector tilepos;
			tilepos._x = x;
			tilepos._y = y;

			auto objects = FIELD->GetTiledObjects(tilepos);

			if (!objects.empty()) {
				bool isRemainingObject = false;
				for (int i = 0; i < objects.size(); i++) {
					for (int j = 0; j < RemainingObjectList_Trap.size(); j++) {
						if (objects[i] == RemainingObjectList_Trap[j].tiledObject) {
							isRemainingObject = true;
						}
					}
					for (int j = 0; j < RemainingObjectList_Block.size(); j++) {
						if (objects[i] == RemainingObjectList_Block[j].tiledObject) {
							isRemainingObject = true;
						}
					}
					for (int j = 0; j < RemainingObjectList_Monster.size(); j++) {
						if (objects[i] == RemainingObjectList_Monster[j].MagicSquareObject) {
							isRemainingObject = true;
						}
					}
				}
				if (!isRemainingObject) {
					NotPutObjectPos.push_back(tilepos.GetWorldPos());
				}
			}

		}
	}

	_PutPlanObjectImage.Load(Main::RESOURCEFILENAME + "resource/graph/ui/result/result_Black.png");
	_PutPlanObjectImage.SetRenderType(Texture2D::RenderType::DEFAULT);
	_PutPlanObjectImage.SetPriority(static_cast<int>(Sprite::Priority::UI) - 5);
	_PutPlanObjectImage.SetPosition(Vector2D(0, 0));
	_PutPlanObjectImage.SetDisplayMode(false);
	_PutPlanObjectImage.SetScale(Vector2D(TILE_SIZE / 32.0, TILE_SIZE / 32.0));

	//ゴミ箱のパネルを保存
	for (auto& p : PANEL_MGR->_objects) {
		if (p == nullptr)
			continue;

		auto aff_str = std::string(typeid(*p).name());
		if (aff_str.find("PanelTrashObject") != NPOS) {
			trashPanel = dynamic_pointer_cast<PanelTrashObject>(p);
		}
	}

	popUpWindow.InitSetup(Main::RESOURCEFILENAME + "resource/graph/ui/SystemMessage3.png", Main::RESOURCEFILENAME + "resource/graph/ui/OK.png");

	popUpWindow.SetIsDisplay(false);

	
	isInit = false;

}

bool EditMap::IsFirstWave()
{
    CSVReader reader;

    std::string filePass = Main::RESOURCEFILENAME + "csv/StageData/";
    std::string fileName = filePass + "template.csv";
    std::vector<std::string> stage_data;

    reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, stage_data, 0);
    auto b = stage_data.size();
    //ステージデータが無い(=第1ウェーブ)の場合
    if (stage_data.empty()) {
        //templateファイルの生成
        //std::ofstream(RESOURCE_TABLE->GetFolderPath() + fileName);
        return false;
    }
    return true;
}

SceneBase* EditMap::PanelFunction()
{

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
            auto str = std::string(typeid(*p).name());

            //クリックされたパネルの名前が"AffectObjects"だった場合
            if (str.find("AffectObjects") != NPOS) {
                PanelAffectObjectsFunction(p);
            }
            else if (str.find("Displayer") != NPOS) {
                PanelDisplayerFunction(p);
            }
            else if (str.find("SceneTrans") != NPOS) {

                //ここにスタートからゴールへの経路探索処理を追加する
                if (!Start_Connect_Goal()) {
                    _cancelSE.Play();
                    continue;
                }

				if (payOffCost > MoneyManager::getInstance()->getMoney()) {
					_cancelSE.Play();
					continue;
				}

				MoneyManager::getInstance()->decreaseMoney(payOffCost);

                PanelSceneTransFunction(p);

                if (_dungeon != nullptr)
                {
                    _dungeon.reset();
                    _dungeon = nullptr;
                }
                INPUT_MGR->Clear();
                RESOURCE_TABLE->Refresh();

				int partNum = 0;
				if (this->isLoadNewMap) {
					partNum = 0;
				}
				else {
					partNum = 1;
				}
				PANEL_MGR->Clear();
				PANEL_MGR->Refresh();

				_decideSE.Play();

                return new Game(area_num, stage_num, point_num, partNum);
            }
            else if (str.find("SettingObject") != NPOS) {
                PanelSettingObjectFunction(p);
            }
			else if (str.find("PanelPageChangeObject") != NPOS) {
				PanelPageChangeObjectFunction(p);
			}
			else if (str.find("PanelTrashObject") != NPOS) {
				PanelTrashObjectFunction(p);
			}

            p->Update();
        }

    }

    return this;
}

void EditMap::PanelAffectObjectsFunction(std::shared_ptr<PanelBase> panel)
{
    std::vector<std::shared_ptr<PanelBase>> temp_p;

    /*値を変化させる他パネルを検索する*/
    for (auto& ps : PANEL_MGR->_objects) {

        if (ps == nullptr)
            continue;

        auto aff_str = std::string(typeid(*ps).name());
        if (aff_str.find("SettingObject") != NPOS) {
            temp_p.push_back(ps);
        }
    }


	std::shared_ptr<PanelAffectObjects> ps = dynamic_pointer_cast<PanelAffectObjects>(panel);
	ps->SetSettingObject(temp_p, editObject);
    int j = 0;

    auto& obj = PANEL_MGR->_objects;

    for (int i = 0; i < obj.size(); i++) {

        if (obj[i] == nullptr)
            continue;

        auto aff_str = std::string(typeid(*obj[i]).name());
        if (aff_str.find("SettingObject") != NPOS) {
            obj[i] = temp_p[j];
            j++;
        }
    }


    /*値を変化させる他パネルを検索する*/
    //SettingObjectは未選択に
    for (auto& p : PANEL_MGR->_objects) {

        if (p == nullptr)
            continue;

        auto aff_str = std::string(typeid(*p).name());
        if (aff_str.find("SettingObject") != NPOS) {

            std::shared_ptr<PanelSettingObject> psTemp = dynamic_pointer_cast<PanelSettingObject>(p);

            if (psTemp) {
                psTemp->setIsSelected(false);
            }

        }
		else if (aff_str.find("PanelTrashObject") != NPOS) {
			std::shared_ptr<PanelTrashObject> psTemp = dynamic_pointer_cast<PanelTrashObject>(p);

			if (psTemp) {
				psTemp->NotSelect();
			}
		}
    }

    selectedObject.reset();

    selectPanelCategory = panel->GetCategoryName();

    if (selectPanelCategory.find("MONSTER") != std::string::npos) {
        _selectCategoryGr.SetDisplayMode(true);
        _selectCategoryGr.SetPosition(Vector2D(panel->GetPosition()._x - 1, panel->GetPosition()._y - 1));
        _selectObjectGr.SetDisplayMode(false);
    }
    else if (selectPanelCategory.find("TRAP") != std::string::npos) {
        _selectCategoryGr.SetDisplayMode(true);
        _selectCategoryGr.SetPosition(Vector2D(panel->GetPosition()._x - 1, panel->GetPosition()._y - 1));
        _selectObjectGr.SetDisplayMode(false);
    }
    else if (selectPanelCategory.find("BLOCK") != std::string::npos) {
        _selectCategoryGr.SetDisplayMode(true);
        _selectCategoryGr.SetPosition(Vector2D(panel->GetPosition()._x - 1, panel->GetPosition()._y - 1));
        _selectObjectGr.SetDisplayMode(false);
    }
    else {
        _selectCategoryGr.SetDisplayMode(false);
        _selectObjectGr.SetDisplayMode(false);
    }

	objectTextPanel->ResetMessage();

	if (editObject.GetEditObjectNum(selectPanelCategory) == 0) {
		pageNum = 0;
		pageMaxNum = 0;
	}
	else {
		pageNum = 1;
		pageMaxNum = (editObject.GetEditObjectNum(selectPanelCategory) - 1) / 8 + 1;
	}
	
	if(!isInit)
		_blockSelect.Play();

}

void EditMap::PanelDisplayerFunction(std::shared_ptr<PanelBase> panel)
{
}

void EditMap::PanelSceneTransFunction(std::shared_ptr<PanelBase> panel)
{


	std::vector<std::string> _stageArray;
	//フィールドの大本となるデータを読み込む
	std::string filename = Main::RESOURCEFILENAME + "csv/StageData/";
	filename += "MapData/Area"+ area_num + "/map" + stage_num + ".csv";
	CSVReader reader;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + filename, _stageArray);


    TiledVector temp = FIELD->GetFieldSize();

    int countX = 0;
    int countY = 0;

	DungeonInfomation::GetInstance()->SetEditMapData("");

    for (auto data : _stageArray) {
        
        for (int i = 0; i < addTiledObjectList_Trap.size(); i++) {

            if (addTiledObjectList_Trap[i].tiledObject->GetTilePos() == TiledVector(countX, countY)) {

                data = addTiledObjectList_Trap[i].GenerateText;

            }

        }
		
		for (int i = 0; i < RemainingObjectList_Trap.size(); i++) {

			if (RemainingObjectList_Trap[i].tiledObject->GetTilePos() == TiledVector(countX, countY)) {

				data = RemainingObjectList_Trap[i].GenerateText;

			}

		}

		for (int i = 0; i < addTiledObjectList_Block.size(); i++) {

			if (addTiledObjectList_Block[i].tiledObject->GetTilePos() == TiledVector(countX, countY)) {

				data = addTiledObjectList_Block[i].GenerateText;

			}

		}

        for (int i = 0; i < RemainingObjectList_Block.size(); i++) {

            if (RemainingObjectList_Block[i].tiledObject->GetTilePos() == TiledVector(countX, countY)) {

                data = RemainingObjectList_Block[i].GenerateText;

            }

        }


		DungeonInfomation::GetInstance()->AddEditMapData(data);


        countX++;

        if (countX == temp._x) {
            countX = 0;
            countY++;

			DungeonInfomation::GetInstance()->AddEditMapData("\n");

            if (countY == temp._y) {
                break;
            }
        }

        if (countX != 0) {
			DungeonInfomation::GetInstance()->AddEditMapData(",");
        }

    }

	filename = Main::RESOURCEFILENAME + "csv/StageData/";
	filename += "MapData/Area" + area_num + "/monsters" + stage_num + ".csv";

	std::vector<std::string> dataArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + filename, dataArray, 1);

	DungeonInfomation::GetInstance()->SetEditMonsterData("");

	DungeonInfomation::GetInstance()->AddEditMonsterData("hp, attack, defence, magic_attack, magic_defence, speed, startX, startY, name, resourceName, displayName, skill\n");

    

    const int parameterNum = 12;
    countX = 0;

    for (auto data : dataArray) {

        if (data.empty()) {
            break;
        }

		DungeonInfomation::GetInstance()->AddEditMonsterData(data);

        countX++;
        
        if (countX == parameterNum) {
			DungeonInfomation::GetInstance()->AddEditMonsterData("\n");
            countX = 0;
        }
        else {
			DungeonInfomation::GetInstance()->AddEditMonsterData(",");
        }

    }

	for (int i = 0; i < addTiledObjectList_Monster.size(); i++) {

		DungeonInfomation::GetInstance()->AddEditMonsterData(addTiledObjectList_Monster[i].GenerateText + "\n");

	}

    for (int i = 0; i < RemainingObjectList_Monster.size(); i++) {

		DungeonInfomation::GetInstance()->AddEditMonsterData(RemainingObjectList_Monster[i].GenerateText + "\n");

    }


	DungeonInfomation::GetInstance()->SetEditMap_AddObjectData("");

	DungeonInfomation::GetInstance()->AddEditMap_AddObjectData("ObjectCategory, ObjectName, AddObjectVecX, AddObjectVecY, GenerateText\n");

	if (isLoadNewMap) {

		for (int i = 0; i < addTiledObjectList_Trap.size(); i++) {

			DungeonInfomation::GetInstance()->AddEditMap_AddObjectData("TRAP," + addTiledObjectList_Trap[i].ObjectName + "," + std::to_string(addTiledObjectList_Trap[i].tiledObject->GetTilePos()._x) + "," + std::to_string(addTiledObjectList_Trap[i].tiledObject->GetTilePos()._y) + "," + addTiledObjectList_Trap[i].GenerateText + "\n");

		}

		for (int i = 0; i < addTiledObjectList_Block.size(); i++) {

			DungeonInfomation::GetInstance()->AddEditMap_AddObjectData("BLOCK," + addTiledObjectList_Block[i].ObjectName + "," + std::to_string(addTiledObjectList_Block[i].tiledObject->GetTilePos()._x) + "," + std::to_string(addTiledObjectList_Block[i].tiledObject->GetTilePos()._y) + "," + addTiledObjectList_Block[i].GenerateText + "\n");
		}

		std::regex regex(",");

		for (int i = 0; i < addTiledObjectList_Monster.size(); i++) {

			DungeonInfomation::GetInstance()->AddEditMap_AddObjectData("MONSTER," + addTiledObjectList_Monster[i].ObjectName + "," + std::to_string(addTiledObjectList_Monster[i].MagicSquareObject->GetTilePos()._x) + "," + std::to_string(addTiledObjectList_Monster[i].MagicSquareObject->GetTilePos()._y) + "," + std::regex_replace(addTiledObjectList_Monster[i].GenerateText, regex, "|") + "\n");

		}

	}



}

void EditMap::PanelSettingObjectFunction(std::shared_ptr<PanelBase> panel)
{

    //試験的にダウンキャストを使用
    //まずかったら変えます

    std::shared_ptr<PanelSettingObject> ps = dynamic_pointer_cast<PanelSettingObject>(panel);

    if (ps) {

        if (ps->getPanelObjectName().find("Lv") != NPOS)
            return;

        if (ps->getPanelObjectName().empty())
            return;

        if (ps->getIsSelected()) {
            ps->setIsSelected(false);
            selectedObject.reset();
            _selectObjectGr.SetDisplayMode(false);
			objectTextPanel->ResetMessage();
        }
        else {
            ps->setIsSelected(true);
            selectedObject = ps;
            _selectObjectGr.SetDisplayMode(true);
            _selectObjectGr.SetPosition(Vector2D(ps->GetPosition()._x - 8, ps->GetPosition()._y - 8));

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

			_PutPlanObjectImage.Load(selectedObject->GetPanelGraphPath());

        }
        
    }
    else {
        return;
    }


    /*値を変化させる他パネルを検索する*/
    //他のSettingObjectは未選択に
    for (auto& p : PANEL_MGR->_objects) {

        if (p == nullptr)
            continue;

        if (p == ps)
            continue;

        auto aff_str = std::string(typeid(*p).name());
        if (aff_str.find("SettingObject") != NPOS) {
            
            std::shared_ptr<PanelSettingObject> psTemp = dynamic_pointer_cast<PanelSettingObject>(p);

            if (psTemp) {
                psTemp->setIsSelected(false);
            }

        }
		else if (aff_str.find("PanelTrashObject") != NPOS) {
			std::shared_ptr<PanelTrashObject> psTemp = dynamic_pointer_cast<PanelTrashObject>(p);

			if (psTemp) {
				psTemp->NotSelect();
			}
		}
    }

	_decideSE.Play();

}


void EditMap::PanelPageChangeObjectFunction(std::shared_ptr<PanelBase> panel) {
	
	


	std::vector<std::shared_ptr<PanelBase>> temp_p;

	/*値を変化させる他パネルを検索する*/
	for (auto& ps : PANEL_MGR->_objects) {

		if (ps == nullptr)
			continue;

		auto aff_str = std::string(typeid(*ps).name());
		if (aff_str.find("SettingObject") != NPOS) {
			temp_p.push_back(ps);
		}
	}


	std::shared_ptr<PanelPageChangeObject> ps = dynamic_pointer_cast<PanelPageChangeObject>(panel);

	if (ps->IsPageUP()) {
		pageNum++;
	}
	else {
		pageNum--;
	}

	if (pageNum < 1) {
		pageNum = 1;
	}
	else if (pageNum > pageMaxNum) {
		pageNum = pageMaxNum;
	}

	ps->SetSettingObject(temp_p, editObject, selectPanelCategory, pageNum);
	int j = 0;

	auto& obj = PANEL_MGR->_objects;

	for (int i = 0; i < obj.size(); i++) {

		if (obj[i] == nullptr)
			continue;

		auto aff_str = std::string(typeid(*obj[i]).name());
		if (aff_str.find("SettingObject") != NPOS) {
			obj[i] = temp_p[j];
			j++;
		}
	}


	/*値を変化させる他パネルを検索する*/
	//SettingObjectは未選択に
	for (auto& p : PANEL_MGR->_objects) {

		if (p == nullptr)
			continue;

		auto aff_str = std::string(typeid(*p).name());
		if (aff_str.find("SettingObject") != NPOS) {

			std::shared_ptr<PanelSettingObject> psTemp = dynamic_pointer_cast<PanelSettingObject>(p);

			if (psTemp) {
				psTemp->setIsSelected(false);
			}

		}
		else if (aff_str.find("PanelTrashObject") != NPOS) {
			std::shared_ptr<PanelTrashObject> psTemp = dynamic_pointer_cast<PanelTrashObject>(p);

			if (psTemp) {
				psTemp->NotSelect();
			}
		}
	}

	selectedObject.reset();

	if (selectPanelCategory.find("MONSTER") != std::string::npos) {
		_selectObjectGr.SetDisplayMode(false);
	}
	else if (selectPanelCategory.find("TRAP") != std::string::npos) {
		_selectObjectGr.SetDisplayMode(false);
	}
	else if (selectPanelCategory.find("BLOCK") != std::string::npos) {
		_selectObjectGr.SetDisplayMode(false);
	}
	else {
		_selectCategoryGr.SetDisplayMode(false);
		_selectObjectGr.SetDisplayMode(false);
	}

	objectTextPanel->ResetMessage();

	_blockSelect.Play();

}


void EditMap::PanelTrashObjectFunction(std::shared_ptr<PanelBase> panel) {

	std::shared_ptr<PanelTrashObject> ps = dynamic_pointer_cast<PanelTrashObject>(panel);

	if (ps->GetSelect()) {
		ps->NotSelect();
	}
	else {
		ps->Select();
	}
	
	
	/*値を変化させる他パネルを検索する*/
	//SettingObjectは未選択に
	for (auto& p : PANEL_MGR->_objects) {

		if (p == nullptr)
			continue;

		auto aff_str = std::string(typeid(*p).name());
		if (aff_str.find("SettingObject") != NPOS) {

			std::shared_ptr<PanelSettingObject> psTemp = dynamic_pointer_cast<PanelSettingObject>(p);

			if (psTemp) {
				psTemp->setIsSelected(false);
			}

		}
	}

	selectedObject.reset();

	if (selectPanelCategory.find("MONSTER") != std::string::npos) {
		_selectObjectGr.SetDisplayMode(false);
	}
	else if (selectPanelCategory.find("TRAP") != std::string::npos) {
		_selectObjectGr.SetDisplayMode(false);
	}
	else if (selectPanelCategory.find("BLOCK") != std::string::npos) {
		_selectObjectGr.SetDisplayMode(false);
	}
	else {
		_selectCategoryGr.SetDisplayMode(false);
		_selectObjectGr.SetDisplayMode(false);
	}

	objectTextPanel->ResetMessage();

	_decideSE.Play();

}




void EditMap::SetPanelInstance(std::string key_name, std::shared_ptr<PanelBase>& panel, PanelContent& temp)
{
    if(key_name == "CHANGE_LIST"){
        panel = std::make_shared<PanelAffectObjects>(temp);
    }else if(key_name == "MOVE"){
        panel = std::make_shared<PanelSceneTransition>(temp);
    }
    else if(key_name == "SELECT_OBJ"){
        panel = std::make_shared<PanelSettingObject>(temp);
    }else if(key_name == "SHOW"){
        panel = std::make_shared<PanelDisplayer>(temp);
	}
	else if (key_name == "PAGE_CHANGE") {
		panel = std::make_shared<PanelPageChangeObject>(temp);
	}
	else if (key_name == "TRASH_OBJ") {
		panel = std::make_shared<PanelTrashObject>(temp);
	}
}

void EditMap::DebugOutputFile()
{
    std::ofstream writing_file;
    writing_file.open(RESOURCE_TABLE->GetFolderPath() + "test.csv", std::ios::out);
    writing_file.close();
}





void EditMap::SetObject() {

    if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
        return;

	

	//ゴミ箱が選択されているか
	if (trashPanel->GetSelect()) {
		DeleteAddedObject();
	}
	

    //パネルが選択されているか
    if (!selectedObject)
        return;


	if (payOffCost > MoneyManager::getInstance()->getMoney()) {
		_cancelSE.Play();
		return;
	}

    //クリック位置がフィールド内かチェック
    auto cursorPos = MOUSE->GetCursorPos();
    auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
    if (!FIELD->IsInside(tiledCursorPos))
        return;

	//スタートやゴール前の一マスには置けない
	if (_dungeon->GetStartFrontPosition() == tiledCursorPos || _dungeon->GetGoalFrontPosition() == tiledCursorPos) {
		_cancelSE.Play();
		return;
	}


    bool isSetting = SetObjectCheck(tiledCursorPos);


    if (isSetting) {

        if (selectPanelCategory == "MONSTER" /*&& set_count[selectPanelCategory] < LIMIT_MONSTER*/) {

			std::string fileName = Main::RESOURCEFILENAME + "csv/Edit/MONSTER_DATA/" + selectedObject->getPanelObjectName() + "/" + selectedObject->getPanelObjectName() + ".csv";

            std::string GenerateText = "";

            std::vector<TiledObject*> temp = _dungeon->GenerateMonster(fileName, tiledCursorPos, &GenerateText, selectedObject->getLevel());

            addTileObject_Monster temp_m;

            for (auto to : temp) {

                to->Init();

                auto aff_str = std::string(typeid(*to).name());
                if (aff_str.find("Monster") != NPOS) {
                    temp_m.MonsterObject = to;
                }else{
                    temp_m.MagicSquareObject = to;
                }
            }

			temp_m.ObjectName = selectedObject->getPanelObjectName();
            temp_m.GenerateText = GenerateText;

			temp_m.Cost = CheckCost(selectPanelCategory, selectedObject->getPanelObjectName());

            addTiledObjectList_Monster.push_back(temp_m);

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

            FIELD->Setup();
            OBJECT_MGR->Refresh();

            //set_count[selectPanelCategory]++;
        }
        else if (selectPanelCategory == "TRAP" /*&& set_count[selectPanelCategory] < LIMIT_TRAP*/) {
            TiledObject* temp = _dungeon->GenerateAddObject(selectedObject->GenerateText, tiledCursorPos._x, tiledCursorPos._y, cursorPos);
            temp->Init();

            addTileObject atemp;
            atemp.tiledObject = temp;

			atemp.ObjectName = selectedObject->getPanelObjectName();
            atemp.GenerateText = selectedObject->GenerateText;

			atemp.Cost = CheckCost(selectPanelCategory, selectedObject->getPanelObjectName());

            addTiledObjectList_Trap.push_back(atemp);
            
			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

            FIELD->SetFieldType(tiledCursorPos, _dungeon->GetFieldType());

            FIELD->Setup();
            OBJECT_MGR->Refresh();

            //set_count[selectPanelCategory]++;

        }
        else if (selectPanelCategory == "BLOCK" /*&& set_count[selectPanelCategory] < LIMIT_BLOCK*/) {
            TiledObject* temp = _dungeon->GenerateAddObject(selectedObject->GenerateText, tiledCursorPos._x, tiledCursorPos._y, cursorPos);
            temp->Init();

            addTileObject atemp;
            atemp.tiledObject = temp;

			atemp.ObjectName = selectedObject->getPanelObjectName();
            atemp.GenerateText = selectedObject->GenerateText;

			atemp.Cost = CheckCost(selectPanelCategory, selectedObject->getPanelObjectName());

            addTiledObjectList_Block.push_back(atemp);

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

            FIELD->SetFieldType(tiledCursorPos, _dungeon->GetFieldType());

            FIELD->Setup();
            OBJECT_MGR->Refresh();

            //set_count[selectPanelCategory]++;

        }

		_setbloksD.Play();

    }

	payOffCost = CheckPayOff();

	if (!Start_Connect_Goal() || payOffCost > MoneyManager::getInstance()->getMoney()) {
		if (gameStartButton) {
			gameStartButton->SetResource(cannotGameStartImage);
		}
	}
	else {
		if (gameStartButton) {
			gameStartButton->SetResource(canGameStartImage);
		}
	}

	

}

void EditMap::RotateObject()
{

	if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_R))
		return;

	//クリック位置がフィールド内かチェック
	auto cursorPos = MOUSE->GetCursorPos();
	auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
	if (!FIELD->IsInside(tiledCursorPos))
		return;

	auto objs = FIELD->GetTiledObjects<Emplacement>(tiledCursorPos);

	for (int i = 0; i < objs.size(); i++) {
		TiledVector::Direction* direction = objs[i]->GetDirection();
		if (direction != nullptr) {

			for (int j = 0; j < addTiledObjectList_Trap.size(); j++) {

				if (addTiledObjectList_Trap[j].tiledObject == objs[i]) {

					int afterDirection = (((int)(*direction) - 1) + 4) % 4;
					objs[i]->SetDirection((TiledVector::Direction)(afterDirection));

					auto directionStr = LoadLabeledElem("direction:", addTiledObjectList_Trap[j].GenerateText);

					std::string afterDirectionStr = "direction:up";

					switch ((TiledVector::Direction)(afterDirection)) {
					case TiledVector::Direction::BACK:
						afterDirectionStr = "direction:up";
						break;
					case TiledVector::Direction::FORWARD:
						afterDirectionStr = "direction:down";
						break;
					case TiledVector::Direction::LEFT:
						afterDirectionStr = "direction:left";
						break;
					case TiledVector::Direction::RIGHT:
						afterDirectionStr = "direction:right";
						break;
					}

					std::regex regex("direction:" + directionStr);

					addTiledObjectList_Trap[j].GenerateText = std::regex_replace(addTiledObjectList_Trap[j].GenerateText, regex, afterDirectionStr);

				}

			}


		}
	}
	

}




bool EditMap::SetObjectCheck(TiledVector tiledCursorPos) {

    auto objects = FIELD->GetTiledObjects(tiledCursorPos);

    if (objects.empty()) {
        return true;
    }

    return false;

}



void EditMap::DeleteAddedObject() {

	/*
    if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_R))
        return;
	*/

    //クリック位置がフィールド内かチェック
    auto cursorPos = MOUSE->GetCursorPos();
    auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
    if (!FIELD->IsInside(tiledCursorPos))
        return;

    for (int i = 0; i < addTiledObjectList_Trap.size(); i++) {

        if (addTiledObjectList_Trap[i].tiledObject->GetTilePos() == tiledCursorPos) {

			std::string om = addTiledObjectList_Trap[i].ObjectName;

            FIELD->SetRawNumber(addTiledObjectList_Trap[i].tiledObject->GetTilePos(), 0);
            OBJECT_MGR->Remove(addTiledObjectList_Trap[i].tiledObject);
            addTiledObjectList_Trap.erase(addTiledObjectList_Trap.begin() + i);
            OBJECT_MGR->Refresh();

            /****オブジェクト設置数の減算処理ここから****/
            //set_count["TRAP"]--;
            /****オブジェクト設置数の減算処理ここまで****/

			ResetCost("TRAP", om);

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

			_setbloksD.Play();

        }

    }

    for (int i = 0; i < addTiledObjectList_Block.size(); i++) {

        if (addTiledObjectList_Block[i].tiledObject->GetTilePos() == tiledCursorPos) {

			std::string om = addTiledObjectList_Block[i].ObjectName;

            FIELD->SetRawNumber(addTiledObjectList_Block[i].tiledObject->GetTilePos(), 0);
            OBJECT_MGR->Remove(addTiledObjectList_Block[i].tiledObject);
            addTiledObjectList_Block.erase(addTiledObjectList_Block.begin() + i);
            OBJECT_MGR->Refresh();


            /****オブジェクト設置数の減算処理ここから****/
            //set_count["BLOCK"]--;
            /****オブジェクト設置数の減算処理ここまで****/

			ResetCost("BLOCK", om);

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

			_setbloksD.Play();

        }

    }

    for (int i = 0; i < addTiledObjectList_Monster.size(); i++) {

        if (addTiledObjectList_Monster[i].MagicSquareObject->GetTilePos() == tiledCursorPos) {

			std::string om = addTiledObjectList_Monster[i].ObjectName;

            OBJECT_MGR->Remove(addTiledObjectList_Monster[i].MonsterObject);
            OBJECT_MGR->Remove(addTiledObjectList_Monster[i].MagicSquareObject);
            addTiledObjectList_Monster.erase(addTiledObjectList_Monster.begin() + i);
            OBJECT_MGR->Refresh();


            /****オブジェクト設置数の減算処理ここから****/
            //set_count["MONSTER"]--;
            /****オブジェクト設置数の減算処理ここまで****/

			ResetCost("MONSTER", om);

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

			_setbloksD.Play();
			
        }

    }



	for (int i = 0; i < RemainingObjectList_Trap.size(); i++) {

		if (RemainingObjectList_Trap[i].tiledObject->GetTilePos() == tiledCursorPos) {

			std::string om = RemainingObjectList_Trap[i].ObjectName;

			FIELD->SetRawNumber(RemainingObjectList_Trap[i].tiledObject->GetTilePos(), 0);
			OBJECT_MGR->Remove(RemainingObjectList_Trap[i].tiledObject);
			RemainingObjectList_Trap.erase(RemainingObjectList_Trap.begin() + i);
			OBJECT_MGR->Refresh();

			/****オブジェクト設置数の減算処理ここから****/
			//set_count["TRAP"]--;
			/****オブジェクト設置数の減算処理ここまで****/

			ResetCost("TRAP", om);

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

			_setbloksD.Play();

		}

	}

	for (int i = 0; i < RemainingObjectList_Block.size(); i++) {

		if (RemainingObjectList_Block[i].tiledObject->GetTilePos() == tiledCursorPos) {

			std::string om = RemainingObjectList_Block[i].ObjectName;

			FIELD->SetRawNumber(RemainingObjectList_Block[i].tiledObject->GetTilePos(), 0);
			OBJECT_MGR->Remove(RemainingObjectList_Block[i].tiledObject);
			RemainingObjectList_Block.erase(RemainingObjectList_Block.begin() + i);
			OBJECT_MGR->Refresh();


			/****オブジェクト設置数の減算処理ここから****/
			//set_count["BLOCK"]--;
			/****オブジェクト設置数の減算処理ここまで****/

			ResetCost("BLOCK", om);

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

			_setbloksD.Play();

		}

	}

	for (int i = 0; i < RemainingObjectList_Monster.size(); i++) {

		if (RemainingObjectList_Monster[i].MagicSquareObject->GetTilePos() == tiledCursorPos) {

			std::string om = RemainingObjectList_Monster[i].ObjectName;

			OBJECT_MGR->Remove(RemainingObjectList_Monster[i].MonsterObject);
			OBJECT_MGR->Remove(RemainingObjectList_Monster[i].MagicSquareObject);
			RemainingObjectList_Monster.erase(RemainingObjectList_Monster.begin() + i);
			OBJECT_MGR->Refresh();


			/****オブジェクト設置数の減算処理ここから****/
			//set_count["MONSTER"]--;
			/****オブジェクト設置数の減算処理ここまで****/

			ResetCost("MONSTER", om);

			payOffCost = CheckPayOff();

			if (selectedObject != nullptr) {
				objectTextPanel->SetMessage(selectedObject, CheckCost(selectPanelCategory, selectedObject->getPanelObjectName()), payOffCost);
			}

			_setbloksD.Play();

		}

	}

	if (!Start_Connect_Goal() || payOffCost > MoneyManager::getInstance()->getMoney()) {
		if (gameStartButton) {
			gameStartButton->SetResource(cannotGameStartImage);
		}
	}
	else {
		if (gameStartButton) {
			gameStartButton->SetResource(canGameStartImage);
		}
	}


}

//スタートとゴールがつながっているか
bool EditMap::Start_Connect_Goal() {
    
    ColleagueNotifyer notifyer;
    BattleParameter param(10, 10, 10, 10, 10, 10);
    Enemy* dummy = new Enemy(_dungeon->getStart()->GetTilePos(), param, *_dungeon->getGoal().get(), notifyer, "blaver", "blaver","blaver", Character::EnemyAIType::Normal);

    std::vector<TiledVector> _pathToTarget;

    AstarPathFinder* _astar = new AstarPathFinder(*dummy, _pathToTarget, true);

    std::vector<TiledVector> _pathRef = _astar->GetPathToTarget(_dungeon->getStart()->GetTilePos(), _dungeon->getGoal()->GetTilePos(), 100);

    OBJECT_MGR->Remove(dummy);
    OBJECT_MGR->Refresh();

    if (_pathRef.size() == 0) {
        return false;
    }
    else {
        return true;
    }

}


int EditMap::CheckCost(std::string selectPanelCategory, std::string ObjectName) {

	int Cost = editObject.GetCost(selectPanelCategory, ObjectName);

	int IncreaseCost = editObject.GetIncreaseCost(selectPanelCategory, ObjectName);

	int ObjectNum = CheckObjectNum(selectPanelCategory, ObjectName);


	for (int i = 0; i < ObjectNum; i++) {
		Cost = Cost * (1.0 + ((double)IncreaseCost) / 100.0);
	}

	int first = Cost % 10;

	if (first >= 5) {
		Cost = Cost / 10;
		Cost += 1;
		Cost = Cost * 10;
	}
	else {
		Cost = Cost / 10;
		Cost = Cost * 10;
	}

	return Cost;


}

int EditMap::CheckObjectNum(std::string selectPanelCategory, std::string ObjectName) {

	if (selectPanelCategory == "MONSTER") {

		int num = 0;

		for (EditMap::addTileObject_Monster temp : addTiledObjectList_Monster) {
			if (temp.ObjectName == ObjectName) {
				num++;
			}
		}

		for (EditMap::addTileObject_Monster temp : RemainingObjectList_Monster) {
			if (temp.ObjectName == ObjectName) {
				num++;
			}
		}

		return num;

	}
	else if (selectPanelCategory == "TRAP") {
		
		int num = 0;

		for (EditMap::addTileObject temp : addTiledObjectList_Trap) {
			if (temp.ObjectName == ObjectName) {
				num++;
			}
		}

		for (EditMap::addTileObject temp : RemainingObjectList_Trap) {
			if (temp.ObjectName == ObjectName) {
				num++;
			}
		}

		return num;


	}
	else if (selectPanelCategory == "BLOCK") {

		int num = 0;

		for (EditMap::addTileObject temp : addTiledObjectList_Block) {
			if (temp.ObjectName == ObjectName) {
				num++;
			}
		}

		for (EditMap::addTileObject temp : RemainingObjectList_Block) {
			if (temp.ObjectName == ObjectName) {
				num++;
			}
		}

		return num;
		



	}

	return -1;


}



int EditMap::CheckPayOff() {

	int result = 0;

	for (EditMap::addTileObject_Monster temp : addTiledObjectList_Monster) {
		result += temp.Cost;
	}

	for (EditMap::addTileObject temp : addTiledObjectList_Trap) {
		result += temp.Cost;
	}

	for (EditMap::addTileObject temp : addTiledObjectList_Block) {
		result += temp.Cost;
	}

	return result;

}


void EditMap::ResetCost(std::string selectCategory, std::string ObjectName) {

	if (selectCategory == "MONSTER") {

		int num = 0;

		for (int i = 0; i < RemainingObjectList_Monster.size(); i++) {
			if (RemainingObjectList_Monster[i].ObjectName == ObjectName) {
				num++;
			}
		}

		for (int i = 0; i < addTiledObjectList_Monster.size();i++) {
			if (addTiledObjectList_Monster[i].ObjectName == ObjectName) {
				int Cost = editObject.GetCost(selectCategory, ObjectName);

				int IncreaseCost = editObject.GetIncreaseCost(selectCategory, ObjectName);

				for (int i = 0; i < num; i++) {
					Cost = Cost * (1.0 + ((double)IncreaseCost) / 100.0);
				}

				int first = Cost % 10;

				if (first >= 5) {
					Cost = Cost / 10;
					Cost += 1;
					Cost = Cost * 10;
				}
				else {
					Cost = Cost / 10;
					Cost = Cost * 10;
				}
				addTiledObjectList_Monster[i].Cost = Cost;
				num++;
			}
		}

	}
	else if (selectCategory == "TRAP") {

		int num = 0;

		for (int i = 0; i < RemainingObjectList_Trap.size(); i++) {
			if (RemainingObjectList_Trap[i].ObjectName == ObjectName) {
				num++;
			}
		}

		for (int i = 0; i < addTiledObjectList_Trap.size(); i++) {
			if (addTiledObjectList_Trap[i].ObjectName == ObjectName) {
				int Cost = editObject.GetCost(selectCategory, ObjectName);

				int IncreaseCost = editObject.GetIncreaseCost(selectCategory, ObjectName);

				for (int i = 0; i < num; i++) {
					Cost = Cost * (1.0 + ((double)IncreaseCost) / 100.0);
				}

				int first = Cost % 10;

				if (first >= 5) {
					Cost = Cost / 10;
					Cost += 1;
					Cost = Cost * 10;
				}
				else {
					Cost = Cost / 10;
					Cost = Cost * 10;
				}
				addTiledObjectList_Trap[i].Cost = Cost;
				num++;
			}
		}
	}
	else if (selectCategory == "BLOCK") {

		int num = 0;

		for (int i = 0; i < RemainingObjectList_Block.size(); i++) {
			if (RemainingObjectList_Block[i].ObjectName == ObjectName) {
				num++;
			}
		}

		for (int i = 0; i < addTiledObjectList_Block.size(); i++ ) {
			if (addTiledObjectList_Block[i].ObjectName == ObjectName) {
				int Cost = editObject.GetCost(selectCategory, ObjectName);

				int IncreaseCost = editObject.GetIncreaseCost(selectCategory, ObjectName);

				for (int i = 0; i < num; i++) {
					Cost = Cost * (1.0 + ((double)IncreaseCost) / 100.0);
				}

				int first = Cost % 10;

				if (first >= 5) {
					Cost = Cost / 10;
					Cost += 1;
					Cost = Cost * 10;
				}
				else {
					Cost = Cost / 10;
					Cost = Cost * 10;
				}
				addTiledObjectList_Block[i].Cost = Cost;
				num++;
			}
		}

	}

}


bool EditMap::PauseUpdate()
{

	PauseUI.SetDisplayMode(true);

	auto backColor = Color4(0, 0, 0, 0.9f);
	_black.SetDisplayMode(true);
	_black.SetBaseColor(backColor);

	if (KEYBOARD->ButtonDown(KeyInput::KeyType::KEY_ESCAPE)) {
		isPause = false;
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
			isPause = false;
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

void EditMap::PauseDraw()
{
}


//ラベルを取り払い、ラベル後続の値を読み取る
std::string EditMap::LoadLabeledElem(const std::string& label, const std::string& rawData)
{
	size_t it = rawData.find(label);
	std::string cutData = rawData.substr(it + label.length());

	size_t dataEnd = cutData.find("&");
	cutData = cutData.substr(0, dataEnd);
	return cutData;
}

//ラベル後続の値を書き換える
std::string EditMap::WriteLabeledElem(const std::string& label, const std::string& rawData, std::string writeData)
{
	size_t it = rawData.find(label);
	std::string cutData = rawData.substr(it + label.length());

	size_t dataEnd = cutData.find("&");
	cutData = cutData.substr(0, dataEnd);
	return cutData;
}


