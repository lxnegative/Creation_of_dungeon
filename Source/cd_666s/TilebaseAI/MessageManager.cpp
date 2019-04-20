#include "MessageManager.h"
#include <Windows.h>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>
#include <algorithm>
#include <assert.h>

#include "DxLib.h"
#include "../DebugDraw.h"
#include <regex>
#include "../Utility/CSVReader.h"

#pragma once




MessageManager::MessageManager() {

    Init();

}

MessageManager::MessageManager(std::string situation) {
}


MessageManager::~MessageManager() {
}


TalkDatabase MessageManager::CreateTalkData(std::string fileName, Talk_Type type)
{
	int FileHandle;
	char str[512];

	FileHandle = FileRead_open(fileName.c_str());

    int load = 0;   //0でロードする文章を発見していない 1でロードする文章を発見し読み込み中 2でロード終了
    int talkNum = 0;

    TalkDatabase tdb;


	// ファイルの終端が来るまで表示する
	while (FileRead_eof(FileHandle) == 0)
	{

		// 一行読み込み
		FileRead_gets(str, 512, FileHandle);

		std::string s = str;

		std::string::size_type index = s.find("#");  // "#"を検索
													   //"#"が入っていた行は飛ばす
		if (index != std::string::npos) {
			continue;
		}

		if (load == 0) {

			index = s.find("*talkNum");  // "*talkNum"を検索

			if (index == std::string::npos && load == 0) {
				continue;                       //"*talkNum"が入っていない行は飛ばす
			}

		}

		MessageData mes;

		CSVReader reader;

		std::vector<std::string> strVec = reader.split(s, ',');


		for (int i = 0; i < strVec.size(); i++) {

			if (load == 0) {
				if (i == 1) {
					tdb.TalkGroupNum = stoi(strVec[i]);
				}
				if (i == 2) {
					tdb.talkData.importance = stoi(strVec[i]);
					load = 1;
					break;
				}
			}
			else if (load == 1) {

				if (i == 0) {
					if (strVec[i] == "*end") {
						load = 2;
						break;
					}
				}

				switch (i) {
				case 0:
					mes.speakPeople = stoi(strVec[i]);
					break;
				case 1:
					mes.speakFace = stoi(strVec[i]);
					break;
				case 2:
					mes.waitFrame = stoi(strVec[i]);
					break;
				case 3:
					mes.lastWaitFrame = stoi(strVec[i]);
					break;
				case 4:
					mes.message.allMessage = strVec[i];
					mes.message.hiddenMessage = strVec[i];
					mes.message.displayedMessage = "";
					break;
				default:
					break;
				}


			}

		}

		if (load == 2) {

			load = 0;
			tdb.talkData.isStartMessage = true;
			tdb.TalkType = type;

			if (!tdb.talkData.messageData.empty()) {
				talkDatabase.push_back(tdb);
				tdb.talkData.messageData.clear();
			}
		}

		if (load == 1 && mes.message.allMessage.empty() == 0) {

			std::regex regex("\\\\n");
			mes.message.allMessage = std::regex_replace(mes.message.allMessage, regex, "\n");
			mes.message.hiddenMessage = std::regex_replace(mes.message.hiddenMessage, regex, "\n");

			tdb.talkData.messageData.push_back(mes);
		}

		


	}

	// ファイルを閉じる
	FileRead_close(FileHandle);

	/*
    while (getline(ifs, str)) {
        std::string::size_type index = str.find("#");  // "#"を検索
                                                       //"#"が入っていた行は飛ばす
        if (index != std::string::npos) {
            continue;
        }

        index = str.find("*talkNum");  // "*talkNum"を検索

        if (index == std::string::npos && load == 0) {
            continue;                       //"*talkNum"が入っていない行は飛ばす
        }

        std::string token;
        std::istringstream stream(str);

        MessageData mes;

        int num = 0;

        //1行のうち、文字列とコンマを分割する
        while (getline(stream, token, ',')) {

            if (load == 0) {
                if (num == 1) {
                    tdb.TalkGroupNum = stoi(token);
                }
                if (num == 2) {
                    tdb.talkData.importance = stoi(token);
                    load = 1;
                    break;
                }
            }
            else if (load == 1) {

                if (num == 0) {
                    if (token == "*end") {
                        load = 2;
                        break;
                    }
                }

				switch (num) {
				case 0:
					mes.speakPeople = stoi(token);
					break;
				case 1:
					mes.speakFace = stoi(token);
					break;
				case 2:
					mes.waitFrame = stoi(token);
					break;
				case 3:
					mes.lastWaitFrame = stoi(token);
					break;
				case 4:
					mes.message.allMessage = token;
					mes.message.hiddenMessage = token;
					mes.message.displayedMessage = "";
					break;
				default:
					break;
				}


            }

            num++;

        }

        if (load == 2) {

            load = 0;
            tdb.talkData.isStartMessage = true;
            tdb.TalkType = type;

			if (!tdb.talkData.messageData.empty()) {
				talkDatabase.push_back(tdb);
				tdb.talkData.messageData.clear();
			}
        }

        if (load == 1 && mes.message.allMessage.empty() == 0) {

			std::regex regex("\\\\n");
			mes.message.allMessage = std::regex_replace(mes.message.allMessage, regex, "\n");
			mes.message.hiddenMessage = std::regex_replace(mes.message.hiddenMessage, regex, "\n");

            tdb.talkData.messageData.push_back(mes);
        }
    }
	*/

    return tdb;
}

