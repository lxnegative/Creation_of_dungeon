#ifndef MagicSquare_h
#define MagicSquare_h

#include "TiledObject.h"
#include "../../mw_animation/Animator.h"

class Monster;

class MagicSquare : public TiledObject
{
public:
    MagicSquare(TiledVector tilePos, Monster& monster);
    ~MagicSquare();
    
    bool IsOverwritable(TiledObject* overwriter) override;
    void Interact(Character& character) override;
    
    void Update() override;
    void Draw() override;
    
    //�����������������@�w���ǂ���
    bool IsOwner(Monster& monster) { return (&_monster == &monster); }
    
private:
    
    
    //�������Ă���g��������������
    void ProduceMonster();
    
    //�������Ă���g�������Ăі߂�
    void WarpHomeMonster();
    
    Monster& _monster;

    //�����\�ɂȂ����Ƃ��̉摜
    GraphArray _animation;
    Sprite _currentGraph;

	bool isFastInteract;

};


#endif /* MagicSquare_h */
