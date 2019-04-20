#include "Enemy.h"
#include "TileField.h"
#include "AI/AstarChaser.h"
#include "../DebugDraw.h"
#include "ColleagueNotifyer.h"
#include "TiledObjectMnager.h"
#include "BattlingTile.h"
#include "EnemysItem.h"

#include "StartPoint.h"

#include "EnemysSearch.h"

int Enemy::_defeatedNum = 0;
int Enemy::_enemysNum = 0;
std::vector<RobTresureItem> Enemy::_robTresureItem;

Enemy::Enemy(TiledVector startPos, BattleParameter params, TiledObject &baseTarget, ColleagueNotifyer& notifyer, std::string enemyName, std::string resourceName, std::string displayName, EnemyAIType aiType)
	: Character(startPos, params, notifyer, enemyName, TiledObject::Type::ENEMY, resourceName, displayName)
	, _baseTarget(baseTarget)
	, _damageTimer(30, false, false)
	, _tempParam({ 0,0,0,0,0,0 })
	, aiType(aiType)
{
	_target = &baseTarget;

	_astar = std::make_unique<AstarChaser>(_target, *this, _pathToTarget, 20, true);
	_ai = _astar.get();

	_appearSE.Load(Main::RESOURCEFILENAME + "resource/sound/blockSelect.wav");
	_hasTreasureIcon.Load(Main::RESOURCEFILENAME + "resource/graph/icon/treasure_attached.png");
	_hasTreasureIcon.SetScale(Vector2D(2.0, 2.0));
	_hasTreasureIcon.SetDisplayMode(false);
	_hasTreasureIcon.SetPriority(Sprite::Priority::UI);

	_consumableItems.resize(3);
	_consumableItemGraphs.resize(3);

	for (size_t i = 0; i < _consumableItemGraphs.size(); ++i)
	{
		_consumableItemGraphs[i].SetPriority(1090);
	}
	_equipmentsGraph.SetPriority(1090);

	BuildSearchingRuleList();

	for (size_t i = 0; i < _consumableItems.size(); ++i)
	{
		if (_consumableItems[i] != nullptr) {
			_consumableItemGraphs[i].SetDisplayMode(false);
		}

	}

	_equipmentsGraph.SetDisplayMode(false);

	_enemy_fall2.Load(Main::RESOURCEFILENAME + "resource/sound/enemy_fall2.wav");
	_enemy_fall2.SetVolume(200);

}


Enemy::~Enemy()
{
}


BattleParameter Enemy::GetAffectedParameter()
{
	BattleParameter param = Character::GetAffectedParameter();

	if (_equipItem) {
		param += _equipItem->GetBonusParam();
	}


	return param;
}

void Enemy::Think()
{
	_target = SearhTargetByRuleList();
	if (_target == nullptr)
		_target = &_baseTarget;

	//�͈͓��ɓ�������ǐՂ�
	if (_target != &_baseTarget)
	{
		_astar->SetTarget(_target);
		_astar->SetSearchRange(20); //���X��6
	}
	//�Ώۂ�������Ȃ��Ă��Ō�Ɍ��������n�_�܂ł��ǂ蒅������
	else
	{
		if (_pathToTarget.size() == 0)
		{
			//�S�[����ڎw������
			_target = &_baseTarget;
			_astar->SetTarget(&_baseTarget);
			_astar->SetSearchRange(20);
		}
	}

	_ai->Update();

}

void Enemy::Draw() {

	Character::Draw();

	if (Main::DEBUGFLAG) {

		for (int i = 0; i < _sight.size(); i++) {
			Debug::DrawRectWithSize(_sight[i].GetWorldPos(), Vector2D(TILE_SIZE, TILE_SIZE), Color4(1.0, 1.0, 0.0, 0.3), true);
		}

	}

}

void Enemy::Update()
{
	Character::Update();

	if (_damageTimer.IsCounting())
		_damageTimer.Update();

	//�퓬���ׂ����m�F
	if (!_afterBattleTimer.IsCounting()
		&& !_isBattling)
	{
		auto monster = OBJECT_MGR->GetContainedObject<Monster>(_position + Vector2D(TILE_SIZE / 2, TILE_SIZE / 2));
		if (monster != nullptr
			&& monster->IsEnable())
		{
			auto offset = monster->GetPosition() - _position;
			if (offset.GetLength() < TILE_SIZE / 4)
			{
				Battle(monster);
				return;
			}
		}
	}

	if (!CheckActable())
		return;

	//��Ԋm�F�t�F�C�Y
	//�ڕW���������Ă����猳�̕W�I��
	if (_target == nullptr || !_target->IsEnable())
	{
		_target = &_baseTarget;
		_astar->SetTarget(_target);
	}

	if (_damageTimer.IsCounting())
		return;

	Character::Action();

	auto graphSize = _hasTreasureIcon.GetSize();
	_hasTreasureIcon.SetPosition(_position + Vector2D(TILE_SIZE - graphSize._x, 0));

	auto param = GetAffectedParameter();
	if (param._hp == param._maxHP) {
		isFullHP = true;
	}
	else {
		isFullHP = false;
	}


}


