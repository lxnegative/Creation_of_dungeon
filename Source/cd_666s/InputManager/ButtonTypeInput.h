#pragma once

class ButtonTypeInput
{
public:
    ButtonTypeInput();
    ~ButtonTypeInput();
    
    //�O�t���[���ŉ����ꂸ�A���t���[���ŉ�����Ă��邩
    bool ButtonDown() const { return (_isPressing && !_beforeFramePressed); };
    //�O�t���[���ŉ�����A���t���[���ŗ����ꂽ��
    bool ButtonUp() const { return (!_isPressing && _beforeFramePressed); };
    //���t���[���ŉ�����Ă��邩
    bool ButtonBeingPressed() const { return _isPressing; };
    
    //���t���[���ŉ����ꑱ���ĉ��t���[���ڂ�
    int GetInputingFrame() const { return _inputingFrame; }
    
    //���͏�Ԃ��m�F
    void UpdateState(bool state);
    
private:
    
    //�{�^���������ꑱ���Ă�t���[����
    int _inputingFrame;
    //���t���[���ŉ�����Ă��邩
    bool _isPressing;
    //�O�t���[���ŉ�����Ă�����
    bool _beforeFramePressed;
};

