#include "TutorialPanel.h"
#include "Tutorial.h"
#include "cd_666s/Resources/ResourceManager.h"
#include "cd_666s/Render/Texture2D.h"
#include "cd_666s/InputManager/InputManager.h"

TutorialPanel::TutorialPanel(std::string timingName)
{

	this->timingName = timingName;
	int pageNum = Tutorial::getInstance()->GetTutorialPageNum(timingName);

	for (int i = 1; i <= pageNum; i++) {

		tutorialSlideList.push_back(IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/tutorial/" + timingName + "_" + std::to_string(i) + ".PNG"));

	}

	nowPageNum = 1;
	isTutorial = true;

	tutorialSlide.SetResource(tutorialSlideList[nowPageNum - 1]);
	tutorialSlide.SetRenderType(Texture2D::RenderType::UI);
	tutorialSlide.SetPriority(Sprite::Priority::UI);
	tutorialSlide.SetPosition(Vector2D((1280 / 2) - (tutorialSlide.GetSize()._x / 2), (720 / 2) - (tutorialSlide.GetSize()._y / 2)));

	okButton.Load(Main::RESOURCEFILENAME + "resource/graph/tutorial/UI/OKButton.png");
	okButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	okButton.SetRenderType(Texture2D::RenderType::UI);
	okButton.SetPosition(Vector2D((1280 / 2) - (okButton.GetSize()._x / 2), (720 / 2) - (okButton.GetSize()._y / 2) + 575 / 2 - 35));

	backButton.Load(Main::RESOURCEFILENAME + "resource/graph/tutorial/UI/backButton.png");
	backButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	backButton.SetRenderType(Texture2D::RenderType::UI);
	backButton.SetPosition(Vector2D((1280 / 2) - (backButton.GetSize()._x / 2) - (backButton.GetSize()._x / 1.6), (720 / 2) - (backButton.GetSize()._y / 2) + 575 / 2 - 35));
	backButton.SetDisplayMode(false);

	nextButton.Load(Main::RESOURCEFILENAME + "resource/graph/tutorial/UI/nextButton.png");
	nextButton.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
	nextButton.SetRenderType(Texture2D::RenderType::UI);
	nextButton.SetPosition(Vector2D((1280 / 2) - (nextButton.GetSize()._x / 2) + (nextButton.GetSize()._x / 1.6), (720 / 2) - (nextButton.GetSize()._y / 2) + 575 / 2 - 35));
	nextButton.SetDisplayMode(false);

	_decideSE.Load(Main::RESOURCEFILENAME + "resource/sound/decide.wav");
	_decideSE.SetVolume(200);

}

TutorialPanel::~TutorialPanel()
{
}


void TutorialPanel::Update() {

	if (!isTutorial) {
		return;
	}

	if (timingName != "List/page") {
		okButton.SetDisplayMode(true);
		backButton.SetDisplayMode(false);
		nextButton.SetDisplayMode(false);
		if (ClickCheck(okButton.GetPosition(), okButton.GetTexturePtr().lock()->GetBaseSize())) {
			_decideSE.Play();
			nowPageNum++;
			if (Tutorial::getInstance()->GetTutorialPageNum(timingName) < nowPageNum) {
				isTutorial = false;
				tutorialSlide.SetDisplayMode(false);
				okButton.SetDisplayMode(false);
				backButton.SetDisplayMode(false);
				nextButton.SetDisplayMode(false);
				return;
			}

			tutorialSlide.SetResource(tutorialSlideList[nowPageNum - 1]);

		}
	}
	else {
		okButton.SetDisplayMode(false);
		backButton.SetDisplayMode((nowPageNum > 1));
		nextButton.SetDisplayMode(Tutorial::getInstance()->GetTutorialPageNum(timingName) > nowPageNum);

		if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L)) {
			if (!ClickCheck(tutorialSlide.GetPosition(), tutorialSlide.GetTexturePtr().lock()->GetBaseSize())) {

				isTutorial = false;
				tutorialSlide.SetDisplayMode(false);
				okButton.SetDisplayMode(false);
				backButton.SetDisplayMode(false);
				nextButton.SetDisplayMode(false);
				return;

			}
		}
		
		if (nowPageNum > 1 && ClickCheck(backButton.GetPosition(), backButton.GetTexturePtr().lock()->GetBaseSize())) {

			nowPageNum--;

			tutorialSlide.SetResource(tutorialSlideList[nowPageNum - 1]);

		}
		else if (Tutorial::getInstance()->GetTutorialPageNum(timingName) > nowPageNum && ClickCheck(nextButton.GetPosition(), nextButton.GetTexturePtr().lock()->GetBaseSize())) {

			nowPageNum++;

			tutorialSlide.SetResource(tutorialSlideList[nowPageNum - 1]);

		}

	}


}



bool TutorialPanel::ClickCheck(Vector2D pos, Vector2D size) {

	if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
		return false;

	//クリック位置を取得
	auto cursorPos = MOUSE->GetCursorPos();

	if (cursorPos._x < pos._x)
		return false;
	if (cursorPos._y < pos._y)
		return false;
	if (pos._x + size._x < cursorPos._x)
		return false;
	if (pos._y + size._y < cursorPos._y)
		return false;

	return true;

}