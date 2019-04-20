#ifndef ButtonInputData_h
#define ButtonInputData_h

#include "ButtonTypeInput.h"
#include "ButtonTypeInputEvent.h"


struct ButtonInputData
{
    ButtonInputData(int value)
    : _inputCode(value)
    , _inputEvents(&_inputData)
    {
    }
    
    //API�ɗp�ӂ���Ă�L�[���͂̒l
    int _inputCode;
    //�{�^���̓��͏��
    ButtonTypeInput _inputData;
    //���͏�ԂɊ�Â����C�x���g
    ButtonTypeInputEvent _inputEvents;
};

#endif /* ButtonInputData_h */
