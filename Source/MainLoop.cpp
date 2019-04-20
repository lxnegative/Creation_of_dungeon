#include "MainLoop.h"
#include "SceneManager.h"
#include "cd_666s/FPSCounter/FPSCounter.h"
#include "cd_666s/FPSCounter/FPSCountDraw.h"
#include "cd_666s/Resources/AllResourceManager.h"
#include "cd_666s/InputManager/InputManager.h"
#include "cd_666s/Render/RenderManager.h"
#include "DxLib.h"
#include <Windows.h>
#include <direct.h>
#include <string>

void MainLoop::Init()
{
    //���\�[�X�Ǘ����W���[��������
	/*
    char cwd[512];
    memset(cwd, 0, 512);
    _getcwd(cwd, 512);
    std::string temp(cwd, 512);
    auto it = temp.find('\0');
    temp.resize(it);
    temp.append("/");
    RESOURCE_TABLE->SetAllTypesFolderPath(temp);
    RENDER_MGR->Init();
	*/

	scene_manager.Init();
}


void MainLoop::Terminate()
{
    INPUT_MGR->Clear();
    RENDER_MGR->ClearModel();
    RESOURCE_TABLE->Clear();
}


MainLoop::MainLoop() //: scene_manager(SceneManager())
{
    //���샂�W���[���Q������
    Init();
}


MainLoop::~MainLoop()
{
    Terminate();
}

void MainLoop::GameLoop()
{
    FPSCounter fpsCounter(60);
    FPSDrawer fpsDrawer(fpsCounter);

    SetDrawScreen(DX_SCREEN_BACK);

    while (ProcessMessage() == 0) {

        ClearDrawScreen();

        //Essential Updates
        fpsCounter.Update();
        INPUT_MGR->Update();

        if(!scene_manager.SceneManagement()){
            break;
        }

        //Rendering
        RENDER_MGR->Update();
        RENDER_MGR->RenderImage();
        fpsDrawer.Draw();

        //���\�[�X�̎Q�Ə󋵂��X�V
        RESOURCE_TABLE->Update();

        ScreenFlip();
    }
}
