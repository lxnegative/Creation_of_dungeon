#include "TileField.h"
#include "TiledObject.h"


TileField::TileField()
    : _defaultFieldType(FieldType::Cave)
{
}


TileField::~TileField()
{
}


void TileField::Clear()
{
    _gobjs.clear();
    _gobjs.resize(0);
    _fieldSize.Set(0, 0);
}


void TileField::Init(int width, int height)
{
    _fieldSize.Set(width, height);
    _field.resize(height);
    
    for (size_t i = 0; i < _field.size(); i++)
    {
        _field[i].resize(width);
    }
    
    for (size_t i = 0; i < height; i++)
    {
        for (size_t j = 0; j < width; j++)
        {
            _gobjs.push_back(std::make_shared<MapTile>(i, j));
            _field[i][j] = _gobjs[_gobjs.size() - 1];
        }
    }
}


void TileField::Setup()
{
    for (auto obj : _gobjs)
        obj->Init();
}

void TileField::Draw()
{
    for (auto obj : _gobjs)
        obj->Draw();
}


const bool TileField::IsInside(const TiledVector &pos) const
{
    return !(    pos._x < 0 || _field[0].size() <= pos._x
             ||  pos._y < 0 || _field.size() <= pos._y);
}

//�^�C���ɂ�����I�u�W�F�N�g���w��̃^�C���ɓo�^
void TileField::RegistObject(TiledObject &obj, TiledVector pos)
{
    //�o�^�s�Ȃ�A�o�^���Ȃ�
    if (!IsInside(pos))
        return;
    
    auto tile = _field[pos._y][pos._x].lock();

    if (!tile->IsRegistable(obj))
        return;
    
    MapTile::Regist(&obj, tile);
}


//�^�C���ɂ�����I�u�W�F�N�g�����O
void TileField::RemoveObject(TiledObject &obj)
{
    auto pos = obj.GetTilePos();
    
    //�o�^�s�Ȃ�A�o�^���Ȃ�
    if (!IsInside(pos))
        return;
    
    auto tile = _field[pos._y][pos._x].lock();
    tile->Remove(&obj, tile);
}


//���O���ēo�^���鏈������C�ɍs��
void TileField::MoveObject(TiledObject &obj, TiledVector pos)
{
    //�o�^�s�Ȃ�A�o�^���Ȃ�
    if (!IsInside(pos))
        return;
    
    if (!_field[pos._y][pos._x].lock()->IsRegistable(obj))
        return;
    
    RemoveObject(obj);
    RegistObject(obj, pos);
}


int TileField::GetRawNumber(const TiledVector &pos) const
{
    //�͈͊O���Q�Ƃ��悤�Ƃ�����-1��Ԃ�
    if (!IsInside(pos))
        return -1;

    return _field[pos._y][pos._x].lock()->_rawNumber;
}


void TileField::SetRawNumber(const TiledVector &pos, int number)
{
    //�͈͊O���Q�Ƃ��悤�Ƃ�����-1��Ԃ�
    if (!IsInside(pos))
        return;

    _field[pos._y][pos._x].lock()->_rawNumber = number;
}


FieldType TileField::GetFieldType(const TiledVector &pos) const
{
    //�͈͊O���Q�Ƃ��悤�Ƃ�����NONE��Ԃ�
    if (!IsInside(pos))
        return FieldType::None;

    return _field[pos._y][pos._x].lock()->_type;
}

void TileField::SetFieldType(const TiledVector &pos, std::string data)
{
    //�͈͊O���Q�Ƃ��悤�Ƃ�����-1��Ԃ�
    if (!IsInside(pos))
        return;

    _field[pos._y][pos._x].lock()->SetFieldType(data, FieldType::Cave);
}


void TileField::SetDefaultFieldType(std::string data)
{

}

//�^�C���ɂ�����I�u�W�F�N�g���w��̃^�C���ɓo�^
void TileField::RegistBreadcrumb(Breadcrumb *crumb, TiledVector pos)
{
    //�o�^�s�Ȃ�A�o�^���Ȃ�
    if (!IsInside(pos))
        return;
    
    _field[pos._y][pos._x].lock()->Regist(crumb);
}


//�^�C���ɂ�����I�u�W�F�N�g�����O
void TileField::RemoveBreadcrumb(Breadcrumb *crumb)
{
    auto pos = crumb->GetTilePos();
    
    //�o�^�s�Ȃ�A�o�^���Ȃ�
    if (!IsInside(pos))
        return;
    
    _field[pos._y][pos._x].lock()->Remove(crumb);
    delete crumb;
}


