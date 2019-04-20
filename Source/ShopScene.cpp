#include "cd_666s/InputManager/MouseInput.h"
#include "ShopScene.h"
#include "ShopAssortment.h"
#include "DxLib.h"
#include "cd_666s/DebugDraw.h"
#include "WorldMap.h"
#include "cd_666s\Utility\CSVReader.h"
#include "cd_666s\Resources\AllResourceManager.h"
#include <regex>
#include "ShopPanel.h"
#include "MoneyManager.h"
#include "TutorialPanel.h"

ShopScene::ShopScene()
	:_bgm(Main::RESOURCEFILENAME + "resource/sound/shop.wav", true){

	_bgm.Play();

}

ShopScene::ShopScene(std::string shopNum, int pointNum) 
	:_bgm(Main::RESOURCEFILENAME + "resource/sound/shop.wav", true) {

	_bgm.Play();

	this->class_name = "shop";

	std::regex regex("shop");
	this->shopNum = std::regex_replace(shopNum, regex, "");

	Init();

	returnPointNum = pointNum;

}

void ShopScene::Init() {

	std::vector<Vector2D> shopPanelPosition;

	shopPanelPosition.push_back(Vector2D(270, 130));
	shopPanelPosition.push_back(Vector2D(540, 130));
	shopPanelPosition.push_back(Vector2D(810, 130));
	shopPanelPosition.push_back(Vector2D(270, 330));
	shopPanelPosition.push_back(Vector2D(540, 330));
	shopPanelPosition.push_back(Vector2D(810, 330));

	CSVReader reader;

	std::string fileName = Main::RESOURCEFILENAME + "csv/ShopItem/shop_Area" + this->shopNum + ".csv";
	std::vector<std::string> shopItemArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, shopItemArray, 1);


	for (int i = 0; i < shopItemArray.size(); i += 4) {
		std::shared_ptr<ShopPanel> sp = std::make_shared<ShopPanel>(shopItemArray[i + 1], shopItemArray[i], shopPanelPosition[i / 4], std::stoi(shopItemArray[i + 2]), shopItemArray[i + 3]);
		shopPanelList.push_back(sp);
	}

	_background.Load(Main::RESOURCEFILENAME + "resource/graph/shop/shop_background.png");
	_background.SetPosition(Vector2D(0, 0));
	_background.SetPriority(Sprite::Priority::BACKGROUND);

	_exitButton.Load(Main::RESOURCEFILENAME + "resource/graph/shop/exitShop.png");
	_exitButton.SetPosition(Vector2D(1050, 20));
	_exitButton.SetPriority(Sprite::Priority::UI);

	_moneyBox.Load(Main::RESOURCEFILENAME + "resource/graph/shop/MoneyBox.png");
	_moneyBox.SetPosition(Vector2D(750, 20));
	_moneyBox.SetPriority(Sprite::Priority::UI);

	_shopTable_Up.Load(Main::RESOURCEFILENAME + "resource/graph/shop/shopTable.png");
	_shopTable_Up.SetPosition(Vector2D(150, 250));
	_shopTable_Up.SetPriority(Sprite::Priority::UI);

	_shopTable_Down.Load(Main::RESOURCEFILENAME + "resource/graph/shop/shopTable.png");
	_shopTable_Down.SetPosition(Vector2D(150, 450));
	_shopTable_Down.SetPriority(Sprite::Priority::UI);



	_selectFrame.Load(Main::RESOURCEFILENAME + "resource/graph/shop/Marker.png");
	_selectFrame.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_selectFrame.SetPosition(Vector2D(0, 0));
	_selectFrame.SetScale(Vector2D((128.0 + 24) / _selectFrame.GetSize()._x, (128.0 + 24) / _selectFrame.GetSize()._y));
	_selectFrame.SetDisplayMode(false);

	objectTextPanel = std::make_shared<ObjectTextPanel>(class_name);
	objectTextPanel->isInShop = true;
	objectTextPanel->isOutShop = false;
	objectTextPanel->SetShopFace(std::stoi(shopNum));

	_decideSE.Load(Main::RESOURCEFILENAME + "resource/sound/decide.wav");
	_decideSE.SetVolume(200);

	_buybycoinD.Load(Main::RESOURCEFILENAME + "resource/sound/buy by coinD.wav");
	_buybycoinD.SetVolume(200);


}



ShopScene::~ShopScene() {

}

SceneBase* ShopScene::Update(UIManager _ui)
{
	//_ui.Update();
	return Update();
}


SceneBase* ShopScene::Update() {

	if (tutorialPanel == nullptr) {
		if (!Tutorial::getInstance()->isLookingTutorial("Shop")) {
			tutorialPanel = new TutorialPanel("Shop");
		}

	}

	if (tutorialPanel != nullptr) {
		tutorialPanel->Update();
		if (!tutorialPanel->GetisTutorial()) {
			Tutorial::getInstance()->LookTutorial(tutorialPanel->GetTimingName());
			delete(tutorialPanel);
			tutorialPanel = nullptr;
		}
		return this;
	}

	if (ClickCheck(_exitButton.GetPosition(), _exitButton.GetTexturePtr().lock()->GetBaseSize())) {
		objectTextPanel->isInShop = false;
		objectTextPanel->isOutShop = true;
		ShopOutCnt = 0;
		_decideSE.Play();
	}

	if (objectTextPanel->isOutShop) {
		ShopOutCnt++;
		if (ShopOutCnt > 80) {
			return new WorldMap(std::stoi(shopNum), returnPointNum);
		}
	}

	for (auto shopPanel : shopPanelList) {
		shopPanel->Update();
		if (shopPanel->ClickCheck()) {
			objectTextPanel->isInShop = false;
			objectTextPanel->isOutShop = false;
			_decideSE.Play();
			setSelectShopPanel(shopPanel);
			objectTextPanel->SetMessage(shopPanel);
		}
	}

	if (objectTextPanel->BuyCheck()) {
		_buybycoinD.Play();
		ShopAssortment::getInstance()->BuyItem(selectShopPanel);
		for (auto shopPanel : shopPanelList) {
			shopPanel->ResetCanBuy();
		}
		objectTextPanel->isInShop = false;
		objectTextPanel->isOutShop = false;
		objectTextPanel->SetMessage(selectShopPanel);
	}

	/*
	if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_R)) {
		MoneyManager::getInstance()->increaseMoney(1000);
	}
	*/

	return this;

}


void ShopScene::Draw() {

	if (tutorialPanel != nullptr) {
		return;
	}

	clsDx();

	for (auto shopPanel : shopPanelList) {
		shopPanel->Draw();
	}

	objectTextPanel->Draw();

	int money = MoneyManager::getInstance()->getMoney();

	int digit = 9 - std::to_string(money).length();
	std::string digit_s = "";

	for (int i = 0; i < digit; i++) {
		digit_s += " ";
	}

	Debug::DrawString(Vector2D(800, 40), digit_s + std::to_string(money) + "Ｍ", Color4(0, 0, 0, 1), 32);


}




void ShopScene::setSelectShopPanel(std::shared_ptr<ShopPanel> selectShopPanel) {

	this->selectShopPanel = selectShopPanel;

	_selectFrame.SetPosition(selectShopPanel->GetPotision() - Vector2D(12, 12));
	_selectFrame.SetDisplayMode(true);

}


bool ShopScene::ClickCheck(Vector2D pos, Vector2D size) {

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



