#ifndef MouseInput_h
#define MouseInput_h

#include <unordered_map>
#include <memory>
#include "../Utility/Singleton.h"
#include "ButtonTypeInput.h"
#include "MouseTypeInputEvent.h"
#include "DxLib.h"

class MouseInput : public Singleton<MouseInput>
{
    friend class Singleton<MouseInput>;
    friend class InputManager;

public:

    enum class MouseButtonCode
    {
        MOUSE_L = 0,
        MOUSE_R,
        MOUSE_CENTER
    };

    //Event�̏�Ԃ��m�F���邽�߂̃I�u�W�F�N�g
    struct MouseEvent
    {
        MouseEvent(std::function<void(Vector2D)>* eve
            , MouseButtonCode code
            , MouseTypeInputEvent::State state)
            : _event(eve), _code(code), _state(state) {}

        const std::function<void(Vector2D)>* _event;
        const MouseButtonCode _code;
        const MouseTypeInputEvent::State _state;
    };

    //�w��̃{�^�����e��Ԃɂ��邩�ǂ���
    bool ButtonUp(MouseButtonCode button) const { return _inputMap.at(button)->_inputData.ButtonUp(); };
    bool ButtonDown(MouseButtonCode button) const { return _inputMap.at(button)->_inputData.ButtonDown(); };
    bool ButtonBeingPressed(MouseButtonCode button) const { return _inputMap.at(button)->_inputData.ButtonBeingPressed(); };

    //���N���b�N���_�u���N���b�N���ꂽ���ǂ���
    bool DoubleClicked() const { 
		if (isDoubleClicked) {
			return _hasDoubleClicked;
		}
		else {
			return false;
		}
		
	};

    //�J�[�\���̍��W���擾
    Vector2D GetCursorPos() const { return _cursorPos; };

    std::shared_ptr<MouseEvent> AddEvent(MouseButtonCode code, MouseTypeInputEvent::State state, std::function<void(Vector2D)> eve)
    {
        auto eventRef = GetEvent(code)->AddEvent(state, eve);
        return std::make_shared<MouseEvent>(eventRef, code, state);
    }
    void RemoveEvent(const std::shared_ptr<MouseEvent> eventRef)
    {
        GetEvent(eventRef->_code)->RemoveEvent(eventRef->_state, eventRef->_event);
    }

	void SetIsDoubleClicked(bool a) {
		isDoubleClicked = a;
	}

private:

    MouseInput();
    ~MouseInput();

    void RegistRecords();
    void Update();
    void Clear();

    ButtonTypeInput *GetButton(MouseButtonCode button) { return &_inputMap.at(button)->_inputData; }
    MouseTypeInputEvent *GetEvent(MouseButtonCode button) { return &_inputMap.at(button)->_inputEvents; }

    void UpdateDoubleClickStaet();

    struct MouseData
    {
        MouseData(MouseButtonCode code, int value)
            : _code(code)
            , _inputCode(value)
            , _inputEvents(&_inputData)
        {
        }

        //���C�u�����ŗp�ӂ����L�[���͎��ʂ̗񋓌^
        MouseButtonCode _code;
        //API�ɗp�ӂ���Ă�L�[���͂̒l
        int _inputCode;
        //�{�^���̓��͏��
        ButtonTypeInput _inputData;
        //���͏�ԂɊ�Â����C�x���g
        MouseTypeInputEvent _inputEvents;
    };

    void AddRecord(MouseButtonCode code, int value);
    std::unordered_map<MouseButtonCode, MouseData *> _inputMap;
    std::vector<MouseData *> _inputs;

    Vector2D _cursorPos;

    //�_�u���N���b�N���o�p
    bool _hasClicked;
    bool _hasDoubleClicked;
    int _countFromFirstClick;

	bool isDoubleClicked;

};

#define MOUSE MouseInput::GetInstance()

#endif /* MouseInput_h */
