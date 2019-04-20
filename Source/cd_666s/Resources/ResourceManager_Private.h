#include "ResourceManager.h"
using namespace std;

template <typename T>
ResourceManager<T>::ResourceManager()
: _filePath(Main::RESOURCEFILENAME + "resource/")    //���\�[�X�f�B���N�g���̍ŏ�ʃp�X
, _empty(nullptr)
{
}

template <typename T>
std::weak_ptr<T> ResourceManager<T>::GetResource(std::string name)
{
    auto it = _nameMap.find(name);
    bool hasLoaded = (it != _nameMap.end()
                      && _resourceTable[it->second] != nullptr);
    
    //���[�h�ς݂Ȃ�|�C���^��
    if (hasLoaded)
    {
        return _resourceTable[(*it).second];
    }
    //�������null��Ԃ�
    else
    {
        return _empty;
    }
}


template <typename T>
shared_ptr<T> ResourceManager<T>::Create(std::string fileName)
{
    //���[�h�ς݂��`�F�b�N
    auto it = _nameMap.find(fileName);
    bool hasLoaded = (it != _nameMap.end()
                      && _resourceTable[it->second] != nullptr);
    
    //���[�h�ς݂Ȃ�|�C���^��
    if (hasLoaded)
        return _resourceTable[(*it).second];
    
    for (size_t i = 0; i < _resourceTable.size(); ++i)
    {
        //null�̂Ƃ��낪�����
        if (_resourceTable[i].get() == nullptr)
        {
            //�����ɐ������ďI��
            _resourceTable[i] = std::make_shared<T>(fileName);
            _nameMap.insert(make_pair(fileName, i));
            return _resourceTable[i];
        }
    }
    
    //null��������΍Ō�ɐ������ďI��
    _resourceTable.push_back(std::make_shared<T>(fileName));
    _nameMap.insert(make_pair(fileName, _resourceTable.size() - 1));
    
    return _resourceTable[_resourceTable.size() - 1];
}


template <typename T>
void ResourceManager<T>::Update()
{
    for (size_t i = 0; i < _resourceTable.size(); ++i)
    {
        //�󃊃\�[�X�Ȃ疳��
        if (_resourceTable[i].get() == nullptr)
            continue;
        
        size_t count = _resourceTable[i].use_count();
        
        //�g�p���� vector�Ɋi�[����Q�� + �ꎞ�ϐ�resource �����Ȃ�
        // = ������Q�Ƃ���Ȃ��Ȃ����@��폜����
        if (1 < count)
            continue;
        
        Remove(_resourceTable[i].get()->GetName());
    }
}


template <typename T>
void ResourceManager<T>::Debug()
{
    /*
     for (size_t i = 0; i < _resourceTable.size(); ++i)
     {
     std::weak_ptr<Resource> resource = _resourceTable[i];
     
     //�󃊃\�[�X�Ȃ疳��
     if (resource._Get() == nullptr)
     continue;
     
     DrawFormatString(0,  0 + (i + 1) * 60, GetColor(0x00, 0x00, 0x00), (_resourceTable[i]->GetName()).c_str());
     DrawFormatString(0, 30 + (i + 1) * 60, GetColor(0x00, 0x00, 0x00), "%d:", _resourceTable[i].use_count());
     }
     
     DrawFormatString(0, 0, GetColor(0x00, 0x00, 0x00), "list size : %d", _resourceTable.size());
     */
}


template <typename T>
void ResourceManager<T>::Remove(const string name)
{
    if (    _nameMap.size() == 0
        && _resourceTable.size() == 0)
        return;
    
    //���[�h�ς݃}�b�v����폜
    size_t index = _nameMap[name];
    _nameMap.erase(_nameMap.find(name));
    _resourceTable[index].reset();
    _resourceTable[index] = _empty;
}


template <typename T>
void ResourceManager<T>::Refresh()
{
    //�܂��J���v�f���X�V
    Update();

    vector<shared_ptr<T>> swaped;
    
    for (size_t i = 0; i < _resourceTable.size(); ++i)
    {
        if (_resourceTable[i].get() == nullptr)
            continue;
        
        //empty�ȊO��ǉ�����
        swaped.push_back( _resourceTable[i] );
        
        //�؂�l�ߎ��̃C���f�N�X�̕ύX��K�p����
        _nameMap[_resourceTable[i]->GetName()] = swaped.size() - 1;
    }
    
    //�؂�l�߂����X�g�Ɠ���ւ���
    _resourceTable = std::move(swaped);
}


template <typename T>
void ResourceManager<T>::Clear()
{
    for (size_t i = 0; i < _resourceTable.size(); ++i)
    {
        //�󃊃\�[�X�Ȃ疳��
        if (_resourceTable[i].get() == nullptr)
            continue;
        
        Remove(_resourceTable[i].get()->GetName());
    }
    
    _resourceTable.clear();
    _resourceTable.resize(0);
    _nameMap.clear();
}


template <typename T>
ResourceManager<T>::~ResourceManager()
{
    _resourceTable.clear();
    _nameMap.clear();
}