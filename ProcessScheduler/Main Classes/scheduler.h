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
	int TimeStep;

	//processors lists(dynamic allocation)
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

	//input values
	int RTF;  
	int MaxW; 
	int STL;
	int ForkProb;
	int RRtimeSlice;
	
	//statistics variables
	int TotalWaitingTime;
	int TotalResponseTime;
	int TotalTurnAroundtime;	

	int RTFCount;						//no. of migrated processes due to RTF
	int MaxWCount;						//no. of migrated processes due to MaxW
	int StealCount;						//no of stolen processes
	int ForkCount;						//no. of forking instances
	int KillCount;						//no. of kills
	int CompletedBeforeDeadlineCount;

	Queue<KillSignal*> KillSignalQ;

	//Indices of the processors with the smallest & biggest finish time
	int MinIndex;
	int MaxIndex;

	//Largest and smallest finish time between all the processors
	int SQF;
	int LQF;

	int ProcessedIO_D;

	//Pointer to the User Interface that will work throughout the simulation
	UI* ProgramUI;

	void setProcessors(int, int, int, int, int);  //used locally when input is loaded from the file

public:
	Scheduler();

	//Reads input file, sets data, allocates processes & processors
	bool ReadInputFile(string);

	//Generates output file and writes all statistics
	void WriteOutputFile();

	//------------- Process operations -------------

	//Steals processes from the longest queue and gives it to the shortest
	void Steal();

	//Kills processes if they receive a kill signal at current time step & are scheduled by FCFS
	void Kill();

	//Forks a child process
	void Fork(Process*);

	
	void HandleIODuration();
	bool MigrateToSJF(Process*);
	
	//Process moving functions
	void BlockProcess(Process*);
	void UnBlockProcess();
	void TerminateProcess(Process*);

	//Moves all processes arriving at timestep to shortest ready queues 
	void MoveNEWtoRDY();

	//The main function that runs the simulation
	void Simulate();

	//Sets the index of the processor with the smallest finish time
	void SetMinIndex(int x = 0);

	//Sets the index of the processor with the biggest finish time
	void SetMaxIndex();

	//Calculates and returns the steal limit
	int CalcStealLimit();

	//Statistics calculation functions
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
};

#endif