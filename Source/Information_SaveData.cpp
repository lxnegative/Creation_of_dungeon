#include "Information_SaveData.h"
#include <fstream>
#include <iostream>


Information_SaveData::Information_SaveData()
{
}


Information_SaveData::~Information_SaveData()
{
}

void Information_SaveData::Init()
{

	save_PointData.clear();

	//ポイント情報の初期化
	Save_PointData temp;
	temp.AreaNum = 1;
	temp.PointNum = 1;
	temp.isPoint = 1;
	temp.isClear = 0;

	save_PointData.push_back(temp);


	isDebug = false;

}

int Information_SaveData::getIsPoint(int areanum, int pointnum)
{
	bool isLook = false;

	for (int i = 0; i < save_PointData.size(); i++) {
		if (save_PointData[i].AreaNum == areanum && save_PointData[i].PointNum == pointnum) {
			isLook = true;
			return save_PointData[i].isPoint;
		}
	}

	if (!isLook && !isDebug) {
		Save_PointData temp;
		temp.AreaNum = areanum;
		temp.PointNum = pointnum;
		temp.isPoint = 0;
		temp.isClear = 0;

		save_PointData.push_back(temp);
	}
	else if (!isLook && isDebug) {
		Save_PointData temp;
		temp.AreaNum = areanum;
		temp.PointNum = pointnum;
		temp.isPoint = 1;
		temp.isClear = 0;

		save_PointData.push_back(temp);

		return 1;
	}

	return 0;
}

int Information_SaveData::getIsClear(int areanum, int pointnum)
{

	bool isLook = false;

	for (int i = 0; i < save_PointData.size(); i++) {
		if (save_PointData[i].AreaNum == areanum && save_PointData[i].PointNum == pointnum) {
			isLook = true;
			return save_PointData[i].isClear;
		}
	}

	if (!isLook && !isDebug) {
		Save_PointData temp;
		temp.AreaNum = areanum;
		temp.PointNum = pointnum;
		temp.isPoint = 0;
		temp.isClear = 0;

		save_PointData.push_back(temp);
	}
	else if (!isLook && isDebug) {
		Save_PointData temp;
		temp.AreaNum = areanum;
		temp.PointNum = pointnum;
		temp.isPoint = 1;
		temp.isClear = 0;

		save_PointData.push_back(temp);

		return 1;
	}

	return 0;
}

void Information_SaveData::setIsPointActivation(int areanum, int pointnum)
{

	bool isLook = false;

	for (int i = 0; i < save_PointData.size(); i++) {
		if (save_PointData[i].AreaNum == areanum && save_PointData[i].PointNum == pointnum) {
			isLook = true;
			save_PointData[i].isPoint = 1;
		}
	}

	if (!isLook && !isDebug) {
		Save_PointData temp;
		temp.AreaNum = areanum;
		temp.PointNum = pointnum;
		temp.isPoint = 1;
		temp.isClear = 0;

		save_PointData.push_back(temp);
	}
	else if (!isLook && isDebug) {
		Save_PointData temp;
		temp.AreaNum = areanum;
		temp.PointNum = pointnum;
		temp.isPoint = 1;
		temp.isClear = 1;

		save_PointData.push_back(temp);

		return;
	}

	return;
}

void Information_SaveData::setIsClearActivation(int areanum, int pointnum)
{

	bool isLook = false;

	for (int i = 0; i < save_PointData.size(); i++) {
		if (save_PointData[i].AreaNum == areanum && save_PointData[i].PointNum == pointnum) {
			isLook = true;
			save_PointData[i].isClear = 1;
		}
	}

	if (!isLook && !isDebug) {
		Save_PointData temp;
		temp.AreaNum = areanum;
		temp.PointNum = pointnum;
		temp.isPoint = 1;
		temp.isClear = 1;

		save_PointData.push_back(temp);
	}
	else if (!isLook && isDebug) {
		Save_PointData temp;
		temp.AreaNum = areanum;
		temp.PointNum = pointnum;
		temp.isPoint = 1;
		temp.isClear = 1;

		save_PointData.push_back(temp);

		return;
	}

	return;
}

void Information_SaveData::SetAllStageActivation()
{
	save_PointData.clear();
	isDebug = true;
}