void MessageManager::Init()
{
    count = 0;

    lastMessage = false;

    push = false;

    autoMessage = true;

    TalkStorage.clear();

    talkDatabase.clear();
    talkDatabase.resize(0);
}


void MessageManager::Update() {

    
    if (TalkStorage.empty()) {
        count = 0;
        lastMessage = false;
        return; //TalkStorageが空なら何もしない
    }
    else {

        count++;
        if (count > TalkStorage[0].messageData[0].waitFrame && lastMessage == false) {
            count = 0;

            //単バイト文字
            if (IsDBCSLeadByte(TalkStorage[0].messageData[0].message.hiddenMessage[0]) == 0) {
                std::string c = (TalkStorage[0].messageData[0].message.hiddenMessage).substr(0, 1).c_str();
                TalkStorage[0].messageData[0].message.displayedMessage += c;
                TalkStorage[0].messageData[0].message.hiddenMessage.erase(0, 1);
            }

            //マルチバイト文字
            else {
                std::string c = (TalkStorage[0].messageData[0].message.hiddenMessage).substr(0, 2);
                TalkStorage[0].messageData[0].message.displayedMessage += c;
                TalkStorage[0].messageData[0].message.hiddenMessage.erase(0, 2);
            }

            //未表示メッセージが全て読み込まれたか判定
            if (TalkStorage[0].messageData[0].message.hiddenMessage.empty()) {
                lastMessage = true;
            }
            else {
                lastMessage = false;
            }
        }

        if (count > TalkStorage[0].messageData[0].waitFrame && lastMessage == true) {

            if (autoMessage == true) {
                if (count > TalkStorage[0].messageData[0].lastWaitFrame) {
                    if (TalkStorage[0].messageData[0].message.hiddenMessage.empty()) {
                        lastMessage = false;
                        TalkStorage[0].messageData.erase(TalkStorage[0].messageData.begin());
                        TalkStorage[0].isStartMessage = false;
                        count = 0;

                        if (TalkStorage[0].messageData.empty()) {
                            TalkStorage.erase(TalkStorage.begin());
                        }

                    }
                }
            }
            else{
                if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
                {
                    if (push == false) {
                        // 押されている
                        push = true;
                        if (TalkStorage[0].messageData[0].message.hiddenMessage.empty()) {
                            lastMessage = false;
                            TalkStorage[0].messageData.erase(TalkStorage[0].messageData.begin());

                            count = 0;

                            if (TalkStorage[0].messageData.empty()) {
                                TalkStorage.erase(TalkStorage.begin());
                            }

                        }

                    }
                }
                else
                {
                    // 押されていない
                    push = false;
                }
            }

            
            
            
        }

    }


}

void MessageManager::Draw(Vector2D pos) {

    if (TalkStorage.empty()) {
        count = 0;
        return; //TalkStorageが空なら何もしない
    }
    else {

        //auto speakNum = TalkStorage[0].messageData[0].speakPeople;
        //auto faceNum = TalkStorage[0].messageData[0].speakFace;
        //Debug::DrawString(pos, "%d番の人が%dの表情で話しています");
		Debug::DrawString(pos + Vector2D(0, 50), TalkStorage[0].messageData[0].message.displayedMessage, Color4(0, 0, 0, 1), 24);
    }

}

