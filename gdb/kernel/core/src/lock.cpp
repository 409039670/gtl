#include "config.h"
#include "lock.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


static    Mutex g_Mutex;
static    UniqueLlock  g_UniqueLlock(g_Mutex);

void Lock::lock(){
	g_UniqueLlock.lock();
}
void Lock::unlock(){
	g_UniqueLlock.unlock();
}
bool Lock::tryLock(){
	return g_UniqueLlock.try_lock();
}
UniqueLlock & Lock::getUniqueLock(){
	return g_UniqueLlock;
}


//Mutex::Mutex(){
//	_hMutex=0;
//	_hMutex = ::CreateMutex( 0, false,0 );
//}
//
//Mutex::~Mutex(){
//	ReleaseMutex(_hMutex); 
//}
//
//Mutex* Mutex::create(){
//	return new Mutex();
//}
//
//void Mutex::release(Mutex* pm){
//	delete pm;
//	pm=0;
//}
//
//void Mutex::lock ()	{		
//	if (_hMutex == 0 )
//		_hMutex = ::CreateMutex( 0, false,0 );
//	WaitForSingleObject( _hMutex, INFINITE );			
//}     	
//
//void Mutex::unlock () { 
//	ReleaseMutex(_hMutex); 
//	_hMutex=0;
//}  
//
////////////////////////////////////////////////////////////////////
//CriticalSection::CriticalSection() {
//	InitializeCriticalSection(&_sect);
//}
//
//CriticalSection::~CriticalSection(){
//	DeleteCriticalSection(&_sect);
//}
//
//void CriticalSection::lock() {
//	EnterCriticalSection(&_sect);
//}
//
//void CriticalSection::unlock(){
//	LeaveCriticalSection(&_sect);
//}
//
///////////////////////////////////////////////////////////////////
//#define SEM_VALUE_MAX ((int) ((~0u) >> 1))
//
//Semaphore::Semaphore( int init  )
//{ _sema = CreateSemaphore(0,init,SEM_VALUE_MAX,0); }
//
//Semaphore::~Semaphore()
//{ CloseHandle(_sema); }
//
//void Semaphore::wait() const
//{ WaitForSingleObject((HANDLE)_sema,INFINITE); }
//
//
//
//int Semaphore::post() const
//{ return (ReleaseSemaphore((HANDLE)_sema,1,0)?0:ERANGE); }
//
//int Semaphore::getValue() const
//{ LONG V = -1; ReleaseSemaphore((HANDLE)_sema,0,&V); return V; }
//
//void Semaphore::reset( int init  )
//{
//	CloseHandle(_sema);
//	_sema = CreateSemaphore(0,init,SEM_VALUE_MAX,0);
//}


///////////////////////////////////////////////////////////////////
//Thread::Thread(string name ){
//	_name = name;
//}
//Thread::Thread(string name  ,ThreadProcSharedPtr  p  ) {
//	_name = name;
//	_proc=p;
//}
//Thread::~ Thread() {
//	//_endthread();
//	::CloseHandle((HANDLE)_handle);
//}
//unsigned Thread::getID() {
//	return _id;
//}
//string Thread::getName() {
//	return _name;
//}
//void Thread::setName(string name) {
//	_name=name;
//}
//void Thread::setProcedure(ThreadProcSharedPtr p) {
//	_proc=p;
//}
//ThreadProcSharedPtr Thread::getProcedure() {
//	return _proc;
//}
//long Thread::run(){
//	if(_proc)
//		return _proc->run();
//	return 0;
//}
//
//unsigned  Thread::threadEntryPoint(void * p){
//	return ((Thread*)p)->run();
//}
//
//unsigned long Thread::resume(){
//	return ::ResumeThread((HANDLE)_handle);
//}
//////////////////////////////////////////////////////////////////////////////////
//ThreadSharedPtr ThreadPool::createThread(string name,  ThreadProcSharedPtr pp, bool bSuspended ){
//	ThreadSharedPtr p (new Thread(name,pp));
//	p->_handle = (void*)_beginthreadex(0,//security
//		0,//stack size
//		Thread::threadEntryPoint,
//		p.get(),//arg list		
//		bSuspended? CREATE_SUSPENDED:0,// so we can later call ResumeThread()
//		&(p->_id));
//	p->run();
//	return p;
//}   
//ThreadPool::ThreadPool() {
//
//}
//ThreadPool::~ThreadPool() {
//	_pool.clear();
//}
//std::vector<ThreadSharedPtr> & ThreadPool::getPool() {
//	return _pool;
//}
//

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
