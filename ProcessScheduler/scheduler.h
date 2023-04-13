#pragma once
#include"PriorityQueue.h"
#include"Queue.h"
#include"Process.h"
#include"Processor.h"
#include"UI.h"
#define MAXSIZE 10000
using namespace std;

class scheduler
{
private:
	int Timestep;

	//processors lists(dynamic allocation)
	Processor* FCFS_List;
	Processor* SJF_List;
	Processor* RR_List;

	//Processors counter
	int FCFSCount;
	int SJFCount;
	int RRCount;
	int ProcessorsCount;


	//Processes states
	Queue<Process*> NEW;
	Queue<Process*> BLK;
	Queue<Process*> TRM;
	Process* RUN;	  //only one process can run in the same time

	//input values
	int RTF;  
	int MaxW; 
	int STL;

	//statistics variables
	int RTFCount;  //no. of processors migrated by RTF
	int MaxWCount;  //no. of processors migrated by MaxW
	int STLCount;  //no of stolen processes
	int Forkcount;  //no. of forking 
	int KillCount;  //no. of kills

	int SIGKILL[MAXSIZE][2]; //pairs for kill-time & PID respectively

	void setProcessors(int, int, int);  //used locally when input is loaded from the file
public:
	scheduler();
	
	//getters (used in UI class)
	int getTimeStep();
	Processor* getFCFS_List();
	Processor* getSJF_List();
	Processor* getRR_List();
	Queue<Process*> getBLK();
	Queue<Process*> getTRM();
	Process* getRUN();
	int getFCFSCount();
	int getSJFCount();
	int getRRCount();
	int getProcessorsCount();

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

