#pragma once
#include "Trap.h"
#include "../../mw_animation/GraphArray.h"

class Enemy;

class MineBomb : public Trap
{
public:
    MineBomb(TiledVector trap, int cost, int range, int power, int attack, int stuckTime, bool mine, std::string graphName);
    ~MineBomb();

    void Draw() override;
    void Activate() override;
    bool IsActivatable() const override;

    static std::shared_ptr<MineBomb> Create(std::string data, TiledVector pos);

private:

    // TODO : ������bareTrap�ƃN���X��������
    bool _isMine;
    Sound _explodeSounde;

    //�����͈�
    int _range;

    //�����ɂ��_���[�W
    int _power;
    int _attack;

    //�d������
    int _stuckedTime;

    GraphArray _graphArray;

    mutable Enemy* _targetCache;
};