void Information_SaveData::SaveProcesses(int areaNum, int pointNum)
{

	SaveData_t SaveData;

	std::vector<Save_PointData> save_pointdata;
	std::vector<Tutorial_Timing_SaveData> tutorialTimingList;
	std::vector<ShopItem_Block_SaveData> shopItemBlockList;
	std::vector<ShopItem_Trap_SaveData> shopItemTrapList;
	std::vector<ShopItem_Monster_SaveData> shopItemMonsterList;

	SaveData.areaNum = areaNum;
	SaveData.pointNum = pointNum;
	SaveData.havemoney = MoneyManager::getInstance()->getMoney();
	save_pointdata = save_PointData;
	std::vector<Tutorial::Tutorial_Timing> ttl = Tutorial::getInstance()->GetTutorialTimingList();
	std::vector<ShopAssortment::ShopItem_Block> sibl = ShopAssortment::getInstance()->GetShopItemBlockList();
	std::vector<ShopAssortment::ShopItem_Trap> sitl = ShopAssortment::getInstance()->GetShopItemTrapList();
	std::vector<ShopAssortment::ShopItem_Monster> siml = ShopAssortment::getInstance()->GetShopItemMonsterList();

	for (int i = 0; i < ttl.size(); i++) {
		Tutorial_Timing_SaveData temp;
		temp.islooking = ttl[i].islooking;
		temp.pageNum = ttl[i].pageNum;

		if (ttl[i].TimingName.size() < CHAR_MAX) {
			// 配列のサイズを超えない場合にのみ書き込む
			std::char_traits<char>::copy(temp.TimingName, ttl[i].TimingName.c_str(), ttl[i].TimingName.size() + 1);
		}
		else {
			temp.TimingName[0] = '\0';
		}

		tutorialTimingList.push_back(temp);
	}

	for (int i = 0; i < sibl.size(); i++) {
		ShopItem_Block_SaveData temp;
		temp.isHaveing = sibl[i].isHaveing;

		if (sibl[i].ObjectName.size() < CHAR_MAX) {
			// 配列のサイズを超えない場合にのみ書き込む
			std::char_traits<char>::copy(temp.ObjectName, sibl[i].ObjectName.c_str(), sibl[i].ObjectName.size() + 1);
		}
		else {
			temp.ObjectName[0] = '\0';
		}

		shopItemBlockList.push_back(temp);
	}

	for (int i = 0; i < sitl.size(); i++) {
		ShopItem_Trap_SaveData temp;
		temp.isHaveing = sitl[i].isHaveing;
		temp.level = sitl[i].level;

		if (sitl[i].ObjectName.size() < CHAR_MAX) {
			// 配列のサイズを超えない場合にのみ書き込む
			std::char_traits<char>::copy(temp.ObjectName, sitl[i].ObjectName.c_str(), sitl[i].ObjectName.size() + 1);
		}
		else {
			temp.ObjectName[0] = '\0';
		}

		shopItemTrapList.push_back(temp);
	}

	for (int i = 0; i < siml.size(); i++) {
		ShopItem_Monster_SaveData temp;
		temp.isHaveing = siml[i].isHaveing;
		temp.level = siml[i].level;

		if (siml[i].ObjectName.size() < CHAR_MAX) {
			// 配列のサイズを超えない場合にのみ書き込む
			std::char_traits<char>::copy(temp.ObjectName, siml[i].ObjectName.c_str(), siml[i].ObjectName.size() + 1);
		}
		else {
			temp.ObjectName[0] = '\0';
		}

		shopItemMonsterList.push_back(temp);
	}

	std::fstream file;
	file.open("./Save/SaveData.dat", std::ios::binary | std::ios::out);
	//file.open("./filename.dat", ios::binary | ios::in);

	size_t tempSize;

	// 書き込む
	file.write((char*)&SaveData, sizeof(SaveData));

	tempSize = save_pointdata.size();

	file.write((char*)&tempSize, sizeof(tempSize));
	file.write((char *)&save_pointdata[0], tempSize * sizeof(Save_PointData));

	tempSize = tutorialTimingList.size();

	file.write((char*)&tempSize, sizeof(tempSize));
	file.write((char *)&tutorialTimingList[0], tempSize * sizeof(Tutorial_Timing_SaveData));
	

	tempSize = shopItemBlockList.size();

	file.write((char*)&tempSize, sizeof(tempSize));
	file.write((char *)&shopItemBlockList[0], tempSize * sizeof(ShopItem_Block_SaveData));

	tempSize = shopItemTrapList.size();

	file.write((char*)&tempSize, sizeof(tempSize));
	file.write((char *)&shopItemTrapList[0], tempSize * sizeof(ShopItem_Trap_SaveData));

	tempSize = shopItemMonsterList.size();

	file.write((char*)&tempSize, sizeof(tempSize));
	file.write((char *)&shopItemMonsterList[0], tempSize * sizeof(ShopItem_Monster_SaveData));


	file.close();

}

