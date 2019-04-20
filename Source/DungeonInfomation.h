#include "cd_666s/Utility/Singleton.h"
#include <vector>
#include <string>
#pragma once


class DungeonInfomation : public Singleton<DungeonInfomation>
{
private:

	int DungeonRemainingPermition;

	std::string EditMapData;

	std::string EditMonsterData;

	std::string EditMap_AddObjectData;

public:

	void Reset();

	void SetEditMapData(std::string data);
	void AddEditMapData(std::string data);
	std::string GetEditMapData();
	std::vector<std::string> GetEditMapDataList();

	size_t GetLineSize();
	size_t GetLineNum();

	void SetEditMonsterData(std::string data);
	void AddEditMonsterData(std::string data);
	std::string GetEditMonsterData();
	std::vector<std::string> GetEditMonsterDataList();

	void SetEditMap_AddObjectData(std::string data);
	void AddEditMap_AddObjectData(std::string data);
	std::string GetEditMap_AddObjectData();
	std::vector<std::string> GetEditMap_AddObjectDataList();

	void SetDungeonRemainingPermition(int drp);
	int GetDungeonRemainingPermition();

	DungeonInfomation();
	~DungeonInfomation();


};

