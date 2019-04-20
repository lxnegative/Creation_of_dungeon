#include "ObjectInformationDrawer.h"
#include "Character.h"
#include "BattlingTile.h"
#include "TiledObjectMnager.h"
#include "TileField.h"
#include "../Resources/ResourceManager.h"
#include "../InputManager/InputManager.h"
#include "../DebugDraw.h"

ObjectInformationDrawer::ObjectInformationDrawer(const TiledObjectDictionary& iconDictionary)
: _character1(nullptr)
, _character2(nullptr)
, _thumbnail1(iconDictionary, { 920, 140 })
, _thumbnail2(iconDictionary, { 920, 430 })
, _selectSE(Main::RESOURCEFILENAME + "resource/sound/decide.wav")
{
    Init();
}


ObjectInformationDrawer::~ObjectInformationDrawer()
{
    Clear();
}


void ObjectInformationDrawer::Clear()
{
    _character1 = nullptr;
    _character2 = nullptr;

    _thumbnail1.Clear();
    _thumbnail2.Clear();
}


void ObjectInformationDrawer::Init()
{
    _character1 = nullptr;
    _character2 = nullptr;

    _thumbnail1.Init();
    _thumbnail2.Init();
}


void ObjectInformationDrawer::SetCharacter(Character *chara)
{
    if (chara == _character1 || chara == _character2)
        return;

    //1�Ԗڂɏ�񂪂�������
    if (_character1 != nullptr)
    {
        //2�Ԗڂɏ�񂪂Ȃ��Ȃ�2�Ԃ�
        if (_character2 == nullptr)
        {
            _character2 = chara;
            return;
        }

        //�����łȂ��Ȃ牟���o��
		_character2->ResetItemGraph();
        _character2 = _character1;
        _character1 = chara;
        return;
    }
    
    //�Ȃ�������1�Ԃɏ����Z�b�g
    _character1 = chara;
}


void ObjectInformationDrawer::SetCharacter(Character *chara, Character *chara2)
{
    _character1 = chara;
    _character2 = chara2;
}


void ObjectInformationDrawer::SelectObject()
{
    if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
        return;
    
    auto cursorPos = MOUSE->GetCursorPos();
    auto tiledCursorPos = TiledVector::ConvertToTiledPos(cursorPos);
    
    //�Ώۂ��N���b�N����Ă��邩�`�F�b�N
    auto targets = FIELD->GetTiledObjects(tiledCursorPos);
    
    //�^�C����؂�ŉ���������Ȃ��Ȃ�摜��؂�ŒT��
    if (targets.size() == 0)
        targets = std::move(OBJECT_MGR->GetContainedObjects<TiledObject>(cursorPos));
    
    //�{���ɉ����Ȃ��Ƃ���Ȃ�I��
    if (targets.size() == 0)
        return;

    //��Ƀo�g�������ǂ����m�F
    for (auto target : targets)
    {
        //�o�g�����N���b�N�����痼���ݒu
        if (target->GetType() == TiledObject::Type::BATTLE)
        {
            _selectSE.Play();
            auto battle = dynamic_cast<BattlingTile*>(target);
            auto enemy = battle->GetEmemy();
            auto monster = battle->GetMonster();
            if (enemy != nullptr && monster != nullptr)
                SetCharacter(monster, enemy);

            return;
        }
    }

    for (auto target : targets)
    {
        //������G���N���b�N������
        if (target->GetType() == TiledObject::Type::MONSTER
            || target->GetType() == TiledObject::Type::ENEMY)
        {
            auto chara = dynamic_cast<Character*>(target);
            if (chara != nullptr && chara->IsEnable())
            {
                _selectSE.Play();
                SetCharacter(chara);
            }
            return;
        }
    }
}


void ObjectInformationDrawer::Update()
{
    SelectObject();
    
    if (_character1 != nullptr)
    {
        if (!_character1->IsAlive() || !_character1->IsEnable())
            _character1 = nullptr;
    }
    if (_character2 != nullptr)
    {
        if (!_character2->IsAlive() || !_character2->IsEnable())
            _character2 = nullptr;
    }
    
    //1�̖ڂ��I������Ă��邩�����Ă�����I��
    if (_character1 != nullptr)
        return;
    
    //2�̖ڂ��I������ĂȂ��������ĂȂ��Ȃ�I��
    if (_character2 == nullptr)
        return;
        
    //�����܂Ŕ�������2�̖ڂ��������Ă���̂�
    //����1�̖ڂ̕��֋l�ߍ���
    _character1 = _character2;
    _character2 = nullptr;
}


void ObjectInformationDrawer::Draw()
{
    _thumbnail1.SetCharacter(_character1);
    _thumbnail2.SetCharacter(_character2);

    _thumbnail1.Draw();
    _thumbnail2.Draw();
}