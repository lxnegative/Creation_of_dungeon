#pragma once

#include "Equipment.h"
#include "EnemysItem.h"
#include "Monster.h"
#include "EnemysSearch.h"
#include "ColleagueNotifyer.h"
#include "FinateTimer.h"
#include "SearchingPriorityNode.h"
#include <memory>
#include <string>

#include "ConsumableItem.h"
#include "../../RobTresureItem.h"
#include "Character.h"

class AstarChaser;
class ColleagueNotifyer;
class StartPoint;
class Goal;
class ConsumableItem;

class RobTresureItem;

class Enemy : public Character
{
public:
    Enemy(TiledVector, BattleParameter params, TiledObject &baseTarget, ColleagueNotifyer& notifyer, std::string enemyName, std::string resourceName, std::string displayName, EnemyAIType aiType);
    ~Enemy();
    
    void SetTarget(Character *target);
    void ResetTarget() override;
    
	void Draw() override;
    void Update() override;
    void DrawParameter(Vector2D anchorPos) override;
    
    bool IsOverwritable(TiledObject* overwriter) override;
   
    int GetDefeatedNum() const override { return (_defeatedNum); };

	static int GetDefeatedNum_Static() { return (_defeatedNum); };

    static void LoadEnemys(std::vector<std::shared_ptr<TiledObject>>& objects, StartPoint& point, Goal& goal, ColleagueNotifyer& notifyer, std::string fileName);

    static bool HasWipeOuted()
    {
        if (_defeatedNum == 0 || _enemysNum == 0)
            return false;
		
        return (_enemysNum <= _defeatedNum);
    }

    void SetItem(std::unique_ptr<Equipment> itemContents) 
    {
        _equipItem = std::move(itemContents); 
        _equipmentsGraph.SetResource(_equipItem->_image);
        _hasTreasureIcon.SetDisplayMode(true);
    };

    void SetItem(std::unique_ptr<ConsumableItem> itemContents) 
    {
        for (size_t i = 0; i < _consumableItems.size(); ++i)
        {
            if (_consumableItems[i] == nullptr)
            {
                _consumableItems[i] = (std::move(itemContents));
                _consumableItemGraphs[i].SetResource(_consumableItems[i]->_image);
                _hasTreasureIcon.SetDisplayMode(true);
                return;
            }
        }
    };

    void StuckOn(int interval)  { _damageTimer.Reset(interval, true, false); };

	static std::vector<RobTresureItem> GetRobTresureItem() { return _robTresureItem; };

	std::string GetName() const override { return Character::GetName(); };

	BattleParameter GetAffectedParameter() override;

	bool GetIsFullHP() const { return  isFullHP; }

	void ResetItemGraph() override {
		for (size_t i = 0; i < _consumableItems.size(); ++i)
		{
			if (_consumableItems[i] != nullptr) {
				_consumableItemGraphs[i].SetDisplayMode(false);
			}

		}

		_equipmentsGraph.SetDisplayMode(false);
	};

	

	EnemyAIType GetEnemyAIType() const override {
		return aiType;
	};

private:
	
    //�ӎv����
    virtual void Think() override;
    
    //�ӎv���s
    virtual void Act() override;

    virtual void OnDie() override;

    //�G�Ύ푰�Ɛ퓬
    void Battle(TiledObject* target);
    //�A�C�e���̎擾
    void ObtainItem(TiledObject* target);
    //�S�[���ւ̓��B
    void ArriveAtGoal(TiledObject* target);

    void UseItem(BattleParameter& param);
    void MoveToNext();
    
    //�_���[�W�d���p�^�C�}�[
    FinateTimer _damageTimer;

    //AI�s���̊�ƂȂ�Ώ�
    TiledObject& _baseTarget;

    //�����i
    std::unique_ptr<Equipment> _equipItem;
    //�����i
    std::vector<std::unique_ptr<ConsumableItem>> _consumableItems;

    std::unique_ptr<AstarChaser> _astar;
    
    static int _defeatedNum;
    static int _enemysNum;

	static std::vector<RobTresureItem> _robTresureItem;

	bool isFullHP;

    Sprite _equipmentsGraph, _hasTreasureIcon;
    std::vector<Sprite> _consumableItemGraphs;

    std::vector<SearchPriorityNode> _searchRuleList;
    TiledObject* SearhTargetByRuleList();
    void BuildSearchingRuleList();

	BattleParameter _tempParam;	//�{�X�퓬�ւ̒l�n���ɗ��p

    template <typename T>
    void SearchTarget(std::vector<TiledObject*>& targets, const TiledVector& tilePos, const SearchPriorityNode::SearchingRange& rangeType) {};

	template <typename T>
	void SearchTarget_damage(std::vector<TiledObject*>& targets, const TiledVector& tilePos, const SearchPriorityNode::SearchingRange& rangeType) {};

	EnemyAIType aiType;

	Sound _enemy_fall2;

};
