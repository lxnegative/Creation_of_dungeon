#include "cd_666s\Utility\Singleton.h"
#include <vector>
#include "ShopAssortment.h"
#include "MoneyManager.h"
#include "Tutorial.h"

#define CHAR_MAX 64

#pragma once
class Information_SaveData : public Singleton<Information_SaveData>
{
private:
	bool isDebug;

public:

	struct Save_PointData {

		int AreaNum;    //�G���A�ԍ�
		int PointNum;	//�|�C���g�ԍ�
		int isPoint;    //�|�C���g���L�����ǂ����i�N���A���ł��̃|�C���g���o����悤�ɂȂ��Ă��邩�j0�Ŗ����@1�ŗL��
		int isClear;	//���̃|�C���g���N���A�ς݂�

	};

	struct Tutorial_Timing_SaveData {
		char TimingName[CHAR_MAX];   //�^�C�~���O�̖��O�i�摜���ƃ����N���Ă�j
		bool islooking;             //�������Ƃ����邩
		int pageNum;             //�y�[�W��
	};

	struct ShopItem_Block_SaveData {
		char ObjectName[CHAR_MAX];   //�I�u�W�F�N�g��
		bool isHaveing;             //�������Ă��邩�ǂ���   true�ŏ������Ă���i�w���ς݁j
	};

	struct ShopItem_Trap_SaveData {
		char ObjectName[CHAR_MAX];   //�I�u�W�F�N�g��
		bool isHaveing;             //�������Ă��邩�ǂ���   true�ŏ������Ă���i�w���ς݁j
		int level;
	};

	struct ShopItem_Monster_SaveData {
		char ObjectName[CHAR_MAX];   //�I�u�W�F�N�g��
		bool isHaveing;             //�������Ă��邩�ǂ���   true�ŏ������Ă���i�w���ς݁j
		int level;
	};


	typedef struct {
		int areaNum;
		int pointNum;
		int havemoney;
	}SaveData_t;

	Information_SaveData();
	~Information_SaveData();



	std::vector<Save_PointData> save_PointData;



	SaveData_t tempSaveData;

	std::vector<Save_PointData> tempSave_pointdata;
	std::vector<Tutorial_Timing_SaveData> tempTutorialTimingList;
	std::vector<ShopItem_Block_SaveData> tempShopItemBlockList;
	std::vector<ShopItem_Trap_SaveData> tempShopItemTrapList;
	std::vector<ShopItem_Monster_SaveData> tempShopItemMonsterList;




	void Init();


	int getIsPoint(int areanum, int pointnum);
	int getIsClear(int areanum, int pointnum);

	void setIsPointActivation(int areanum, int pointnum);
	void setIsClearActivation(int areanum, int pointnum);

	void SetAllStageActivation();

	void SaveProcesses(int areaNum, int pointNum);
	bool LoadProcesses(int* areaNum, int* pointNum);


	void TempSaveProcesses(int areaNum, int pointNum);
	bool TempLoadProcesses(int* areaNum, int* pointNum);

	void TempInit();

};

