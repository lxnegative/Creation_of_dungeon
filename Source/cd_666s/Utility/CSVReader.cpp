#include "CSVReader.h"
#include "DxLib.h"

using namespace std;

CSVReader::CSVReader()
{
}


size_t CSVReader::GetLineNum(const std::string &fileName)
{

	int FileHandle;
	char str[512];

	FileHandle = FileRead_open(fileName.c_str());

	size_t lineNum = 0;

	// �t�@�C���̏I�[������܂ŕ\������
	while (FileRead_eof(FileHandle) == 0)
	{

		// ��s�ǂݍ���
		FileRead_gets(str, 512, FileHandle);

		lineNum++;

	}

	// �t�@�C�������
	FileRead_close(FileHandle);



	/*
	// �Z�[�u�t�@�C���I�[�v��
	fstream fs(fileName);
	if (!fs.is_open())
		return 0;

	string str;
	size_t lineNum = 0;

	while (!fs.eof())
	{
		//�s���܂� "," ��؂�œǂݍ���
		bool readBR = false;

		while (!readBR)
		{
			//read
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find("\n");
			readBR = (it != basic_string<char>::npos);

			//���s���܂񂾕������ǂݍ��񂾂�
			if (readBR)
				lineNum++;
		}
	}

	//�Z�[�u�t�@�C�������
	fs.close();
	*/


	return lineNum;
}


void CSVReader::Read(const string &fileName, DataArray &dataArray, const std::string &seekString)
{

	int FileHandle;
	char str[512];

	FileHandle = FileRead_open(fileName.c_str());

	//�ǂݏ��߂ƂȂ镶����̎w�肪�����
	if (seekString != "")
	{
		//�ړI�̕�����ɓ�����܂œǂݔ�΂�
		while (FileRead_eof(FileHandle) == 0)
		{
			bool isSeeking = false;

			FileRead_gets(str, 512, FileHandle);

			std::string s = str;

			if (s.find(seekString) != std::string::npos) {

				std::vector<std::string> strVec = split(s, ',');

				for (int i = 0; i < strVec.size(); i++) {

					if (isSeeking) {
						dataArray.push_back(strVec[i]);
						continue;
					}

					if (strVec[i].find(seekString) != std::string::npos) {
						isSeeking = true;
						dataArray.push_back(strVec[i]);
					}

				}

			}

			if (isSeeking) {
				break;
			}

		}
	}

	// �t�@�C���̏I�[������܂ŕ\������
	while (FileRead_eof(FileHandle) == 0)
	{

		// ��s�ǂݍ���
		FileRead_gets(str, 512, FileHandle);

		std::string s = str;

		std::vector<std::string> strVec = split(s, ',');

		for (int i = 0; i < strVec.size(); i++) {
			dataArray.push_back(strVec[i]);
		}


	}

	// �t�@�C�������
	FileRead_close(FileHandle);


	/*
	// �Z�[�u�t�@�C���I�[�v��
	fstream fs(fileName);
	if (!fs.is_open())
		return;

	string str;

	//�ǂݏ��߂ƂȂ镶����̎w�肪�����
	if (seekString != "")
	{
		//�ړI�̕�����ɓ�����܂œǂݔ�΂�
		while (str != seekString)
		{
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find(seekString);
			if (it != basic_string<char>::npos)
			{
				str.erase(0, it);

				//�����܂ŗ����
				//str �� �w�W�ƂȂ镶���� ��ǂݍ���ł���̂ł����ԂȂ̂�
				//��������[�h����
				dataArray.push_back(str);
				break;
			}
		}
	}

	while (!fs.eof())
	{
		//�s���܂� "," ��؂�œǂݍ���
		bool readBR = false;

		while (!readBR)
		{
			//read
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find("\n");
			readBR = (it != basic_string<char>::npos);

			//���s���܂񂾕������ǂݍ��񂾂�
			if (readBR)
			{
				//���s�����2�����B
				string cut = str.substr(0, it);
				str = str.substr(it + 1, str.size());

				dataArray.push_back(cut);
			}

			dataArray.push_back(str);
		}
	}

	//�Z�[�u�t�@�C�������
	fs.close();
	*/


}


void CSVReader::Read(const string &fileName, DataArray &dataArray, const int ignoreLine)
{

	int FileHandle;
	char str[512];

	FileHandle = FileRead_open(fileName.c_str());

	int lineNum = 0;

	//�ړI�̕�����ɓ�����܂œǂݔ�΂�
	while (FileRead_eof(FileHandle) == 0 && lineNum < ignoreLine)
	{

		FileRead_gets(str, 512, FileHandle);

		lineNum++;

	}


	// �t�@�C���̏I�[������܂ŕ\������
	while (FileRead_eof(FileHandle) == 0)
	{

		// ��s�ǂݍ���
		FileRead_gets(str, 512, FileHandle);

		std::string s = str;

		std::vector<std::string> strVec = split(s, ',');

		for (int i = 0; i < strVec.size(); i++) {
			dataArray.push_back(strVec[i]);
		}


	}

	// �t�@�C�������
	FileRead_close(FileHandle);


	/*
	// �Z�[�u�t�@�C���I�[�v��
	fstream fs(fileName);
	if (!fs.is_open())
		return;

	string str;

	//�ǂݏ��߂ƂȂ镶����̎w�肪�����
	for (int i = 0; i < ignoreLine; ++i)
	{
		//�s���܂� "," ��؂�œǂݍ���
		bool readBR = false;

		while (!readBR)
		{
			//read
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find("\n");
			readBR = (it != basic_string<char>::npos);

			//���s���܂񂾕������ǂݍ��񂾂�
			if (readBR)
			{
				//���s�����2�����B
				string cut = str.substr(0, it);
				str = str.substr(it + 1, str.size());

				//���s����ɕ����������Ƃ̃f�[�^�ɉ��s���܂܂�Ă�����
				auto it2 = str.find("\n");
				if (it2 != basic_string<char>::npos)
					str = str.substr(0, it2);

				//���s�̒���̕������ǉ�
				if (ignoreLine - 1 == i)
					dataArray.push_back(str);
			}
		}
	}
	

	while (!fs.eof())
	{
		//�s���܂� "," ��؂�œǂݍ���
		bool readBR = false;

		while (!readBR)
		{
			//read
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find("\n");
			readBR = (it != basic_string<char>::npos);

			//���s���܂񂾕������ǂݍ��񂾂�
			if (readBR)
			{
				//���s�����2�����B
				string cut = str.substr(0, it);
				str = str.substr(it + 1, str.size());
				if (cut != "")
					dataArray.push_back(cut);
				else
					break;
			}

			//�󕶎��`�F�b�N
			if (str != "")
				dataArray.push_back(str);
		}
	}

	//�Z�[�u�t�@�C�������
	fs.close();
	*/

}


CSVReader::~CSVReader()
{
}

std::vector<std::string> CSVReader::split(std::string str, char del) {
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