std::vector<TiledVector> TileField::GetMovableCell(TiledVector pos, int movableRange)
{
    StepTable stepTable(_fieldSize._y);
    for (size_t i = 0; i < stepTable.size(); i++)
        stepTable[i].resize(_fieldSize._x, 0);
    
    //1��𒲂ׂ�(�㉺���E�𒲂ׂ�)
    CalcMovableCell(pos, stepTable, movableRange, 1, TiledVector::Direction::FORWARD);
    CalcMovableCell(pos, stepTable, movableRange, 1, TiledVector::Direction::BACK);
    CalcMovableCell(pos, stepTable, movableRange, 1, TiledVector::Direction::LEFT);
    CalcMovableCell(pos, stepTable, movableRange, 1, TiledVector::Direction::RIGHT);
    
    std::vector<TiledVector> movableTiles;
    
    for (size_t i = 0; i < _fieldSize._y; i++)
    {
        for (size_t j = 0; j < _fieldSize._x; j++)
        {
            if (stepTable[i][j] != 0)
                movableTiles.push_back(TiledVector(j, i));
        }
    }
    
    return movableTiles;
}


void TileField::CalcMovableCell(const TiledVector &pos, StepTable& stepTable, int step, int degree, TiledVector::Direction dir)
{
    //�w������̈ʒu���Z�o
    int x = pos._x;
    int y = pos._y;
    
    if (dir == TiledVector::Direction::LEFT)
        x--;
    else if (dir == TiledVector::Direction::RIGHT)
        x++;
    
    if (dir == TiledVector::Direction::FORWARD)
        y--;
    else if (dir == TiledVector::Direction::BACK)
        y++;
    
    if(!IsInside(TiledVector(x, y)))
        return;
    
    //�w��ʒu��Cell���ړ��\�����ׂ�
    std::weak_ptr<MapTile> cell = _field[y][x];
    TiledObject *obj = cell.lock()->GetTiledObject<TiledObject>();
    bool isMovable = (obj == nullptr) || (obj != nullptr && obj->GetType() != TiledObject::Type::BLOCK);
    
    //�w��ʒu�̃}�X���ړ��\�Ȃ�
    if (isMovable)
    {
        //���̃}�X�����`�F�b�N(=�����L�^���Ȃ�)��
        //�`�F�b�N�ς݂ł���菭�Ȃ��萔�ňړ��ł���Ȃ�
        if (stepTable[y][x] == 0
            || degree < stepTable[y][x])
        {
            //�萔����
            stepTable[y][x] = degree;
        }
        
        //�ړ��\�ʂ��J�E���g�_�E��
        step--;
        //�J�n�n�_����̋������J�E���g�A�b�v
        degree++;
        
        //�ړ��\�ʂ�0���傫���Ȃ�
        if (0 < step)
        {
            //4aa. ����ɏ㉺���E�𒲂ׂ�
            //���̂Ƃ��A�i��ł��������ɖ߂�Ȃ��悤�ɂ���(���ɒ��ׂĂ��邽��)
            if (dir != TiledVector::Direction::BACK)
                CalcMovableCell(TiledVector(x, y), stepTable, step, degree, TiledVector::Direction::FORWARD);
            if (dir != TiledVector::Direction::FORWARD)
                CalcMovableCell(TiledVector(x, y), stepTable, step, degree, TiledVector::Direction::BACK);
            if (dir != TiledVector::Direction::RIGHT)
                CalcMovableCell(TiledVector(x, y), stepTable, step, degree, TiledVector::Direction::LEFT);
            if (dir != TiledVector::Direction::LEFT)
                CalcMovableCell(TiledVector(x, y), stepTable, step, degree, TiledVector::Direction::RIGHT);
        }
        //�����łȂ��Ȃ�
        else
        {
            //�I��
            return;
        }
    }
    //�����łȂ����
    else
    {
        //�I��
        return;
    }
}


