#ifndef RESOURCE_MGR
#define RESOURCE_MGR

#include <memory>
#include <map>
#include <vector>
#include "../Utility/Singleton.h"
#include "Resource.h"
#include "ImageResource.h"
#include "SoundResource.h"

/*
 ��������郊�\�[�X�n���h����
 ��ӂȃt�@�C���� �� shared_ptr�̃C���f�N�X�̃y�A �ŊǗ�����N���X
 */

template <typename T>
class ResourceManager : public Singleton<ResourceManager<T>>
{
    friend class Singleton<ResourceManager>;
    
public:
    
    //���\�[�X���쐬�E�擾
    std::shared_ptr<T> Create(std::string fileName);
    
    //���\�[�X�̎Q�ƃJ�E���^���`�F�b�N���ĕs�v�Ȃ��̂��Ǘ�����O��
    void Update();
    //���\�[�X���Ƃ̎Q�Ɛ���DrawFormatString�ŏo��
    void Debug();
    
    void Clear();
    
    //���X�g����empty��؂�l�߂�
    void Refresh();
    
    //���\�[�X�Q�̍ŏ�ʃt�H���_�����擾
    std::string GetFolderPath() const { return _filePath; }
    void SetFolderPath(std::string str){ _filePath = str; }
    
    //���\�[�X��weak_ptr���擾
    std::weak_ptr<T> GetResource(std::string name);
    
private:
    
    //���\�[�X���Ǘ�����O���č폜
    void Remove(const std::string name);

    ResourceManager();
    ~ResourceManager();
    
    //���\�[�X�f�B���N�g���̍ŏ�ʃp�X
    std::string _filePath;
    
    //�폜���ꂽ���\�[�X�̈ʒu�ɑ�����Ă������߂̋�̃��\�[�X
    const std::shared_ptr<T> _empty;
    
    //���\�[�X�̔z��
    std::vector<std::shared_ptr<T>> _resourceTable;
    
    //���\�[�X�̊Ǘ����̔z��
    std::map<std::string, size_t> _nameMap;
    
};

#include "ResourceManager_Private.h"

#define IMAGE_RESOURCE_TABLE ResourceManager<ImageResource>::GetInstance()
#define SOUND_RESOURCE_TABLE ResourceManager<SoundResource>::GetInstance()

#endif