#pragma once
#include "PathFindingAIBase.h"
#include "../TiledVector.h"
#include <vector>

/*
 ��{�ړ�  : �����ړ�
 �I�񃂁[�h: �����_���ړ�
 */

class Character;
class TiledObject;

class RandomMovement : public PathFindingAIBase
{
public:
    RandomMovement(Character &owner, TiledObject *target, bool diagonal, std::vector<TiledVector> &pathRef);
    ~RandomMovement();
    
    void Reset() override;
    void Update() override;
    
private:
    
    //�ʏ�ړ�
    void MoveToTarget();
    //��Q�����
    void AvoidObstacle();
    
    //��Q���ɂ��ړI�n�ɐi�߂Ȃ������`�F�b�N
    bool IsPreventedFrom();
    
    bool _diagonal;
    bool _isAvoiding;
    
    TiledObject* _target;
};

