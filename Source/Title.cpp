#include "Title.h"
#include "FunctionTransScene.h"
#include "cd_666s/Resources/ResourceManager.h"
#include "ShopAssortment.h"
#include "MoneyManager.h"
#include "TutorialPanel.h"
#include "Information_SaveData.h"
#include "WorldMap.h"

#include "Main.h"

Title::Title() 
    : class_name("title")
    , title(Main::RESOURCEFILENAME + "resource/title.png")
    , _bgm(Main::RESOURCEFILENAME + "resource/sound/Area1.ogg",true)
{
    _functions.reserve(20);
    title.SetPosition(Vector2D(0, 0));

    _bgm.Play();
    _bgm.SetVolume(200);

	_cancelSE.Load(Main::RESOURCEFILENAME + "resource/sound/cancelA.wav");
	_cancelSE.SetVolume(200);

	_decideSE.Load(Main::RESOURCEFILENAME + "resource/sound/decide.wav");
	_decideSE.SetVolume(200);

}


Title::~Title()
{
    _functions.clear();
    _bgm.Stop();
}

void Title::Init()
{
}

SceneBase * Title::Update()
{
    //nowUI->Update();

    mouse_status = MouseManager::GetMouseStatus(mouse_status);

    return this;
}

SceneBase * Title::Update(UIManager _ui)
{
    mouse_status = MouseManager::GetMouseStatus(mouse_status);

    _functions.resize(0);

    std::string _ui_name;

    _ui.Update(_functions,_ui_name);

    for (auto f : _functions) {
        if (f.find("_B") != std::string::npos) {
			_decideSE.Play();
            return FunctionTransScene::GetSceneBase("edit");
        }
        else if (f.find("MOVE") != std::string::npos) {
			_decideSE.Play();
			ShopAssortment::getInstance()->setShopItemList();
			MoneyManager::getInstance()->InitMoney();
			Tutorial::getInstance()->Init();
			Information_SaveData::GetInstance()->Init();
            return FunctionTransScene::GetSceneBase("edit");
        }
        else if (f.find("QUIT") != std::string::npos) {
			_decideSE.Play();
            return nullptr;
		}
		else if(f.find("LOAD") != std::string::npos){
			ShopAssortment::getInstance()->setShopItemList();
			MoneyManager::getInstance()->InitMoney();
			Tutorial::getInstance()->Init();
			Information_SaveData::GetInstance()->Init();

			int a, p;
			bool is = Information_SaveData::GetInstance()->LoadProcesses(&a, &p);
			if (is) {
				_decideSE.Play();
				return new WorldMap(a, p);
			}
			if (!_cancelSE.IsPlaying()) {
				_cancelSE.Play();
			}
			
		}
    }

	if (Main::DEBUGFLAG) {
		if (KEYBOARD->ButtonDown(KeyInput::KeyType::KEY_F1)) {
			_decideSE.Play();
			ShopAssortment::getInstance()->setShopItemList();
			MoneyManager::getInstance()->InitMoney();
			Tutorial::getInstance()->Init();
			Tutorial::getInstance()->NotTutorial();
			Information_SaveData::GetInstance()->SetAllStageActivation();
			return FunctionTransScene::GetSceneBase("world_map");
		}
	}


	if (!_bgm.IsPlaying()) {
		_bgm.Play();
		_bgm.SetVolume(200);
	}
        


    return this;
}

void Title::Draw()
{
    title.Draw();
}

SceneBase * Title::UpdateFunctions()
{
    return nullptr;
}
