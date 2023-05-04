#include "Process.h"
#include "Scheduler.h"
//Process Constructor
Process::Process(int ID, int AT, int CT, int DL, int IO_N , Scheduler* SchPtr)
	: PID(ID), ArrivalTime(AT), CPUTime(CT), Deadline(DL), IO_N(IO_N), CrntState(NEW) , SchedulerPtr(SchPtr)
{
	//initializing all data members
	TerminationTime = 0;
	TurnAroundTime = 0;
	WaitingTime = 0;
	ResponseTime = 0;
	totalIO_D = 0;
	ProcessedTime = 0;
	ChildPtr = nullptr;
	ParentPtr = nullptr;
	firstTimeExecution = 1;
}

ostream& operator<<(ostream& out, const Process* P)
{
	out << P->PID;
	return out;
}

void Process::SetChild(Process* ForkedProcess)
{
	ChildPtr = ForkedProcess;
}

void Process::SetParent(Process* Ptr)
{
	ParentPtr = Ptr;
}

//Set the response time as FCPU is the time when the process is ready to be processed at first time
void Process::SetResponseTime(int FCPU)
{
	ResponseTime = FCPU - ArrivalTime;

	//since this function will only be used once for each process
	//which is when it is being executed for the first time,
	//the function changes this data member to false 
	//to stop the processors from overrwriting the RT

	firstTimeExecution = 0;
}

void Process::AddIORequest(int IO_R, int IO_D)
{
	IO_Request* newRequest = new IO_Request(IO_R, IO_D);

	IO_RequestQ.Enqueue(newRequest);

	totalIO_D += IO_D;
}

int Process::GetCPUTime() const
{
	return CPUTime;
}

void Process::SetTerminationTime(int TT)
{
	TerminationTime = TT;
	TurnAroundTime = TerminationTime - ArrivalTime;
	WaitingTime = TurnAroundTime - ProcessedTime;
}

int Process::GetID() const 
{
	return PID;
}

int Process::GetTurnAroundTime() const
{
	return TurnAroundTime;
}

int Process::GetResponseTime() const
{
	return ResponseTime;
}

int Process::GetArrivalTime() const
{
	return ArrivalTime;
}

int Process::GetWaitingTime() const
{
	return WaitingTime;
}

int Process::GetTerminationTime() const
{
	return TerminationTime;
}

int Process::GetDeadline() const
{
	return Deadline;
}

int Process::GetTotalIO_D() const
{
	return totalIO_D;
}

ProcessState Process::GetProcessState() const
{
	return CrntState;
}

Process* Process::GetChild() const
{
	return ChildPtr;
}

Process* Process::GetParent() const
{
	return ParentPtr;
}

int Process::GetProcessedTime() const
{
	return ProcessedTime;
}

int Process::GetRemainingCPUTime() const
{
	return CPUTime - ProcessedTime;
}

int Process::GetIO_D()
{
	return IO_RequestQ.QueueFront()->IO_D;
}

void Process::ChangeProcessState(ProcessState NewState)
{
	CrntState = NewState;
}

bool Process::TimeForIO()
{
	if (!IO_RequestQ.isEmpty() && ProcessedTime == IO_RequestQ.QueueFront()->IO_R)
	{
		return true;
	}
	else
		return false;
}

void Process::DeleteIO_Request() 
{
	IO_Request* CompletedIO_Request;
	IO_RequestQ.Dequeue(CompletedIO_Request);
	delete CompletedIO_Request;
}

void Process::ExecuteProcess()
{
	ProcessedTime++;
}

bool Process::isFirstExecution() const
{
	return firstTimeExecution;
}

bool Process::IsChild() const
{
	if (ParentPtr)
		return true;
	return false;
}

bool Process::IsParent() const
{
	return (ChildPtr);
}

void Process::UpdateTotalWaitingTime()
{
	WaitingTime = (SchedulerPtr->GetTimeStep()-ArrivalTime)-ProcessedTime;
}



Process::~Process()
{
	IO_Request* DeleteIO_Request = nullptr;
	while (!IO_RequestQ.isEmpty())
	{
		IO_RequestQ.Dequeue(DeleteIO_Request);
		delete DeleteIO_Request;
	}
}


