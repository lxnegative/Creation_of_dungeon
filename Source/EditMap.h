#pragma once
#include "SceneBase.h"
#include "MakeDungeon.h"

#include "PanelBase.h"
#include "PanelContent.h"
#include "PanelSettingObject.h"
#include "EditObject.h"
#include "ObjectTextPanel.h"
#include "TutorialPanel.h"
#include "PanelSceneTransition.h"
#include "PanelTrashObject.h"

#include "PopUpWindow_YES.h"
#include "Tutorial_Balloon.h"

class EditMap :
    public SceneBase
{
public:
	EditMap(std::string _area_num, string _stage_num, std::string _point_num);
    EditMap(std::string _area_num ,string _stage_num, std::string _point_num, bool isLoadNewMap);
    ~EditMap();

    SceneBase* Update(UIManager _ui);
    void Draw();

    std::string GetClassNameData() const {
        return class_name;
    }

    void Init();

private:
    const size_t NPOS;

    /****����J�E���g�p****/
    std::map<std::string, int> set_count;
    //int set_count[3];
    /****����J�E���g�p****/

    //����ݒ�
    //���̒萔(���Ƃ�const�Ő錾��������)
    int LIMIT_TRAP;
    int LIMIT_MONSTER;
    int LIMIT_BLOCK;

    std::string class_name;

    UIManager _uiManager;

    std::string stage_num;
	std::string stage_num_temp;

	std::string area_num;
	std::string point_num;

    std::shared_ptr<MakeDungeon> _dungeon;

    std::vector<std::string> _functions;

	std::shared_ptr<ObjectTextPanel> objectTextPanel;

    //std::vector<std::shared_ptr<PanelBase>> panels;  //UI

    bool IsFirstWave();
    
    SceneBase* PanelFunction();
    void PanelAffectObjectsFunction(std::shared_ptr<PanelBase> panel);
    void PanelDisplayerFunction(std::shared_ptr<PanelBase> panel);
    void PanelSceneTransFunction(std::shared_ptr<PanelBase> panel);
    void PanelSettingObjectFunction(std::shared_ptr<PanelBase> panel);

	void PanelPageChangeObjectFunction(std::shared_ptr<PanelBase> panel);
	void PanelTrashObjectFunction(std::shared_ptr<PanelBase> panel);

    void SetPanelInstance(std::string key_name, ::shared_ptr<PanelBase>& panel, PanelContent& temp);

    void DebugOutputFile();

	void ResetCost(std::string selectCategory, std::string ObjectName);

	bool PauseUpdate();

	void PauseDraw();

	std::string LoadLabeledElem(const std::string & label, const std::string & rawData);

	std::string WriteLabeledElem(const std::string & label, const std::string & rawData, std::string writeData);





    void SetObject();

	void RotateObject();

    bool SetObjectCheck(TiledVector tiledCursorPos);

    void DeleteAddedObject();


    //void GenerateObject(std::string typeName, int countX, int countY);

    bool Start_Connect_Goal();

	int CheckCost(std::string selectPanelCategory, std::string ObjectName);
	int CheckObjectNum(std::string selectPanelCategory, std::string ObjectName);
	int CheckPayOff();

    std::shared_ptr<PanelSettingObject> selectedObject;


    std::string selectPanelCategory;


    struct addTileObject {

		std::string ObjectName;
        TiledObject* tiledObject;
		int Cost;
        std::string GenerateText;

    };

    struct addTileObject_Monster {

		std::string ObjectName;
        TiledObject* MonsterObject;
        TiledObject* MagicSquareObject;
		int Cost;
        std::string GenerateText;

    };



    //�G�f�B�b�g�Œǉ������I�u�W�F�N�g�̎Q�Ɛ��\��
    //�
    std::vector<addTileObject> addTiledObjectList_Trap;
    //�u���b�N
    std::vector<addTileObject> addTiledObjectList_Block;
    //�����X�^�[
    std::vector<addTileObject_Monster> addTiledObjectList_Monster;

	//�O�p�[�g�Œǉ������I�u�W�F�N�g�̈ʒu�̃��X�g
	//�
	std::vector<addTileObject> RemainingObjectList_Trap;
	//�u���b�N
	std::vector<addTileObject> RemainingObjectList_Block;
	//�����X�^�[
	std::vector<addTileObject_Monster> RemainingObjectList_Monster;
	


	int payOffCost;
    
    Sound _cancelSE;

    Sprite _selectCategoryGr;
    Sprite _selectObjectGr;

	EditObject editObject;

	//�y�[�W�֌W
	int pageNum;
	int pageMaxNum;


	bool isLoadNewMap;

	Sprite _moneyPouch;

	TutorialPanel* tutorialPanel;
	Tutorial_Balloon* tutorial_Balloon;

	std::shared_ptr<PanelSceneTransition> gameStartButton;
	std::shared_ptr<ImageResource> canGameStartImage;
	std::shared_ptr<ImageResource> cannotGameStartImage;

	bool isPause;

	bool isTutorial;

	Sprite PauseUI;
	Sprite _black;

	std::vector<Vector2D> NotPutObjectPos;

	Sprite _PutPlanObjectImage;
	
	std::shared_ptr<PanelTrashObject> trashPanel;

	Sound _bgm;

	PopUpWindow_YES popUpWindow;

	Sound _blockSelect, _setbloksD, _decideSE, _buybycoinD;

	bool isInit;

};










