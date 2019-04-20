#ifndef BattlingTile_h
#define BattlingTile_h

#include "Character.h"
#include "Enemy.h"
#include "Monster.h"
#include "TiledObject.h"

class BattlingTile : public TiledObject
{
public:
    BattlingTile(std::weak_ptr<Enemy> enemy, std::weak_ptr<Monster> monster, TiledVector tilePos);
    ~BattlingTile();
    
    void Update() override;
    void Draw() override;
    
    //�������U��
    void AttackFromOutside(int power, int attack, bool isPhysical, TiledObject::Type defenderType);
    //���R�U���p�_���[�W�v�Z
    void PhysicalAttack(int power, int attack);
    void MagicalAttack(int power, int attack);
    //���R�h��p�_���[�W�v�Z
    void PhysicalDamaged(int power, int attack);
    void MagicalDamaged(int power, int attack);

    //void Interact(const Character& atkObj);
    
    //�퓬���痣�E
    void RunAway();
    void FinishBattle();

    bool IsOverwritable(TiledObject* overwriter) override;
    
    Enemy* GetEmemy()
    {
        return (_enemy.expired() ? nullptr : _enemy.lock().get());
    }

    Monster* GetMonster()
    {
        return (_monster.expired() ? nullptr : _monster.lock().get());
    }

private:

    //�퓬�㔻��
    void CheckAlive(const Enemy& enemy, const Monster& monster);
    
    //�퓬�^�C�}�[
    int _count;

    bool _attackMonster;

    Sprite _dust2;

    std::weak_ptr<Enemy>_enemy;
    std::weak_ptr<Monster> _monster;
};

#endif /* BattlingTile_h */
