#pragma once
#include "SceneBase.h"
#include "cd_666s/Sound/Sound.h"
#include "cd_666s/TilebaseAI/Dungeon.h"
#include "GameResult.h"
#include "EditMap.h"

class Game : public SceneBase
{
public:
	Game(std::string areaNum, std::string stageNumber, std::string pointNumber);
	Game(std::string areaNum, std::string stageNumber, std::string pointNumber, int PartNum);
    ~Game();

    SceneBase* Update(UIManager _ui);
    void Draw();

    std::string GetClassNameData() const {
        return class_name;
    };

private:
    std::string class_name;

    std::string stage_num;

    std::vector<std::string> _functions;

    Graph test;
    Sound _bgm;

    //negative==================================
    enum class GameState
    {
        READY,
        GAMING,
        GAME_OVER,
        WAVE_CLEAR,
        //GAME_CLEAR,
        PAUSE
    };
    GameState _state;

    //演出用の暗転時間
    const int _fadeinInterval;  //暗転から途中まで
    const int _fadingInterval; //通常から途中まで
    const int _fadeoutInterval; //途中から暗転まで

    //演出用の暗転カウント
    int _fadeoutCount;
    bool _fadingout;

    //ここから下が追加した部分(negative)
    bool GameOverUpdate();
    void GameOverDraw();

    bool StageClearUpdate();
    void StageClearDraw();
    //bool GameClearUpdate();
    //void GameClearDraw();


	int PartNum;	//朝パート:0	夜パート:1 


    //666=======================================
    bool GameReadyUpdate();
    void GameReadyDraw();

    void Init();
    void Clear();

    void GamingUpdate();
    void GamingDraw();

    bool PauseUpdate();
    void PauseDraw();

    // TODO : あとでpointerにせずとも済むようにする
    Dungeon* _dungeon;
    std::string _stageNumber;
	std::string _areaNumber;
	std::string _pointNumber;

    bool isBoss;

	GameResult gameResult;

	Sprite _black;

	TutorialPanel* tutorialPanel;
	Tutorial_Balloon* tutorial_Balloon;

	Sprite _gameOverUI;
	Sprite Click_go_to_Next;
	Sprite Click_Click_and_Start;

	Sprite PauseUI;

	bool isTutorial;



	Sprite GameEndAlarmUI;
	std::shared_ptr<ImageResource> TimeUpAlarmUI;

	int GameEndCnt;
	const int GameEndCntMax = 120;

	Sound _game_end;
};

