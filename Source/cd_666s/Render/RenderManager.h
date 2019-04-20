#ifndef RenderManager_h
#define RenderManager_h

#include <vector>
#include <functional>
#include <memory>
#include "../Utility/Singleton.h"
#include "../Utility/PoolingSystem.h"
#include "../Color.h"
#include "../DebugDraw.h"
#include "Camera.h"

class Texture2D;
class DebugDraw;

class RenderManager : public Singleton<RenderManager>
{
    friend class Singleton<RenderManager>;

public:
    RenderManager();
    ~RenderManager();
    
    void AddDebugCall(std::unique_ptr<Debug::DebugDraw> debugDrawCall) { _debugDrawCalls.push_back(std::move(debugDrawCall)); }
	void AddTopPriorityDebugCall(std::unique_ptr<Debug::DebugDraw> debugDrawCall) { _topPriorityDebugDrawCalls.push_back(std::move(debugDrawCall)); }
    void AddRenderModel(std::weak_ptr<Texture2D> model);
    void RemoveRenderModel(std::weak_ptr<Texture2D> model);
    void ClearModel();
    
    void Init();
    void ClearScreen();
    void RenderImage();
    void Update();
    
    //��ʂ̏������F��ݒ肷��
    void SetClearColor(Color4 color) { _clearColor = color; };
    void ChangePriority(std::weak_ptr<Texture2D> model, int oldPriority, int newPriority);
    
private:
    
    Color4 _clearColor;
    Camera _defaultCamera, _uiCamera;

    //TODO : �����ƃf�o�b�O�`��p�̂��̂����
    std::vector<std::unique_ptr<Debug::DebugDraw>> _debugDrawCalls;

	std::vector<std::unique_ptr<Debug::DebugDraw>> _topPriorityDebugDrawCalls;
};


#define RENDER_MGR RenderManager::GetInstance()

#endif /* RenderManager_h */
