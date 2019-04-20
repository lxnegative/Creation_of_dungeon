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

	// ファイルの終端が来るまで表示する
	while (FileRead_eof(FileHandle) == 0)
	{

		// 一行読み込み
		FileRead_gets(str, 512, FileHandle);

		lineNum++;

	}

	// ファイルを閉じる
	FileRead_close(FileHandle);



	/*
	// セーブファイルオープン
	fstream fs(fileName);
	if (!fs.is_open())
		return 0;

	string str;
	size_t lineNum = 0;

	while (!fs.eof())
	{
		//行末まで "," 区切りで読み込む
		bool readBR = false;

		while (!readBR)
		{
			//read
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find("\n");
			readBR = (it != basic_string<char>::npos);

			//改行を含んだ文字列を読み込んだら
			if (readBR)
				lineNum++;
		}
	}

	//セーブファイルを閉じる
	fs.close();
	*/


	return lineNum;
}


void CSVReader::Read(const string &fileName, DataArray &dataArray, const std::string &seekString)
{

	int FileHandle;
	char str[512];

	FileHandle = FileRead_open(fileName.c_str());

	//読み初めとなる文字列の指定があれば
	if (seekString != "")
	{
		//目的の文字列に当たるまで読み飛ばす
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

	// ファイルの終端が来るまで表示する
	while (FileRead_eof(FileHandle) == 0)
	{

		// 一行読み込み
		FileRead_gets(str, 512, FileHandle);

		std::string s = str;

		std::vector<std::string> strVec = split(s, ',');

		for (int i = 0; i < strVec.size(); i++) {
			dataArray.push_back(strVec[i]);
		}


	}

	// ファイルを閉じる
	FileRead_close(FileHandle);


	/*
	// セーブファイルオープン
	fstream fs(fileName);
	if (!fs.is_open())
		return;

	string str;

	//読み初めとなる文字列の指定があれば
	if (seekString != "")
	{
		//目的の文字列に当たるまで読み飛ばす
		while (str != seekString)
		{
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find(seekString);
			if (it != basic_string<char>::npos)
			{
				str.erase(0, it);

				//ここまで来ると
				//str に 指標となる文字列 を読み込んであるのである状態なので
				//それをロードする
				dataArray.push_back(str);
				break;
			}
		}
	}

	while (!fs.eof())
	{
		//行末まで "," 区切りで読み込む
		bool readBR = false;

		while (!readBR)
		{
			//read
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find("\n");
			readBR = (it != basic_string<char>::npos);

			//改行を含んだ文字列を読み込んだら
			if (readBR)
			{
				//改行を基準に2分割。
				string cut = str.substr(0, it);
				str = str.substr(it + 1, str.size());

				dataArray.push_back(cut);
			}

			dataArray.push_back(str);
		}
	}

	//セーブファイルを閉じる
	fs.close();
	*/


}


void CSVReader::Read(const string &fileName, DataArray &dataArray, const int ignoreLine)
{

	int FileHandle;
	char str[512];

	FileHandle = FileRead_open(fileName.c_str());

	int lineNum = 0;

	//目的の文字列に当たるまで読み飛ばす
	while (FileRead_eof(FileHandle) == 0 && lineNum < ignoreLine)
	{

		FileRead_gets(str, 512, FileHandle);

		lineNum++;

	}


	// ファイルの終端が来るまで表示する
	while (FileRead_eof(FileHandle) == 0)
	{

		// 一行読み込み
		FileRead_gets(str, 512, FileHandle);

		std::string s = str;

		std::vector<std::string> strVec = split(s, ',');

		for (int i = 0; i < strVec.size(); i++) {
			dataArray.push_back(strVec[i]);
		}


	}

	// ファイルを閉じる
	FileRead_close(FileHandle);


	/*
	// セーブファイルオープン
	fstream fs(fileName);
	if (!fs.is_open())
		return;

	string str;

	//読み初めとなる文字列の指定があれば
	for (int i = 0; i < ignoreLine; ++i)
	{
		//行末まで "," 区切りで読み込む
		bool readBR = false;

		while (!readBR)
		{
			//read
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find("\n");
			readBR = (it != basic_string<char>::npos);

			//改行を含んだ文字列を読み込んだら
			if (readBR)
			{
				//改行を基準に2分割。
				string cut = str.substr(0, it);
				str = str.substr(it + 1, str.size());

				//改行を基準に分割したあとのデータに改行が含まれていたら
				auto it2 = str.find("\n");
				if (it2 != basic_string<char>::npos)
					str = str.substr(0, it2);

				//改行の直後の文字列を追加
				if (ignoreLine - 1 == i)
					dataArray.push_back(str);
			}
		}
	}
	

	while (!fs.eof())
	{
		//行末まで "," 区切りで読み込む
		bool readBR = false;

		while (!readBR)
		{
			//read
			getline(fs.seekg(0, ios_base::cur), str, ',');

			auto it = str.find("\n");
			readBR = (it != basic_string<char>::npos);

			//改行を含んだ文字列を読み込んだら
			if (readBR)
			{
				//改行を基準に2分割。
				string cut = str.substr(0, it);
				str = str.substr(it + 1, str.size());
				if (cut != "")
					dataArray.push_back(cut);
				else
					break;
			}

			//空文字チェック
			if (str != "")
				dataArray.push_back(str);
		}
	}

	//セーブファイルを閉じる
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

