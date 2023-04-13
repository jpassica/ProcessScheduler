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
