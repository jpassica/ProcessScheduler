#pragma once

#include "..\Defs.h"
#include "..\DS Classes\Queue.h"

class Scheduler;

class Process 
{
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

	int totalIO_D;
	Queue<IO_Request*> IO_RequestQ;

	int ProcessedTime;

	//Is true if the process has forked once in its lifetime 
	bool HasForked;
	Process* ChildPtr;
	Process* ParentPtr;

	//To mark the response Time
	bool FirstTimeExecution;

public:
	//Non-default constructor
	Process(int ID, int AT, int CT, int DL);

	//Overoaded insertion operator
	friend ostream& operator<<(ostream&, const Process*);

	//Setter functions
	void SetChild(Process*);
	void SetParent(Process*);
	void SetTerminationTime(int);
	void SetResponseTime(int);

	//Takes data and creates IO request and adds it to queue
	void AddIORequest(int, int);

	//Getter functions
	int GetCPUTime() const;
	int GetID() const;
	int GetArrivalTime() const;
	int GetTurnAroundTime() const;
	int GetResponseTime() const;
	int GetWaitingTime() const;
	int GetTerminationTime() const;
	int GetDeadline() const;

	int GetTotalIO_D() const;
	Process* GetChild() const;
	Process* GetParent() const;

	int GetProcessedTime() const;
	int GetRemainingCPUTime() const;

	int GetIO_D();

	//Returns true if the process has ever forked in its lifetime
	bool HasForkedBefore() const;

	bool TimeForIO();
	void DeleteIO_Request();

	//Increments the ProcessedTime of the process currently being executed
	void ExecuteProcess();

	//returns true if this the process had not received the CPU before
	bool isFirstExecution() const;

	bool IsChild() const;
	bool IsParent() const;

	//Cuts off connection between process and its parent
	void SeparateFromParent();

	//Cuts off connection between process and its child
	void SeparateFromChild();

	void UpdateWaitingTime(int);

	~Process();
};
