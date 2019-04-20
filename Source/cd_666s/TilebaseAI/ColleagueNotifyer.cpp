#include "ColleagueNotifyer.h"
#include "Character.h"

ColleagueNotifyer::ColleagueNotifyer()
{
}


ColleagueNotifyer::~ColleagueNotifyer()
{
}


void ColleagueNotifyer::Update()
{
    _removes.clear();
    _removes.resize(0);
}


void ColleagueNotifyer::AddColleague(Character& obj)
{
    _colleagues.push_back(&obj);
}


void ColleagueNotifyer::RemoveColleague(Character& obj)
{
    auto it = std::find(_colleagues.begin(), _colleagues.end(), &obj);
    _colleagues.erase(it);
}


void ColleagueNotifyer::NotifyRemoveTarget(TiledObject& obj)
{
    for (auto chara : _colleagues)
    {
        if (chara->_target == nullptr)
            continue;
        
        if (chara->_target->IsEnable())
            continue;
        
        //�폜�����I�u�W�F�N�g��Ώۂɂ��Ă�����A���߂�悤�ɂ���
        chara->ResetTarget();
    }
    
    _removes.push_back(&obj);
}


bool ColleagueNotifyer::IsChasable(const TiledObject& target) const
{
    //�폜�\��̃I�u�W�F�N�g�Ȃ�
    for (auto obj : _removes)
    {
        //"�o�^�ł��Ȃ�"�ƕԂ�
        if (obj == &target)
            return false;
    }
    
    return true;
}
