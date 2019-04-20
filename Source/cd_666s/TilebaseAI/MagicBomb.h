#pragma once
#include "Trap.h"
#include "../../mw_animation/GraphArray.h"

class MagicBomb : public Trap
{
public:
    MagicBomb(TiledVector trap, int cost, int range, int power, int attack, std::string graphName);
    ~MagicBomb();

    void Activate() override;
    bool IsActivatable() const override;

    static std::shared_ptr<MagicBomb> Create(std::string data, TiledVector pos);

private:

    //�����͈�
    int _range;

    //�����ɂ��_���[�W
    int _power;
    int _attack;

    std::shared_ptr<ImageResource> _image;
    GraphArray _graphArray;
};

