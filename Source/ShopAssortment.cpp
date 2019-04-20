#include "ShopAssortment.h"
#include "cd_666s\Utility\CSVReader.h"
#include "cd_666s\Resources\AllResourceManager.h"
#include <fstream>
#include <string>
#include <sstream>
#include "DxLib.h"
#include "MoneyManager.h"



void ShopAssortment::setShopItemList() {

	shopItemBlockList.clear();
	shopItemTrapList.clear();
	shopItemMonsterList.clear();

	CSVReader reader;

	std::string fileName = Main::RESOURCEFILENAME + "csv/Edit/BLOCK.csv";

	std::vector<std::string> ShopItemArray;

	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, ShopItemArray, 1);

	int DataNum = 5;

	for (int j = 0; j < ShopItemArray.size(); j += DataNum) {
		std::shared_ptr<ShopItem_Block> shopItem = make_shared<ShopItem_Block>();

		shopItem->ObjectName = ShopItemArray[j];
		shopItem->isHaveing = false;

		shopItemBlockList.push_back(shopItem);
	}

	fileName = Main::RESOURCEFILENAME + "csv/Edit/TRAP.csv";

	DataNum = 5;

	ShopItemArray.clear();

	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, ShopItemArray, 1);

	for (int j = 0; j < ShopItemArray.size(); j += DataNum) {
		std::shared_ptr<ShopItem_Trap> shopItem = make_shared<ShopItem_Trap>();

		shopItem->ObjectName = ShopItemArray[j];
		shopItem->isHaveing = false;
		shopItem->level = -1;

		shopItemTrapList.push_back(shopItem);
	}

	fileName = Main::RESOURCEFILENAME + "csv/Edit/MONSTER.csv";

	DataNum = 2;

	ShopItemArray.clear();

	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, ShopItemArray, 1);

	for (int j = 0; j < ShopItemArray.size(); j += DataNum) {
		std::shared_ptr<ShopItem_Monster> shopItem = make_shared<ShopItem_Monster>();

		shopItem->ObjectName = ShopItemArray[j];
		shopItem->isHaveing = false;
		shopItem->level = -1;

		shopItemMonsterList.push_back(shopItem);
	}

	fileName = Main::RESOURCEFILENAME + "csv/Edit/InitHave.csv";

	ShopItemArray.clear();

	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, ShopItemArray, 1);

	for (int i = 0; i < ShopItemArray.size(); i += 2) {
		if (ShopItemArray[i + 1] == "BLOCK") {
			for (int j = 0; j < shopItemBlockList.size(); j++) {
				if (shopItemBlockList[j]->ObjectName == ShopItemArray[i]) {
					shopItemBlockList[j]->isHaveing = true;
				}
			}
		}
		else if (ShopItemArray[i + 1] == "TRAP") {
			for (int j = 0; j < shopItemTrapList.size(); j++) {
				if (shopItemTrapList[j]->ObjectName == ShopItemArray[i]) {
					shopItemTrapList[j]->isHaveing = true;
					shopItemTrapList[j]->level = 1;
				}
			}
		}
		else if (ShopItemArray[i + 1] == "MONSTER") {
			for (int j = 0; j < shopItemMonsterList.size(); j++) {
				if (shopItemMonsterList[j]->ObjectName == ShopItemArray[i]) {
					shopItemMonsterList[j]->isHaveing = true;
					shopItemMonsterList[j]->level = 1;
				}
			}
		}
	}

}



void ShopAssortment::BuyItem(std::shared_ptr<ShopPanel> shopPanel) { //ˆø”‚ÍshopItemList‚Ì“Y‚¦Žš‚Ì”Ô† –ß‚è’l‚Íw“ü¬Œ÷‚Å1AŽ¸”s‚Å0

	MoneyManager::getInstance()->decreaseMoney(shopPanel->GetPrice());

	ShopPanel::PanelCategory pc = shopPanel->GetShopPanelCategoryName();
	std::string objectName = shopPanel->GetShopPanelName();

	if (pc == ShopPanel::PanelCategory::BLOCK) {
		for (int i = 0; i < shopItemBlockList.size(); i++) {
			if (shopItemBlockList[i]->ObjectName == objectName) {
				shopItemBlockList[i]->isHaveing = true;
			}
		}
	}
	else if (pc == ShopPanel::PanelCategory::TRAP) {
		for (int i = 0; i < shopItemTrapList.size(); i++) {
			if (shopItemTrapList[i]->ObjectName == objectName) {
				shopItemTrapList[i]->isHaveing = true;
				shopItemTrapList[i]->level = 1;
			}
		}
	}
	else if (pc == ShopPanel::PanelCategory::MONSTER) {
		for (int i = 0; i < shopItemMonsterList.size(); i++) {
			if (shopItemMonsterList[i]->ObjectName == objectName) {
				shopItemMonsterList[i]->isHaveing = true;
				shopItemMonsterList[i]->level = 1;
			}
		}
	}

}


bool ShopAssortment::getCanBuy(std::string objectName, ShopPanel::PanelCategory pc) {

	if (pc == ShopPanel::PanelCategory::BLOCK) {
		for (int i = 0; i < shopItemBlockList.size(); i++) {
			if (shopItemBlockList[i]->ObjectName == objectName) {
				return !shopItemBlockList[i]->isHaveing;
			}
		}
	}
	else if (pc == ShopPanel::PanelCategory::TRAP) {
		for (int i = 0; i < shopItemTrapList.size(); i++) {
			if (shopItemTrapList[i]->ObjectName == objectName) {
				return !shopItemTrapList[i]->isHaveing;
			}
		}
	}
	else if (pc == ShopPanel::PanelCategory::MONSTER) {
		for (int i = 0; i < shopItemMonsterList.size(); i++) {
			if (shopItemMonsterList[i]->ObjectName == objectName) {
				return !shopItemMonsterList[i]->isHaveing;
			}
		}
	}

	return false;

}


