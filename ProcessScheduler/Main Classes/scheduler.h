#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../DS Classes/PriorityQueue.h"
#include "../DS Classes/Queue.h"
#include "FCFS_Processor.h";
#include "SJF_Processor.h";
#include "RR_Processor.h";
#include "UI.h"
#define MAXSIZE 10000

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
	int totalWT;
	int totalRT;
	int totalTRT;	
	int avgUtilization;

	int RTFCount;						//no. of migrated processes due to RTF
	int MaxWCount;						//no. of migrated processes due to MaxW
	int StealCount;						//no of stolen processes
	int Forkcount;						//no. of forking instances
	int KillCount;						//no. of kills

	Queue<KillSignal*> KillSignalQ;

	void setProcessors(int, int, int, int);  //used locally when input is loaded from the file

public:
	Scheduler();
	
	//getters 
	int getFCFSCount() const;
	int getSJFCount() const;
	int getRRCount() const;
	int getProcessorsCount() const; 

	//function responsible for reading input file
	bool ReadInputFile(string filename);

	//function resposible for generating output file
	bool WriteOutputFile();

	//process operations
	void migrate(Process*);
	void steal(Process*);
	void Kill(Process*);
	void Fork(Process*);
	
	//process moving
	bool FromRUNToBLK(Processor*);
	bool FromBLKToRDY(Processor*);
	bool ToTRM(Process*);
	bool ToRDY(Process* , Processor*);

	void FromNEWtoRDY(Process*);

	//simulation function
	void Simulate();

	//statistics functions
	int calcAvgUtilization();
	int calcAvgTRT();
	int calcAvgWT();
	int calcAvgRT();
};

#endif