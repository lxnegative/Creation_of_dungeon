#pragma once
#include <queue>
#include "cd_666s\TilebaseAI\Character.h"
#include "BossBattleObject.h"

class BossBattle
{
public:
	BossBattle();
	~BossBattle();

	void Init(std::string areaName, std::string stageNum, std::vector<std::shared_ptr<TiledObject>>& objects, ColleagueNotifyer &notifyer);

	void Update();

//	void SetBattleObject(std::vector<std::shared_ptr<TiledObject>>& objects, std::shared_ptr<TiledObject> intruder);
	void SetBattleObject( std::shared_ptr<TiledObject>& intruder, ColleagueNotifyer& notifyer);

	//�{�X�����ɂ���N���҂̏�����납��擾
	std::shared_ptr<TiledObject> GetIntruderObject() const { return (_intruders.empty() ? nullptr : _intruders.back()); }

	bool WasBossKilled() const { return _wasBossKilled; }


	std::weak_ptr<BossBattleObject> GetBossObject() { return _boss; }

private:
	/*
	struct Party {
		bool _isLead;			//�퓬���ۂ�
		BattleParameter _param; //
	};
	*/

	enum class BattleSequence{
		None,
		Ready,
		EnemyAttack,
		BossAttack,
		BossDied,
	};

	bool HasKilled(BattleParameter& atk, BattleParameter& dff);

	void GenerateObject(std::string typeName);

	void Ready();
	void EnemyAttack();
	void BossAttack();
	void BossDied();

	BattleSequence now_situation;

	std::deque<std::shared_ptr<BossBattleObject>> _intruders;	//�킢�ɎQ�����Ă���`���҂���
	std::shared_ptr<BossBattleObject> _boss;

	Sprite _bossFace;
	Sprite _enemyFace;
	Animator _enemySkills;

	Sprite _messageUI;

	//BattleParameter _boss;

	int _sequenceTimer;
	const int _readyTime;	//���̐퓬�O�V�[���t���[��
	int _battleTime;	//���̍U���V�[���t���[��
	const int _waitTime;	//���̍U����̑ҋ@�V�[���t���[��
	const int _diedTime;	//�{�X���S���o�V�[���t���[��

	bool _wasBossKilled;
};

