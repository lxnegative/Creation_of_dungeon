#include "MouseInput.h"
#include "DxLib.h"
#include "../../Vector2D.h"

MouseInput::MouseInput()
{
    RegistRecords();
	isDoubleClicked = true;
}



MouseInput::~MouseInput()
{
    Clear();
}


void MouseInput::RegistRecords()
{
    AddRecord(MouseButtonCode::MOUSE_L, MOUSE_INPUT_LEFT);
    AddRecord(MouseButtonCode::MOUSE_CENTER, MOUSE_INPUT_MIDDLE);
    AddRecord(MouseButtonCode::MOUSE_R, MOUSE_INPUT_RIGHT);
}


void MouseInput::AddRecord(MouseButtonCode code, int value)
{
    MouseData *data = new MouseData(code, value);
    _inputMap.insert(std::make_pair(code, data));
    _inputs.push_back(data);
}


void MouseInput::Update()
{
    //�C�w�J���g�����o��
    //glfwPollEvents();
    
    int x = 0;
    int y = 0;

    GetMousePoint(&x, &y);
    _cursorPos.Set(x, y);

	int value = GetMouseInput();

    for (auto input : _inputs)
    {
        ButtonTypeInput &data = input->_inputData;
		int state = value & input->_inputCode;
		if (state != 0)
		{
			data.UpdateState(true);
		}
		else
		{
			data.UpdateState(false);
		}
        MouseTypeInputEvent &events = input->_inputEvents;
        events.Update(_cursorPos);
    }

    UpdateDoubleClickStaet();
}


void MouseInput::UpdateDoubleClickStaet()
{
    _hasDoubleClicked = false;

    //�_�u���N���b�N���o
    if (_hasClicked)
        _countFromFirstClick++;

    if (ButtonDown(MouseInput::MouseButtonCode::MOUSE_L))
    {
        //����N���b�N�����o�Ȃ�
        if (!_hasClicked)
        {
            _hasClicked = true;
            _countFromFirstClick = 0;
        }
        //�K��t���[�����ɃN���b�N�����o���Ă�Ȃ�
        else if (_countFromFirstClick < 30)
        {
            _hasDoubleClicked = true;

            //�J�E���g���Z�b�g
            _hasClicked = false;
            _countFromFirstClick = 0;
            return;
        }
    }

    //�J�E���g���Z�b�g
    if (_countFromFirstClick == 30)
    {
        _hasClicked = false;
        _countFromFirstClick = 0;
    }
}


void MouseInput::Clear()
{
    for (auto input : _inputs)
    {
        MouseTypeInputEvent &events = input->_inputEvents;
        events.ClearEventsAll();
    }
}