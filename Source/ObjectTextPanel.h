#include <string>
#include "cd_666s\Render\Sprite.h"
#include <vector>
#include "ShopPanel.h"
#include "PanelSettingObject.h"
#pragma once

class ObjectTextPanel
{

private:

	enum PanelType {
		Shop = 0,
		Edit = 1,
		WorldMap = 2
	};

	PanelType panelType;

	Sprite _messageWindow;
	Sprite _ObjectImage;

	std::shared_ptr<ImageResource> shopFace;

	struct MessageText {

		std::string ObjectName;
		std::string ResourceName;
		std::string DisplayName;
		std::string ItemText;
	};

	std::vector<std::vector<std::shared_ptr<MessageText>>> messageText;

	ShopPanel::PanelCategory messageTextCategory;
	int messageTextArrayNum;

	Sprite _buyButton;
	std::shared_ptr<ImageResource> _LevelUpButtonImage;
	std::shared_ptr<ImageResource> _DoNotBuyButtonImage;
	std::shared_ptr<ImageResource> _LevelMaxButtonImage;

	bool existBuy;	//�w���{�^���͑��݂��邩
	bool canBuy;	//�w���\���i����������Ă��邩���܂ށj

	bool existLevelUp;	//���x���A�b�v��͑��݂��邩
	bool canLevelUp;	//���x���A�b�v�\���i����������Ă��邩���܂ށj

	int Level;
	int LevelUpCost;

	int putCost;

	Sound _cancelSE;
	
	EditObject editObject;

	std::string ShopInText;
	std::string ShopOutText;

public:

	ObjectTextPanel();
	ObjectTextPanel(std::string className);

	~ObjectTextPanel();

	void Init();
	void Init_Shop();
	void Init_Edit();

	void Update();
	void Draw();

	void ReadMessageText();
	void SetMessage(std::shared_ptr<ShopPanel> selectShopPanel);
	void SetMessage(std::shared_ptr<PanelSettingObject> selectPanel);
	void SetMessage(std::shared_ptr<PanelSettingObject> selectPanel, int cost, int payoffCost);
	
	void ResetMessage();

	bool BuyCheck();

	bool EvolCheck();

	void SetActive(bool flag);


	void SetShopFace(int num);

	bool isInShop;
	bool isOutShop;

};

