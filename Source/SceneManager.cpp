#include "SceneManager.h"
#include "Title.h"
#include "EditMap.h"
#include "cd_666s//DebugDraw.h"
#include "Main.h"


SceneManager::SceneManager() : now_scene(new Title()), test(Graph(Main::RESOURCEFILENAME + "resource/test_666/omote.png"))
{
    auto class_name = now_scene->GetClassNameData();
    now_scene->Init(class_name);

	interval = -1;

}


SceneManager::~SceneManager()
{
}

bool SceneManager::SceneManagement()
{

	if (interval >= 0) {
		interval++;

		if (interval > maxInterval) {
			now_scene = temp_scene;
			interval = -1;
			_black.SetDisplayMode(false);
		}
		else {
			_black.SetDisplayMode(true);
		}
	}

	if (now_scene == nullptr) {
		return true;
	}

    SceneBase* next_scene = now_scene->MainUpdate();
    
    if(next_scene == nullptr){
        return false;
    }
    else if (next_scene != now_scene) {
        delete(now_scene);
		now_scene = nullptr;
		interval = 0;
		temp_scene = next_scene;
		temp_scene->Init(temp_scene->GetClassNameData());
		_black.SetDisplayMode(true);

    }

    //test.Draw();
	if (now_scene != nullptr) {
		now_scene->MainDraw();
	}
    

    return true;
}