bool Information_SaveData::LoadProcesses(int* areaNum, int* pointNum)
{

	SaveData_t SaveData;

	std::vector<Save_PointData> save_pointdata_local;
	std::vector<Tutorial_Timing_SaveData> tutorialTimingList;
	std::vector<ShopItem_Block_SaveData> shopItemBlockList;
	std::vector<ShopItem_Trap_SaveData> shopItemTrapList;
	std::vector<ShopItem_Monster_SaveData> shopItemMonsterList;

	std::fstream file;
	file.open("./Save/SaveData.dat", ios::binary | ios::in);
	if (file.fail()) {
		return false;
	}

	size_t tempSize = 0;

	//読み込む
	file.read((char*)&SaveData, sizeof(SaveData));

	
	file.read((char*)&tempSize, sizeof(tempSize));
	Save_PointData init_spd;
	init_spd.AreaNum = 0;
	init_spd.isClear = 0;
	init_spd.isPoint = 0;
	init_spd.PointNum = 0;
	save_pointdata_local.assign(tempSize, init_spd);
	file.read((char *)&save_pointdata_local[0], tempSize * sizeof(Save_PointData));


	file.read((char*)&tempSize, sizeof(tempSize));
	Tutorial_Timing_SaveData init_tt;
	init_tt.islooking = false;
	init_tt.pageNum = 0;
	memset(init_tt.TimingName, 0, sizeof(init_tt.TimingName));
	tutorialTimingList.assign(tempSize, init_tt);
	file.read((char *)&tutorialTimingList[0], tempSize * sizeof(Tutorial_Timing_SaveData));


	file.read((char*)&tempSize, sizeof(tempSize));
	ShopItem_Block_SaveData init_sib;
	init_sib.isHaveing = false;
	memset(init_sib.ObjectName, 0, sizeof(init_sib.ObjectName));
	shopItemBlockList.assign(tempSize, init_sib);
	file.read((char *)&shopItemBlockList[0], tempSize * sizeof(ShopItem_Block_SaveData));


	file.read((char*)&tempSize, sizeof(tempSize));
	ShopItem_Trap_SaveData init_sit;
	init_sit.isHaveing = false;
	init_sit.level = 0;
	memset(init_sit.ObjectName, 0, sizeof(init_sit.ObjectName));
	shopItemTrapList.assign(tempSize, init_sit);
	file.read((char *)&shopItemTrapList[0], tempSize * sizeof(ShopItem_Trap_SaveData));


	file.read((char*)&tempSize, sizeof(tempSize));
	ShopItem_Monster_SaveData init_sim;
	init_sim.isHaveing = false;
	init_sim.level = 0;
	memset(init_sim.ObjectName, 0, sizeof(init_sim.ObjectName));
	shopItemMonsterList.assign(tempSize, init_sim);
	file.read((char *)&shopItemMonsterList[0], tempSize * sizeof(ShopItem_Monster_SaveData));



	file.close();

	*areaNum = SaveData.areaNum;
	*pointNum = SaveData.pointNum;

	MoneyManager::getInstance()->setMoney(SaveData.havemoney);

	for (int i = 0; i < save_pointdata_local.size(); i++) {
		Save_PointData spd;
		spd.AreaNum = save_pointdata_local[i].AreaNum;
		spd.isClear = save_pointdata_local[i].isClear;
		spd.isPoint = save_pointdata_local[i].isPoint;
		spd.PointNum = save_pointdata_local[i].PointNum;

		save_PointData.push_back(spd);
	}


	std::vector<ShopAssortment::ShopItem_Block> sasib;
	for (int i = 0; i < shopItemBlockList.size(); i++) {
		ShopAssortment::ShopItem_Block tempab;
		tempab.isHaveing = shopItemBlockList[i].isHaveing;
		tempab.ObjectName = std::string(shopItemBlockList[i].ObjectName, std::strlen(shopItemBlockList[i].ObjectName));

		sasib.push_back(tempab);
	}

	std::vector<ShopAssortment::ShopItem_Trap> sasit;
	for (int i = 0; i < shopItemTrapList.size(); i++) {
		ShopAssortment::ShopItem_Trap tempat;
		tempat.isHaveing = shopItemTrapList[i].isHaveing;
		tempat.level = shopItemTrapList[i].level;
		tempat.ObjectName = std::string(shopItemTrapList[i].ObjectName, std::strlen(shopItemTrapList[i].ObjectName));

		sasit.push_back(tempat);
	}

	std::vector<ShopAssortment::ShopItem_Monster> sasim;
	for (int i = 0; i < shopItemMonsterList.size(); i++) {
		ShopAssortment::ShopItem_Monster tempam;
		tempam.isHaveing = shopItemMonsterList[i].isHaveing;
		tempam.level = shopItemMonsterList[i].level;
		tempam.ObjectName = std::string(shopItemMonsterList[i].ObjectName, std::strlen(shopItemMonsterList[i].ObjectName));

		sasim.push_back(tempam);
	}



	ShopAssortment::getInstance()->LoadData(sasib, sasit, sasim);


	std::vector<Tutorial::Tutorial_Timing> t;
	for (int i = 0; i < tutorialTimingList.size(); i++) {
		Tutorial::Tutorial_Timing tempt;
		tempt.islooking = tutorialTimingList[i].islooking;
		tempt.pageNum = tutorialTimingList[i].pageNum;
		tempt.TimingName = std::string(tutorialTimingList[i].TimingName, std::strlen(tutorialTimingList[i].TimingName));

		t.push_back(tempt);
	}

	Tutorial::getInstance()->SetTutorialTimingList(t);


	return true;


}

