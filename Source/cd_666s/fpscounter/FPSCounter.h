#pragma once
#include <vector>
#include <chrono>

class FPSCounter
{
public:
    FPSCounter(int samplingFrame);
    ~FPSCounter();

    void Update();

    int GetCountingFrame() const { return _countingFrame; }
    double GetFPS() const { return _avarageFPS; }
    static double GetDeltaTime() { return _deltaTime; }

private:

    //�ړ����ς�K�p����t���[����
    int _samplingFrame;
    //�v���J�n���獡�܂ł̃t���[����
    int _countingFrame;

    //�\������FPS
    long double _avarageFPS;
    //���t���[�����J�E���g�����l�̑��a
    long double _totalTime;
    //�O�t���[������̌o�ߎ���
    static long double _deltaTime;

    //�O�t���[���ł̎���
    std::chrono::system_clock::time_point _beforeTime;
    //���݃t���[���ł̎���
    std::chrono::system_clock::time_point _currentTime;
};

