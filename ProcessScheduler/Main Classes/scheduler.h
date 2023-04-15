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
	Queue<Process*> NEW;
	Queue<Process*> BLK;
	Queue<Process*> TRM;
	Process** RUN;	  
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
	
	//getters (used in UI class)
	int getTimeStep();
	Processor** getProcessors_List();
	const Queue<Process*>& getBLK();
	const Queue<Process*>& getTRM();
	Process** getRUN();
	int getFCFSCount();
	int getSJFCount();
	int getRRCount();
	int getProcessorsCount();

	//the main function for reading from files
	bool ReadInputFile(string filename);

	//process operations
	void migrate();
	void steal();
	void Kill();
	void Fork();
	
	//process moving
	void FromRUNToBLK();
	void FromBLKToRDY();
	void ToTRM();

	//Loading Function
	void LoadData();
	~scheduler();

};

