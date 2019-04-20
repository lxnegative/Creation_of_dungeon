#pragma once
#include "../TiledVector.h"
#include <vector>

class Character;

class PathFindingAIBase
{
public:
    PathFindingAIBase(Character &owner, std::vector<TiledVector>& pathRef);
    virtual ~PathFindingAIBase();

    virtual void Reset() = 0;
    virtual void Update() = 0;
    virtual void Draw();
    
protected:
    
    //�v�l������{��
    Character &_owner;
    
    //���g�̌o�H
    std::vector<TiledVector> &_pathRef;
};

