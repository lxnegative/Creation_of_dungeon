#pragma once
#include "cd_666s\InputManager\MouseInput.h"
#include "cd_666s\TilebaseAI\MapTile.h"
#include "cd_666s/TilebaseAI/ColleagueNotifyer.h"

#include "cd_666s/TilebaseAI/Goal.h"
#include "cd_666s/TilebaseAI/StartPoint.h"

#include "UIManager.h"

class MakeDungeon
{
public:
    MakeDungeon();
    MakeDungeon(std::string areaNum, std::string stage_num);
    ~MakeDungeon();

    void Update();

    void Draw();

    void Init(std::string file_name);
	void Init(std::string file_name,bool isLoadNewMap);

    void PickupObject();

    void LoadTileSize(std::string stageName, std::vector<std::string>& rawData);

    std::shared_ptr<Goal> getGoal();
    std::shared_ptr<StartPoint> getStart();

    TiledObject* GenerateAddObject(std::string typeName, int countX, int countY, Vector2D mousePos);

    std::vector<TiledObject*> GenerateMonster(std::string fileName, TiledVector startPos, std::string* GenerateText, int level);

	void SetFieldType();
	std::string GetFieldType();

	TiledVector GetStartFrontPosition();
	TiledVector GetGoalFrontPosition();

private:

    std::string _stage_num;
	std::string _stage_num_a;	//�����̂�
    
    void GenerateObject(std::string typeName, int countX, int countY);


    //�J�n�_�ƏI���n�_�̓O���[�o���ȃ��x���ŃC���X�^���X�������Ă���
    std::shared_ptr<Goal> _goal;
    std::shared_ptr<StartPoint> _start;

    //�����Ԃ̏�񋤗L��

    ColleagueNotifyer _monsters,_enemys;

    //���ݑI�΂�Ă���I�u�W�F�N�g
    std::string _selectingObj;

    //���b�Z�[�W�E�B���h�E
    MessageReciever _messageReciever;

    Sprite _mainsFrame, _background, _windowBackground;

	//�_���W�����̒n�`����ۑ����Ă����i���A�Ȃ�#CAV���j
	std::string fieldTypeStr;

	std::string _areaNumber;

	Sprite InUI, OutUI;
	Sprite _NotPutObjectPlace[2];

};

