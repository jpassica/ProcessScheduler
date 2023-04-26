#include "scheduler.h"
#include <fstream>

Scheduler::Scheduler()
{
	timeStep = 0;

	Processors_List = nullptr;

	FCFSCount = 0;
	SJFCount = 0;
	RRCount = 0;
	RRtimeSlice = 0;
	ProcessorsCount = 0;
	ProcessesCount = 0;

	RTFCount = 0;
	MaxWCount = 0;
	StealCount = 0;
	Forkcount = 0;
	KillCount = 0;

	RTF = 0;
	STL = 0;
	ForkProb = 0;

	avgResponseTime = 0;
	avgTurnAroundTime = 0;
	avgWaitingTime = 0;
}

void Scheduler::setProcessors(int NF, int NS, int NR, int RRtimeSlice)
{
	Processors_List = new Processor * [ProcessorsCount];
	for (int i = 0; i < ProcessorsCount; i++)
	{
		if (i < NF)
			Processors_List[i] = new FCFS_Processor(i + 1, this);

		else if (i >= NF && i < NF + NR)
			Processors_List[i] = new SJF_Processor(i + 1, this);

		else
			Processors_List[i] = new RR_Processor(i + 1, RRtimeSlice, this);
	}
}

int Scheduler::getFCFSCount() const
{
	return 	FCFSCount;
}

int Scheduler::getSJFCount() const
{
	return SJFCount;
}

int Scheduler::getRRCount() const
{
	return RRCount;
}

int Scheduler::getProcessorsCount() const
{
	return ProcessorsCount;
}

bool Scheduler::ReadInputFile(string filename)
{
	//creating input stream object and opening file
	fstream IP_Stream;
	filename += ".txt";
	IP_Stream.open(filename);

	if (!IP_Stream.is_open() || !IP_Stream.good())			//if file open is not successful, abort
		return false;										//if file is not good or corrupted, abort

	//now we can safely read from file

	//reading the main parameters
	IP_Stream >> FCFSCount >> SJFCount >> RRCount;
	IP_Stream >> RRtimeSlice;
	IP_Stream >> RTF >> MaxW >> STL >> ForkProb;
	IP_Stream >> ProcessesCount;

	//setting ProcessorsCount
	ProcessorsCount = FCFSCount + SJFCount + RRCount;

	//creating the processors according to the read data
	setProcessors(FCFSCount, SJFCount, RRCount, RRtimeSlice);

	//variables to be read for each process and sent to ctor
	int AT(0), PID(0), CT(0), IO_N(0), IO_R(0), IO_D(0);

	//pointer to be used for creating and allocating all the processes
	Process* newProcess(nullptr);

	//used for reading the string of IO pairs
	string IO_st, IO_R_st, IO_D_st;

	//reading data of all processes
	for (size_t i(0); i < ProcessesCount; i++)
	{
		IP_Stream >> AT >> PID >> CT >> IO_N;

		//read string only if there are IO requests
		if (IO_N)
			IP_Stream >> IO_st;

		newProcess = new Process(PID, AT, CT, IO_N);

		//iterator for reading the IO requests string
		int StIndex(1);

		for (size_t j = 0; j < IO_N; j++)
		{
			//start from 1st digit

			while (IO_st[StIndex] != ',')
			{
				IO_R_st += IO_st[StIndex]; StIndex++;
			}

			StIndex++;				//skip comma between numbers

			while (IO_st[StIndex] != ')')
			{
				IO_D_st += IO_st[StIndex]; StIndex++;
			}

			StIndex += 3;			//skip closing bracket, intermediary comma, and opening bracket  "),("

			IO_R = stoi(IO_R_st);
			IO_D = stoi(IO_D_st);

			IO_R_st.clear();
			IO_D_st.clear();

			newProcess->AddIORequest(IO_R, IO_D);
		}

		//enqueue process in New List once all its data has been read
		this->NEW_List.Enqueue(newProcess);
	}

	//reading SIGKILLs

	//reading each kill signal's data until there is no more data
	int time(0), ID(0);
	while (!IP_Stream.eof())
	{
		IP_Stream >> time >> PID;

		KillSignal* newKillSignal = new KillSignal(time, PID);

		KillSignalQ.Enqueue(newKillSignal);
	}

	//after successfully reading all data
	return true;
}

bool Scheduler::WriteOutputFile()
{
	fstream OP_Stream;
	string fileName = "OutputFile";
	OP_Stream.open(fileName);

	if (!OP_Stream.is_open() || !OP_Stream.good())
		return false;

}


bool Scheduler::FromRUNToBLK(Processor* pro)
{
	//checking if there is a processor in the RUN state or not
	if (pro->getProcessorState() == IDLE)
		return false;

	//checking if this process is updated in the current timestep
	if (pro->getRunPtr()->isRecentlyUpdated(timeStep))
		return false;

	//moving & updating states
	BLK_List.Enqueue(pro->getRunPtr());				//adding to BLK
	pro->getRunPtr()->ChangeProcessState(BLK);		//changing Process state to BLK
	pro->getRunPtr()->SetLastUpdateTime(timeStep);	//time Update
	pro->setRunptr(nullptr);						//removing the process from Runptr
	pro->FlipProcessorState();						//changing processor state
	return true;
}

