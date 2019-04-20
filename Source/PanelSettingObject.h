#pragma once
#include "PanelBase.h"
#include "ShopPanel.h"
class PanelSettingObject :
    public PanelBase
{
public:
    PanelSettingObject();
    PanelSettingObject(std::string _panel_name);
    PanelSettingObject(PanelContent _panelContent);
    ~PanelSettingObject();

    void SettingObj(PanelContent& pc);

    void Update() override;
    void Draw() override;

    void Init(PanelContent& _panelContent);

    bool IsClicked();

    void DrawDebugPrint() override;

    void PanelSettingObject_SettingPanel(std::string panelName, std::string GenerateText, std::string CategoryName, std::string graphName, int level, int LevelUpCost);

    void setIsSelected(bool flag);
    bool getIsSelected();

	bool GetCanLevelUp();
	int GetLevelUpCost();

    std::string getPanelObjectName();
	ShopPanel::PanelCategory GetPanelCategory();

	std::string GetPanelGraphPath();

	int getLevel();


    //��������ۂ̃e�L�X�g
    std::string GenerateText;

	void SetActive(bool flag);

private:
    PanelContent panel;

    //���ݕ\������Ă���I�u�W�F�N�g�̖��O
    //Lv���킩��Ȃ��̂łƂ肠����Lv�ɂ��Ă���
    std::string objectName;

    std::string graphName;
    
	int level;

	int LevelUpCost;
	bool canLevelUp;	//���x���A�b�v�ł��邩�i����������Ă��邩�͊܂܂Ȃ��j

    bool isSelected;

	ShopPanel::PanelCategory panelCategory;

	std::string PanelGraphPath;

};

