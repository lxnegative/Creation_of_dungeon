#pragma once
#include "TiledObject.h"
#include "../Sound/Sound.h"

class GraphArray;

class Trap : public TiledObject
{
public:
    Trap(TiledVector startPos, int duravity, int level);
    ~Trap();

    virtual void Update() override;
    virtual void Draw() override;

    bool IsBrakeable(int brokerLevel) const { return _level <= brokerLevel; };

    //�\������ł���I�u�W�F�N�g����Ăяo�����֐�
    virtual void Interact(Character& character) override {};

    //���̃I�u�W�F�N�g�̏�ɕʂ̃I�u�W�F�N�g�����邩�ǂ���
    //overwriter : ��ɏ�낤�Ƃ���I�u�W�F�N�g
    virtual bool IsOverwritable(TiledObject* overwriter) override { return true; };

    //�A�C�e���擾�ł̏����⏢�����ꂽ���ǂ����ȂǁA
    //�I�u�W�F�N�g�͐������Ă�����ɗp���邩�ǂ����𔻒f����̂ɗp����.
    virtual bool IsEnable() const { return (_cost <= _duravity); }

    //�g���b�v���N���ł��邩�ǂ���
    virtual bool IsActivatable() const;

    //�g���b�v���N��
    virtual void Activate();

    static void CreateTrap(std::string typeName, int countX, int countY, std::vector<std::shared_ptr<TiledObject>>& objects);

	virtual TiledVector::Direction* GetDirection() { return nullptr; };
	virtual void SetDirection(TiledVector::Direction dir) {};
	
protected:

    static bool Contained(const GraphArray& animation, const Vector2D& pos);

    //�ϋv��
    int _duravity;

    //�������ɏ��Ղ����
    int _cost;

    //�j��\���������l
    int _level;

    Sound _activateSound;
};

