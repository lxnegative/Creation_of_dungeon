#include "MonsterController.h"
#include "BattlingTile.h"
#include "Monster.h"
#include "MagicSquare.h"
#include "TileField.h"
#include "TiledObjectMnager.h"
#include "../DebugDraw.h"
#include "../InputManager/InputManager.h"

MonsterController::MonsterController()
    : _monster(nullptr)
{
}


MonsterController::~MonsterController()
{
}


void MonsterController::Init()
{
    _monster = nullptr;
}


void MonsterController::Update()
{
    if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L)) {
        //return;

        //�N���b�N�ʒu���t�B�[���h�����`�F�b�N
        auto cursorPos = MOUSE->GetCursorPos();
        auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
        if (FIELD->IsInside(tiledCursorPos)) {
            //return;

            if (_monster == nullptr)
                SelectMonster(cursorPos);
            else
                ControlMonster(cursorPos);
        }
    }

    if (MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_R)) {

        //�N���b�N�ʒu���t�B�[���h�����`�F�b�N
        auto cursorPos = MOUSE->GetCursorPos();
        auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
        if (!FIELD->IsInside(tiledCursorPos))
            return;

        //�Ώۂ��N���b�N����Ă��邩�`�F�b�N
        auto targets = FIELD->GetTiledObjects(tiledCursorPos);

        //�^�C����؂�ŉ���������Ȃ��Ȃ�摜��؂�ŒT��
        if (targets.size() == 0)
            targets = std::move(OBJECT_MGR->GetContainedObjects<TiledObject>(cursorPos));

        //�������N���b�N���ꂽ��
        if (_monster != nullptr) {
            if (tiledCursorPos == _monster->GetTilePos())
            {
                if (_monster->IsEnable())
                {

                    TiledVector::Direction d = _monster->GetDirection();

                    switch (d)
                    {
                    case TiledVector::LEFT:
                        d = TiledVector::BACK;
                        break;
                    case TiledVector::FORWARD:
                        d = TiledVector::LEFT;
                        break;
                    case TiledVector::RIGHT:
                        d = TiledVector::FORWARD;
                        break;
                    case TiledVector::BACK:
                        d = TiledVector::RIGHT;
                        break;
                    default:
                        break;
                    }

                    _monster->SetDirection(d);

                    return;
                }
            }

            //�����ȊO���E�N���b�N������I������
            _monster = nullptr;

        }
    }


}


void MonsterController::Draw()
{
    if (_monster == nullptr)
        return;

    if (!_monster->IsEnable())
        return;

    auto worldTilePos = _monster->GetTilePos().GetWorldPos();
    Debug::DrawRectWithSize(worldTilePos, Vector2D(TILE_SIZE, TILE_SIZE),
        Color4(0.0, 1.0, 0.0, 0.15), true);

    _monster->DrawTargetMarker();
}


void MonsterController::SetControlingMonster(Monster* monster)
{
    if (monster == _monster)
        return;

    if (_monster != nullptr)
        _monster->ReleaseMonster();

    //���̖�����I����Ԃ�
    _monster = monster;
    _monster->PickMonster();
}


//�����̑I��
void MonsterController::SelectMonster(const Vector2D cursorPos)
{
    auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);

    //�Ώۂ��N���b�N����Ă��邩�`�F�b�N
    auto target = FIELD->GetTiledObject<Monster>(tiledCursorPos);

    //�^�C����؂�ŉ���������Ȃ��Ȃ�摜��؂�ŒT��
    if (target == nullptr)
        target = std::move(OBJECT_MGR->GetContainedObject<Monster>(cursorPos));

    //�{���ɉ����Ȃ��Ƃ���Ȃ�I��
    if (target == nullptr)
        return;

    SetControlingMonster(target);
}

//�����̑���
void MonsterController::ControlMonster(const Vector2D cursorPos)
{
    auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);

    //�Ώۂ��N���b�N����Ă��邩�`�F�b�N
    auto targets = FIELD->GetTiledObjects(tiledCursorPos);

    //�^�C����؂�ŉ���������Ȃ��Ȃ�摜��؂�ŒT��
    if (targets.size() == 0)
        targets = std::move(OBJECT_MGR->GetContainedObjects<TiledObject>(cursorPos));

    //�{���ɉ����Ȃ��Ƃ���Ȃ�I��
    if (targets.size() == 0)
    {
        _monster->SetTarget(tiledCursorPos);
        return;
    }

    //�������N���b�N���ꂽ��
    if (tiledCursorPos == _monster->GetTilePos())
    {
        if (_monster->IsEnable())
        {
            //�I��������
            //_monster = nullptr;
            return;
        }
    }

    //�����������Ȃ�
    for (auto target : targets)
    {
        //���̒��Ԃ��N���b�N���ꂽ
        if (target->GetType() == TiledObject::Type::MONSTER)
        {
            //���̒��Ԃ�I��
            auto monster = dynamic_cast<Monster*>(target);
            //���̖�����I����Ԃ�
            _monster = monster;
            return;
        }
    }
    for (auto target : targets)
    {
        //���@�w��
        if (target->GetType() == TiledObject::Type::MAGIC_SQUARE)
        {
            //�������������������̂łȂ���΃N���b�N�ł��Ȃ�
            auto magicSquare = dynamic_cast<MagicSquare*>(target);
            if (!magicSquare->IsOwner(*_monster))
                return;
        }
    }
    for (auto target : targets)
    {
        //�G��I��������
        if (target->GetType() == TiledObject::Type::ENEMY
            && target->IsEnable())
        {
            //�G��ǐ�
            _monster->SetTarget(target);
            return;
        }
    }

    //�����܂Ŕ������烂���X�^�[�̑���ɉe��������͖̂������ƂɂȂ�.
    //�����X�^�[���N���b�N�����ʒu�܂ňړ�������
    _monster->SetTarget(tiledCursorPos);
}
