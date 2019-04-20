#include "Sound.h"
#include "DxLib.h"
#include "../Resources/ResourceManager.h"

//��{���ʂ̏����l
int Sound::BGM_BASE_VOLUME = 50;
int Sound::SE_BASE_VOLUME = 75;

Sound::Sound(std::string fileName, bool isLoopType)
    : _isLoop(isLoopType)
{
    if (fileName == "")
        return;

    //�t�@�C����ǂݍ���ŉ����f�[�^����
    _soundResource = SOUND_RESOURCE_TABLE->Create(SOUND_RESOURCE_TABLE->GetFolderPath() + fileName);
}


Sound::~Sound()
{
	if (_soundResource) {
		Stop();
	}
    _soundResource = nullptr;
}


void Sound::Load(std::string fileName)
{
    //�t�@�C����ǂݍ���ŉ����f�[�^����
    _soundResource = SOUND_RESOURCE_TABLE->Create(SOUND_RESOURCE_TABLE->GetFolderPath() + fileName);
}


void Sound::Play()
{
    if (_isLoop)
        PlaySoundMem(_soundResource->GetHandle(), DX_PLAYTYPE_LOOP);
    else
        PlaySoundMem(_soundResource->GetHandle(), DX_PLAYTYPE_BACK);
}


void Sound::Stop()
{
    StopSoundMem(_soundResource->GetHandle());
}

void Sound::SetLoop(int loopPos)
{
    SetLoopPosSoundMem(loopPos, _soundResource->GetHandle());
}

void Sound::SetVolume(int volume)
{
    //��{���ʂ�����������
    _volume = volume;

    //�����t�@�C���^�C�v�̊�{���ʂ��擾����
    // MEMO : _isBGM ���ꎞ�I�ɔp�~���Ă���̂� _isLoop���ǂ����Ŕ���
    double ratio = (_isLoop) ? static_cast<double>(BGM_BASE_VOLUME)
        : static_cast<double>(SE_BASE_VOLUME);
    ratio /= 100.0;

    //0 - 255 �̒l��BASE_VOLUME�̔{���ōŏI�I�ȉ��ʌ���
    ChangeVolumeSoundMem(static_cast<int>(_volume * ratio), _soundResource->GetHandle());
}

bool Sound::IsPlaying() const
{
    return (CheckSoundMem(_soundResource->GetHandle()) == 1);
}