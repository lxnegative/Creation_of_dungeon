#include "ObjectTextPanel.h"
#include "cd_666s\Utility\CSVReader.h"
#include "cd_666s\Resources\AllResourceManager.h"
#include "cd_666s/DebugDraw.h"
#include <regex>
#include "cd_666s/InputManager/MouseInput.h"
#include "MoneyManager.h"
#include "ShopAssortment.h"

ObjectTextPanel::ObjectTextPanel()
{



}


ObjectTextPanel::ObjectTextPanel(std::string className) {

	if (className == "shop") {

		panelType = ObjectTextPanel::PanelType::Shop;

	}
	else if (className == "worldmap") {

		panelType = ObjectTextPanel::PanelType::WorldMap;

	}
	else {

		panelType = ObjectTextPanel::PanelType::Edit;

	}

	Init();


}

ObjectTextPanel::~ObjectTextPanel()
{



}


void ObjectTextPanel::Init() {

	_cancelSE.Load(Main::RESOURCEFILENAME + "resource/sound/cancelA.wav");
	_cancelSE.SetVolume(200);

	if (panelType == ObjectTextPanel::PanelType::Shop) {
		Init_Shop();
	}
	else if (panelType == ObjectTextPanel::PanelType::Edit || panelType == ObjectTextPanel::PanelType::WorldMap) {
		Init_Edit();
	}

}

void ObjectTextPanel::Init_Shop() {

	_messageWindow.Load(Main::RESOURCEFILENAME + "resource/graph/ui/message_window.png");
	_messageWindow.SetPosition(Vector2D(150, 540));
	_messageWindow.SetPriority(Sprite::Priority::UI);

	_buyButton.Load(Main::RESOURCEFILENAME + "resource/graph/shop/buyButton.png");
	_buyButton.SetPosition(Vector2D(775, 650));
	_buyButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_buyButton.SetDisplayMode(false);

	_ObjectImage.SetPosition(Vector2D(170 + 6, 565 + 6));
	_ObjectImage.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_ObjectImage.SetDisplayMode(false);

	ReadMessageText();

	messageTextCategory = ShopPanel::PanelCategory::MONSTER;

	messageTextArrayNum = -1;

	canBuy = false;
	existBuy = false;
	canLevelUp = false;
	existLevelUp = false;

	isInShop = false;
	isOutShop = false;
}

void ObjectTextPanel::Init_Edit() {

	_messageWindow.Load(Main::RESOURCEFILENAME + "resource/graph/ui/message_window.png");
	_messageWindow.SetPosition(Vector2D(20, 520));
	_messageWindow.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	_messageWindow.SetDisplayMode(true);

	_LevelUpButtonImage = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/Level_Up.png");
	_DoNotBuyButtonImage = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/EvolButtonDoNot.png");
	_LevelMaxButtonImage = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/ui/Level_Max.png");

	_buyButton.SetResource(_LevelUpButtonImage);
	_buyButton.SetPosition(Vector2D(645, 630));
	_buyButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 2);
	_buyButton.SetDisplayMode(false);

	_ObjectImage.SetPosition(Vector2D(40 + 6, 545 + 6));
	_ObjectImage.SetPriority(static_cast<int>(Sprite::Priority::UI) + 2);
	_ObjectImage.SetDisplayMode(false);

	ReadMessageText();

	messageTextCategory = ShopPanel::PanelCategory::MONSTER;

	messageTextArrayNum = -1;

	canBuy = false;
	existBuy = false;
	canLevelUp = false;
	existLevelUp = false;

	Level = -1;
	LevelUpCost = -1;
	putCost = -1;

	isInShop = false;
	isOutShop = false;

}


void ObjectTextPanel::Update() {

}