void MessageManager::IntoTalkStorage(std::string talkType, int talkNum) {

    Talk_Type tt;

    if (talkType == "nomal") {
        tt = nomal;
    }
    else if (talkType == "dynamic") {
        tt = dynamic;
    }


    std::vector<TalkDatabase>::iterator itr1;

    for (itr1 = talkDatabase.begin(); itr1 != talkDatabase.end(); itr1++) {

        if (itr1->TalkType == tt && itr1->TalkGroupNum == talkNum) {
            break;
        }

    }

    if (itr1 == talkDatabase.end()) {
        return;
    }
    

    if (TalkStorage.empty() == 0) {


        std::vector<TalkData>::iterator itr2;

        for (itr2 = TalkStorage.begin(); itr2 != TalkStorage.end(); itr2++) {
            if (itr1->talkData.importance > itr2->importance) {
                if (itr2->isStartMessage == false) {
                    TalkStorage.erase(TalkStorage.begin());
                    TalkStorage.insert(TalkStorage.begin(), itr1->talkData);
                    count = 0;
                    lastMessage = false;
                    itr2 = TalkStorage.begin();
                    break;
                }
                else if (itr2->messageData[0].message.displayedMessage.empty() == 0) {
                    TalkStorage.erase(TalkStorage.begin());
                    TalkStorage.insert(TalkStorage.begin(), itr1->talkData);
                    count = 0;
                    lastMessage = false;
                    itr2 = TalkStorage.begin();
                    break;
                }else{
                    TalkStorage.insert(itr2, itr1->talkData);
                    itr2 = TalkStorage.begin();
                    break;
                }
            }
        }

        if (itr2 == TalkStorage.end()) {
            TalkStorage.push_back(itr1->talkData);
        }

    }
    else {
        TalkStorage.push_back(itr1->talkData);
    }

    

}

void MessageManager::IntoTalkStorage(std::string talkType, int talkNum, std::string* str, int strSize) {


    Talk_Type tt;

    if (talkType == "nomal") {
        tt = nomal;
    }
    else if (talkType == "dynamic") {
        tt = dynamic;
    }


    std::vector<TalkDatabase>::iterator itr1;

    for (itr1 = talkDatabase.begin(); itr1 != talkDatabase.end(); itr1++) {

        if (itr1->TalkType == tt && itr1->TalkGroupNum == talkNum) {
            break;
        }

    }

    if (itr1 == talkDatabase.end()) {
        return;
    }

    TalkDatabase tdb = (*itr1);

    std::string::size_type index;

    for (int i = 0; i < strSize; i++) {
        for (int j = 0; j < tdb.talkData.messageData.size(); j++) {
            index = tdb.talkData.messageData[j].message.allMessage.find("<%=%>");
            if (index != std::string::npos) {
                tdb.talkData.messageData[j].message.allMessage.replace(index, 5, str[i]);
                tdb.talkData.messageData[j].message.hiddenMessage.replace(index, 5, str[i]);
                break;
            }
        }
        if (index == std::string::npos) {
            break;
        }
    }


    if (TalkStorage.empty() == 0) {


        std::vector<TalkData>::iterator itr2;

        for (itr2 = TalkStorage.begin(); itr2 != TalkStorage.end(); itr2++) {
            if (tdb.talkData.importance > itr2->importance) {
                if (itr2->isStartMessage == false) {
                    TalkStorage.erase(TalkStorage.begin());
                    TalkStorage.insert(TalkStorage.begin(), tdb.talkData);
                    count = 0;
                    lastMessage = false;
                    itr2 = TalkStorage.begin();
                    break;
                }
                else if (itr2->messageData[0].message.displayedMessage.empty() == 0) {
                    TalkStorage.erase(TalkStorage.begin());
                    TalkStorage.insert(TalkStorage.begin(), tdb.talkData);
                    count = 0;
                    lastMessage = false;
                    itr2 = TalkStorage.begin();
                    break;
                }
                else {
                    TalkStorage.insert(itr2, tdb.talkData);
                    itr2 = TalkStorage.begin();
                    break;
                }
            }
        }

        if (itr2 == TalkStorage.end()) {
            TalkStorage.push_back(tdb.talkData);
        }

    }
    else {
        TalkStorage.push_back(tdb.talkData);
    }


}






bool MessageManager::isEmpty() {
    if (TalkStorage.empty()) {
        return true;
    }
    else {
        return false;
    }
}


void MessageManager::setAutoMessage(bool a) {
    autoMessage = a;
}

bool MessageManager::getAutoMessage() {
    return autoMessage;
}





