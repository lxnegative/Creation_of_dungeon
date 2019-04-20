#ifndef Sprite_h
#define Sprite_h

#include "Texture2D.h"
#include "../Resources/ImageResource.h"
#include <string>
#include <memory>

class Sprite
{
public:
    Sprite();
    Sprite(std::string fileName, Vector2D pos = Vector2D::zero);
    ~Sprite();

    enum class Priority
    {
        BACKGROUND = -100,  //�w�i�摜
        FIELD_BASE = -2,    //�}�b�v�`�b�v�̒n�ʂȂǈ�Ԍ��ɕ`��������
        FIELD_OBJECT = -1,  //�n�ʂ��͑O�ɁA�L�����N�^�[�����͌��ɕ`��������
        UI = 100,
    };

    int GetResourceHandle() const 
    {
        if (_textureResource == nullptr)
            return -1;
        
        return _textureResource->GetHandle();
    }

    Vector2D GetPosition() const { return _position; }
    Vector2D GetSize() const    { return (_tex != nullptr) ? _tex->GetSize()    : Vector2D::zero; }
    Vector2D GetScale() const   { return (_tex != nullptr) ? _tex->GetScale()   : Vector2D::zero; }

    void SetPosition(Vector2D pos);
    void SetScale(Vector2D scale);
    void SetBaseColor(Color4 color){ if(_tex != nullptr) _tex->SetBaseColor(color); }
    void SetDisplayMode(bool isDisplaying) { if (_tex != nullptr) _tex->SetDisplayMode(isDisplaying); }
    void SetRenderType(Texture2D::RenderType type) { if (_tex != nullptr) _tex->SetRenderType(type); }
    void SetPriority(int priority) { if (_tex != nullptr) _tex->SetPriority(priority); }
    void SetPriority(Priority priority) { if (_tex != nullptr) _tex->SetPriority(static_cast<int>(priority)); }

    std::weak_ptr<Texture2D> GetTexturePtr()  const { return _tex; }
    
    void SetResource(std::shared_ptr<ImageResource> resource);
    void Load(std::string fileName);
    
    void Rotate(double angle) { if (_tex != nullptr) _tex->_angle = angle; }
    void LookAt(Vector2D dir);
    
    //�摜���ǂݍ��܂�Ă��邩�ǂ���
    bool HasLoaded() const { return (_textureResource.get() != nullptr); }

protected:
    
    std::shared_ptr<ImageResource> _textureResource;
    std::shared_ptr<Texture2D> _tex;
    Vector2D _position;
};

#endif /* Sprite_h */
