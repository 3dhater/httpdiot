// MIT license
#pragma once
#ifndef __HTTPDIOTMUTEX_H_
#define __HTTPDIOTMUTEX_H_

namespace httpdiot
{
	class Mutex
	{
		SRWLOCK m_srwLock = SRWLOCK_INIT;
	public:
		Mutex() 
		{
			AcquireSRWLockExclusive(&m_srwLock);
		}

		~Mutex()
		{
			ReleaseSRWLockExclusive(&m_srwLock);
		}
	};
}

#endif
