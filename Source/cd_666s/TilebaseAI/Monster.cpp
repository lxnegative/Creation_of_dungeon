#include "Monster.h"
#include "TileField.h"
#include "AI/AstarChaser.h"
#include "ColleagueNotifyer.h"
#include "BattlingTile.h"
#include "TiledObjectMnager.h"
#include "../DebugDraw.h"
#include "../InputManager/InputManager.h"

Monster::Monster(TiledVector startPos, BattleParameter param, TiledObject *target, ColleagueNotifyer& notifyer, std::string monsterName, std::string skillData, std::string resourceName, std::string displayName)
: Character(startPos, param, notifyer, monsterName, TiledObject::Type::MONSTER, resourceName, displayName)
, _hasChoosed(false)
, _countAfterUsingSkill(30)
{
    _target = target;
    _astar = std::make_unique<AstarChaser>(_target, *this, _pathToTarget, 100, true);
    /*
    _astar->SetAdditionalFunc(std::move([&](TiledObject* obj)
    {
        if (obj->GetType() != Type::ENEMY)
            return false;

        auto enemy = dynamic_cast<Enemy*>(obj);
        return (!enemy->_isBattling); 
    }));
    */

    _skill = CreateSkillFromName(monsterName, skillData);
    _ai = _astar.get();

    _appearSE.Load(Main::RESOURCEFILENAME + "resource/sound/flame.wav");
    _appearSE.SetVolume(200);
}


Monster::~Monster()
{
}


void Monster::Think()
{
    if (_target != nullptr)
        _ai->Update();
}


void Monster::Update()
{
    Character::Update();

    //���@�w�̏�ɂ���Ȃ炳��ɉ�
    auto magicSquare = FIELD->GetTiledObject<MagicSquare>(GetTilePos());
    if (magicSquare != nullptr)
         magicSquare->Interact(*this);
    
    //�s���\���`�F�b�N
    if (!CheckActable())
        return;

    if (_skill.get() != nullptr
        && _skill->IsReadyToUse()
        && MOUSE->DoubleClicked()
        && Contain(MOUSE->GetCursorPos()))
    {
        _skill->Activate();
    }

    if (_skill.get() != nullptr
        && _countAfterUsingSkill < 30)
    {
        _countAfterUsingSkill++;
        return;
    }

    Character::Action();
}


void Monster::Act()
{
    //�ړ���Ƃ̍�������������X�V
    //UpdateAttitude();
   
    //���̃^�C���ֈړ�
    MoveToNext();
}


void Monster::MoveToNext()
{
    if (_pathToTarget.size() == 0)
        return;

    /*
    if (!_afterBattleTimer.IsCounting())
    {
        //�G�����ʂ��痈��Ȃ�҂��\����
        auto obj = FIELD->GetTiledObject<Enemy>(GetTilePos());
        if (obj != nullptr)
            return;
    }
    */
    
    //�ړ�������o����
    TiledVector pos = _pathToTarget[0];
    //�L���[����폜
    _pathToTarget.erase(_pathToTarget.begin());
    FIELD->MoveObject(*this, pos);
}


void Monster::ResetTarget()
{
    Character::ResetTarget();
    _astar->SetTarget(nullptr);
}


void Monster::DrawTargetMarker()
{
    if (_target != nullptr)
    {
        auto worldPos = _target->GetPosition() + Vector2D(TILE_SIZE / 2, TILE_SIZE / 2);
        Debug::DrawCircle(worldPos, TILE_SIZE, ColorPalette::BLACK4, false);
        Debug::DrawLine(worldPos + Vector2D(-TILE_SIZE / 2, 0), worldPos + Vector2D(TILE_SIZE / 2, 0), ColorPalette::BLACK4);
        Debug::DrawLine(worldPos + Vector2D(0, -TILE_SIZE / 2), worldPos + Vector2D(0, TILE_SIZE / 2), ColorPalette::BLACK4);
    }
}

//���@�w�ւ̌Ăі߂�
void Monster::WarpToHome(const MagicSquare& square)
{
    //�o�g�����Ȃ�o�g�����痣�E
    if (_isBattling)
        _battle->RunAway();
//        ResetCounter();

    OnDie();
}


void Monster::SetTarget(TiledObject *target)
{
    if (!IsEnable() || !IsAlive() || _isBattling)
        return;
        
    //�ڕW�̏�����
    ResetTarget();
    
    //�ڕW�ݒ�
    _target = target;
    _astar->SetTarget(target);
}


void Monster::SetTarget(TiledVector pos)
{
    if (!IsEnable() || !IsAlive() || _isBattling)
        return;
    
    //�����ֈړ�
    _target = nullptr;
    _astar->SetTarget(pos);
    _astar->FindPath();
}


void Monster::OnDie()
{
    Character::OnDie();

    //�w�ɖ߂�
    auto homePos = _home->GetTilePos();
    _beforeTilePos = homePos;
    _position = homePos.GetWorldPos();
    FIELD->MoveObject(*this, homePos);

    //�e�p�����[�^�����Z�b�g
    ResetTarget();
    _hasChoosed = false;
}


bool Monster::IsRunnable()
{
    auto tilePos = GetTilePos();
    
    if (FIELD->IsMovableThere(tilePos + TiledVector(0, 1), *this))
        return true;
    
    if (FIELD->IsMovableThere(tilePos + TiledVector(0, -1), *this))
        return true;

    if (FIELD->IsMovableThere(tilePos + TiledVector(1, 0), *this))
        return true;

    if (FIELD->IsMovableThere(tilePos + TiledVector(-1, 0), *this))
        return true;
    
    return false;
}

//�����\���ǂ���
bool Monster::IsReadyToProduce()
{
    auto param = GetAffectedParameter();
    //�̗͂��ő� & ��������ĂȂ��Ȃ珢���\
    return (param._maxHP == param._hp)
            && (!_hasAppeared);
}

bool Monster::IsOverwritable(TiledObject* overwriter)
{
    if (overwriter == this)
        return true;

    if (overwriter->GetType() == Type::ENEMY)
        return !_isBattling;

    if (overwriter->GetType() == Type::MONSTER)
        return false;
    
    return true;
}