#ifndef AllResourceManager_h
#define AllResourceManager_h

#include "../Utility/Singleton.h"
#include "ResourceManager.h"

class AllResourceManager : public Singleton<AllResourceManager>
{
    friend class Singleton<AllResourceManager>;
    
public:
    
    //���\�[�X�̎Q�ƃJ�E���^���`�F�b�N���ĕs�v�Ȃ��̂��Ǘ�����O��
    void Update();
    
    //���\�[�X���Ƃ̎Q�Ɛ���DrawFormatString�ŏo��
    void Debug();
    
    void Clear();
    
    //���X�g����empty��؂�l�߂�
    void Refresh();
    
    //���\�[�X�Q�̍ŏ�ʃt�H���_�����擾
    std::string GetFolderPath() const { return _filePath; }
    void SetFolderPath(std::string str) { _filePath = str; }
    void SetAllTypesFolderPath(std::string str);

private:
    
    AllResourceManager();
    ~AllResourceManager();
    
    //���\�[�X�f�B���N�g���̍ŏ�ʃp�X
    std::string _filePath;
};

#define RESOURCE_TABLE AllResourceManager::GetInstance()
#endif /* AllResourceManager_h */