std::vector<TiledVector> TileField::GetParabolicMovableCell(TiledVector pos, int movableRange, TiledVector::Direction directon)
{
    StepTable stepTable(_fieldSize._y);
    for (size_t i = 0; i < stepTable.size(); i++)
        stepTable[i].resize(_fieldSize._x, 0);
    
    //�����ȊO��3������1�}�X��������ׂ�
	//�w��ȊO��2�}�X���ׂ�
    if (directon == TiledVector::Direction::LEFT)
    {
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::RIGHT);
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::FORWARD);
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::BACK);

		CalcMovableCell(pos + TiledVector::up, stepTable, 1, 1, TiledVector::Direction::BACK);
		CalcMovableCell(pos + TiledVector::down, stepTable, 1, 1, TiledVector::Direction::FORWARD);
    }
    else if (directon == TiledVector::Direction::RIGHT)
    {
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::LEFT);
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::FORWARD);
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::BACK);

		CalcMovableCell(pos + TiledVector::up, stepTable, 1, 1, TiledVector::Direction::BACK);
		CalcMovableCell(pos + TiledVector::down, stepTable, 1, 1, TiledVector::Direction::FORWARD);
    }
    else if (directon == TiledVector::Direction::FORWARD)
    {
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::RIGHT);
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::LEFT);
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::FORWARD);

		CalcMovableCell(pos + TiledVector::left, stepTable, 1, 1, TiledVector::Direction::LEFT);
		CalcMovableCell(pos + TiledVector::right, stepTable, 1, 1, TiledVector::Direction::RIGHT);
    }
    else if (directon == TiledVector::Direction::BACK)
    {
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::RIGHT);
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::BACK);
        CalcMovableCell(pos, stepTable, 1, 1, TiledVector::Direction::LEFT);

		CalcMovableCell(pos + TiledVector::left, stepTable, 1, 1, TiledVector::Direction::LEFT);
		CalcMovableCell(pos + TiledVector::right, stepTable, 1, 1, TiledVector::Direction::RIGHT);
    }
    
	
    //���炵�Ă���
    int offset = 1;
    if (directon == TiledVector::Direction::LEFT)
        pos._x += offset;
    else if (directon == TiledVector::Direction::RIGHT)
        pos._x -= offset;
    
    if (directon == TiledVector::Direction::FORWARD)
        pos._y -= offset;
    else if (directon == TiledVector::Direction::BACK)
        pos._y += offset;
		
    
    //�����̐�𒲂ׂ�
    CalcParabolicMovableCell(pos, pos, stepTable, movableRange + offset, directon, directon);
    
    std::vector<TiledVector> movableTiles;
    
    for (size_t i = 0; i < _fieldSize._y; i++)
    {
        for (size_t j = 0; j < _fieldSize._x; j++)
        {
            if (stepTable[i][j] != 0)
                movableTiles.push_back(TiledVector(j, i));
        }
    }
    
    return movableTiles;
}


void TileField::CalcParabolicMovableCell(TiledVector pos, const TiledVector &basePos, StepTable& stepTable, int range, TiledVector::Direction dir, TiledVector::Direction baseDir)
{
    //�w������̈ʒu���Z�o
    int x = pos._x;
    int y = pos._y;
    
    if (dir == TiledVector::Direction::LEFT)
        x--;
    else if (dir == TiledVector::Direction::RIGHT)
        x++;
    
    if (dir == TiledVector::Direction::FORWARD)
        y++;
    else if (dir == TiledVector::Direction::BACK)
        y--;
    
    if (baseDir != dir)
    {
        if (baseDir == TiledVector::Direction::LEFT)
            x--;
        else if (baseDir == TiledVector::Direction::RIGHT)
            x++;
        
        if (baseDir == TiledVector::Direction::FORWARD)
            y++;
        else if (baseDir == TiledVector::Direction::BACK)
            y--;
    }
    
    pos.Set(x, y);
    
    //�ړ��\������艓����ΏI��
    int degree = (pos - basePos).GetBresenhamLength(true);
    if (range < degree)
        return;
    
    if(!IsInside(TiledVector(x, y)))
        return;
    
    //�w��ʒu��Cell���ړ��\�����ׂ�
    std::weak_ptr<MapTile> cell = _field[y][x];
    TiledObject *obj = cell.lock()->GetTiledObject<TiledObject>();
    bool isMovable = (obj == nullptr) || (obj != nullptr && obj->GetType() != TiledObject::Type::BLOCK);
    
    //�w��ʒu�̃}�X���ړ��\�Ȃ�
    if (!isMovable)
        return;
    
    //���̃}�X�����`�F�b�N(=�����L�^���Ȃ�)��
    //�`�F�b�N�ς݂ł���菭�Ȃ��萔�ňړ��ł���Ȃ�
    if (stepTable[y][x] == 0
        || degree < stepTable[y][x])
    {
        //�萔����
        stepTable[y][x] = degree;
    }
    
    //4aa. ����ɏ㉺���E�𒲂ׂ�
    //���̂Ƃ��A�i��ł��������ɖ߂�Ȃ��悤�ɂ���(���ɒ��ׂĂ��邽��)
    if (    dir != TiledVector::Direction::BACK
        &&  baseDir != TiledVector::Direction::BACK)
        CalcParabolicMovableCell(pos, basePos, stepTable, range, TiledVector::Direction::FORWARD, baseDir);
    if (    dir != TiledVector::Direction::FORWARD
        &&  baseDir != TiledVector::Direction::FORWARD)
        CalcParabolicMovableCell(pos, basePos, stepTable, range, TiledVector::Direction::BACK, baseDir);
    if (    dir != TiledVector::Direction::RIGHT
        &&  baseDir != TiledVector::Direction::RIGHT)
        CalcParabolicMovableCell(pos, basePos, stepTable, range, TiledVector::Direction::LEFT, baseDir);
    if (    dir != TiledVector::Direction::LEFT
        &&  baseDir != TiledVector::Direction::LEFT)
        CalcParabolicMovableCell(pos, basePos, stepTable, range, TiledVector::Direction::RIGHT, baseDir);
}