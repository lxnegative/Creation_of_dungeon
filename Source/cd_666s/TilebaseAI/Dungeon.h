#ifndef Dungeon_h
#define Dungeon_h

#include <string>
#include <unordered_map>
#include "MonsterController.h"
#include "TiledVector.h"
#include "ColleagueNotifyer.h"
#include "ObjectInformationDrawer.h"
#include "IntruderInfomation.h"
#include "TiledObjectDictionary.h"
#include "DungeonTimer.h"
#include "../../DestroyedCounter.h"
#include "MessageReciever.h"

#include "../Render/Sprite.h"
#include "../Sound/Sound.h"

#include "../../BossBattle.h"

class Goal;
class StartPoint;

class Dungeon
{
public:
	Dungeon(std::string areaNum, std::string stageName, int PartNum);
    ~Dungeon();
    
    //void InitWithLoad(std::string stageName);
    void Init();
    void Clear();
    void Update();
    void Draw();

    bool HasClear();
    bool HasGameOver();

	bool HasTimeUp();

	int GetDefeatedNum() { return _defeatedNum; };


	void SetFieldType();

	bool Get_Is_Boss() { return _is_boss; }

	int GetPassedNum();

	std::vector<std::string> GetFileList(const std::string& path);

	std::string GetFieldType() {
		return fieldTypeStr;
	};

	void SetIsTimeStop(bool a) {
		IsTimeStop = a;
	}

	void SetIsControlStop(bool a) {
		IsControlStop = a;
	}
    
private:
    
    void DrawEnemyTimer();	//MEMO: これを別のクラスに移動させる

    void LoadTileSize(std::string stageName, std::vector<std::string>& rawData);

    //ゲーム時間と時間通知モジュール群
    DungeonTimer _timer;

    //敵を倒した数のカウント
    DestroyedCounter _counter;

    //勇者を通してもいい数
    int _permitivePassedNum;
    
    std::string _stageName;	//記号を含めたステージ名
    std::string _stageNum;	//整数のステージ番号のみ
	std::string _areaNum;
    
    void GenerateObject(std::string typeName, int countX, int countY);

	bool GetIsBossStage(const std::vector<std::string>& _bossList);
    
    //同盟間の情報共有網
    ColleagueNotifyer _enemys, _monsters, _boss, _intruders;
    
    //開始点と終了地点はグローバルなレベルでインスタンス生成しておく
    std::shared_ptr<Goal> _goal;
    std::shared_ptr<StartPoint> _start;

    //モンスター操作モジュール
    MonsterController _controller;

    //アイコンと名前の対応表
    const TiledObjectDictionary _dictionary;

    //選択したモンスターの情報
    ObjectInformationDrawer _infoDrawer;

    //次に侵入してくるモンスターの情報
    IntruderInfomation _intruderInformation;

    //メッセージウィンドウ
    MessageReciever _messageReciever;

	//ボスバトル進行用のモジュール
	BossBattle _bossBattle;

    // MEMO : debug用
    Sprite _mainsFrame, _background, _windowBackground, _waveInfomartionTimer;
	Sprite _hart, _hartFrame;

	Sprite _bossFace;
	
	//std::vector<Sprite> _hart;
	std::vector<Sprite> _hartN;

	std::shared_ptr<ImageResource> hartFullImage;
	std::shared_ptr<ImageResource> hartHalfImage;
	std::shared_ptr<ImageResource> hartNullImage;
 
	Sprite StageNamePlate;

    // MEMO : 突貫工事用
    void LoadMessage(std::string stageName);
    void UpdateSecretary();
    std::unordered_map<std::string, TalkDatabase> _dungeonMessage;
    bool _intrudeLastCharacter;

    bool _is_boss;
	bool isBossMessageDisplay;

    int _defeatedNum;

	std::string fieldTypeStr;

	int PartNum;	//朝パート:0	夜パート:1 

	int beforePartPassedNum;	//前回のパートで通してしまった勇者の数

	bool IsTimeStop;
	bool IsControlStop;
};

#endif /* Dungeon_h */
