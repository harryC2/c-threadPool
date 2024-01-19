# c-threadPool
thread pool

class Task
{
 public:
  Task();
	Task(void* data); 
  virtual ~Task();
  
  void* get_data();
  void set_data(void* data); // 任务数据
   
   virtual void run() = 0; //  任务实现
   virtual void destory() =0; // 任务销毁
protected: 
   void * m_data;
   Mutex  m_mutex;
};


int thread = 8;

Singleton<TaskDispathcher>:: instance()->init(threads); // 初始化线程池，8个线程


//任务分发
Task * task = new EchoTask();
Singlton<TaskDispatcher>::instance()->assign(task); //分派任务

