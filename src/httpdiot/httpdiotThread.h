// MIT license
#pragma once
#ifndef __HTTPDIOTTHREAD_H_
#define __HTTPDIOTTHREAD_H_

#include <thread>

namespace httpdiot
{

	enum class ThreadState
	{
		BrandNew,
		Live,
		NeedToStop,
		Dead
	};


	struct ThreadContext
	{
		ThreadState m_state = ThreadState::BrandNew;
		void* m_userData = 0;
	};

	void thread_accept(ThreadContext*);
	void thread_work(ThreadContext*);
}

#endif
