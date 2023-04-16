#pragma once
#include <iostream>
#include "..\Defs.h"
#include "..\DS Classes\Queue.h"
using namespace std;

class Process {

	const int PID;
	int ArrivalTime;
	int ResponseTime;
	int CPUTime;
	int TerminationTime;
	int TurnAroundTime;
	int WaitingTime;
	int IO_N;
	//int** IOPairs = new int* [2];
	Queue<IO_Pairs*> IO_PairsQ;
	ProcessState CrntState;

public:
	//Non-default ctor
	Process(int pid, int AT, int CT, int IO_N);

	//Overoaded insertion operator
	friend ostream& operator<<(ostream&, const Process&);

	//Setter functions
	void SetTerminationTime(int n);
	void SetResponseTime(int n);

	//Takes data and creates IO request and adds it to queue
	void AddIORequest(int IO_R, int IO_D);

	//Getter functions
	int GetCPUTime() const;
	int GetPID() const;
	int GetTurnAroundTime() const;
	int GetResponseTime() const;
	int GetWaitingTime() const;
	ProcessState GetProcessState() const;

	void ChangeProcessState(ProcessState NewState);

	//Decrements the CT of the process as it is executing
	void RunProcess();

	~Process();
};
