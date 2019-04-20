#pragma once
#include "SceneBase.h"
#include "cd_666s/Sound/Sound.h"
#include <vector>
#include "cd_666s/Render/Sprite.h"
#include "mw_animation/Animator.h"
#include "WorldMap_Menu.h"
#include "TutorialPanel.h"
#include "Information_SaveData.h"
#include "PopUpWindow_YES.h"
#include "PopUpWindowClass.h"

class WorldMap :
    public SceneBase
{
private:

    //�\���̂̐錾�@���Ԃ͉���
    struct Point;
    struct PointConnect;
    struct RoadConnect;
    struct SearchPointConnect;

    struct AreaConnectPoint;
    
    //�g���\���̂��ȉ���
    struct Point    //�|�C���g�̏���\���@�|�C���g�͈ړ����邱�Ƃ��o����ꏊ
    {

        virtual ~Point() {};

        int AreaNum;

        int pointNum;   //�|�C���g�ԍ��@�X�e�[�W�Ƃ͈Ⴂ���p�n�_�ɂ�����U����ԍ�
        std::string stageNum;   //�X�e�[�W�ԍ��@������Ƃ��ĊǗ��@b�������Ă����ꍇ�A�{�X�}�b�v�@���p�n�_�̏ꍇ��-1


        double x;   //�|�C���g��X�ʒu�@�G���A�ړ��̃|�C���g�̏ꍇ�A�����蔻��Ƃ͈Ⴄ���ۂɃL����������X�ʒu
        double y;   //�|�C���g��Y�ʒu�@�G���A�ړ��̃|�C���g�̏ꍇ�A�����蔻��Ƃ͈Ⴄ���ۂɃL����������Y�ʒu

        int hitType;    //�����蔻��̃^�C�v


        int isStayPoint;   //�~�܂��|�C���g���ǂ���   0�Ŏ~�܂�Ȃ� 1�Ŏ~�܂��

        int isPoint;        //�|�C���g���L�����ǂ����i�N���A���ł��̃|�C���g���o����悤�ɂȂ��Ă��邩�j0�Ŗ����@1�ŗL��
		int isClear;		//���̃|�C���g���N���A�ς݂�

        std::vector<PointConnect*> pointConect;

		std::shared_ptr<Sprite> _stageIcon;
		std::shared_ptr<Sprite> _stagePoint;

    };


    struct PointConnect {

        int connectPointNum;    //�q�����Ă���|�C���g�ԍ�
        double PointCost;          //���̃|�C���g�Ƃ̋���


    };

    struct RoadConnect {

        int road[2];    //�q�����Ă���|�C���g���


    };



    struct SearchPointConnect {

        int pointNum;           //�|�C���g�ԍ�
        int connectPointNum;    //���̔ԍ��֍s���ŒZ�o�H�̔ԍ�

        bool isSearch;

        double PointCost;          //���̃|�C���g�ɍs���܂ł̃R�X�g


    };


    struct AreaConnectPoint : public  Point {

        int moveArea;

        double buttonX;  //�G���A�ړ��̃|�C���g�̏ꍇ�A�����蔻���X�ʒu
        double buttonY;  //�G���A�ړ��̃|�C���g�̏ꍇ�A�����蔻���Y�ʒu

        int firstMovePoint; //�G���A�ړ����I�������A�ŏ��ɖڎw���ړI�n

        double firstX;  //�G���A�ړ���A�ŏ��ɂ���X�ʒu
        double firstY;  //�G���A�ړ���A�ŏ��ɂ���Y�ʒu

		std::shared_ptr<Sprite> _signboard;

        ~AreaConnectPoint() {};

    };






    //int nowMyStageNum;    //���݂���X�e�[�W�@���p�n�_�̏ꍇ��-1�A�ړ�����-2
    int nowMyPointNum;    //���݂���|�C���g�@�X�e�[�W�Ƃ͈Ⴄ�ԍ��@���S�Ƀ|�C���g�ʂ̔ԍ�
    int nowMyAreaNum;      //���݂���G���A

    //bool isMyMove;    //�ړ����Ă��邩�ǂ����A�ړ�����true�A�ړ����Ă��Ȃ����false

    //int finishPointNum; //�ړ������ꍇ�̍ŏI�ڕW�|�C���g�̔ԍ� �ړ����Ă��Ȃ����-1

    double myX; //���݂�X���W
    double myY; //���݂�Y���W
    double mySpeed; //�����̈ړ��X�s�[�h�i�ρj
	double mySpeedFrameNum; //�����̈ړ��X�s�[�h�i�����B���t���[���ňړ����I���邩�j

    double myVx;    //x�����̑��x
    double myVy;    //y�����̑��x


    const double pointR = 9;   //�|�C���g�̕`�ʂ���~�̔��a�@�f�o�b�N�p�@����������


    std::vector<std::shared_ptr<Point>> nowAreaPointList;   //���݂̃G���A�̃|�C���g�̃��X�g
    std::vector<RoadConnect*> nowRoadConnect;   //���݂̃G���A�̓��̌q����̏��

    std::vector<int> movePointList;

    int AreaGrBuffer[8];
    int* AreaGr;

	Animator _animator;

    int blend;
    int blendFlag;

    bool isPause;
    bool keyFlag;
    bool clickFlag;

    std::string stageNum;

    std::string class_name;

    Sound _bgm;

    bool debugFlag;

	Sprite _shopIcon;

	Sprite _MenuButton;

	Sprite _StageSelectMenuButton;
	Sprite _StageSelectMenuButton_Open;

	bool canStageSelectButton1;
	bool canStageSelectButton2;
	bool canStageSelectButton3;
	Sprite _StageSelectButton1;
	Sprite _StageSelectButton2;
	Sprite _StageSelectButton3;

	std::vector<int> stageSelectNumber;

	bool isStageSelectButtonOpen;

	Sprite _SaveButton;

	std::shared_ptr<WorldMap_Menu> worldMap_Menu;

	TutorialPanel* tutorialPanel;

	Sprite PauseUI;
	Sprite _black;

	Sprite _AreaIcon;

	PopUpWindow_YES  popUpWindow;

	Sound _decide,_flame,_openTreasure;

public:
    WorldMap();
	WorldMap(int AreaNum, int PointNum);
	WorldMap(int AreaNum, int PointNum, bool isClear);
    ~WorldMap();


    std::string GetClassNameData() const {
        return class_name;
    }

    SceneBase* Update();
    SceneBase* Update(UIManager _ui) override;
    void Draw() override;

    void Init(int AreaNum, int PointNum);    //������

    bool myPointSet_PointNum(int PointNum);  //�����̃|�C���g��ݒ肷��@�}�b�v�ړ��͂��Ȃ��@���s�i�ړ�����|�C���g���ʂ̃}�b�v���j������false�A������true��Ԃ�
    bool myPointSet_StageNum(std::string StageNum);  //�����̃|�C���g��ݒ肷��(�X�e�[�W�ԍ�)�@�}�b�v�ړ��͂��Ȃ��@���s�i�ړ�����|�C���g���ʂ̃}�b�v���j������false�A������true��Ԃ�
    int PointSearch(int pointNum);  //�|�C���g�ԍ����炻�̃|�C���g�ԍ��̈�v����|�C���g����nowMapPointList�̔z��ԍ���Ԃ��@�|�C���g�̏ꍇ���s����

    void setNowAreaPointList();
    void setNowRoadConnect();

    void setPointCost();

	std::weak_ptr<WorldMap::Point> getPoint(int pointNum);

    void DrawMap();

    int ClickCheckBox(int x, int y, int rx, int ry);    //�N���b�N�����Ƃ��̔���i�l�p�`�j
    int ClickCheckCircle(int x, int y, int r);    //�N���b�N�����Ƃ��̔���i�~�`�j

    bool checkMoveReach(std::weak_ptr<WorldMap::Point> point);

    void movePoint();

    void searchPath(int pointNum);

    void setSearchPointConnect(int pointNum, std::vector<SearchPointConnect*>* spc);

    void setMovePath(int pointNum, std::vector<SearchPointConnect*>* spc);

    void setAreaNum(int areaNum);

	void setPointNum(int pointNum);

	void SetAreaAndPoint(int areaNum, int pointNum);

	bool ClickCheck(Vector2D pos, Vector2D size);

	std::string GetFieldType(std::string stageNum);


	bool CheckLineOfPoint(Vector2D P, Vector2D A, Vector2D B);


	void SetPointInformation();
	bool CanAreaMove(AreaConnectPoint a);

};





