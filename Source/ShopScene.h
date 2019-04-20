#pragma once
#include "SceneBase.h"
#include "ShopPanel.h"
#include "ObjectTextPanel.h"
#include "TutorialPanel.h"

class ShopScene :
    public SceneBase
{

private:

    std::string class_name;

    std::string shopNum;

    std::vector<std::shared_ptr<ShopPanel>> shopPanelList;
	std::shared_ptr<ShopPanel> selectShopPanel;

	Sprite _background;
	Sprite _exitButton;
	Sprite _moneyBox;
	Sprite _shopTable_Up;
	Sprite _shopTable_Down;

	

	Sprite _selectFrame;

	std::shared_ptr<ObjectTextPanel> objectTextPanel;

	int returnPointNum;

	TutorialPanel* tutorialPanel;

	int ShopOutCnt;

	Sound _decideSE, _buybycoinD, _bgm;

public:

    ShopScene();
    ShopScene(std::string shopNum, int pointNum);
    ~ShopScene();

    std::string GetClassNameData() const {
        return class_name;
    }


	void Init();

    SceneBase* Update();
    SceneBase* Update(UIManager _ui) override;
    void Draw();

	bool ClickCheck(Vector2D pos, Vector2D size);

	void setSelectShopPanel(std::shared_ptr<ShopPanel> selectShopPanel);


};
