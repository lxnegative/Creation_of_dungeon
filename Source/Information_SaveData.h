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

		int AreaNum;    //エリア番号
		int PointNum;	//ポイント番号
		int isPoint;    //ポイントが有効かどうか（クリア等でそのポイントが出来るようになっているか）0で無効　1で有効
		int isClear;	//そのポイントがクリア済みか

	};

	struct Tutorial_Timing_SaveData {
		char TimingName[CHAR_MAX];   //タイミングの名前（画像名とリンクしてる）
		bool islooking;             //見たことがあるか
		int pageNum;             //ページ数
	};

	struct ShopItem_Block_SaveData {
		char ObjectName[CHAR_MAX];   //オブジェクト名
		bool isHaveing;             //所持しているかどうか   trueで所持している（購入済み）
	};

	struct ShopItem_Trap_SaveData {
		char ObjectName[CHAR_MAX];   //オブジェクト名
		bool isHaveing;             //所持しているかどうか   trueで所持している（購入済み）
		int level;
	};

	struct ShopItem_Monster_SaveData {
		char ObjectName[CHAR_MAX];   //オブジェクト名
		bool isHaveing;             //所持しているかどうか   trueで所持している（購入済み）
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

