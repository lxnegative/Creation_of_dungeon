#include "FPSCounter.h"
using namespace std::chrono;

long double FPSCounter::_deltaTime = 0.0;

FPSCounter::FPSCounter(int samplingFrame)
    : _samplingFrame(samplingFrame)
    , _countingFrame(0)
    , _totalTime(0)
    , _avarageFPS(0)
{
}


FPSCounter::~FPSCounter()
{
}


void FPSCounter::Update()
{
    //�O�t���[�����炱���܂ł̎��Ԃ�micro�b�Ōv��
    _currentTime = system_clock::now();
    auto delta = duration<long double, std::micro>(_currentTime - _beforeTime).count();
    _deltaTime = delta / 1000000; 
    _totalTime += delta;

    //�v�����ԍX�V
    _beforeTime = _currentTime;

    //�t���[���J�E���g�X�V
    _countingFrame++;

    //�T���v�����O���܂ŒB������Čv�Z
    if (_countingFrame == _samplingFrame)
    {
        //1�t���[��������̎��Ԃ��o����
        long double avarageTime = (_totalTime / _samplingFrame);
        //��(�}�C�N��) = 1.000.000
        _avarageFPS = 1000000 / (avarageTime);

        //�t���[���J�E���^�����Z�b�g
        _countingFrame = 0;
        _totalTime = 0;

        return;
    }
}