void Information_SaveData::TempSaveProcesses(int areaNum, int pointNum)
{

	tempSaveData.areaNum = areaNum;
	tempSaveData.pointNum = pointNum;
	tempSaveData.havemoney = MoneyManager::getInstance()->getMoney();
	tempSave_pointdata = save_PointData;
	std::vector<Tutorial::Tutorial_Timing> ttl = Tutorial::getInstance()->GetTutorialTimingList();
	std::vector<ShopAssortment::ShopItem_Block> sibl = ShopAssortment::getInstance()->GetShopItemBlockList();
	std::vector<ShopAssortment::ShopItem_Trap> sitl = ShopAssortment::getInstance()->GetShopItemTrapList();
	std::vector<ShopAssortment::ShopItem_Monster> siml = ShopAssortment::getInstance()->GetShopItemMonsterList();

	for (int i = 0; i < ttl.size(); i++) {
		Tutorial_Timing_SaveData temp;
		temp.islooking = ttl[i].islooking;
		temp.pageNum = ttl[i].pageNum;

		if (ttl[i].TimingName.size() < CHAR_MAX) {
			// 配列のサイズを超えない場合にのみ書き込む
			std::char_traits<char>::copy(temp.TimingName, ttl[i].TimingName.c_str(), ttl[i].TimingName.size() + 1);
		}
		else {
			temp.TimingName[0] = '\0';
		}

		tempTutorialTimingList.push_back(temp);
	}

	for (int i = 0; i < sibl.size(); i++) {
		ShopItem_Block_SaveData temp;
		temp.isHaveing = sibl[i].isHaveing;

		if (sibl[i].ObjectName.size() < CHAR_MAX) {
			// 配列のサイズを超えない場合にのみ書き込む
			std::char_traits<char>::copy(temp.ObjectName, sibl[i].ObjectName.c_str(), sibl[i].ObjectName.size() + 1);
		}
		else {
			temp.ObjectName[0] = '\0';
		}

		tempShopItemBlockList.push_back(temp);
	}

	for (int i = 0; i < sitl.size(); i++) {
		ShopItem_Trap_SaveData temp;
		temp.isHaveing = sitl[i].isHaveing;
		temp.level = sitl[i].level;

		if (sitl[i].ObjectName.size() < CHAR_MAX) {
			// 配列のサイズを超えない場合にのみ書き込む
			std::char_traits<char>::copy(temp.ObjectName, sitl[i].ObjectName.c_str(), sitl[i].ObjectName.size() + 1);
		}
		else {
			temp.ObjectName[0] = '\0';
		}

		tempShopItemTrapList.push_back(temp);
	}

	for (int i = 0; i < siml.size(); i++) {
		ShopItem_Monster_SaveData temp;
		temp.isHaveing = siml[i].isHaveing;
		temp.level = siml[i].level;

		if (siml[i].ObjectName.size() < CHAR_MAX) {
			// 配列のサイズを超えない場合にのみ書き込む
			std::char_traits<char>::copy(temp.ObjectName, siml[i].ObjectName.c_str(), siml[i].ObjectName.size() + 1);
		}
		else {
			temp.ObjectName[0] = '\0';
		}

		tempShopItemMonsterList.push_back(temp);
	}
}