bool ShopAssortment::getCanSetObject(std::string objectName, ShopPanel::PanelCategory pc) {

	if (pc == ShopPanel::PanelCategory::BLOCK) {
		for (int i = 0; i < shopItemBlockList.size(); i++) {
			if (shopItemBlockList[i]->ObjectName == objectName) {
				return shopItemBlockList[i]->isHaveing;
			}
		}
	}
	else if (pc == ShopPanel::PanelCategory::TRAP) {
		for (int i = 0; i < shopItemTrapList.size(); i++) {
			if (shopItemTrapList[i]->ObjectName == objectName) {
				return shopItemTrapList[i]->isHaveing;
			}
		}
	}
	else if (pc == ShopPanel::PanelCategory::MONSTER) {
		for (int i = 0; i < shopItemMonsterList.size(); i++) {
			if (shopItemMonsterList[i]->ObjectName == objectName) {
				return shopItemMonsterList[i]->isHaveing;
			}
		}
	}

	return false;

}

int ShopAssortment::getMonsterLevel(std::string objectName) {

	for (int i = 0; i < shopItemMonsterList.size(); i++) {
		if (shopItemMonsterList[i]->ObjectName == objectName) {
			return shopItemMonsterList[i]->level;
		}
	}

	return -1;

}


int ShopAssortment::getTrapLevel(std::string objectName) {

	for (int i = 0; i < shopItemTrapList.size(); i++) {
		if (shopItemTrapList[i]->ObjectName == objectName) {
			return shopItemTrapList[i]->level;
		}
	}

	return -1;

}

void ShopAssortment::EvolMonster(std::shared_ptr<PanelSettingObject> PanelObject) {

	MoneyManager::getInstance()->decreaseMoney(PanelObject->GetLevelUpCost());

	std::string objectName = PanelObject->getPanelObjectName();

	for (int i = 0; i < shopItemMonsterList.size(); i++) {
		if (shopItemMonsterList[i]->ObjectName == objectName) {
			shopItemMonsterList[i]->level++;
		}
	}

}


void ShopAssortment::AllAssortmentGet() {

	for (int i = 0; i < shopItemBlockList.size(); i++) {
		shopItemBlockList[i]->isHaveing = true;
	}

	for (int i = 0; i < shopItemTrapList.size(); i++) {
		shopItemTrapList[i]->isHaveing = true;
		shopItemTrapList[i]->level = 1;
	}
	for (int i = 0; i < shopItemMonsterList.size(); i++) {
		shopItemMonsterList[i]->isHaveing = true;
		shopItemMonsterList[i]->level = 1;
	}


}

std::vector<ShopAssortment::ShopItem_Block> ShopAssortment::GetShopItemBlockList()
{

	std::vector<ShopAssortment::ShopItem_Block> temp;

	temp.clear();

	for (int i = 0; i < shopItemBlockList.size(); i++) {
		temp.push_back(*shopItemBlockList[i]);
	}

	return temp;
}

std::vector<ShopAssortment::ShopItem_Trap> ShopAssortment::GetShopItemTrapList()
{
	std::vector<ShopAssortment::ShopItem_Trap> temp;

	temp.clear();

	for (int i = 0; i < shopItemTrapList.size(); i++) {
		temp.push_back(*shopItemTrapList[i]);
	}

	return temp;
}

std::vector<ShopAssortment::ShopItem_Monster> ShopAssortment::GetShopItemMonsterList()
{
	std::vector<ShopAssortment::ShopItem_Monster> temp;

	temp.clear();

	for (int i = 0; i < shopItemMonsterList.size(); i++) {
		temp.push_back(*shopItemMonsterList[i]);
	}

	return temp;
}

void ShopAssortment::LoadData(std::vector<ShopItem_Block> b, std::vector<ShopItem_Trap> t, std::vector<ShopItem_Monster> m)
{
	shopItemBlockList.clear();
	shopItemTrapList.clear();
	shopItemMonsterList.clear();

	for (int i = 0; i < b.size(); i++) {
		std::shared_ptr<ShopItem_Block> temp = std::make_shared<ShopItem_Block>();

		temp->isHaveing = b[i].isHaveing;
		temp->ObjectName = b[i].ObjectName;

		shopItemBlockList.push_back(temp);
	}

	for (int i = 0; i < t.size(); i++) {
		std::shared_ptr<ShopItem_Trap> temp = std::make_shared<ShopItem_Trap>();

		temp->isHaveing = t[i].isHaveing;
		temp->ObjectName = t[i].ObjectName;
		temp->level = t[i].level;

		shopItemTrapList.push_back(temp);
	}

	for (int i = 0; i < m.size(); i++) {
		std::shared_ptr<ShopItem_Monster> temp = std::make_shared<ShopItem_Monster>();

		temp->isHaveing = m[i].isHaveing;
		temp->ObjectName = m[i].ObjectName;
		temp->level = m[i].level;

		shopItemMonsterList.push_back(temp);
	}

}


