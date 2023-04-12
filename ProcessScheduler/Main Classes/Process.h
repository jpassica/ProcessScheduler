#pragma once

class Process {

	const int PID;
	int ArrivalTime;
	int ResponseTime;
	int CPUTime;
	int TermenationTime;
	int TurnaroundTime;
	int WaitingTime;
	int** IOPairs = new int* [2];

public:
	Process(int pid, int AT, int CT);
	void InitialIO(int n);
	void SetIO(int i, int IO_D, int IO_R);
	void SetTermenationTime(int n);
	void SetResponseTime(int n);
	int GetPID() const;
	~Process();
};

//Process Constructor
Process::Process(int pid, int AT, int CT) :PID(pid), ArrivalTime(AT), CPUTime(CT) {}

//Initialization for (IO_R,IO_D) 2_d array
void Process::InitialIO(int n) {
	for (int i = 0; i < 2; i++)
	{
		IOPairs[i] = new int[n];
	}
}

//Set the values of IO_R, IO_D for the process
void Process::SetIO(int i, int IO_D, int IO_R) {
	IOPairs[0][i] = IO_D;
	IOPairs[1][i] = IO_R;
}
//Set the response time as FCPU is the time when the process is ready to be processed at first time
void Process::SetResponseTime(int FCPU) {
	ResponseTime = FCPU - ArrivalTime;
}

// Set the termination time as TT is the time when the process go to TRM list after processing 
void Process::SetTermenationTime(int TT) {
	TermenationTime = TT;
	TurnaroundTime = TermenationTime - ArrivalTime;
	WaitingTime = TurnaroundTime - CPUTime;
}

//Getter for Process ID
int Process::GetPID() const {
	return PID;
}

//Destructor for deallocating the dynamic array we used
Process::~Process() {
	delete[] IOPairs[0];
	delete[] IOPairs[1];
	delete[] IOPairs;
}