#include "CSVDataLoader.h"
#include <fstream>
#include <sstream>

CSVDataLoader::CSVDataLoader()
{
}


CSVDataLoader::~CSVDataLoader()
{
}

/*
int,temp_i[]
		 [0]: x
		 [1]: y
		 [2]: width
		 [3]: height
str,temp_s  : UI_type_name
data_name : UI�̖���, �摜�t�@�C��������Ō���
div_num_x : �摜�f�[�^��X���̕�����
div_num_y : �摜�f�[�^��Y���̕�����(�ꖇ�G�̏ꍇ�͗��҂Ƃ�1)
*/
void CSVDataLoader::LoadUICSV(std::vector<UIContent> &ui_data, std::string scene_name)
{
	std::string filename = Main::RESOURCEFILENAME + "csv\\UI\\" + scene_name + ".csv";


	int FileHandle;
	char str[512];

	FileHandle = FileRead_open(filename.c_str());

	// �t�@�C���̏I�[������܂ŕ\������
	while (FileRead_eof(FileHandle) == 0)
	{

		std::vector<std::string> temp;
		temp.reserve(9);

		// ��s�ǂݍ���
		FileRead_gets(str, 512, FileHandle);

		std::string s = str;


		std::vector<std::string> strVec = split(s, ',');

		for (int i = 0; i < strVec.size(); i++) {
			if (strVec[i].find("#") == std::string::npos) {
				temp.push_back(strVec[i]);
			}
		}


		if (temp.size() <= 6) {
			continue;
		}

		if (temp[4] != "" && temp[5] != "") {
			ui_data.push_back(UIContent(stoi(temp[0]), stoi(temp[1]), temp[2], temp[3], temp[4], stoi(temp[5]), stoi(temp[6])));
		}



	}

	// �t�@�C�������
	FileRead_close(FileHandle);


	/*
	std::ifstream ifs(filename);
	if (!ifs) {
		return;
	}

	//csv�t�@�C����1�s���ǂݍ���
	std::string str;
	while (getline(ifs, str)) {
		std::string token;
		std::istringstream stream(str);

		int i = 0;
		int temp_i[4] = { -1,-1,-1,-1 };
		int temp_div[2] = { 1,1 };
		std::string temp_s = "";
		std::string temp_data_name = "";

		std::vector<std::string> temp;
		temp.reserve(9);

		while (getline(stream, token, ',')) {
			auto n = token.find("#");
			auto m = token.find("\n");
			if (n == std::string::npos && m == std::string::npos) {
				/*
				if (i < 4) {
					temp_i[i] = stoi(token);
				}
				else if (i == 4) {
					temp_s = token;
				}
				else if (i == 5) {
					temp_data_name = token;
				}
				else {
					temp_div[i - 6] = stoi(token);
				}
				*//*

				temp.push_back(token);
				i++;
			}
		}

		if (temp.size() <= 0) {
			continue;
		}

		for (int i = 0; i < temp.size(); i++) {
			auto b = temp[i];
		}

		if (temp[4] != "" && temp[5] != ""/*temp_s != "" && temp_data_name != ""*//*) {
			//          ui_data.push_back(UIContent(temp_i[0], temp_i[1], temp_i[2], temp_i[3], temp_s, temp_data_name, temp_div[0], temp_div[1]));
			ui_data.push_back(UIContent(stoi(temp[0]), stoi(temp[1]), temp[2], temp[3], temp[4], stoi(temp[5]), stoi(temp[6])));
		}

		temp.clear();
		temp.resize(0);
	}
	*/


}


std::vector<std::string> CSVDataLoader::split(std::string str, char del) {
	int first = 0;
	int last = str.find_first_of(del);

	std::vector<std::string> result;

	while (first < str.size()) {
		std::string subStr(str, first, last - first);

		result.push_back(subStr);

		first = last + 1;
		last = str.find_first_of(del, first);

		if (last == std::string::npos) {
			last = str.size();
		}
	}

	return result;
}

