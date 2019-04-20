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
    
    void DrawEnemyTimer();	//MEMO: �����ʂ̃N���X�Ɉړ�������

    void LoadTileSize(std::string stageName, std::vector<std::string>& rawData);

    //�Q�[�����ԂƎ��Ԓʒm���W���[���Q
    DungeonTimer _timer;

    //�G��|�������̃J�E���g
    DestroyedCounter _counter;

    //�E�҂�ʂ��Ă�������
    int _permitivePassedNum;
    
    std::string _stageName;	//�L�����܂߂��X�e�[�W��
    std::string _stageNum;	//�����̃X�e�[�W�ԍ��̂�
	std::string _areaNum;
    
    void GenerateObject(std::string typeName, int countX, int countY);

	bool GetIsBossStage(const std::vector<std::string>& _bossList);
    
    //�����Ԃ̏�񋤗L��
    ColleagueNotifyer _enemys, _monsters, _boss, _intruders;
    
    //�J�n�_�ƏI���n�_�̓O���[�o���ȃ��x���ŃC���X�^���X�������Ă���
    std::shared_ptr<Goal> _goal;
    std::shared_ptr<StartPoint> _start;

    //�����X�^�[���샂�W���[��
    MonsterController _controller;

    //�A�C�R���Ɩ��O�̑Ή��\
    const TiledObjectDictionary _dictionary;

    //�I�����������X�^�[�̏��
    ObjectInformationDrawer _infoDrawer;

    //���ɐN�����Ă��郂���X�^�[�̏��
    IntruderInfomation _intruderInformation;

    //���b�Z�[�W�E�B���h�E
    MessageReciever _messageReciever;

	//�{�X�o�g���i�s�p�̃��W���[��
	BossBattle _bossBattle;

    // MEMO : debug�p
    Sprite _mainsFrame, _background, _windowBackground, _waveInfomartionTimer;
	Sprite _hart, _hartFrame;

	Sprite _bossFace;
	
	//std::vector<Sprite> _hart;
	std::vector<Sprite> _hartN;

	std::shared_ptr<ImageResource> hartFullImage;
	std::shared_ptr<ImageResource> hartHalfImage;
	std::shared_ptr<ImageResource> hartNullImage;
 
	Sprite StageNamePlate;

    // MEMO : �ˊэH���p
    void LoadMessage(std::string stageName);
    void UpdateSecretary();
    std::unordered_map<std::string, TalkDatabase> _dungeonMessage;
    bool _intrudeLastCharacter;

    bool _is_boss;
	bool isBossMessageDisplay;

    int _defeatedNum;

	std::string fieldTypeStr;

	int PartNum;	//���p�[�g:0	��p�[�g:1 

	int beforePartPassedNum;	//�O��̃p�[�g�Œʂ��Ă��܂����E�҂̐�

	bool IsTimeStop;
	bool IsControlStop;
};

#endif /* Dungeon_h */
