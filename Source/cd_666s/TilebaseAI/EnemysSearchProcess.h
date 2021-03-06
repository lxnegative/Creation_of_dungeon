#pragma once
#include "Enemy.h"
#include "AI/AstarChaser.h"

template <>
void Enemy::SearchTarget<Monster>(std::vector<TiledObject*>& targets, const TiledVector& tilePos, const SearchPriorityNode::SearchingRange& rangeType)
{

	SearchExpressions<Monster> expressions;
	expressions.push_back([&](const Monster* target, const TiledVector& pos)
	{

		if (target->GetTilePos() != this->GetTilePos()) {
			//経路が存在しなかったら無視
			_astar->SetTarget(target->GetTilePos());
			_astar->SetSearchRange(20);
			_astar->FindPath();
			if (_pathToTarget.size() == 0)
			{
				return false;
			}
		}

		return true;
	});

    auto target = (rangeType == SearchPriorityNode::SearchingRange::Sight)
        ? SearchTargetInSight<Monster>(tilePos, _sight, expressions, true)
        : SearchTargetInField<Monster>(tilePos, expressions, true);

    if (target != nullptr)
        targets.push_back(target);
}


template <>
void Enemy::SearchTarget<EnemysItem<Equipment>>(std::vector<TiledObject*>& targets, const TiledVector& tilePos, const SearchPriorityNode::SearchingRange& rangeType)
{
    SearchExpressions<EnemysItem<Equipment>> expressions;
    expressions.push_back([&](const EnemysItem<Equipment>* target, const TiledVector& pos)
    {
        if (target->IsEmpty())
            return false;

        if (_equipItem != nullptr)
            return false;

        //別の仲間がすでに対象にしているなら無視
        if (!_notifyer.IsChasable(*target))
            return false;

		//経路が存在しなかったら無視
		if (target->GetTilePos() != this->GetTilePos()) {
			_astar->SetTarget(target->GetTilePos());
			_astar->SetSearchRange(20);
			_astar->FindPath();
			if (_pathToTarget.size() == 0)
			{
				return false;
			}
		}

        return true;
    });

    auto target = (rangeType == SearchPriorityNode::SearchingRange::Sight)
        ? SearchTargetInSight<EnemysItem<Equipment>>(tilePos, _sight, expressions, true)
        : SearchTargetInField<EnemysItem<Equipment>>(tilePos, expressions, true);
    if (target != nullptr)
        targets.push_back(target);
}


template <>
void Enemy::SearchTarget<EnemysItem<ConsumableItem>>(std::vector<TiledObject*>& targets, const TiledVector& tilePos, const SearchPriorityNode::SearchingRange& rangeType)
{
    SearchExpressions<EnemysItem<ConsumableItem>> expressions;
    expressions.push_back([&](const EnemysItem<ConsumableItem>* target, const TiledVector& pos)
    {
        if (target->IsEmpty())
            return false;

        //別の仲間がすでに対象にしているなら無視
        if (!_notifyer.IsChasable(*target))
            return false;

		//経路が存在しなかったら無視
		if (target->GetTilePos() != this->GetTilePos()) {
			_astar->SetTarget(target->GetTilePos());
			_astar->SetSearchRange(20);
			_astar->FindPath();
			if (_pathToTarget.size() == 0)
			{
				return false;
			}
		}

        //手持ちに空きがあれば取りに行く
        for (size_t i = 0; i < _consumableItems.size(); ++i)
        {
            if (_consumableItems[i] == nullptr)
                return true;
        }

        return false;
    });

    auto target = (rangeType == SearchPriorityNode::SearchingRange::Sight)
        ? SearchTargetInSight<EnemysItem<ConsumableItem>>(tilePos, _sight, expressions, true)
        : SearchTargetInField<EnemysItem<ConsumableItem>>(tilePos, expressions, true);

    if (target != nullptr)
        targets.push_back(target);
}

template <>
void Enemy::SearchTarget<Enemy>(std::vector<TiledObject*>& targets, const TiledVector& tilePos, const SearchPriorityNode::SearchingRange& rangeType)
{
    SearchExpressions<Enemy> expressions;
    expressions.push_back([&](const Enemy* target, const TiledVector& pos)
    {
        //発見対象が自分なら無視
        if (target == this)
            return false;

		//経路が存在しなかったら無視
		if (target->GetTilePos() != this->GetTilePos()) {
			_astar->SetTarget(target->GetTilePos());
			_astar->SetSearchRange(20);
			_astar->FindPath();
			if (_pathToTarget.size() == 0)
			{
				return false;
			}
		}

        return true;
    });

    auto target = (rangeType == SearchPriorityNode::SearchingRange::Sight)
        ? SearchTargetInSight<Enemy>(tilePos, _sight, expressions, true)
        : SearchTargetInField<Enemy>(tilePos, expressions, true);

    if (target != nullptr)
        targets.push_back(target);
}


template <>
void Enemy::SearchTarget_damage<Enemy>(std::vector<TiledObject*>& targets, const TiledVector& tilePos, const SearchPriorityNode::SearchingRange& rangeType)
{
	SearchExpressions<Enemy> expressions;
	expressions.push_back([&](const Enemy* target, const TiledVector& pos)
	{
		//発見対象が自分なら無視
		if (target == this)
			return false;

		//経路が存在しなかったら無視
		if (target->GetTilePos() != this->GetTilePos()) {
			_astar->SetTarget(target->GetTilePos());
			_astar->SetSearchRange(20);
			_astar->FindPath();
			if (_pathToTarget.size() == 0)
			{
				return false;
			}
		}

		//発見対象が冒険者を追いかけるAIであるならばダメージを受けていなければ無視
		if (target->GetEnemyAIType() == EnemyAIType::Healer) {
			if (target->GetIsFullHP()) {
				return false;
			}
		}

		return true;
	});

	auto target = (rangeType == SearchPriorityNode::SearchingRange::Sight)
		? SearchTargetInSight<Enemy>(tilePos, _sight, expressions, true)
		: SearchTargetInField<Enemy>(tilePos, expressions, true);

	if (target != nullptr)
		targets.push_back(target);
}


