#pragma once
#include <iostream>
#include "..\Defs.h"
#include "..\DS Classes\Queue.h"
using namespace std;

class Process {
private:
	const int PID;
	int ArrivalTime;
	int ResponseTime;
	int CPUTime;
	int TerminationTime;
	int TurnAroundTime;
	int WaitingTime;
	int IO_N;
	int totalIO_D;
	Queue<IO_Pairs*> IO_PairsQ;
	ProcessState CrntState;
	int processedTime;
	Process* child;
	int lastUpdatetime; //To check if this process was updated at the current timestep to update it only once

public:
	//Non-default ctor
	Process(int pid, int AT, int CT, int IO_N);

	//Overoaded insertion operator
	friend ostream& operator<<(ostream&, const Process*);


	//Setter functions
	void Setchild(Process*);
	void SetTerminationTime(int n);
	void SetResponseTime(int n);
	void SetProcessedTime(int);
	void SetLastUpdateTime(int);

	//Takes data and creates IO request and adds it to queue
	void AddIORequest(int IO_R, int IO_D);

	//Getter functions
	int getCPUTime() const;
	int getPID() const;
	int getAT() const;
	int getTRT() const;
	int getRT() const;
	int getWT() const;
	int getTT() const;
	int getTotalIO_D() const;
	ProcessState GetProcessState() const;
	Process* GetChild() const;
	int GetProcessedTime() const;
	int GetLastUpdateTime()const;
	void ChangeProcessState(ProcessState NewState);


	//Decrements the CT of the process as it is executing
	void RunProcess();

	//checks if the process was previously updated in the current timestep
	bool isRecentlyUpdated(int crntTimeStep) const;

	~Process();
};
