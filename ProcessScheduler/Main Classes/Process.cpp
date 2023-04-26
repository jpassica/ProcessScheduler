#include "Process.h"

//Process Constructor
Process::Process(int pid, int AT, int CT, int IO_N) 
	: PID(pid), ArrivalTime(AT), CPUTime(CT), IO_N(IO_N), CrntState(NEW)
{
	//initializing all data members
	TerminationTime = 0;
	TurnAroundTime = 0;
	WaitingTime = 0;
	ResponseTime = 0;
	lastUpdatetime = 0;
	totalIO_D = 0;
	processedTime = 0;
	child = nullptr;
}

ostream& operator<<(ostream& out, const Process* P) 
{
	out << P->PID;
	return out;
}

//Set the response time as FCPU is the time when the process is ready to be processed at first time
void Process::SetResponseTime(int FCPU) {
	ResponseTime = FCPU - ArrivalTime;
}

void Process::SetProcessedTime(int t)
{
	processedTime = t;
}

void Process::SetLastUpdateTime(int t)
{
	lastUpdatetime = t;
}

void Process::AddIORequest(int IO_R, int IO_D)
{
	IO_Pairs* newRequest = new IO_Pairs(IO_R, IO_D);

	IO_PairsQ.Enqueue(newRequest);

	totalIO_D += IO_D;
}

int Process::getCPUTime() const
{
	return CPUTime;
}

// Set the termination time as TT is the time when the process go to TRM list after processing 
void Process::SetTerminationTime(int TT) 
{
	TerminationTime = TT;
	TurnAroundTime = TerminationTime - ArrivalTime;
	WaitingTime = TurnAroundTime - CPUTime;
}

//Getter for Process ID
int Process::getPID() const {
	return PID;
}

int Process::getTRT() const
{
	return TurnAroundTime;
}

int Process::getRT() const
{
	return ResponseTime;
}

int Process::getAT() const
{
	return ArrivalTime;
}

int Process::getWT() const
{
	return WaitingTime;
}

int Process::getTT() const
{
	return TerminationTime;
}

int Process::getTotalIO_D() const
{
	return totalIO_D;
}

ProcessState Process::GetProcessState() const
{
	return CrntState;
}

Process* Process::GetChild() const
{
	return child;
}



int Process::GetProcessedTime() const
{
	return processedTime;
}

int Process::GetLastUpdateTime() const
{
	return lastUpdatetime;
}

void Process::ChangeProcessState(ProcessState NewState)
{
	CrntState = NewState;
}

void Process::RunProcess()
{
	CPUTime--;
}

bool Process::isRecentlyUpdated(int crntTimeStep) const
{
	if (crntTimeStep == lastUpdatetime)
		return true;
	else
		return false;
}

Process::~Process() {
}

void Process::Setchild(Process* c)
{
	child = c;
}
