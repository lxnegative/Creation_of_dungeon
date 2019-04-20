#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton
{
public:
	static T *GetInstance()
	{
		static T instance;	    // �܂����Ԃ�����Ă��Ȃ����̂ݍ쐬
		return &instance;		// ���̂�Ԃ�
	}

protected:
	Singleton();
	virtual ~Singleton();

private:
	//�R�s�[�Ƒ���̋֎~
	Singleton(const Singleton &arg);
	Singleton &operator = (const Singleton &arg);
};

#include "Singleton_Private.h"

#endif