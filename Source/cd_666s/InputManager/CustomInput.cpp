#include "CustomInput.h"
#include "ButtonTypeInput.h"
#include "../../Vector2D.h"

CustomInput::CustomInput()
{
    RegistRecords();
}



CustomInput::~CustomInput()
{
    Clear();
}


void CustomInput::RegistRecords()
{
    AddRecord(CustomInputType::BUTTON0, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_Z));
    AddRecord(CustomInputType::BUTTON1, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_X));
    AddRecord(CustomInputType::BUTTON2, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_C));
    //AddRecord(CustomInputType::BUTTON3, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_V));
    AddRecord(CustomInputType::BUTTON4, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_LSHIHT));
    AddRecord(CustomInputType::BUTTON5, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_SPACE));
    AddRecord(CustomInputType::BUTTON6, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_ARROW_LEFT));
    AddRecord(CustomInputType::BUTTON7, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_ARROW_RIGHT));
    AddRecord(CustomInputType::BUTTON8, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_ARROW_UP));
    AddRecord(CustomInputType::BUTTON9, KEYBOARD->GetButtonInputData(KeyInput::KeyType::KEY_ARROW_DOWN));
}


void CustomInput::AddRecord(CustomInputType type, ButtonInputData* data)
{
    auto input = new CustomInputData(type, data);
    _inputMap.insert(std::make_pair(type, input));
    _usingInputs.push_back(input);
}


void CustomInput::Clear()
{
    for (auto input : _usingInputs)
    {
        delete input;
    }
    _usingInputs.clear();
    _usingInputs.resize(0);
}


/*
 �Ή�������͂̎Q�Ɛ������������
 
 �Q�Ɛ�����������邾���ł����āA
 Event�ȂǎQ�ƑO�̓��͂Ɋւ���f�[�^�͕ێ�����Ȃ��B
 
 ��) 1. Z�L�[�̃C�x���g��BUTTON0�ɐݒ�
        ��
     2. BUTTON0�̎Q�Ɛ��V�L�[��
        ��
     3. ���̂Ƃ�Z�L�[�̃C�x���g�͎c�����܂܂Ȃ̂Œ���
 */
void CustomInput::ReplaceButtonCode(CustomInputType type, KeyInput::KeyType key)
{
    for (auto input : _usingInputs)
    {
        if (input->_type == type)
        {
            input->_origin = KEYBOARD->GetButtonInputData(key);
            return;
        }
    }
}