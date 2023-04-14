#pragma once

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
	Process();						//adding this to fix an error
	void operator= (const Process & CopyFrom);
	void InitialIO(int n);
	void SetIO(int i, int IO_D, int IO_R);
	void SetTerminationTime(int n);
	void SetResponseTime(int n);
	int GetCPUTime() const;
	int GetPID() const;
	void ChangeProcessState(ProcessState NewState);
	void DecrementCPUTime();
	~Process();
};
