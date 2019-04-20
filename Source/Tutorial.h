#include <string>
#include <vector>
#include "Main.h"

#pragma once
class Tutorial
{

public:

	Tutorial(const Tutorial&) = delete;
	Tutorial& operator=(const Tutorial&) = delete;
	Tutorial(Tutorial&&) = delete;
	Tutorial& operator=(Tutorial&&) = delete;

	static Tutorial* getInstance() {
		static Tutorial inst;
		return &inst;
	}

	//使う構造体を以下に
	struct Tutorial_Timing {   //チュートリアルの表示タイミング
		std::string TimingName;   //タイミングの名前（画像名とリンクしてる）
		bool islooking;             //見たことがあるか
		int pageNum;             //ページ数
	};
	
	void Init();
	bool isLookingTutorial(std::string timingName);
	int GetTutorialPageNum(std::string timingName);
	void LookTutorial(std::string timingName);

	void NotTutorial();

	int SetTutorialPageNum(std::string str);

	bool CheckFileExistence(const std::string& str);

	void SetTutorialTimingList(std::vector<Tutorial_Timing> t) {
		tutorialTimingList = t;
	}
	std::vector<Tutorial_Timing> GetTutorialTimingList() {
		return tutorialTimingList;
	}

private:
	Tutorial() = default;
	~Tutorial() = default;

	
	std::vector<Tutorial_Timing> tutorialTimingList;


};