void ObjectTextPanel::Draw() {

	if (messageTextArrayNum >= 0 && !isInShop && !isOutShop) {

		std::regex regex("\\\\n");
		std::string itemText = std::regex_replace(messageText[messageTextCategory][messageTextArrayNum]->ItemText, regex, "\n");

		std::string displayName = messageText[messageTextCategory][messageTextArrayNum]->DisplayName;

		int textNum = 0;
		int smalltextNum = 0;
		std::string temp = displayName;

		while (!temp.empty()) {
			if (IsDBCSLeadByte(temp[0]) == 0)
			{
				temp.erase(0, 1);
				smalltextNum++;
			}
			else
			{
				temp.erase(0, 2);
				textNum++;
			}
		}

		Debug::DrawString(_messageWindow.GetPosition() + Vector2D(170, 20), displayName, Color4(0, 0, 0, 1), 24);
		Debug::DrawLine(_messageWindow.GetPosition() + Vector2D(170, 45), _messageWindow.GetPosition() + Vector2D(170 + textNum * 25 + smalltextNum * 13, 45), Color4(0, 0, 0, 1));
		Debug::DrawString(_messageWindow.GetPosition() + Vector2D(190, 70), itemText, Color4(0, 0, 0, 1), 22);


	}

	if (isInShop) {
		_buyButton.SetDisplayMode(false);
		_ObjectImage.SetResource(shopFace);
		_ObjectImage.SetDisplayMode(true);
		_ObjectImage.SetScale(Vector2D(1.97, 1.97));
		_ObjectImage.SetPosition(Vector2D(172, 567));
		Debug::DrawString(_messageWindow.GetPosition() + Vector2D(190, 80), ShopInText, Color4(0, 0, 0, 1), 24);
	}

	if (isOutShop) {
		_buyButton.SetDisplayMode(false);
		_ObjectImage.SetResource(shopFace);
		_ObjectImage.SetDisplayMode(true);
		_ObjectImage.SetScale(Vector2D(1.97, 1.97));
		_ObjectImage.SetPosition(Vector2D(172, 567));
		Debug::DrawString(_messageWindow.GetPosition() + Vector2D(190, 80), ShopOutText, Color4(0, 0, 0, 1), 24);
	}

	if ((panelType == ObjectTextPanel::PanelType::Edit || panelType == ObjectTextPanel::PanelType::WorldMap)) {

		if (messageTextCategory == ShopPanel::PanelCategory::MONSTER) {
			if (!(Level < 0)) {

				if (!(LevelUpCost < 0)) {

					int textNum = 0;
					int smalltextNum = 0;
					std::string temp = "NextLevel：";

					while (!temp.empty()) {
						if (IsDBCSLeadByte(temp[0]) == 0)
						{
							temp.erase(0, 1);
							smalltextNum++;
						}
						else
						{
							temp.erase(0, 2);
							textNum++;
						}
					}

					Debug::DrawString(_messageWindow.GetPosition() + Vector2D(650, 80), "NextLevel：", Color4(0, 0, 0, 1), 18);
					Debug::DrawLine(_messageWindow.GetPosition() + Vector2D(650, 99), _messageWindow.GetPosition() + Vector2D(650 + textNum * 18 + smalltextNum * 10, 99), Color4(0, 0, 0, 1));

					textNum = 0;
					smalltextNum = 0;
					temp = std::to_string(LevelUpCost) + "Ｍ";

					while (!temp.empty()) {
						if (IsDBCSLeadByte(temp[0]) == 0)
						{
							temp.erase(0, 1);
							smalltextNum++;
						}
						else
						{
							temp.erase(0, 2);
							textNum++;
						}
					}

					Debug::DrawString(_messageWindow.GetPosition() + Vector2D(760, 80), std::to_string(LevelUpCost) + "Ｍ", Color4(1, 0, 0, 1), 18);
					Debug::DrawLine(_messageWindow.GetPosition() + Vector2D(760 - 5, 99), _messageWindow.GetPosition() + Vector2D(760 + textNum * 18 + smalltextNum * 10, 99), Color4(1, 0, 0, 1));

				}
				else {
					//Debug::DrawString(_messageWindow.GetPosition() + Vector2D(600,80), "Level Max", Color4(0, 0, 0, 1), 18);
				}

			}

			
		}

		if (!(putCost < 0)) {
			Debug::DrawString(_messageWindow.GetPosition() + Vector2D(370, 20), "コスト：" , Color4(1, 0, 0, 1), 20);
			Debug::DrawString(_messageWindow.GetPosition() + Vector2D(458, 20), std::to_string(putCost) + "Ｍ", Color4(0, 0, 0, 1), 20);
		}

	}

}



