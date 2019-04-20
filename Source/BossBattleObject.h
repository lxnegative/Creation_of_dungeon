#pragma once
#include "cd_666s/TilebaseAI/Character.h"
#include "cd_666s/DebugDraw.h"

class BossBattleObject : public Character
{
public:
	BossBattleObject(Vector2D startPos, BattleParameter param, ColleagueNotifyer & notifyer, std::string name, std::string resourceName, std::string displayName, bool _isBoss);
	BossBattleObject(Vector2D startPos, BattleParameter param, ColleagueNotifyer & notifyer, std::string name, bool _isBoss);
	~BossBattleObject();

	enum class ObjectType
	{
		DEFAULT,
		BOSS,
		ENEMY
	};

	enum class Action
	{
		READY,
		ATTACK,
		END
	};

	//���[�v�����̑O��1�x�����Ă΂�鏈��
	void Init();

	void Update() override;
	void Draw() override;

	void Clear();

	void Think() override {};
	void Act() override {};
	bool IsOverwritable(TiledObject* overwriter) override { return false; }

	//�A�j���[�V�����̊J�n/��~
	void SwitchAnime(bool _isAnimate);

	void OutBossRoom();

	//�I�u�W�F�N�g���|���ꂽ���Ƃ������Ƃ��Ɏg�p
	virtual int GetDefeatedNum() const { return 0; }

	//�{�X�����ɋ����Ԃ�ύX����
	void SetInRoom(bool _inRoom) { _isInRoom =  _inRoom; }

	ObjectType GetObjectType() const { return _objType; }

	BattleParameter _param;

protected:

	ObjectType _objType;
	Action _action;		//�s���̓��e

	//�������{�X�����̒��ɂ��邩
	bool _isInRoom;

	
private:
	Vector2D _position;
	double move_sum;
};

