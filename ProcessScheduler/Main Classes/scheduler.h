#pragma once
#include "../DS Classes/PriorityQueue.h"
#include "../DS Classes/Queue.h"
#include "Process.h"
#include "Processor.h"
#include "UI.h"
#include "FCFS_Processor.h"
#include "RR_Processor.h"
#include "SJF_Processor.h"
#define MAXSIZE 10000
using namespace std;

class scheduler
{
private:
	int Timestep;

	//processors lists(dynamic allocation)
	Processor** Processors_List;

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
	int RTFCount;  //no. of processors migrated by RTF
	int MaxWCount;  //no. of processors migrated by MaxW
	int STLCount;  //no of stolen processes
	int Forkcount;  //no. of forking 
	int KillCount;  //no. of kills

	//int SIGKILL[MAXSIZE][2]; //pairs for kill-time & PID respectively
	Queue<KillSignal*> KillSignalQ;

	void setProcessors(int, int, int , int);  //used locally when input is loaded from the file

public:
	scheduler();
	
	//getters 
	int getTimeStep() const ;
	Processor** getProcessors_List() const;
	Queue<Process*>& getBLK()  ;
	Queue<Process*>& getTRM()  ;
	int getFCFSCount() const;
	int getSJFCount() const;
	int getRRCount() const;
	int getProcessorsCount() const; 
	bool isRecentlyUpdated(const Process*) const; //checks if the process is previously updated in the current timestep
	//the main function for reading from files
	bool ReadInputFile(string filename);

	//process operations
	void migrate(Process*);
	void steal(Process*);
	void Kill(Process*);
	void Fork(Process*);
	
	//process moving
	bool FromRUNToBLK(Processor*);
	bool FromBLKToRDY(Processor*);
	bool ToTRM(Process*);
	bool ToRDY(Process* , Processor*);  //To be implemented in phase 2
	bool ToRUN(Processor*); //returns false if the processor is busy
	//simulation function
	void Simulate();
	~scheduler();

};

