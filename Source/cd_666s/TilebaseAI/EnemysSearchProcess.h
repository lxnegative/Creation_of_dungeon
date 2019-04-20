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
			//åoòHÇ™ë∂ç›ÇµÇ»Ç©Ç¡ÇΩÇÁñ≥éã
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

        //ï ÇÃíáä‘Ç™Ç∑Ç≈Ç…ëŒè€Ç…ÇµÇƒÇ¢ÇÈÇ»ÇÁñ≥éã
        if (!_notifyer.IsChasable(*target))
            return false;

		//åoòHÇ™ë∂ç›ÇµÇ»Ç©Ç¡ÇΩÇÁñ≥éã
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

        //ï ÇÃíáä‘Ç™Ç∑Ç≈Ç…ëŒè€Ç…ÇµÇƒÇ¢ÇÈÇ»ÇÁñ≥éã
        if (!_notifyer.IsChasable(*target))
            return false;

		//åoòHÇ™ë∂ç›ÇµÇ»Ç©Ç¡ÇΩÇÁñ≥éã
		if (target->GetTilePos() != this->GetTilePos()) {
			_astar->SetTarget(target->GetTilePos());
			_astar->SetSearchRange(20);
			_astar->FindPath();
			if (_pathToTarget.size() == 0)
			{
				return false;
			}
		}

        //éËéùÇøÇ…ãÛÇ´Ç™Ç†ÇÍÇŒéÊÇËÇ…çsÇ≠
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
        //î≠å©ëŒè€Ç™é©ï™Ç»ÇÁñ≥éã
        if (target == this)
            return false;

		//åoòHÇ™ë∂ç›ÇµÇ»Ç©Ç¡ÇΩÇÁñ≥éã
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
		//î≠å©ëŒè€Ç™é©ï™Ç»ÇÁñ≥éã
		if (target == this)
			return false;

		//åoòHÇ™ë∂ç›ÇµÇ»Ç©Ç¡ÇΩÇÁñ≥éã
		if (target->GetTilePos() != this->GetTilePos()) {
			_astar->SetTarget(target->GetTilePos());
			_astar->SetSearchRange(20);
			_astar->FindPath();
			if (_pathToTarget.size() == 0)
			{
				return false;
			}
		}

		//î≠å©ëŒè€Ç™ñ`åØé“Çí«Ç¢Ç©ÇØÇÈAIÇ≈Ç†ÇÈÇ»ÇÁÇŒÉ_ÉÅÅ[ÉWÇéÛÇØÇƒÇ¢Ç»ÇØÇÍÇŒñ≥éã
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


