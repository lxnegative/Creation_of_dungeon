#include "AllResourceManager.h"

AllResourceManager::AllResourceManager()
{
}

AllResourceManager::~AllResourceManager()
{
}


void AllResourceManager::SetAllTypesFolderPath(std::string str)
{
    SetFolderPath(str);
    IMAGE_RESOURCE_TABLE->SetFolderPath(str);
    SOUND_RESOURCE_TABLE->SetFolderPath(str);
}


//���\�[�X�̎Q�ƃJ�E���^���`�F�b�N���ĕs�v�Ȃ��̂��Ǘ�����O��
void AllResourceManager::Update()
{
    IMAGE_RESOURCE_TABLE->Update();
    SOUND_RESOURCE_TABLE->Update();
}

//���\�[�X���Ƃ̎Q�Ɛ���DrawFormatString�ŏo��
void AllResourceManager::Debug()
{
    IMAGE_RESOURCE_TABLE->Debug();
    SOUND_RESOURCE_TABLE->Debug();
}

void AllResourceManager::Clear()
{
    IMAGE_RESOURCE_TABLE->Clear();
    SOUND_RESOURCE_TABLE->Clear();
}

//���X�g����empty��؂�l�߂�
void AllResourceManager::Refresh()
{
    IMAGE_RESOURCE_TABLE->Refresh();
    SOUND_RESOURCE_TABLE->Refresh();
}