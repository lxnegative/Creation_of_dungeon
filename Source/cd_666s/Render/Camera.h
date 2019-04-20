#ifndef Camera_h
#define Camera_h

#include "Texture2D.h"
#include "../../Vector2D.h"
#include "../Utility/PoolingSystem.h"
#include <vector>

class Camera
{
public:
    Camera(Vector2D pos, double angle, Vector2D viewPortSize);
    ~Camera();
    
    void AddRenderModel(std::weak_ptr<Texture2D> model);
    void RemoveRenderModel(std::weak_ptr<Texture2D> model);
    
    void Clear();
    void Render();
    
    Vector2D GetPosition() const { return _position; }
    Vector2D GetViewPortSize() const { return _viewPortSize; }
    double GetAngle() const { return _angle; }
    
    void SetPosition(Vector2D pos) { _position = pos; }
    void SetViewPortSize(Vector2D size) { _viewPortSize = size; }
    void SetAngle(double angle) { _angle = angle; }
    
    void ChangePriority(int oldPriority, int newPriority);
    
    static Camera* _mainCamera;
    
    //�����N���b�s���O���ʏ�Ɏw��̓_�����邩�ǂ���
    bool IsDisplay(Vector2D point);
    
private:
    
    struct PriorityInfo
    {
        PriorityInfo(int priority, int size, int count)
            : _priority(priority)
            , _size(size)
            , _count(count){};
        int _priority = 0;
        int _size = 0;
        int _count = 0;
    };
    
    std::vector<PriorityInfo> _priorityTable;
    
    void CountPriority(int priority);
    void DiscountPriority(int priority);
    void AlimentDrawCall();
    
    //�`�惂�f���𗭂ߍ���
    PoolingSystem<Texture2D> _renderModels;

    //�`��R�[���𗭂ߍ���
    std::vector<std::weak_ptr<Texture2D>>_renderOrders;
    
    Vector2D _position;
    Vector2D _viewPortSize;
    double _angle;
    
    bool _needResize;
    bool _needAliment;
};

#endif /* Camera_h */
