#include "MessageReciever.h"
#include "../DebugDraw.h"
#include "../Resources/ResourceManager.h"
#include "../Resources/AllResourceManager.h"
#include "../Utility/CSVReader.h"

MessageReciever::MessageReciever()
    : _face(Main::RESOURCEFILENAME + "resource/graph/devilGirlUsual.png", Vector2D(42, 547))
    , _messageUI(Main::RESOURCEFILENAME + "resource/graph/ui/message_window.png", Vector2D(20, 520))
{
    _face.SetScale(Vector2D(1.97, 1.97));
	_face.SetPriority(static_cast<int>(Sprite::Priority::UI) + 1);
    _messageUI.SetPriority(Sprite::Priority::UI);

	std::vector<std::string> dataArray;
	CSVReader reader;

	std::string fileName = Main::RESOURCEFILENAME + "csv/talkData/DefineFace.csv";
	reader.Read(RESOURCE_TABLE->GetFolderPath() + fileName, dataArray, 1);

	int max1 = 0;
	int max2 = 0;
	for (int i = 0; i < dataArray.size(); i += 3) {
		if (std::stoi(dataArray[i]) > max1) {
			max1 = std::stoi(dataArray[i]);
		}
		if (std::stoi(dataArray[i + 1]) > max2) {
			max2 = std::stoi(dataArray[i + 1]);
		}
	}

	_faces.resize(max1 + 1);

	for (int i = 0; i<_faces.size(); i++) {
		_faces[i].resize(max2 + 1);
	}

	for (int i = 0; i < dataArray.size(); i += 3) {
		
		_faces[std::stoi(dataArray[i])][std::stoi(dataArray[i + 1])] = IMAGE_RESOURCE_TABLE->Create(Main::RESOURCEFILENAME + "resource/graph/" + dataArray[i + 2]);

	}

}


MessageReciever::~MessageReciever()
{
}


void MessageReciever::Init()
{
    _processer.Init();
}


void MessageReciever::Recieve(const TalkDatabase& senderData)
{
    std::string typeStr = (senderData.TalkType == Talk_Type::dynamic) ? "dynamic" : "nomal";
    _processer.IntoTalkStorage(typeStr, senderData.TalkGroupNum);
}


void MessageReciever::Update()
{
    _processer.Update();
}


void MessageReciever::Draw()
{
    //î≠òbé“ê›íË
    if (!_processer.isEmpty())
    {
        auto currentData = _processer.GetCurrentTalkData();
		if (0 <= currentData.speakPeople &&  currentData.speakPeople < _faces.size()) {
			if (0 <= currentData.speakFace &&  currentData.speakFace < _faces[currentData.speakPeople].size()) {
				_face.SetResource(_faces[currentData.speakPeople][currentData.speakFace]);
			}
		}
            
	}
	else {
		_face.SetDisplayMode(false);
	}

    //ï∂éöëóÇËï`âÊ
    _processer.Draw(Vector2D(210, 530));
}


void MessageReciever::DrawFalse() {
	_face.SetDisplayMode(false);
	_messageUI.SetDisplayMode(false);
}

void MessageReciever::DrawTrue() {
	_face.SetDisplayMode(true);
	_messageUI.SetDisplayMode(true);
}