void ObjectTextPanel::ReadMessageText() {

	CSVReader reader;

	std::string fileName = Main::RESOURCEFILENAME + "csv/text/block.csv";
	std::vector<std::string> messageArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, messageArray, 1);

	messageText.clear();
	messageText.resize(3);

	std::vector<std::shared_ptr<MessageText>> tempMessage;
	tempMessage.clear();
	if (!messageArray.empty() && !messageArray.front().empty()) {
		for (int i = 0; i < messageArray.size(); i += 4) {

			std::shared_ptr<MessageText> mt = std::make_shared<MessageText>();

			mt->ObjectName = messageArray[i];
			mt->ResourceName = messageArray[i + 1];
			mt->DisplayName = messageArray[i + 2];
			mt->ItemText = messageArray[i + 3];

			tempMessage.push_back(mt);

		}
	}

	messageText[ShopPanel::PanelCategory::BLOCK] = tempMessage;


	editObject.ResetLevel();

	std::vector<std::string> _array;
	_array = editObject.getEditOblectList("MONSTER");

	int DataNum = 6;

	tempMessage.clear();
	messageArray.clear();
	fileName = Main::RESOURCEFILENAME + "csv/text/monster.csv";
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, messageArray, 1);
	if (!messageArray.empty() && !messageArray.front().empty()) {
		for (int i = 0; i < messageArray.size(); i += 2) {

			std::shared_ptr<MessageText> mt = std::make_shared<MessageText>();

			mt->ObjectName = messageArray[i];

			if (ShopAssortment::getInstance()->getCanSetObject(mt->ObjectName, ShopPanel::PanelCategory::MONSTER)) {
				for (int w = 0; w < _array.size(); w += DataNum) {
					if (mt->ObjectName == _array[w]) {
						mt->ResourceName = _array[w + 1];
						mt->DisplayName = _array[w + 2];
					}
				}
			}
			else {

				//ResourceNameを検索
				std::string TempFileName = Main::RESOURCEFILENAME + "csv/Edit/MONSTER_DATA/" + mt->ObjectName + "/" + mt->ObjectName + ".csv";

				std::vector<std::string> dataArray;
				CSVReader reader;
				reader.Read(RESOURCE_TABLE->GetFolderPath() + TempFileName, dataArray, 1);

				const int parameterNum = 11;

				mt->ResourceName = dataArray[parameterNum - 3];
				mt->DisplayName = dataArray[parameterNum - 2];
			}

			mt->ItemText = messageArray[i + 1];

			tempMessage.push_back(mt);

		}
	}

	messageText[ShopPanel::PanelCategory::MONSTER] = tempMessage;


	tempMessage.clear();
	messageArray.clear();
	fileName = Main::RESOURCEFILENAME + "csv/text/trap.csv";
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, messageArray, 1);
	if (!messageArray.empty() && !messageArray.front().empty()) {
		for (int i = 0; i < messageArray.size(); i += 4) {

			std::shared_ptr<MessageText> mt = std::make_shared<MessageText>();

			mt->ObjectName = messageArray[i];
			mt->ResourceName = messageArray[i + 1];
			mt->DisplayName = messageArray[i + 2];
			mt->ItemText = messageArray[i + 3];

			tempMessage.push_back(mt);

		}
	}

	messageText[ShopPanel::PanelCategory::TRAP] = tempMessage;


}

void ObjectTextPanel::SetMessage(std::shared_ptr<ShopPanel> selectShopPanel) {

	canBuy = false;
	existBuy = false;

	messageTextCategory = selectShopPanel->GetShopPanelCategoryName();
	existBuy = selectShopPanel->GetCanBuy();

	_ObjectImage.Load(selectShopPanel->GetShopPanelImagePath());
	_ObjectImage.SetScale(Vector2D((128 - 12) / _ObjectImage.GetSize()._x, (128 - 12) / _ObjectImage.GetSize()._y));
	_ObjectImage.SetPosition(Vector2D(170 + 6, 565 + 6));
	_ObjectImage.SetDisplayMode(true);

	if (existBuy && MoneyManager::getInstance()->getMoney() >= selectShopPanel->GetPrice()) {
		canBuy = true;
		_buyButton.SetDisplayMode(true);
	}
	else if (existBuy) {
		canBuy = false;
		_buyButton.SetDisplayMode(true);
	}
	else {
		canBuy = false;
		_buyButton.SetDisplayMode(false);
	}



	for (int i = 0; i < messageText[messageTextCategory].size(); i++) {
		if (messageText[messageTextCategory][i]->ObjectName == selectShopPanel->GetShopPanelName()) {

			if (messageTextCategory == ShopPanel::PanelCategory::MONSTER) {
				editObject.ResetLevel();

				std::vector<std::string> _array;
				_array = editObject.getEditOblectList("MONSTER");

				int DataNum = 6;

				if (ShopAssortment::getInstance()->getCanSetObject(messageText[messageTextCategory][i]->ObjectName, ShopPanel::PanelCategory::MONSTER)) {
					for (int w = 0; w < _array.size(); w += DataNum) {
						if (messageText[messageTextCategory][i]->ObjectName == _array[w]) {
							messageText[messageTextCategory][i]->ResourceName = _array[w + 1];
							messageText[messageTextCategory][i]->DisplayName = _array[w + 2];
						}
					}
				}
				else {

					//ResourceNameを検索
					std::string TempFileName = Main::RESOURCEFILENAME + "csv/Edit/MONSTER_DATA/" + messageText[messageTextCategory][i]->ObjectName + "/" + messageText[messageTextCategory][i]->ObjectName + ".csv";

					std::vector<std::string> dataArray;
					CSVReader reader;
					reader.Read(RESOURCE_TABLE->GetFolderPath() + TempFileName, dataArray, 1);

					const int parameterNum = 11;

					messageText[messageTextCategory][i]->ResourceName = dataArray[parameterNum - 3];
					messageText[messageTextCategory][i]->DisplayName = dataArray[parameterNum - 2];
				}
			}

			messageTextArrayNum = i;
			return;
		}
		messageTextArrayNum = -1;
	}
	messageTextArrayNum = -1;

}