bool Scheduler::FromBLKToRDY(Processor* p)
{
	//checking if BLK_List is Empty
	if (BLK_List.isEmpty())
		return false;

	//checking process & processor availabilty
	Process* BLKtoRDY = nullptr;
	BLKtoRDY = BLK_List.Queue_front();
	if (!p || !BLKtoRDY)
		return false;

	//checking if this process is updated in the current timestep
	if (BLKtoRDY->isRecentlyUpdated(timeStep))
		return false;

	BLK_List.Dequeue(BLKtoRDY);

	p->AddToReadyQueue(BLKtoRDY);

	//updating process state
	BLKtoRDY->ChangeProcessState(RDY);
	return true;
}

bool Scheduler::ToTRM(Process* ptr)
{
	//checking if process not equal nullptr
	if (!ptr)
		return false;

	//checking if this process is updated in the current timestep
	if (ptr->isRecentlyUpdated(timeStep))
		return false;

	//checking Forking 
	if (ptr->GetChild())
		ToTRM(ptr->GetChild());

	//moving to TRM & changing states
	ptr->SetLastUpdateTime(timeStep);
	TRM_List.Enqueue(ptr);
	ptr->ChangeProcessState(TRM);
	return true;
}

bool Scheduler::ToRDY(Process* f, Processor* pro)
{
	//checking process & processor
	if (!pro || !f)
		return false;

	//checking if this process is updated in the current timestep
	if (f->isRecentlyUpdated(timeStep))
		return false;

	if (pro)
		pro->AddToReadyQueue(f);
	else
		return false;

	//updating states
	if (f->GetProcessState() == RUN)
	{
		pro->setRunptr(nullptr);
		pro->FlipProcessorState();
	}
	f->ChangeProcessState(RDY);
	f->SetLastUpdateTime(timeStep);
	return true;
}

void Scheduler::Simulate(string fileName)
{
	//initializations
	UI out(this);
	Process* pro = nullptr;

	//calling Load function
	bool fileOpened = ReadInputFile(fileName);

	if (!fileOpened) return;

	int count = 0;							//acts as an index to detect which processor will be passed processes from the NEW_List
	while (TRM_List.getCount() != ProcessesCount) //program ends when all processes are in TRM list
	{
		//Moving Arrived processes from NEW to RDY
		pro = nullptr;
		if (!NEW_List.isEmpty()) //checking New_List is not empty
			pro = NEW_List.Queue_front();

		while (pro && pro->GetAT() == timeStep)
		{

			NEW_List.Dequeue(pro);					 //removing from NEW
			if (ToRDY(pro, Processors_List[count]));  //adding to RUN
				count++;

			if (count == ProcessorsCount)
				count = 0;

			if (!NEW_List.isEmpty())
				pro = NEW_List.Queue_front();
			else
				pro = nullptr;
		}


		//Moving to RUN
		for (int i = 0; i < ProcessorsCount; i++)
		{
			Processors_List[i]->fromReadyToRun(timeStep);
		}

		//Moving From RUN
		for (int i = 0; i < ProcessorsCount; i++)
		{
			if (Processors_List[i]->getProcessorState() == BUSY)
			{

				int random = rand() % 100;
				if (random >= 1 && random <= 15)   //probability to terminate process
				{
					FromRUNToBLK(Processors_List[i]);
				}
				else if (random >= 20 && random <= 30)  //probality to move to RDY
				{
					ToRDY(Processors_List[i]->getRunPtr(), Processors_List[i]);
				}
				else if (random >= 50 && random <= 60) //probability to terminate
				{
					if (ToTRM(Processors_List[i]->getRunPtr()))
					{
						Processors_List[i]->FlipProcessorState();
						Processors_List[i]->setRunptr(nullptr);
					}
				}
			}
		}

		//moving from BLK 
		int random = rand() % 100;
		if (random < 10 && !BLK_List.isEmpty())
		{
			pro = BLK_List.Queue_front();
			if (count >= ProcessorsCount)
				count = 0;
			if (ToRDY(pro, Processors_List[count]))
				BLK_List.Dequeue(pro);
		}

		//kill test
		random = rand() % ProcessesCount;				//randoming process ID
		bool killed = false;							    //Detects if the process is found or not

		for (int i = 0; i < FCFSCount && !killed; i++)
		{
			FCFS_Processor* processorPtr = (FCFS_Processor*) Processors_List[i];		//only FCFS processors
			if (processorPtr)
			{
				killed = processorPtr->RandomKill(random);
			}
		}

		//incrementing & printing timestep
		out.TimeStepOut(BLK_List, TRM_List, Processors_List, FCFSCount, SJFCount, RRCount, timeStep);
		timeStep++;
	}
}
