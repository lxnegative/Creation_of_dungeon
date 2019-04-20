#pragma once
#include "../../Main.h"
#include "TiledObject.h"
#include "TiledVector.h"
#include "BattleParameter.h"
#include "ParameterEffecter.h"
#include "CharacterInfomationDisplayer.h"
#include "CharactersSkill.h"
#include "FinateTimer.h"
#include "AI/PathFindingAIBase.h"

#include "../Sound/Sound.h"
#include "../../mw_animation/Animator.h"

class ColleagueNotifyer;
class BattlingTile;

class Character : public TiledObject
{
    friend class ColleagueNotifyer;
    friend class MagicSquare;
    
public:


	enum class EnemyAIType
	{
		Normal = 0,
		SkillMain,
		Hunter,
		Healer,
		None
	};


	Character(TiledVector startPos, BattleParameter param, ColleagueNotifyer& notifyer, std::string name, TiledObject::Type type, std::string resourceName, std::string displayName, bool isBoss = false);
	Character(TiledVector startPos, BattleParameter param, ColleagueNotifyer& notifyer, std::string name, TiledObject::Type type, bool isBoss = false);
    virtual ~Character();

    //�ӎv���聨�s���̏����Ɋ�Â�������
    virtual void Action();

    virtual void Update() override;
    virtual void Draw() override;
    virtual void DrawParameter(Vector2D anchorPos);
    virtual void ResetTarget();

    virtual bool IsOverwritable(TiledObject* overwriter) override { return false; }    
    virtual bool Contain(Vector2D pos) const override;
	
	virtual void ResetItemGraph() {};
	virtual EnemyAIType GetEnemyAIType() const {
		return EnemyAIType::None;
	}
    
    //�o�g���̓��ޏꎞ�̃C�x���g
    void OnOccuredBattle(BattlingTile* battle);
    void OnFinishBattle(BattlingTile* battle);

    void Appear();
    void Damaged(int damage);
    void AddParameterEffecter(std::unique_ptr<ParameterEffecter> effecter) { _effecters.push_back(std::move(effecter)); }

    bool IsEnable() const override { return _hasAppeared; }
    int GetDefeatedNum() const override { return 0; }
    bool IsAlive() const { return (0 < _battleParameter._hp); }
    bool HasArrived() const override { return _hasArrived; }

    std::string GetName() const override { return _name; }
    TiledVector::Direction GetDirection() const { return _direction; }

    virtual BattleParameter GetAffectedParameter();
    BattleParameter GetRawParameter() const override { return _battleParameter; }
    
    void SetDirection(TiledVector::Direction d) { _direction = d; }

	std::string GetResourceName() { return _resourceName; }
	std::string GetDisplayName() { return _displayName; }

	bool GetIsSkillActivate() { return isSkillActivate; }
	void SetIsSkillActivate(bool sa) { isSkillActivate = sa; }
	
    bool _isBattling;

	int GetLevel();
	void SetLevel(int lv);

	bool CheckFileExistence(const std::string& str);

protected:

    void UpdateAttitude();

    //�ӎv����
    virtual void Think() = 0;
    //�ӎv���s
    virtual void Act() = 0;

    //�|���ꂽ�Ƃ�
    virtual void OnDie();
    //�G��|������
    virtual void OnWin();

    bool CheckActable();

    //�L�����N�^�[�ŗL�̃X�L��
    std::unique_ptr<CharactersSkill> _skill;

    TiledVector _beforeTilePos;
    
    //���n��̃L�����Ԃŏ�񋤗L���邽�߂̒ʐM�@
    ColleagueNotifyer& _notifyer;
    
    //AI���W���[���̎Q��
    PathFindingAIBase *_ai;

    //AI�s���̊�ƂȂ�L����
    TiledObject *_target;
    
    //AI�ŎZ�o�����p�X
    std::vector<TiledVector> _pathToTarget;

    //�p�����
    TiledVector::Direction _direction;

    //���E
    std::vector<TiledVector> _sight;
    
    //�����̎Q�����Ă���o�g�����
    BattlingTile* _battle;
    
    FinateTimer _afterBattleTimer;

    //�����������ς݂��ǂ���
    bool _hasAppeared;

    bool _hasArrived;

    Animator _animator;

    Sound _appearSE, _defeatSE;

    //���̍s���܂ł̃J�E���^
    FinateTimer _actCounter;
    
    std::vector<std::unique_ptr<ParameterEffecter>> _effecters;

	bool isSkillActivate;

	bool ExistSkillAnimation_f;
	bool ExistSkillAnimation_b;
	bool ExistSkillAnimation_r;
	bool ExistSkillAnimation_l;

private:

    //�L�����̖��O
    std::string _name;

	//�摜�̖��O
	std::string _resourceName;

	//�\������閼�O
	std::string _displayName;

    //�퓬�p�p�����[�^
    BattleParameter _battleParameter;

    CharacterInfomationDisplayer _infoIcon;

    //�񕜃G�t�F�N�g�p
    Sprite _healGraph;
    GraphArray _healAnimation;

	int Level;

};

