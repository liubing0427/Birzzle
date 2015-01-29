#ifndef __MyBeefView__ThreadPool__
#define __MyBeefView__ThreadPool__
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>

typedef struct worker
{
	void*(*process)(void* arg);
	void *arg;
	struct worker *next;
}CThread_worker;

class ThreadPool
{
public:
	ThreadPool(void);
	virtual ~ThreadPool(void);

	static ThreadPool* getInstance();

	void pool_init(int max_thread_num);

	int pool_add_worker(void*(*process)(void* arg),void* arg);
	int pool_destroy();

private:
	std::mutex  queue_lock;
	std::condition_variable queue_ready;
	std::vector<std::thread> threads;

	CThread_worker *queue_head;
	int shutdown;
	int max_thread_num;
	int cur_queue_size;
};

#endif /* defined(__MyBeefView__ThreadPool__) */