bool Information_SaveData::TempLoadProcesses(int * areaNum, int * pointNum)
{

	*areaNum = tempSaveData.areaNum;
	*pointNum = tempSaveData.pointNum;

	MoneyManager::getInstance()->setMoney(tempSaveData.havemoney);

	for (int i = 0; i < tempSave_pointdata.size(); i++) {
		Save_PointData spd;
		spd.AreaNum = tempSave_pointdata[i].AreaNum;
		spd.isClear = tempSave_pointdata[i].isClear;
		spd.isPoint = tempSave_pointdata[i].isPoint;
		spd.PointNum = tempSave_pointdata[i].PointNum;

		save_PointData.push_back(spd);
	}


	std::vector<ShopAssortment::ShopItem_Block> sasib;
	for (int i = 0; i < tempShopItemBlockList.size(); i++) {
		ShopAssortment::ShopItem_Block tempab;
		tempab.isHaveing = tempShopItemBlockList[i].isHaveing;
		tempab.ObjectName = std::string(tempShopItemBlockList[i].ObjectName, std::strlen(tempShopItemBlockList[i].ObjectName));

		sasib.push_back(tempab);
	}

	std::vector<ShopAssortment::ShopItem_Trap> sasit;
	for (int i = 0; i < tempShopItemTrapList.size(); i++) {
		ShopAssortment::ShopItem_Trap tempat;
		tempat.isHaveing = tempShopItemTrapList[i].isHaveing;
		tempat.level = tempShopItemTrapList[i].level;
		tempat.ObjectName = std::string(tempShopItemTrapList[i].ObjectName, std::strlen(tempShopItemTrapList[i].ObjectName));

		sasit.push_back(tempat);
	}

	std::vector<ShopAssortment::ShopItem_Monster> sasim;
	for (int i = 0; i < tempShopItemMonsterList.size(); i++) {
		ShopAssortment::ShopItem_Monster tempam;
		tempam.isHaveing = tempShopItemMonsterList[i].isHaveing;
		tempam.level = tempShopItemMonsterList[i].level;
		tempam.ObjectName = std::string(tempShopItemMonsterList[i].ObjectName, std::strlen(tempShopItemMonsterList[i].ObjectName));

		sasim.push_back(tempam);
	}



	ShopAssortment::getInstance()->LoadData(sasib, sasit, sasim);


	std::vector<Tutorial::Tutorial_Timing> t;
	for (int i = 0; i < tempTutorialTimingList.size(); i++) {
		Tutorial::Tutorial_Timing tempt;
		tempt.islooking = tempTutorialTimingList[i].islooking;
		tempt.pageNum = tempTutorialTimingList[i].pageNum;
		tempt.TimingName = std::string(tempTutorialTimingList[i].TimingName, std::strlen(tempTutorialTimingList[i].TimingName));

		t.push_back(tempt);
	}

	Tutorial::getInstance()->SetTutorialTimingList(t);


	return true;

}

void Information_SaveData::TempInit()
{

	tempSaveData.areaNum = -1;
	tempSaveData.havemoney = -1;
	tempSaveData.pointNum = -1;

	tempSave_pointdata.clear();
	tempTutorialTimingList.clear();
	tempShopItemBlockList.clear();
	tempShopItemTrapList.clear();
	tempShopItemMonsterList.clear();

}