void ObjectTextPanel::SetMessage(std::shared_ptr<PanelSettingObject> selectPanel) {

	canLevelUp = false;
	existLevelUp = false;

	messageTextCategory = selectPanel->GetPanelCategory();
	existLevelUp = selectPanel->GetCanLevelUp();

	_ObjectImage.Load(selectPanel->GetPanelGraphPath());
	_ObjectImage.SetScale(Vector2D((128 - 12) / _ObjectImage.GetSize()._x, (128 - 12) / _ObjectImage.GetSize()._y));
	_ObjectImage.SetDisplayMode(true);

	Level = selectPanel->getLevel();
	LevelUpCost = selectPanel->GetLevelUpCost();

	if (existLevelUp && MoneyManager::getInstance()->getMoney() >= selectPanel->GetLevelUpCost()) {
		canLevelUp = true;
		_buyButton.SetResource(_LevelUpButtonImage);
		_buyButton.SetDisplayMode(true);
	}
	else if (existLevelUp) {
		canLevelUp = false;
		_buyButton.SetResource(_DoNotBuyButtonImage);
		_buyButton.SetDisplayMode(true);
	}
	else {
		canLevelUp = false;
		if (messageTextCategory == ShopPanel::PanelCategory::MONSTER) {
			_buyButton.SetDisplayMode(true);
			_buyButton.SetResource(_LevelMaxButtonImage);
		}
		else {
			_buyButton.SetDisplayMode(false);
		}
	}

	for (int i = 0; i < messageText[messageTextCategory].size(); i++) {
		if (messageText[messageTextCategory][i]->ObjectName == selectPanel->getPanelObjectName()) {

			if (messageTextCategory == ShopPanel::PanelCategory::MONSTER) {
				editObject.ResetLevel();

				std::vector<std::string> _array;
				_array = editObject.getEditOblectList("MONSTER");

				int DataNum = 6;

				if (ShopAssortment::getInstance()->getCanSetObject(messageText[messageTextCategory][i]->ObjectName, ShopPanel::PanelCategory::MONSTER)) {
					for (int w = 0; w < _array.size(); w += DataNum) {
						if (messageText[messageTextCategory][i]->ObjectName == _array[w]) {
							messageText[messageTextCategory][i]->ResourceName = _array[w + 1];
							messageText[messageTextCategory][i]->DisplayName = _array[w + 2];
						}
					}
				}
				else {

					//ResourceNameを検索
					std::string TempFileName = Main::RESOURCEFILENAME + "csv/Edit/MONSTER_DATA/" + messageText[messageTextCategory][i]->ObjectName + "/" + messageText[messageTextCategory][i]->ObjectName + ".csv";

					std::vector<std::string> dataArray;
					CSVReader reader;
					reader.Read(RESOURCE_TABLE->GetFolderPath() + TempFileName, dataArray, 1);

					const int parameterNum = 11;

					messageText[messageTextCategory][i]->ResourceName = dataArray[parameterNum - 3];
					messageText[messageTextCategory][i]->DisplayName = dataArray[parameterNum - 2];
				}
			}

			

			messageTextArrayNum = i;
			return;
		}
		messageTextArrayNum = -1;
	}
	messageTextArrayNum = -1;



}

