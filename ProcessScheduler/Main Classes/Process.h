#pragma once
#include <iostream>
#include "..\Defs.h"
#include "..\DS Classes\Queue.h"
using namespace std;

class Process {
private:

	//Main parameters
	const int PID;
	int ArrivalTime;
	int ResponseTime;
	int CPUTime;
	int TerminationTime;
	int TurnAroundTime;
	int WaitingTime;

	int Deadline;

	int IO_N;
	int totalIO_D;
	Queue<IO_Pairs*> IO_PairsQ;

	ProcessState CrntState;

	int ProcessedTime;
	Process* child;

	//to put down the response time
	bool firstTimeExecution;

public:
	//Non-default ctor
	Process(int ID, int AT, int CT, int DL, int IO_N);

	//Overoaded insertion operator
	friend ostream& operator<<(ostream&, const Process*);


	//Setter functions
	void Setchild(Process*);
	void SetTerminationTime(int n);
	void SetResponseTime(int n);

	//Takes data and creates IO request and adds it to queue
	void AddIORequest(int IO_R, int IO_D);

	//Getter functions
	int GetCPUTime() const;
	int GetPID() const;
	int GetArrivalTime() const;
	int GetTurnAroundTime() const;
	int GetResponseTime() const;
	int GetWaitingTime() const;
	int GetTerminationTime() const;
	int GetDeadline() const;

	int GetTotalIO_D() const;
	ProcessState GetProcessState() const;
	Process* GetChild() const;

	int GetProcessedTime() const;
	int GetRemainingCPUTime() const;
	void ChangeProcessState(ProcessState NewState);


	//Increments the ProcessedTime of the process currently being executed
	void ExecuteProcess();

	//returns true if this the process had not received the CPU before
	bool isFirstExecution() const;

	~Process();
};
