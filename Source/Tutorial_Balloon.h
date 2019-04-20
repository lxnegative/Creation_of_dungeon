#include <string>
#include <vector>
#include "cd_666s/Render/Sprite.h"
#include "cd_666s/DebugDraw.h"
#include "PopUpWindow_Click_Go_To_Next.h"
#include "cd_666s/TilebaseAI/TileField.h"

#pragma once
class Tutorial_Balloon
{
private:
	Sprite balloon;
	PopUpWindow_Click_Go_To_Next  popUpWindow;

	Vector2D spotLightPoint;
	Vector2D spotLightSize;

	Sprite SpotImage1;
	Sprite SpotImage2;
	Sprite SpotImage3;
	Sprite SpotImage4;

	Sprite BlackImage;

	Sprite Spot_Circle;

	bool isSpot_Circle;

	std::string timingName;

	enum class TutorialType
	{
		Edit_1_1 = 0,
		Battle_1_1,
		Edit_1_2,
		Edit_1_4,
		Battle_1_4
	};

	TutorialType tutorialType;

	int stepNum;

	bool isFinish;

	bool ControlOrder;
	bool isControlStop;

	int WaitCount;


public:
	Tutorial_Balloon();
	Tutorial_Balloon(std::string timingName);
	~Tutorial_Balloon();

	bool GetIsFinish() {
		return isFinish;
	}
	bool GetIsControlStop() {
		return isControlStop;
	}

	std::string GetTimingName() {
		return timingName;
	}

	bool Update();
	void Draw();

	void SpotLightDraw();

	void Tutorial_Init_Edit_1_1(int step);
	bool Tutorial_Update_Edit_1_1(int step);

	void Tutorial_Init_Battle_1_1(int step);
	bool Tutorial_Update_Battle_1_1(int step);

	void Tutorial_Init_Edit_1_2(int step);
	bool Tutorial_Update_Edit_1_2(int step);

	void Tutorial_Init_Edit_1_4(int step);
	bool Tutorial_Update_Edit_1_4(int step);

	void Tutorial_Init_Battle_1_4(int step);
	bool Tutorial_Update_Battle_1_4(int step);

};

