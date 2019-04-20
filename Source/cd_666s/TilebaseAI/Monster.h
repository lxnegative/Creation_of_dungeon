#ifndef Monster_h
#define Monster_h

#include "Character.h"
#include "MagicSquare.h"
#include <memory>

class AstarChaser;
class ColleagueNotifyer;
class MagicSquare;

class Monster : public Character
{
public:
    Monster(TiledVector startPos, BattleParameter param, TiledObject *target, ColleagueNotifyer& notifyer, std::string monsterName, std::string skillData, std::string resourceName, std::string displayName);
    ~Monster();
    
    void ResetTarget() override;
    void Update() override;
    void DrawParameter(Vector2D anchorPos) override;
    
    bool IsOverwritable(TiledObject* overwriter) override;
    bool IsRunnable();
    
    static void LoadMonsters(std::vector<std::shared_ptr<TiledObject>>& objects, ColleagueNotifyer& notifyer, std::string fileName);
	static void LoadMonsters_DungeonInfomation(std::vector<std::shared_ptr<TiledObject>>& objects, ColleagueNotifyer& notifyer);

    static std::vector<TiledObject*> GenerateMonster(std::vector<std::shared_ptr<TiledObject>>& objects, ColleagueNotifyer& notifyer, std::string fileName, TiledVector startPos, std::string* GenerateText, int level);
    
    void DrawTargetMarker();
    
//MonsterController����Ăяo���֐�
    void PickMonster() { _hasChoosed = true; };
    void ReleaseMonster() { _hasChoosed = false; };
    void SetTarget(TiledObject *target);
    void SetTarget(TiledVector pos);

//���@�w����Ăяo���֐�
    //�����\���ǂ���
    bool IsReadyToProduce();
    //���@�w�ւ̌Ăі߂�
    void WarpToHome(const MagicSquare& square);

private:

    //���O�ƌ��ʒl����X�L������
    std::unique_ptr<CharactersSkill> CreateSkillFromName(std::string name, std::string skillData);

    void MoveToNext();

    virtual void OnDie() override;
    
    //�ӎv����
    virtual void Think() override;
    
    //�ӎv���s
    virtual void Act() override;
    
    //�����̏������ꂽ�w
    MagicSquare* _home;
    
    std::unique_ptr<AstarChaser> _astar;
    
    //�������N���b�N��Ԃ��ǂ���
    bool _hasChoosed;

    //�X�L���g�p��̍d��
    int _countAfterUsingSkill;
};

#endif /* Monster_h */
