#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
#include "../../Main.h"

//typedef int Handle;

class Resource
{
public:
    
    Resource(std::string fileName);
    virtual ~Resource();
    
    //���\�[�X�̃��[�h
    virtual bool Load();
    //���\�[�X�̍폜
    virtual bool Unload();
    
    std::string GetName() const { return _fileName; }
    //Handle GetHandle() const { return _handle; }
    
protected:
    
    //���\�[�X�n���h���̖{��
    //Handle _handle;
    
    //���\�[�X��ǂݍ��ރt�@�C����
    std::string _fileName;
};

#endif