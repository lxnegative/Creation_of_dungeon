#ifndef CSV_READER_H
#define CSV_READER_H

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "DxLib.h"

/*
	CSV�t�@�C���̓ǂݍ��݂�
	�f�[�^�X�g���[�����s��
 */


typedef std::vector<std::string> DataArray;

class CSVReader
{
    
public:
    CSVReader();
    ~CSVReader();
    
	size_t GetLineSize(const std::string &fileName, int lineNum)
	{

		int FileHandle;
		char str[512];

		FileHandle = FileRead_open(fileName.c_str());

		size_t lineSize = 0;

		int LineNum = 0;

		// �t�@�C���̏I�[������܂ŕ\������
		while (FileRead_eof(FileHandle) == 0)
		{

			LineNum++;

			// ��s�ǂݍ���
			FileRead_gets(str, 512, FileHandle);

			if (LineNum > lineNum) {
				std::string s = str;

				std::vector<std::string> strVec = split(s, ',');

				lineSize = strVec.size();
				break;

			}


		}

		// �t�@�C�������
		FileRead_close(FileHandle);

		/*

		// �Z�[�u�t�@�C���I�[�v��
		fstream fs(fileName);
		if (!fs.is_open())
		return 0;

		string str;
		size_t lineSize = 0;

		//�ǂݏ��߂ƂȂ镶����̎w�肪�����
		for (int i=0; i<=lineNum; ++i)
		{
		//�s���܂� "," ��؂�œǂݍ���
		bool readBR = false;

		while (!readBR)
		{
		//read
		getline(fs.seekg(0, ios_base::cur), str, ',');
		auto it = str.find("\n");
		readBR = (it != basic_string<char>::npos);

		//�ǂݍ��񂾐������J�E���g
		lineSize++;
		}
		}

		//�t�@�C�������
		fs.close();

		*/

		return lineSize;
	}
    size_t GetLineNum(const std::string &fileName);
    
    void Read(const std::string &fileName, DataArray &dataArray, const std::string &seekString = "");
    void Read(const std::string &fileName, DataArray &dataArray, const int ignoreLineNum);

	std::vector<std::string> split(std::string str, char del);
    
private:
    std::stringstream ss;
};

#endif