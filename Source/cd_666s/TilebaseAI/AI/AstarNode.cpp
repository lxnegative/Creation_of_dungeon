#include "AstarNode.h"
#include <math.h>

const int AstarNode::MAX_SCORE = 999;

AstarNode::AstarNode(TiledVector pos)
: _state(STATE::NONE)
, _cost(MAX_SCORE)
, _heuristicCost(MAX_SCORE)
, _score(MAX_SCORE)
, _parent(nullptr)
, _pos(pos)
{
}


void AstarNode::Init()
{
    _state = STATE::NONE;
    _cost = MAX_SCORE;
    _heuristicCost = MAX_SCORE;
    _score = MAX_SCORE;
    _parent = nullptr;
}


void AstarNode::Open(AstarNode *parentNode, AstarNode *aimNode)
{
    //Open�ł��Ȃ�������I��
    if (!IsReady())
        return;
    
    _parent = parentNode;
    
    //���R�X�g
    _cost = (_parent == nullptr) ? 0 : _parent->_cost + 1;
    
    //����R�X�g(�^�e + ���R)
    _heuristicCost = (aimNode->GetChipPos() - _pos).GetBresenhamLength(false);
    
    //�ŏI�I�ȃR�X�g
    _score = _cost + _heuristicCost;
    
    _state = STATE::OPEN;
}


bool AstarNode::IsReady() const
{
    return (_state == STATE::NONE);
}


bool AstarNode::IsClosed() const
{
    return (_state == STATE::CLOSED);
}


void AstarNode::Close()
{
    _state = STATE::CLOSED;
}


AstarNode::~AstarNode()
{
}