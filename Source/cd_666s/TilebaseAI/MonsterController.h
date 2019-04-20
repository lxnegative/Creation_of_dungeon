#ifndef MonsterController_h
#define MonsterController_h

#include "../../Vector2D.h"

class Monster;

class MonsterController
{
public:
    MonsterController();
    ~MonsterController();
    
    void Init();
    void Update();
    void Draw();

private:
    
    //�����̑I��
    void SelectMonster(const Vector2D cursorPos);
    
    //�����̑���
    void ControlMonster(const Vector2D cursorPos);
    
    //�����̌���
    void SetControlingMonster(Monster* monster);
    
    //�I�����Ă��閂��
    Monster* _monster;
    
};

#endif /* MonsterController_h */
