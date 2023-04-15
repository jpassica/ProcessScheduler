#pragma once
#include<iostream>
using namespace std;
enum ProcessState { NEW, RDY, RUN, BLK, TRM, ORPH };

class Process {

	const int PID;
	int ArrivalTime;
	int ResponseTime;
	int CPUTime;
	int TerminationTime;
	int TurnAroundTime;
	int WaitingTime;
	int** IOPairs = new int* [2];
	ProcessState CrntState;

public:
	Process(int pid, int AT, int CT);
	friend ostream& operator<<(ostream&, const Process&);
	void InitialIO(int n);
	void SetIO(int i, int IO_D, int IO_R);
	void SetIO(int IO_N, int IO_R, int IO_D, int j);
	void SetTerminationTime(int n);
	void SetResponseTime(int n);

	int GetCPUTime() const;
	int GetPID() const;
	int GetTurnAroundTime() const;
	int GetResponseTime() const;
	int GetWaitingTime() const;
	ProcessState GetProcessState();

	void ChangeProcessState(ProcessState NewState);
	void DecrementCPUTime();
	~Process();
};
