#ifndef Texture2D_h
#define Texture2D_h

#include <vector>
#include <memory>
#include "../Resources/Resource.h"
#include "../Color.h"
#include "../../Vector2D.h"

class Camera;

class Texture2D : public std::enable_shared_from_this<Texture2D>
{
    friend class GraphArray;
public:
    Texture2D(int handle);
    virtual ~Texture2D();

    enum class RenderType
    {
        DEFAULT,
        UI
    };
    
    enum class AnchorType
    {
        CENTER,
        UPPER_LEFT
    };
    
    void SetPosition(Vector2D pos){ _pos = pos; }
    void BuildTextureWithInit(int handle, int width, int height);
    void BuildTexture(Vector2D pos, int width, int height);
    void BuildTexture(Vector2D pos, int width, int height, Vector2D scale);
    void Render(const Camera& camera);

    //�e�N�X�`���̌��T�C�Y
    Vector2D GetSize() const
    {
        return Vector2D(_baseSize._x / _scale._x, _baseSize._y / _scale._y);
    }
    
    //�e�N�X�`���Ɋg�嗦������������ʏ�̕\���T�C�Y
    Vector2D GetBaseSize() const { return _baseSize; }
    //�g�嗦
    Vector2D GetScale() const { return _scale; }
    void SetScale(Vector2D scale) { _scale = scale; };

    int GetHandle() const { return _handle; }
    RenderType GetRenderType() const { return _type; }
    void SetRenderType(RenderType type);

    AnchorType GetAnchorType() const { return _anchorType; }
    void SetAnchorType(AnchorType type) { _anchorType = type; };
    
    int GetPriority() const { return _priority; }
    void SetPriority(int priority);
    
    void SetBaseColor(Color4 color){ _baseColor = color; }
    void SetDisplayMode(bool isDisplaying) { _isDisplaying = isDisplaying; }

    //�e�N�X�`���̎g�p����̈��(0 ~ 1.0)�Ŏw�肷��
    void SetUVArea(Vector2D ul, Vector2D lr)
    {
        _usingUV_ul = ul;
        _usingUV_lr = lr;
    };
    
    double _angle;
    
protected:

    //�`�掞��OpenGL�ɒʒB����e�N�X�`���ԍ�
    int _handle;

    //�e�N�X�`���̊�{�F(��Z�����F)
    Color4 _baseColor;
    
    //�e�������ւ̊g�嗦
    Vector2D _scale;

    //�������W
    Vector2D _pos;

    //�g�嗦�ƃe�N�X�`���̑傫�������킹����ʏ�ł̑傫��
    Vector2D _baseSize;
    
    
    //�g�p����e�N�X�`���̗̈�(0 ~ 1.0)
    Vector2D _usingUV_ul;
    Vector2D _usingUV_lr;
    
    //�����_�����O���邩�ǂ���(�ꎞ�I��\���p)
    bool _isDisplaying;
    
    //�����_�����O���ɂǂ̃J�����Ɏʂ���
    RenderType _type;
    
    //���S���W�̈ʒu
    AnchorType _anchorType;
    
    //�`��D�揇��
    int _priority;
};

#endif /* Texture2D_h */
