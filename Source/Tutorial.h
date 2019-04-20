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

	//�g���\���̂��ȉ���
	struct Tutorial_Timing {   //�`���[�g���A���̕\���^�C�~���O
		std::string TimingName;   //�^�C�~���O�̖��O�i�摜���ƃ����N���Ă�j
		bool islooking;             //�������Ƃ����邩
		int pageNum;             //�y�[�W��
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

