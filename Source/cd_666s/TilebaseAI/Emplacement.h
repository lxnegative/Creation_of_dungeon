#pragma once
#include "Trap.h"
#include "../../mw_animation/GraphArray.h"

class Enemy;

class Emplacement : public Trap
{
public:
    Emplacement(TiledVector pos, int cost, int power, int attack, TiledVector::Direction direction, std::string graphName, std::string arrowGraphName,bool isPhysical);
    ~Emplacement();

    void Activate() override;
    bool IsActivatable() const override;
    bool Contain(Vector2D pos) const override;

	bool IsOverwritable(TiledObject* overwriter) override;

    static std::shared_ptr<Emplacement> Create(std::string data, TiledVector pos);

	TiledVector::Direction* GetDirection() override { return &_shootDirection; };
	void SetDirection(TiledVector::Direction dir) override;

private:

    //�U����
    int _attack;

    //�З�
    int _power;

    //�U������
    TiledVector::Direction _shootDirection;

    //�U�������ɑΉ������摜
    //GraphArray _directedGraph;

    //�ł��o�����̂̉摜
    std::shared_ptr<ImageResource> _bulletImage;

	std::vector<std::shared_ptr<ImageResource>> _directedbulletImage;
	std::vector<std::shared_ptr<ImageResource>> _directedGraph;

	bool isPhysical;
};

