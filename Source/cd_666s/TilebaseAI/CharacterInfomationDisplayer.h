#pragma once
#include "ParameterEffecter.h"
#include "../Render/Sprite.h"
#include <memory>
#include <vector>

class CharacterInfomationDisplayer
{
    typedef std::vector<std::unique_ptr<ParameterEffecter>> Effecters;

public:
    CharacterInfomationDisplayer(Vector2D& positionRef, Effecters& effecters);
    ~CharacterInfomationDisplayer();

    void Update();

private:

    Vector2D& _characterPos;
    Effecters& _effecters;
    Sprite _statusUp, _statusDown;

    //�A�b�v�ƃ_�E���������������Ă����ꍇ�̃J�E���^
    int _displayCounter;
};