void Enemy::Act()
{
	if (_skill.get() != nullptr
		&& _skill->ShouldActivate()
		&& FIELD->GetTiledObject<StartPoint>(GetTilePos()) == nullptr
		&& _skill->_mp >= _skill->_mpCost)
	{
		isSkillActivate = true;

		_skill->Activate();

		return;
	}

	//�̗͂������ȉ��ŃA�C�e�����������Ă���Ȃ��
	auto param = GetAffectedParameter();
	if (param._hp <= param._maxHP / 2)
		UseItem(param);

	//�ړ����������ĂȂ��Ȃ�ڕW�ֈړ�
	if (0 < _pathToTarget.size())
	{
		//������������ǐՂ��Ă��Ƃ�(�q�[���\��)��
		//����O�Ŏ~�܂�
		if (_target->GetType() == _type)
		{
			if (1 < _pathToTarget.size())
				MoveToNext();
		}
		else
		{
			MoveToNext();
		}

		return;
	}

	//�ڕW�̏�ɂ��Ȃ���ΏI��
	if (_target->GetTile().lock() != GetTile().lock())
		return;

	switch (_target->GetType())
	{
	case TiledObject::Type::ITEM:
		ObtainItem(_target);
		return;

	case TiledObject::Type::GOAL:
		ArriveAtGoal(_target);
		return;
	}
}


void Enemy::ObtainItem(TiledObject* target)
{
	//�A�C�e�����ׂɂȂ���ΏI��
	auto offset = _target->GetTilePos() - GetTilePos();
	if (offset.GetBresenhamLength(false) > 1)
		return;

	auto equipment = dynamic_cast<EnemysItem<Equipment>*>(target);
	if (equipment != nullptr)
	{
		//����΃A�C�e���擾
		equipment->GiveItem(*this);
	}
	else
	{
		auto item = dynamic_cast<EnemysItem<ConsumableItem>*>(target);
		if (item != nullptr)
		{
			//����΃A�C�e���擾
			item->GiveItem(*this);
		}
		else
		{
			return;
		}
	}



	_notifyer.NotifyRemoveTarget(*_target);

	//���̖ڕW��
	_target = &_baseTarget;
	_astar->SetTarget(&_baseTarget);
	_astar->SetSearchRange(20);

	_ai->Update();
}


void Enemy::ArriveAtGoal(TiledObject* target)
{
	_hasAppeared = false;
	_hasArrived = true;

	_target = nullptr;

	target->Interact(*this);
	OBJECT_MGR->Remove(this);
	_notifyer.NotifyRemoveTarget(*this);

	_tempParam = Character::GetRawParameter();

	_enemy_fall2.Play();

}


void Enemy::Battle(TiledObject* target)
{
	// TODO : ���}���u�Ƃ��āB
	auto enemy = OBJECT_MGR->GetSharedObject<Enemy>(this);
	auto monster = OBJECT_MGR->GetSharedObject<Monster>(target);
	OBJECT_MGR->Add(std::make_shared<BattlingTile>(enemy, monster, GetTilePos()));
}


void Enemy::MoveToNext()
{
	//�ړ�������o����
	TiledVector pos = GetTilePos();
	pos = _pathToTarget[0];

	//�L���[����폜
	_pathToTarget.erase(_pathToTarget.begin());

	//�ړI�̌o�H�Ɉړ�
	FIELD->MoveObject(*this, pos);
}


void Enemy::UseItem(BattleParameter& param)
{
	for (size_t i = 0; i < _consumableItems.size(); ++i)
	{
		if (_consumableItems[i] != nullptr)
		{
			Damaged(-_consumableItems[i]->GetRecoveryLife());
			_consumableItems[i] = nullptr;
			_consumableItemGraphs[i].SetResource(nullptr);
			break;
		}
	}

	//�ȉ��A�A�C�e��������ΏI��
	if (_equipItem != nullptr)
		return;

	for (size_t i = 0; i < _consumableItems.size(); ++i)
	{
		if (_consumableItems[i] != nullptr)
			return;
	}

	//�����܂Ŕ�������A�C�e�����������Ă��Ȃ��̂ŃA�C�R������
	_hasTreasureIcon.SetDisplayMode(false);
}


void Enemy::SetTarget(Character *target)
{
	if (!IsEnable() || !IsAlive())
		return;

	//�ڕW�̏�����
	ResetTarget();

	_target = target;
	_astar->SetTarget(target);
}


void Enemy::ResetTarget()
{
	Character::ResetTarget();
	_target = &_baseTarget;
	_astar->SetTarget(_target);
}


void Enemy::OnDie()
{
	Character::OnDie();

	//�e�p�����[�^�����Z�b�g
	_target = nullptr;
	_defeatedNum++;

	RobTresureItem temp;
	if (_equipItem != nullptr) {
		temp.goldValue = _equipItem->GetGoldValue();
		temp.ItemGraphPath = _equipItem->GetfilePath();
		temp.ItemName = _equipItem->GetItemName();
		_robTresureItem.push_back(temp);
	}

	for (int i = 0; i < _consumableItems.size(); i++) {
		if (_consumableItems[i] != nullptr) {
			temp.goldValue = _consumableItems[i]->GetGoldValue();
			temp.ItemGraphPath = _consumableItems[i]->GetfilePath();
			temp.ItemName = _consumableItems[i]->GetItemName();
			_robTresureItem.push_back(temp);
		}
	}


	_enemy_fall2.Play();

	//�t�B�[���h���珜�O
	OBJECT_MGR->Remove(this);

	//������ǐՂ��Ă������Ԃ͂�����߂�悤�ɂ���
	_notifyer.NotifyRemoveTarget(*this);
}


bool Enemy::IsOverwritable(TiledObject* overwriter)
{
	if (overwriter == this)
		return true;

	return true;
}