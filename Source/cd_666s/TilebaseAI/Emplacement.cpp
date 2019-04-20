#include "Emplacement.h"
#include "TileField.h"
#include "TiledObjectMnager.h"
#include "ShottenObject.h"


#include "../InputManager/MouseInput.h"
#include "../DebugDraw.h"
#include "../Resources/ResourceManager.h"


Emplacement::Emplacement(TiledVector pos, int cost, int power, int attack, TiledVector::Direction direction, std::string graphName, std::string arrowGraphName, bool isPhysical)
    : Trap(pos, cost, 5)
    , _power(power)
    , _attack(attack)
    , _shootDirection(direction)
{
    //_graph.Load(Main::RESOURCEFILENAME + "resource/graph/trap/bow.png");

    std::string arrowString = Main::RESOURCEFILENAME + "resource/graph/trap/arrow_";
    
	if (!arrowGraphName.empty()) {
		arrowString = Main::RESOURCEFILENAME + "resource/graph/trap/" + arrowGraphName + "_";
	}
	else {
		arrowString = Main::RESOURCEFILENAME + "resource/graph/trap/arrow_";
	}

	int directionSize = 4;
	
	_directedbulletImage.resize(directionSize);
	_directedGraph.resize(directionSize);

	for (int i = 0; i < directionSize; i++) {

		std::string arrowStringTemp = arrowString;

		switch (i)
		{
		case TiledVector::Direction::BACK:
			arrowStringTemp += "up.png";
			break;

		case TiledVector::Direction::FORWARD:
			arrowStringTemp += "down.png";
			break;

		case TiledVector::Direction::LEFT:
			arrowStringTemp += "left.png";
			break;

		case TiledVector::Direction::RIGHT:
			arrowStringTemp += "right.png";
			break;
		default:
			break;
		}

		_directedbulletImage[i] = IMAGE_RESOURCE_TABLE->Create(arrowStringTemp);

	}

    _bulletImage = _directedbulletImage[_shootDirection];

	std::string _graphPath = Main::RESOURCEFILENAME + "resource/graph/trap/";

	if (!graphName.empty()) {
		_graphPath = _graphPath + graphName;
		_graph.Load(_graphPath + ".png");
	}
	else {
		_graphPath = _graphPath + "bow";
		_graph.Load(_graphPath + ".png");
	}
	
	for (int i = 0; i < directionSize; i++) {

		std::string _graphPathTemp = _graphPath + "_";

		switch (i)
		{
		case TiledVector::Direction::BACK:
			_graphPathTemp += "up.png";
			break;

		case TiledVector::Direction::FORWARD:
			_graphPathTemp += "down.png";
			break;

		case TiledVector::Direction::LEFT:
			_graphPathTemp += "left.png";
			break;

		case TiledVector::Direction::RIGHT:
			_graphPathTemp += "right.png";
			break;
		default:
			break;
		}

		_directedGraph[i] = IMAGE_RESOURCE_TABLE->Create(_graphPathTemp);

	}

	_graph.SetResource(_directedGraph[_shootDirection]);

	_graph.SetPosition(pos.GetWorldPos());

	this->isPhysical = isPhysical;

}


Emplacement::~Emplacement()
{
}


void Emplacement::Activate()
{
    Trap::Activate();

    //ŽË’ö•ûŒü‚©‚ç’²‚×‚é”ÍˆÍ‚ðŒˆ’è
    auto fieldSize = FIELD->GetFieldSize();
    TiledVector move;
    int distance = 0;
    switch (_shootDirection)
    {
    case TiledVector::Direction::LEFT:
        distance = fieldSize._x;
        move = TiledVector::left;
        break;

    case TiledVector::Direction::RIGHT:
        distance = fieldSize._x;
        move = TiledVector::right;
        break;

    case TiledVector::Direction::FORWARD:
        distance = fieldSize._y;
        move = TiledVector::up;
        break;


    case TiledVector::Direction::BACK:
        distance = fieldSize._y;
        move = TiledVector::down;
        break;
    }

    TiledVector tilePos = GetTilePos();
    OBJECT_MGR->Add(std::make_shared<ShottenObject>(_power, _attack, distance, 10, tilePos, _shootDirection, TiledObject::Type::MONSTER, _bulletImage, isPhysical));
}


bool Emplacement::IsActivatable() const
{
    if (!Trap::IsActivatable())
        return false;

    if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
        return false;

    return (Contain(MOUSE->GetCursorPos()));
}


bool Emplacement::Contain(Vector2D pos) const
{
    auto size = _graph.GetTexturePtr().lock()->GetBaseSize();

    if (pos._x < _position._x)
        return false;
    if (pos._y < _position._y)
        return false;
    if (_position._x + size._x * TILE_SCALE < pos._x)
        return false;
    if (_position._y + size._y * TILE_SCALE  < pos._y)
        return false;

    return true;
}

bool Emplacement::IsOverwritable(TiledObject * overwriter)
{
	return false;
}


void Emplacement::SetDirection(TiledVector::Direction dir) {

	_shootDirection = dir;

	_bulletImage = _directedbulletImage[_shootDirection];
	_graph.SetResource(_directedGraph[_shootDirection]);

}
