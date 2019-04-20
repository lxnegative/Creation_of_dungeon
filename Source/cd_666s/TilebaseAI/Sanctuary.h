#pragma once
#include "Trap.h"
#include "ParameterMultiplier.h"

class Sanctuary : public Trap
{
public:
    Sanctuary(TiledVector pos, int range, int cost, ParameterMultiplier multiplier, std::string graphName);
    ~Sanctuary();

    void Activate() override;
    bool IsActivatable() const override;

    static std::shared_ptr<Sanctuary> Create(std::string data, TiledVector pos);

private:

    //�����͈�
    int _range;

    //�p�����[�^�ϓ��l
    ParameterMultiplier _multiplier;

    Sound _sound;
};

