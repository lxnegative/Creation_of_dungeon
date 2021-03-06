#include <string>
#include <vector>
#include <memory>
#include "ShopPanel.h"
#include "PanelSettingObject.h"

#pragma once


class ShopAssortment {

public:

	//使う構造体を以下に
	struct ShopItem_Block {   //店の売り物の情報

		std::string ObjectName;   //オブジェクト名
		bool isHaveing;             //所持しているかどうか   trueで所持している（購入済み）

	};

	struct ShopItem_Trap {   //店の売り物の情報

		std::string ObjectName;   //オブジェクト名
		bool isHaveing;             //所持しているかどうか   trueで所持している（購入済み）
		int level;

	};

	struct ShopItem_Monster {   //店の売り物の情報

		std::string ObjectName;   //オブジェクト名
		bool isHaveing;             //所持しているかどうか   trueで所持している（購入済み）
		int level;

	};

    ShopAssortment(const ShopAssortment&) = delete;
    ShopAssortment& operator=(const ShopAssortment&) = delete;
    ShopAssortment(ShopAssortment&&) = delete;
    ShopAssortment& operator=(ShopAssortment&&) = delete;

    static ShopAssortment* getInstance() {
        static ShopAssortment inst;
        return &inst;
    }

    void setShopItemList();

	bool getCanBuy(std::string objectName, ShopPanel::PanelCategory pc);
	bool getCanSetObject(std::string objectName, ShopPanel::PanelCategory pc);

	int getMonsterLevel(std::string objectName);
	int getTrapLevel(std::string objectName);

    void BuyItem(std::shared_ptr<ShopPanel> shopPanel);
	void EvolMonster(std::shared_ptr<PanelSettingObject> PanelObject);

	void AllAssortmentGet();

	std::vector<ShopItem_Block> GetShopItemBlockList();
	std::vector<ShopItem_Trap> GetShopItemTrapList();
	std::vector<ShopItem_Monster> GetShopItemMonsterList();

	void LoadData(std::vector<ShopItem_Block> b, std::vector<ShopItem_Trap> t, std::vector<ShopItem_Monster> m);

private:
	ShopAssortment() = default;
	~ShopAssortment() = default;



	std::vector<std::shared_ptr<ShopItem_Block>> shopItemBlockList;
	std::vector<std::shared_ptr<ShopItem_Trap>> shopItemTrapList;
	std::vector<std::shared_ptr<ShopItem_Monster>> shopItemMonsterList;

};

