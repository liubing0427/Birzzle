#include "ThreadPool.h"
#include <assert.h>

ThreadPool::ThreadPool(void)
{
	shutdown=0;
	max_thread_num=5;
	cur_queue_size=0;
	this->threads.clear();
}

ThreadPool::~ThreadPool(void)
{
}

static ThreadPool* sharedPool=nullptr;
ThreadPool* ThreadPool::getInstance()
{
	if (!::sharedPool)
	{
		::sharedPool=new ThreadPool;
	}
	return ::sharedPool;
}

void ThreadPool::pool_init(int max_thread_num)
{
	this->max_thread_num=max_thread_num;

	for (auto i=0;i<this->max_thread_num;i++)
	{
		this->threads.push_back(std::thread([&](){
			while (true)
			{
				CThread_worker* worker=nullptr;
				{
					std::unique_lock<std::mutex> locker(this->queue_lock);
					while (this->cur_queue_size==0 &&  !this->shutdown)
					{
						this->queue_ready.wait(locker);
					}

					if (this->shutdown)
					{
						return;
					}
					assert(this->cur_queue_size!=0);
					assert(this->queue_head!=nullptr);

					this->cur_queue_size--;
					worker=this->queue_head;
					this->queue_head=worker->next;
				}
				(*(worker->process))(worker->arg);
				free(worker);
				worker=nullptr;
			}
			return;
		}));
	}
}

int ThreadPool::pool_add_worker(void*(*process)(void* arg),void* arg)
{
	auto newworker=(CThread_worker*)malloc(sizeof(CThread_worker));
	newworker->process=process;
	newworker->arg=arg;

	newworker->next=nullptr;

	std::unique_lock<std::mutex> locker(this->queue_lock);

	auto member=this->queue_head;
	if (member != nullptr)
	{
		while (member->next != nullptr)
		{
			member=member->next;
		}
		member->next=newworker;
	}
	else
	{
		this->queue_head=newworker;
	}

	assert(this->queue_head != nullptr);
	this->cur_queue_size++;

	this->queue_ready.notify_one();

	return 0;
}

int ThreadPool::pool_destroy()
{
	if (this->shutdown)
	{
		return -1;
	}
	this->shutdown=1;

	this->queue_ready.notify_all();

	for (auto& thread1 : this->threads)
	{
		thread1.join();
	}

	CThread_worker* head=nullptr;

	while (this->queue_head != nullptr)
	{
		head=this->queue_head;
		this->queue_head=this->queue_head->next;
		free(head);
	}
	return 0;
}