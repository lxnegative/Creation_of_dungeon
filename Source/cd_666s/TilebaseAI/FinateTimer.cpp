#include "FinateTimer.h"


FinateTimer::FinateTimer(int interval, bool countOnStart, bool isLoop)
    : _interval(interval)
    , _isCounting(countOnStart)
    , _loop(isLoop)
    , _count(0)
{}


FinateTimer::~FinateTimer()
{}


void FinateTimer::Reset(int interval, bool countOnStart, bool isLoop)
{
    _interval = interval;
    _isCounting = countOnStart;
    _loop = isLoop;
}


void FinateTimer::Play()
{
    _isCounting = true;
}


void FinateTimer::Stop()
{
    _isCounting = false;
}


void FinateTimer::Update()
{
    if (!_isCounting)
        return;

    if (_count < _interval)
    {
        _count++;
        return;
    }

    //���ԂɂȂ�����J�E���g���Z�b�g
    _count = 0;

    //loop���Ȃ��Ȃ�J�E���g��~
    if (!_loop)
        _isCounting = false;
}


void FinateTimer::ResetCount(bool withStop)
{
    _count = 0;
    _isCounting = !withStop;
}