void ObjectTextPanel::SetMessage(std::shared_ptr<PanelSettingObject> selectPanel, int cost, int payoffCost) {

	putCost = cost;

	canLevelUp = false;
	existLevelUp = false;

	messageTextCategory = selectPanel->GetPanelCategory();
	existLevelUp = selectPanel->GetCanLevelUp();

	_ObjectImage.Load(selectPanel->GetPanelGraphPath());
	_ObjectImage.SetScale(Vector2D((128 - 12) / _ObjectImage.GetSize()._x, (128 - 12) / _ObjectImage.GetSize()._y));
	_ObjectImage.SetDisplayMode(true);

	Level = selectPanel->getLevel();
	LevelUpCost = selectPanel->GetLevelUpCost();

	if (existLevelUp && MoneyManager::getInstance()->getMoney() - payoffCost >= selectPanel->GetLevelUpCost()) {
		canLevelUp = true;
		_buyButton.SetResource(_LevelUpButtonImage);
		_buyButton.SetDisplayMode(true);
	}
	else if (existLevelUp) {
		canLevelUp = false;
		_buyButton.SetResource(_DoNotBuyButtonImage);
		_buyButton.SetDisplayMode(true);
	}
	else {
		canLevelUp = false;
		if (messageTextCategory == ShopPanel::PanelCategory::MONSTER) {
			_buyButton.SetDisplayMode(true);
			_buyButton.SetResource(_LevelMaxButtonImage);
		}
		else {
			_buyButton.SetDisplayMode(false);
		}
	}

	for (int i = 0; i < messageText[messageTextCategory].size(); i++) {
		if (messageText[messageTextCategory][i]->ObjectName == selectPanel->getPanelObjectName()) {

			if (messageTextCategory == ShopPanel::PanelCategory::MONSTER) {
				editObject.ResetLevel();

				std::vector<std::string> _array;
				_array = editObject.getEditOblectList("MONSTER");

				int DataNum = 6;

				if (ShopAssortment::getInstance()->getCanSetObject(messageText[messageTextCategory][i]->ObjectName, ShopPanel::PanelCategory::MONSTER)) {
					for (int w = 0; w < _array.size(); w += DataNum) {
						if (messageText[messageTextCategory][i]->ObjectName == _array[w]) {
							messageText[messageTextCategory][i]->ResourceName = _array[w + 1];
							messageText[messageTextCategory][i]->DisplayName = _array[w + 2];
						}
					}
				}
				else {

					//ResourceNameを検索
					std::string TempFileName = Main::RESOURCEFILENAME + "csv/Edit/MONSTER_DATA/" + messageText[messageTextCategory][i]->ObjectName + "/" + messageText[messageTextCategory][i]->ObjectName + ".csv";

					std::vector<std::string> dataArray;
					CSVReader reader;
					reader.Read(RESOURCE_TABLE->GetFolderPath() + TempFileName, dataArray, 1);

					const int parameterNum = 11;

					messageText[messageTextCategory][i]->ResourceName = dataArray[parameterNum - 3];
					messageText[messageTextCategory][i]->DisplayName = dataArray[parameterNum - 2];
				}
			}

			messageTextArrayNum = i;
			return;
		}
		messageTextArrayNum = -1;
	}
	messageTextArrayNum = -1;



}

void ObjectTextPanel::ResetMessage() {

	messageTextArrayNum = -1;

	canBuy = false;
	existBuy = false;
	canLevelUp = false;
	existLevelUp = false;
	_ObjectImage.SetDisplayMode(false);
	_buyButton.SetDisplayMode(false);

	Level = -1;
	LevelUpCost = -1;
	putCost = -1;
}


bool ObjectTextPanel::BuyCheck() {

	if (!existBuy)
		return false;

	if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
		return false;

	Vector2D pos = _buyButton.GetPosition();
	Vector2D size = _buyButton.GetTexturePtr().lock()->GetBaseSize();

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

	if (!canBuy) {
		_cancelSE.Play();
		return false;
	}

	return true;

}

bool ObjectTextPanel::EvolCheck() {

	if (!existLevelUp)
		return false;

	if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
		return false;

	Vector2D pos = _buyButton.GetPosition();
	Vector2D size = _buyButton.GetTexturePtr().lock()->GetBaseSize();

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

	if (!canLevelUp) {
		_cancelSE.Play();
		return false;
	}

	return true;

}

void ObjectTextPanel::SetActive(bool flag)
{

	_messageWindow.SetDisplayMode(flag);
	_ObjectImage.SetDisplayMode(flag);

	ResetMessage();

}

void ObjectTextPanel::SetShopFace(int num)
{

	CSVReader reader;

	std::string fileName = Main::RESOURCEFILENAME + "csv/ShopItem/ShopOwner.csv";
	std::vector<std::string> shopArray;
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, shopArray, 1);

	for (int i = 0; i < shopArray.size(); i += 4) {
		if (std::stoi(shopArray[i]) == num) {
			shopFace = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/" + shopArray[i + 1] + ".png");
			ShopInText = shopArray[i + 2];
			ShopOutText = shopArray[i + 3];
		}
	}


}



