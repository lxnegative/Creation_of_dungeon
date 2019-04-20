#include "DestroyedCounter.h"
#include "./cd_666s/DebugDraw.h"


DestroyedCounter::DestroyedCounter()
	: _count(0)
	, _all_enemy(0)
	, _grave(Main::RESOURCEFILENAME + "resource/graph/ui/DeadMansNumVer2.png")
	, _hand(Main::RESOURCEFILENAME + "resource/graph/ui/ClockHand.png")
	, _position(20, 48)
{
	_hand.SetDisplayMode(false);

	_grave.SetPosition(Vector2D(_position));
	_grave.SetRenderType(Texture2D::RenderType::UI);
	_grave.SetScale(Vector2D(0.8, 0.8));

	_hand.SetPosition(Vector2D(0, -110));
	_hand.SetRenderType(Texture2D::RenderType::UI);
	_hand.GetTexturePtr().lock()->SetAnchorType(Texture2D::AnchorType::CENTER);
}

DestroyedCounter::~DestroyedCounter()
{
}

void DestroyedCounter::InitWithSetup(int leftEnemy)
{
	_count = 0;

	_all_enemy = leftEnemy;

	//_hand.Rotate(-10);
	_hand.Rotate(80);
}

void DestroyedCounter::SetDisplayMode(bool active) {

	_grave.SetDisplayMode(active);
	_hand.SetDisplayMode(active);

}

void DestroyedCounter::Init()
{
	_grave.SetPosition(Vector2D::zero);
	_grave.SetRenderType(Texture2D::RenderType::UI);

	_hand.SetPosition(Vector2D::zero);
	_hand.SetRenderType(Texture2D::RenderType::UI);
	_hand.GetTexturePtr().lock()->SetAnchorType(Texture2D::AnchorType::UPPER_LEFT);

}

void DestroyedCounter::Update(int count)
{
	_count = count;
}

void DestroyedCounter::Draw()
{
	//残り時間デバッグ表示
/*	std::string timerStr = "Left:";
	Debug::DrawString(_position + Vector2D(20, 20), timerStr);
	timerStr = std::to_string((_all_enemy - _count) / 60);
	timerStr += "/";
	timerStr += std::to_string(_all_enemy / 60);
	Debug::DrawString(_position + Vector2D(40, 40), timerStr);*/
	/*
		auto offset = 80;
		auto timeRatio = static_cast<double>(_count - _all_enemy) / static_cast<double>(_all_enemy);
		_hand.Rotate(timeRatio * (90 - offset));
		*/

	/*
	std::string leftStr = "倒した数";
	Debug::DrawString(_position, leftStr);
	leftStr = std::to_string(_count) + "/" + std::to_string(_all_enemy);
	Debug::DrawString(_position + Vector2D(10, 20), leftStr, ColorPalette::BLACK4, 20);
	*/
	Debug::DrawString_TopPriority(_position + Vector2D(130 * 0.8, 40 * 0.8), std::to_string(_count), ColorPalette::BLACK4, 30 * 0.8);
	Debug::DrawString_TopPriority(_position + Vector2D(170 * 0.8, 84 * 0.8), std::to_string(_all_enemy), ColorPalette::BLACK4, 30 * 0.8);


	auto offset = 10;
	auto timeRatio = static_cast<double>(_count - _all_enemy) / static_cast<double>(_all_enemy);
	_hand.Rotate(timeRatio * (90 - offset));
}
