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

	//Child Processes can only be Processed by FCFS Processors
	int ShortestFCFSIndex;
	int LongestFCFSIndex;

	//Used in migration
	int ShortestRRIndex;

	//Largest and smallest finish time between all the processors
	int SQF;
	int LQF;


	int ProcessedIO_D;

	//Pointer to the User Interface that will work throughout the simulation
	UI* ProgramUI;

	void setProcessors(int, int, int, int, int);  //used locally when input is loaded from the file

	//returns the processor that the passed Process is currently in
	FCFS_Processor* GetFCFS_ProcessorPtrTo(Process*);
public:
	Scheduler();
	
	//getters 
	int GetTimeStep();
	//function responsible for reading input file
	bool ReadInputFile(string filename);

	//function responsible for generating output file
	bool WriteOutputFile();

	//process operations
	bool MigrateToRR(Processor*);
	void Steal();
	
	bool Kill();
	bool Fork(Process*);
	bool KillOrphan(Process*);

	//process moving
	bool BlockProcess(Processor*);
	bool ReturnBLKtoRDY();
	bool TerminateProcess(Process*);

	void HandleIODuration();
	void HandleIORequest(Processor*);
	

	//Moves all processes arriving at current timestep to shortest ready queues 
	void FromNEWtoRDY();

	//Moves Child processes to Ready 
	void MoveChildToReady(Process*);

	//The main function that for running the simulation
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

	//Updates the index of the shortest & Longest FCFS Processor
	void UpdateShortestFCFSIndex();
	void UpdateLongestFCFSIndex();

	//
	void UpdateShortestRRIndex();

	//Calculates and returns the steal limit
	int CalcStealLimit();
};

#endif