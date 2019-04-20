#include "DungeonInfomation.h"



void DungeonInfomation::Reset()
{
	DungeonRemainingPermition = 0;
	EditMapData = "";
	EditMonsterData = "";
}

void DungeonInfomation::SetEditMapData(std::string data)
{
	EditMapData = data;
}

void DungeonInfomation::AddEditMapData(std::string data)
{
	EditMapData += data;
}

std::string DungeonInfomation::GetEditMapData()
{
	return EditMapData;
}

std::vector<std::string> DungeonInfomation::GetEditMapDataList()
{

	int first = 0;
	size_t lastc = EditMapData.find_first_of(',');
	size_t lastn = EditMapData.find_first_of('\n');
	int last = 0;

	if (lastc != std::string::npos && lastn != std::string::npos) {
		if (lastc < lastn) {
			last = lastc;
		}
		else {
			last = lastn;
		}
	}
	else {
		if (lastc != std::string::npos) {
			last = lastc;
		}
		else if (lastn != std::string::npos) {
			last = lastn;
		}
		else {
			last = EditMapData.size();
		}
	}
	

	std::vector<std::string> result;

	while (first < EditMapData.size()) {
		std::string subStr(EditMapData, first, last - first);

		result.push_back(subStr);

		first = last + 1;

		lastc = EditMapData.find_first_of(',', first);
		lastn = EditMapData.find_first_of('\n', first);

		if (lastc != std::string::npos && lastn != std::string::npos) {
			if (lastc < lastn) {
				last = lastc;
			}
			else {
				last = lastn;
			}
		}
		else {
			if (lastc != std::string::npos) {
				last = lastc;
			}
			else if (lastn != std::string::npos) {
				last = lastn;
			}
			else {
				last = EditMapData.size();
			}
		}

	}

	return result;
}

size_t DungeonInfomation::GetLineSize()
{
	int first = 0;
	int last = EditMapData.find_first_of('\n');

	std::string subStr(EditMapData, first, last - first);

	first = 0;
	last = subStr.find_first_of(',');

	std::vector<std::string> result;

	while (first < subStr.size()) {
		std::string subStr1(subStr, first, last - first);

		result.push_back(subStr1);

		first = last + 1;
		last = subStr.find_first_of(',', first);

		if (last == std::string::npos) {
			last = subStr.size();
		}
	}

	return result.size();
}

size_t DungeonInfomation::GetLineNum()
{
	int first = 0;
	int last = EditMapData.find_first_of('\n');

	std::vector<std::string> result;

	while (first < EditMapData.size()) {
		std::string subStr(EditMapData, first, last - first);

		result.push_back(subStr);

		first = last + 1;
		last = EditMapData.find_first_of('\n', first);

		if (last == std::string::npos) {
			last = EditMapData.size();
		}
	}

	return result.size();
}

void DungeonInfomation::SetEditMonsterData(std::string data)
{
	EditMonsterData = data;
}

void DungeonInfomation::AddEditMonsterData(std::string data)
{
	EditMonsterData += data;
}

std::string DungeonInfomation::GetEditMonsterData()
{
	return EditMonsterData;
}

std::vector<std::string> DungeonInfomation::GetEditMonsterDataList()
{

	int first = 0;
	int last = EditMonsterData.find_first_of('\n');

	std::string subStr(EditMonsterData, first, last - first);
	first = last + 1;

	size_t lastc = EditMonsterData.find_first_of(',', first);
	size_t lastn = EditMonsterData.find_first_of('\n', first);

	if (lastc != std::string::npos && lastn != std::string::npos) {
		if (lastc < lastn) {
			last = lastc;
		}
		else {
			last = lastn;
		}
	}
	else {
		if (lastc != std::string::npos) {
			last = lastc;
		}
		else if (lastn != std::string::npos) {
			last = lastn;
		}
		else {
			last = EditMonsterData.size();
		}
	}

	if (first >= EditMonsterData.size()) {
		return std::vector<std::string>();
	}

	std::vector<std::string> result;

	while (first < EditMonsterData.size()) {
		std::string subStr(EditMonsterData, first, last - first);

		result.push_back(subStr);

		first = last + 1;

		lastc = EditMonsterData.find_first_of(',', first);
		lastn = EditMonsterData.find_first_of('\n', first);

		if (lastc != std::string::npos && lastn != std::string::npos) {
			if (lastc < lastn) {
				last = lastc;
			}
			else {
				last = lastn;
			}
		}
		else {
			if (lastc != std::string::npos) {
				last = lastc;
			}
			else if (lastn != std::string::npos) {
				last = lastn;
			}
			else {
				last = EditMonsterData.size();
			}
		}
	}

	return result;

}

void DungeonInfomation::SetEditMap_AddObjectData(std::string data)
{
	EditMap_AddObjectData = data;
}

void DungeonInfomation::AddEditMap_AddObjectData(std::string data)
{
	EditMap_AddObjectData += data;
}

std::string DungeonInfomation::GetEditMap_AddObjectData()
{
	return EditMap_AddObjectData;
}

std::vector<std::string> DungeonInfomation::GetEditMap_AddObjectDataList()
{

	int first = 0;
	int last = EditMap_AddObjectData.find_first_of('\n');

	std::string subStr(EditMap_AddObjectData, first, last - first);
	first = last + 1;

	size_t lastc = EditMap_AddObjectData.find_first_of(',');
	size_t lastn = EditMap_AddObjectData.find_first_of('\n');

	if (lastc != std::string::npos && lastn != std::string::npos) {
		if (lastc < lastn) {
			last = lastc;
		}
		else {
			last = lastn;
		}
	}
	else {
		if (lastc != std::string::npos) {
			last = lastc;
		}
		else if (lastn != std::string::npos) {
			last = lastn;
		}
		else {
			last = EditMap_AddObjectData.size();
		}
	}

	if (first >= EditMap_AddObjectData.size()) {
		return std::vector<std::string>();
	}

	std::vector<std::string> result;

	while (first < EditMap_AddObjectData.size()) {
		std::string subStr(EditMap_AddObjectData, first, last - first);

		result.push_back(subStr);

		first = last + 1;

		lastc = EditMap_AddObjectData.find_first_of(',', first);
		lastn = EditMap_AddObjectData.find_first_of('\n', first);

		if (lastc != std::string::npos && lastn != std::string::npos) {
			if (lastc < lastn) {
				last = lastc;
			}
			else {
				last = lastn;
			}
		}
		else {
			if (lastc != std::string::npos) {
				last = lastc;
			}
			else if (lastn != std::string::npos) {
				last = lastn;
			}
			else {
				last = EditMap_AddObjectData.size();
			}
		}
	}

	return result;

}

void DungeonInfomation::SetDungeonRemainingPermition(int drp)
{
	DungeonRemainingPermition = drp;
}

int DungeonInfomation::GetDungeonRemainingPermition()
{
	return DungeonRemainingPermition;
}

DungeonInfomation::DungeonInfomation()
{
}


DungeonInfomation::~DungeonInfomation()
{
}
