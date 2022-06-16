// MIT license

#include "httpdiot.h"

using namespace httpdiot;

void httpdiot::thread_work(ThreadContext* c)
{
	c->m_state = ThreadState::Live;
	while (1)
	{
		Sleep(1);



		if (c->m_state == ThreadState::NeedToStop)
			break;
	}
	c->m_state = ThreadState::Dead;
}

