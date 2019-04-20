#include "RandomMovement.h"
#include "../Character.h"
#include "../TileField.h"
#include "../Utility.h"
#include <chrono>
#include <random>


RandomMovement::RandomMovement(Character &owner, TiledObject *target, bool diagonal, std::vector<TiledVector> &pathRef)
: PathFindingAIBase(owner, pathRef)
, _target(target)
, _isAvoiding(false)
, _diagonal(diagonal)
{
}


RandomMovement::~RandomMovement()
{
}


void RandomMovement::Reset()
{
    _isAvoiding = false;
}


void RandomMovement::Update()
{
    //
    if (_isAvoiding)
        AvoidObstacle();
    else
        MoveToTarget();
}


void RandomMovement::MoveToTarget()
{
    //����ȊO�͒P����LOS��1�}�X�������
    Bresenham(_target->GetTilePos(), _owner.GetTilePos(), _pathRef, _diagonal, 1);
    
    //�ǂ����ʂɂ�������p�^�[���؂�ւ�
    if (IsPreventedFrom())
    {
        _isAvoiding = true;
        return;
    }
}


void RandomMovement::AvoidObstacle()
{
    //�����_���ɒl�����o���A�l�ɂ���Đi�ޕ�����ς���
    TiledVector move;
    
    //0:�� 1:�E 2:�� 3:�� 4:���� 5;�E�� 6:�E�� 7:����
    std::random_device r;
    std::mt19937 value(r());
    
    int patternNum = (_diagonal) ? 8 : 4;
    int randomValue = value() % patternNum;
    
    if (randomValue == 0)
    {
        move.Set(0, -1);
    }
    else if (randomValue == 1)
    {
        move.Set(1, 0);
    }
    else if (randomValue == 2)
    {
        move.Set(0, 1);
    }
    else if (randomValue == 3)
    {
        move.Set(-1, 0);
    }
    else if (randomValue == 4)
    {
        move.Set(-1, -1);
    }
    else if (randomValue == 5)
    {
        move.Set(1, -1);
    }
    else if (randomValue == 6)
    {
        move.Set(1, 1);
    }
    else if (randomValue == 7)
    {
        move.Set(-1, 1);
    }
    
    _pathRef.push_back(_owner.GetTilePos() + move);
    _isAvoiding = false;
}


bool RandomMovement::IsPreventedFrom()
{
    for (auto pathTile : _pathRef)
    {
        //�o�H��ŁA�ǂ����ňړ��ł��Ȃ����Ƃ��킩������
        //�W�Q���ꂽ�Ƃ݂Ȃ�
        if (!FIELD->IsMovableThere(pathTile, _owner))
            return true;
    }
    
    return false;
}