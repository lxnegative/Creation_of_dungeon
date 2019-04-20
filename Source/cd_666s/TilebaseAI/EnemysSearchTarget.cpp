#include "Enemy.h"
#include "EnemysSearchProcess.h"

using SearchFunc = std::function<void(Enemy&, std::vector<TiledObject*>& targets, const TiledVector& tilePos, const SearchPriorityNode::SearchingRange& type)>;

SearchPriorityNode CreateSearchListNode(
    int priority, 
    SearchPriorityNode::SearchingRange type, 
    SearchFunc searchFunc)
{
    SearchPriorityNode node;
    node._priority = priority;
    node._rangeType = type;
    node._searchFunc = searchFunc;
    return std::move(node);
}


TiledObject* Enemy::SearhTargetByRuleList()
{
    std::vector<TiledObject*> targets;
    auto tilePos = GetTilePos();

    if (_searchRuleList.size() == 0)
    {
        targets.push_back(&_baseTarget);
        return &_baseTarget;
    }

    int bfPriority = _searchRuleList[0]._priority;
    for (auto& rule : _searchRuleList)
    {
        //priority���؂�ւ�����画���
        if (bfPriority != rule._priority
            &&  0 < targets.size())
        {
            return GetPrimaryTarget<TiledObject>(targets, tilePos);
        }

		bfPriority = rule._priority;

        //�T���͂����ōs��
        rule._searchFunc(*this, targets, tilePos, rule._rangeType);
    }

    //�T���ׂ����̂������Ȃ���΃S�[����
    if (targets.size() == 0)
    {
        targets.push_back(&_baseTarget);
        return &_baseTarget;
    }
    else
    {
        return GetPrimaryTarget<TiledObject>(targets, tilePos);
    }
}


void Enemy::BuildSearchingRuleList()
{

	if (GetEnemyAIType() == EnemyAIType::Normal)
	{
		_searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<EnemysItem<Equipment>>))));
		_searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<EnemysItem<ConsumableItem>>))));
		_searchRuleList.push_back(std::move(CreateSearchListNode(3, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<Monster>))));

		std::sort(_searchRuleList.begin(), _searchRuleList.end(),
			[&](const SearchPriorityNode& lhs, const SearchPriorityNode& rhs)
		{
			return (lhs._priority > rhs._priority);
		});

		return;
	}

	//�X�L�������C����AI�̏ꍇ
	if (GetEnemyAIType() == EnemyAIType::SkillMain)
	{
		_searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<EnemysItem<Equipment>>))));
		_searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<EnemysItem<ConsumableItem>>))));

		std::sort(_searchRuleList.begin(), _searchRuleList.end(),
			[&](const SearchPriorityNode& lhs, const SearchPriorityNode& rhs)
		{
			return (lhs._priority > rhs._priority);
		});

		return;
	}

	//�V�[�t�̏ꍇ
	if (GetEnemyAIType() == EnemyAIType::Hunter)
	{
		_searchRuleList.push_back(std::move(CreateSearchListNode(3, SearchPriorityNode::SearchingRange::Field, std::mem_fn(&Enemy::SearchTarget<EnemysItem<Equipment>>))));
		_searchRuleList.push_back(std::move(CreateSearchListNode(3, SearchPriorityNode::SearchingRange::Field, std::mem_fn(&Enemy::SearchTarget<EnemysItem<ConsumableItem>>))));
		_searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<Monster>))));

		std::sort(_searchRuleList.begin(), _searchRuleList.end(),
			[&](const SearchPriorityNode& lhs, const SearchPriorityNode& rhs)
		{
			return (lhs._priority > rhs._priority);
		});

		return;
	}

    //�q�[���[�̏ꍇ
    if (GetEnemyAIType() == EnemyAIType::Healer)
    {
        _searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<EnemysItem<Equipment>>))));
        _searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<EnemysItem<ConsumableItem>>))));
        _searchRuleList.push_back(std::move(CreateSearchListNode(3, SearchPriorityNode::SearchingRange::Field, std::mem_fn(&Enemy::SearchTarget_damage<Enemy>))));

        std::sort(_searchRuleList.begin(), _searchRuleList.end(),
            [&](const SearchPriorityNode& lhs, const SearchPriorityNode& rhs)
        {
            return (lhs._priority > rhs._priority);
        });

        return;
    }

    

	



    //���̑�
    _searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<EnemysItem<Equipment>>))));
    _searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<EnemysItem<ConsumableItem>>))));
    _searchRuleList.push_back(std::move(CreateSearchListNode(2, SearchPriorityNode::SearchingRange::Sight, std::mem_fn(&Enemy::SearchTarget<Monster>))));

    std::sort(_searchRuleList.begin(), _searchRuleList.end(),
        [&](const SearchPriorityNode& lhs, const SearchPriorityNode& rhs)
    {
        return (lhs._priority > rhs._priority);
    });
}
