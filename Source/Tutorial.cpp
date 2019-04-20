#include "Tutorial.h"
#include "DxLib.h"
#include <iostream>
#include <fstream>


void Tutorial::Init() {

	tutorialTimingList.clear();

	Tutorial_Timing tt;

	tt.TimingName = "Stage1-1_Edit";
	tt.islooking = false;
	tt.pageNum = SetTutorialPageNum(tt.TimingName);

	tutorialTimingList.push_back(tt);

	tt.TimingName = "Stage1-1_Battle";
	tt.islooking = false;
	tt.pageNum = SetTutorialPageNum(tt.TimingName);

	tutorialTimingList.push_back(tt);

	tt.TimingName = "WorldMap";
	tt.islooking = false;
	tt.pageNum = SetTutorialPageNum(tt.TimingName);

	tutorialTimingList.push_back(tt);

	tt.TimingName = "Stage1-2_Edit";
	tt.islooking = false;
	tt.pageNum = SetTutorialPageNum(tt.TimingName);

	tutorialTimingList.push_back(tt);

	tt.TimingName = "Stage1-4_Edit";
	tt.islooking = false;
	tt.pageNum = SetTutorialPageNum(tt.TimingName);

	tutorialTimingList.push_back(tt);

	tt.TimingName = "Stage1-4_Battle";
	tt.islooking = false;
	tt.pageNum = SetTutorialPageNum(tt.TimingName);

	tutorialTimingList.push_back(tt);

	tt.TimingName = "Stage4-4_WorldMap";
	tt.islooking = false;
	tt.pageNum = SetTutorialPageNum(tt.TimingName);

	tutorialTimingList.push_back(tt);

	tt.TimingName = "List/page";
	tt.islooking = false;
	tt.pageNum = SetTutorialPageNum(tt.TimingName);

	tutorialTimingList.push_back(tt);

}

bool Tutorial::isLookingTutorial(std::string timingName) {

	for (int i = 0; i < tutorialTimingList.size(); i++) {
		if (timingName == tutorialTimingList[i].TimingName) {
			return tutorialTimingList[i].islooking;
		}
	}

	return true;

}

int Tutorial::GetTutorialPageNum(std::string timingName) {

	for (int i = 0; i < tutorialTimingList.size(); i++) {
		if (timingName == tutorialTimingList[i].TimingName) {
			return tutorialTimingList[i].pageNum;
		}
	}

	return -1;

}

void Tutorial::LookTutorial(std::string timingName) {

	for (int i = 0; i < tutorialTimingList.size(); i++) {
		if (timingName == tutorialTimingList[i].TimingName) {
			tutorialTimingList[i].islooking = true;
		}
	}

}


void Tutorial::NotTutorial() {

	for (int i = 0; i < tutorialTimingList.size(); i++) {
		tutorialTimingList[i].islooking = true;
	}

}


int Tutorial::SetTutorialPageNum(std::string str) {

	int pageNum = 0;

	while (true) {

		bool d = CheckFileExistence(Main::RESOURCEFILENAME + "resource/graph/tutorial/" + str + "_" + std::to_string(pageNum + 1) + ".PNG");
		
		if (d) {
			pageNum++;
		}
		else {
			break;
		}

	}

	return pageNum;

}


bool Tutorial::CheckFileExistence(const std::string& str)
{
	int a = FileRead_open(str.c_str());
	if (a != 0) {
		return true;
	}

	return false;
}