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

    //構造体の宣言　実態は下に
    struct Point;
    struct PointConnect;
    struct RoadConnect;
    struct SearchPointConnect;

    struct AreaConnectPoint;
    
    //使う構造体を以下に
    struct Point    //ポイントの情報を表す　ポイントは移動することが出来る場所
    {

        virtual ~Point() {};

        int AreaNum;

        int pointNum;   //ポイント番号　ステージとは違い中継地点にも割り振られる番号
        std::string stageNum;   //ステージ番号　文字列として管理　bが入っていた場合、ボスマップ　中継地点の場合は-1


        double x;   //ポイントのX位置　エリア移動のポイントの場合、あたり判定とは違う実際にキャラが動くX位置
        double y;   //ポイントのY位置　エリア移動のポイントの場合、あたり判定とは違う実際にキャラが動くY位置

        int hitType;    //あたり判定のタイプ


        int isStayPoint;   //止まれるポイントかどうか   0で止まれない 1で止まれる

        int isPoint;        //ポイントが有効かどうか（クリア等でそのポイントが出来るようになっているか）0で無効　1で有効
		int isClear;		//そのポイントがクリア済みか

        std::vector<PointConnect*> pointConect;

		std::shared_ptr<Sprite> _stageIcon;
		std::shared_ptr<Sprite> _stagePoint;

    };


    struct PointConnect {

        int connectPointNum;    //繋がっているポイント番号
        double PointCost;          //そのポイントとの距離


    };

    struct RoadConnect {

        int road[2];    //繋がっているポイント二つ


    };



    struct SearchPointConnect {

        int pointNum;           //ポイント番号
        int connectPointNum;    //その番号へ行く最短経路の番号

        bool isSearch;

        double PointCost;          //そのポイントに行くまでのコスト


    };


    struct AreaConnectPoint : public  Point {

        int moveArea;

        double buttonX;  //エリア移動のポイントの場合、あたり判定のX位置
        double buttonY;  //エリア移動のポイントの場合、あたり判定のY位置

        int firstMovePoint; //エリア移動が終わった後、最初に目指す目的地

        double firstX;  //エリア移動後、最初にいるX位置
        double firstY;  //エリア移動後、最初にいるY位置

		std::shared_ptr<Sprite> _signboard;

        ~AreaConnectPoint() {};

    };






    //int nowMyStageNum;    //現在いるステージ　中継地点の場合は-1、移動中は-2
    int nowMyPointNum;    //現在いるポイント　ステージとは違う番号　完全にポイント別の番号
    int nowMyAreaNum;      //現在いるエリア

    //bool isMyMove;    //移動しているかどうか、移動中はtrue、移動していなければfalse

    //int finishPointNum; //移動した場合の最終目標ポイントの番号 移動していなければ-1

    double myX; //現在のX座標
    double myY; //現在のY座標
    double mySpeed; //自分の移動スピード（可変）
	double mySpeedFrameNum; //自分の移動スピード（割合。何フレームで移動を終えるか）

    double myVx;    //x方向の速度
    double myVy;    //y方向の速度


    const double pointR = 9;   //ポイントの描写する円の半径　デバック用　多分消える


    std::vector<std::shared_ptr<Point>> nowAreaPointList;   //現在のエリアのポイントのリスト
    std::vector<RoadConnect*> nowRoadConnect;   //現在のエリアの道の繋がりの情報

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

    void Init(int AreaNum, int PointNum);    //初期化

    bool myPointSet_PointNum(int PointNum);  //自分のポイントを設定する　マップ移動はしない　失敗（移動するポイントが別のマップ等）したらfalse、成功でtrueを返す
    bool myPointSet_StageNum(std::string StageNum);  //自分のポイントを設定する(ステージ番号)　マップ移動はしない　失敗（移動するポイントが別のマップ等）したらfalse、成功でtrueを返す
    int PointSearch(int pointNum);  //ポイント番号からそのポイント番号の一致するポイント情報のnowMapPointListの配列番号を返す　ポイントの場合失敗する

    void setNowAreaPointList();
    void setNowRoadConnect();

    void setPointCost();

	std::weak_ptr<WorldMap::Point> getPoint(int pointNum);

    void DrawMap();

    int ClickCheckBox(int x, int y, int rx, int ry);    //クリックしたときの判定（四角形）
    int ClickCheckCircle(int x, int y, int r);    //クリックしたときの判定（円形）

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





