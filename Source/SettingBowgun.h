#pragma once
#include "cd_666s\TilebaseAI\TiledObject.h"
class SettingBowgun :
    public TiledObject
{
public:
    SettingBowgun(TiledVector tilePos);
    ~SettingBowgun();

    void Init();

    //�\������ł���I�u�W�F�N�g����Ăяo�����֐�
    void Interact(Character& character);

    //���̃I�u�W�F�N�g�̏�ɕʂ̃I�u�W�F�N�g�����邩�ǂ���
    //overwriter : ��ɏ�낤�Ƃ���I�u�W�F�N�g
    bool IsOverwritable(TiledObject* overwriter);

    void OnClicked();

    void Update() override;
    void Draw() override;

private:
    std::string _name;
};

