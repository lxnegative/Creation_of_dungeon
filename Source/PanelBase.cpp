#include "PanelBase.h"



PanelBase::PanelBase()
{
}


PanelBase::~PanelBase()
{
}

void PanelBase::Update()
{
}

void PanelBase::Draw()
{
}

void PanelBase::Init(PanelContent& _panelContent)
{
}

std::string PanelBase::GetCategoryName()
{
    return std::string();
}

void PanelBase::SetSettingObject(std::vector<PanelBase>& _tps)
{
}

bool PanelBase::IsClicked()
{
    return false;
}

void PanelBase::DrawDebugPrint()
{
    printfDx("PanelBase:\n");
}

bool PanelBase::GetIsClicked(PanelContent& panel)
{
    if (!MOUSE->ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
        return false;

    //�N���b�N�ʒu���擾
    auto cursorPos = MOUSE->GetCursorPos();
/*
    /*�p�l���̏c���T�C�Y���擾�A���Lif���Ŕ���*/
/*    auto _w = static_cast<int>(panel._position._x + panel._graph.GetSize()._x);
    auto _h = static_cast<int>(panel._position._y + panel._graph.GetSize()._y);

    return(cursorPos._x >= panel._position._x && cursorPos._x <= _w
        && cursorPos._y >= panel._position._y && cursorPos._y <= _h);
    */
    return Contain(cursorPos); //panel.Contain(cursorPos);
}

void PanelBase::SetActive(bool flag)
{
	_graph.SetDisplayMode(flag);
}
