#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "FCFS_Processor.h"
#include "SJF_Processor.h"
#include "RR_Processor.h"
#include "EDF_Processor.h"
#include "UI.h"

class Scheduler
{
private:
	//
	int TimeStep;

	//List of all processors in the system
	Processor** ProcessorsList;

	//Processor counters
	int FCFSCount;
	int SJFCount;
	int RRCount;
	int EDFCount;
	int ProcessorsCount;

	//Process lists
	Queue<Process*> NEW_List;
	Queue<Process*> BLK_List;
	Queue<Process*> TRM_List;    
	int ProcessesCount;

	//Operation parameters
	int RTF;  
	int MaxW; 
	int STL;
	int RRtimeSlice;
	int HealingTime;			//Period overheated processors have to wait to become functional again
	
	//Statistics counters
	int TotalWaitingTime;
	int TotalResponseTime;
	int TotalTurnAroundtime;	

	int RTFMigrationCount;				//no. of migrated processes due to RTF
	int MaxWMigrationCount;				//no. of migrated processes due to MaxW
	int StealCount;						//no of stolen processes
	int ForkCount;						//no. of forking instances
	int KillCount;						//no. of kills
	int CompletedBeforeDeadlineCount;	//no. of processes finished before their deadline

	//Index of the processor with the shortest expected finish Time
	int MinIndex;

	//Index of the processor with the longest expected finish Time
	int MaxIndex;

	//Total time spent at top of BLK list, receiving IO resources
	int ProcessedIO_D;

	//Pointer to the User Interface that will work throughout the simulation
	UI* ProgramUI;

	//Used locally when input is loaded from the file
	void AllocateProcessors(int, int, int, int, int, int);  

public:
	Scheduler();

	//Reads input file, sets data, allocates processes & processors
	bool ReadInputFile(string);

	//Generates output file and writes all statistics
	void WriteOutputFile(string);

	//------------- Process operations -------------

	//Steals processes from the longest queue and gives it to the shortest
	void Steal();

	//Migration functions
	bool MigrateFromFCFStoRR(Process*);
	bool MigrateFromRRtoSJF(Process*);

	//Forks a child process
	void Fork(Process*);

	void KillOrphan(Process*);

	void HandleIODuration();
	
	//Process moving functions
	void BlockProcess(Process*);
	void UnBlockProcess();
	void TerminateProcess(Process*);

	//Moves all processes arriving at timestep to shortest ready queues 
	void MoveNEWtoRDY();
	
	//move all processes of the stopped processor to shortest queue
	void MovetoRDY(Process*);

	//The main function that runs the simulation	
	void Simulate();

	//Sets the index of the processor with the smallest finish Time
	bool SetMinIndex(int RangeSelect = 0);

	//Sets the index of the processor with the biggest finish Time
	bool SetMaxIndex(int RangeSelect = 0);

	//Calculates and returns the steal limit
	int CalcStealLimit();

	//Called by FCFS processors when they execute a kill signal
	void IncrementKillCount();

	//---------------- Statistics calculation functions -------------------
	double CalcAvgUtilization() const;
	double CalcAvgTRT() const;
	double CalcAvgWT() const;
	double CalcAvgRT() const;

	double CalcRTFMigrationPercentage() const;
	double CalcMaxWMigrationPercentage() const;
	double CalcStealPercentage() const;
	double CalcForkingPercentage() const;
	double CalcKillPercentage() const;
	double CalcBeforeDeadlinePercentage() const;

	~Scheduler();
};

#endif