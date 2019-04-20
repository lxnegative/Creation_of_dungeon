#include <string>
#include "cd_666s/Render/Sprite.h"
#include "cd_666s/Sound/Sound.h"
#include "Tutorial.h"

#pragma once
class TutorialPanel
{
public:
	TutorialPanel(std::string timingName);
	~TutorialPanel();


	void Update();

	bool ClickCheck(Vector2D pos, Vector2D size);

	bool GetisTutorial() { return isTutorial; }
	std::string GetTimingName() { return timingName; }

private:

	std::vector<std::shared_ptr<ImageResource>> tutorialSlideList;

	int nowPageNum;
	std::string timingName;

	Sprite tutorialSlide;
	Sprite okButton;

	Sprite backButton;
	Sprite nextButton;

	Sound _decideSE;

	bool isTutorial;

};

