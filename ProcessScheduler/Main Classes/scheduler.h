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
	int avgUtilization;

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

	int RemainingIO_D;

	//Pointer to the User Interface that will work throughout the simulation
	UI* ProgramUI;

	void setProcessors(int, int, int, int, int);  //used locally when input is loaded from the file

public:
	Scheduler();
	
	//getters 

	//function responsible for reading input file
	bool ReadInputFile(string filename);

	//function resposible for generating output file
	bool WriteOutputFile();

	//process operations
	void Migrate(Process*);
	void Steal();
	//void Kill(Process*);
	bool Kill(Processor*, KillSignal*);
	void Fork(Process*);
	
	//process moving
	bool FromRUNToBLK(Processor*);
	bool FromBLKToRDY();
	bool TerminateProcess(Process*);

	void HandleIODuration();
	void HandleIORequest(Processor*);
	

	//Moves all processes arriving at current timestep to shortest ready queues 
	void FromNEWtoRDY();

	//simulation function
	void Simulate();

	//statistics functions
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


	//Sets the index of the processor with the smallest finish time
	void SetMinIndex();

	//Sets the index of the processor with the biggest finish time
	void SetMaxIndex();

	//Calculates and returns the steal limit
	int CalcStealLimit();
};

#endif