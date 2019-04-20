#include "DungeonTimer.h"
#include "../DebugDraw.h"

DungeonTimer::DungeonTimer()
	: _count(0)
	, _waveInterval(0)
	, _halfSE(Main::RESOURCEFILENAME + "resource/sound/time_half1.wav")
	, _littleSE(Main::RESOURCEFILENAME + "resource/sound/time_little1.wav")
	, _endSE(Main::RESOURCEFILENAME + "resource/sound/game_end.wav")
	, _clock(Main::RESOURCEFILENAME + "resource/graph/ui/ClockDaytime.png")
	, _hand(Main::RESOURCEFILENAME + "resource/graph/ui/ClockHand.png")
	, _position(754, 248)
{
	_halfSE.SetVolume(200);
	_littleSE.SetVolume(200);
	_endSE.SetVolume(200);

	_clock.SetPosition(Vector2D::zero);
	_clock.SetRenderType(Texture2D::RenderType::UI);

	_hand.SetPosition(Vector2D(0, -110));
	_hand.SetRenderType(Texture2D::RenderType::UI);
	_hand.GetTexturePtr().lock()->SetAnchorType(Texture2D::AnchorType::CENTER);
}


DungeonTimer::~DungeonTimer()
{
}

void DungeonTimer::NightSetting() {

	_clock.Load(Main::RESOURCEFILENAME + "resource/graph/ui/ClockNight.png");
	_clock.SetPosition(Vector2D::zero);
	_clock.SetRenderType(Texture2D::RenderType::UI);

	_hand.SetPosition(Vector2D(0, -110));
	_hand.SetRenderType(Texture2D::RenderType::UI);
	_hand.GetTexturePtr().lock()->SetAnchorType(Texture2D::AnchorType::CENTER);

}

void DungeonTimer::SetDisplayMode(bool active) {

	_clock.SetDisplayMode(active);
	_hand.SetDisplayMode(active);

}


void DungeonTimer::InitWithSetup(long waveTime)
{
	_count = 0;

	_waveInterval = waveTime;

	//�Q�[�����Ԋ��Z�ŕb�P�ʂɕϊ�
	_waveInterval *= 60;

	_hand.Rotate(180 + 80);
}


void DungeonTimer::Update()
{
	//���ԂɂȂ����珉����
	if (_waveInterval < _count)
	{
		return;
	}
	else
	{
		_count++;
	}

	auto timeRatio = GetTimeRatio();

	if (timeRatio == 0.5)
	{
		if (!_halfSE.IsPlaying())
			_halfSE.Play();

		return;
	}

	if (timeRatio == 0.8)
	{
		if (!_littleSE.IsPlaying())
			_littleSE.Play();

		return;
	}

	if (timeRatio == 1.0)
	{
		if (!_endSE.IsPlaying())
			_endSE.Play();

		return;
	}
}


void DungeonTimer::Draw()
{
	//�c�莞�ԃf�o�b�O�\��
	std::string timerStr = "Time:";
	//Debug::DrawString(_position + Vector2D(20, 20), timerStr);
	timerStr = std::to_string((_waveInterval - _count) / 60);
	timerStr += "/";
	timerStr += std::to_string(_waveInterval / 60);
	//Debug::DrawString(_position + Vector2D(40, 40), timerStr);

	auto offset = 10;
	auto timeRatio = static_cast<double>(_count - _waveInterval) / static_cast<double>(_waveInterval);
	_hand.Rotate(180 + timeRatio * (90 - offset));
}