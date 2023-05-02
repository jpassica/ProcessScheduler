#include "scheduler.h"
#include <fstream>
#include <iomanip>

Scheduler::Scheduler()
{
	TimeStep = 0;

	ProcessorsList = nullptr;

	FCFSCount = 0;
	SJFCount = 0;
	RRCount = 0;
	EDFCount = 0;
	RRtimeSlice = 0;
	ProcessorsCount = 0;
	ProcessesCount = 0;

	RTFCount = 0;
	MaxWCount = 0;
	StealCount = 0;
	ForkCount = 0;
	KillCount = 0;
	CompletedBeforeDeadlineCount = 0;

	RTF = 0;
	STL = 0;
	ForkProb = 0;
	MaxW = 0;

	TotalResponseTime = 0;
	TotalTurnAroundtime = 0;
	TotalWaitingTime = 0;
	avgUtilization = 0;

	MaxIndex = 0;
	MinIndex = 0;

	LQF = 0;
	SQF = 0;
}

void Scheduler::setProcessors(int NF, int NS, int NR, int NE, int RRtimeSlice)
{
	ProcessorsList = new Processor * [ProcessorsCount];
	for (int i = 0; i < ProcessorsCount; i++)
	{
		if (i < NF)
			ProcessorsList[i] = new FCFS_Processor(i + 1, this);

		else if (i >= NF && i < NF + NS)
			ProcessorsList[i] = new SJF_Processor(i + 1, this);

		else if (i >= NF + NS && i < NF + NS + NR)
			ProcessorsList[i] = new RR_Processor(i + 1, RRtimeSlice, this);

		else
			ProcessorsList[i] = new EDF_Processor(i + 1, this);
	}
}

bool Scheduler::ReadInputFile(string FileName)
{
	//creating input stream object and opening file
	ifstream IP_Stream;
	FileName += ".txt";
	IP_Stream.open(FileName);

	while (!IP_Stream.is_open())							//keep reading file names until we successfully open file
	{
		FileName = ProgramUI->InputFileName();
		FileName += ".txt";
		IP_Stream.open(FileName);
	}

	//if (!IP_Stream.is_open())								//if file opening is unsuccessful, abort
	//	return false;
		
	if(!IP_Stream.good())									//if file is not good or corrupted, abort
		return false;										

	//now we can safely read from file

	//reading the main parameters
	IP_Stream >> FCFSCount >> SJFCount >> RRCount >> EDFCount;
	IP_Stream >> RRtimeSlice;
	IP_Stream >> RTF >> MaxW >> STL >> ForkProb;
	IP_Stream >> ProcessesCount;

	//setting ProcessorsCount
	ProcessorsCount = FCFSCount + SJFCount + RRCount + EDFCount;

	//creating the processors according to the read data
	setProcessors(FCFSCount, SJFCount, RRCount, EDFCount, RRtimeSlice);

	//variables to be read for each process and sent to ctor
	int AT(0), PID(0), CT(0), DL(0), IO_N(0), IO_R(0), IO_D(0);

	//pointer to be used for creating and allocating all the processes
	Process* newProcess(nullptr);

	//used for reading the string of IO pairs
	string IO_st, IO_R_st, IO_D_st;

	//reading data of all processes
	for (size_t i(0); i < ProcessesCount; i++)
	{
		IP_Stream >> AT >> PID >> CT >> DL >> IO_N;

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
		NEW_List.Enqueue(newProcess);
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

	IP_Stream.close();

	//after successfully reading all data
	return true;
}

bool Scheduler::WriteOutputFile()
{
	//creating output stream object and opening file for writing
	ofstream OP_Stream;
	string fileName = "OutputFile.txt";
	OP_Stream.open(fileName);

	//if there is any problem with the file, abort
	if (!OP_Stream || !OP_Stream.good())
		return false;

	//start writing the first line, setting a specific width for each field
	OP_Stream << left << setprecision(1) << fixed
		<< setw(5) << "TT" << setw(5) << "PID" 
		<< setw(5) << "AT" << setw(5) << "CT" 
		<< setw(8) << "IO_D" << setw(5) << "WT" 
		<< setw(5) << "RT" << setw(5) << "TRT" << endl;

	//ProcessToTerminate used to dequeue from TRM, write all needed info, and then deallocate process
	Process* deletePtr(nullptr);

	//
	for (size_t i = 0; i < ProcessesCount && !TRM_List.isEmpty(); i++)
	{
		TRM_List.Dequeue(deletePtr);

		OP_Stream << setw(5) << deletePtr->GetTerminationTime()
			<< setw(5) << deletePtr->GetPID()
			<< setw(5) << deletePtr->GetArrivalTime()
			<< setw(6) << deletePtr->GetCPUTime()
			<< setw(7) << deletePtr->GetTotalIO_D()
			<< setw(5) << deletePtr->GetWaitingTime()
			<< setw(5) << deletePtr->GetResponseTime()
			<< setw(5) << deletePtr->GetTurnAroundTime()
			<< endl;
		
		TotalWaitingTime += deletePtr->GetWaitingTime();
		TotalResponseTime += deletePtr->GetResponseTime();
		TotalTurnAroundtime += deletePtr->GetTurnAroundTime();

		delete deletePtr;
	}

	OP_Stream << "\nProcesses: " << ProcessesCount << endl
		<< "Avg WT = " << CalcAvgWT() << ",     "
		<< "Avg RT = " << CalcAvgRT() << ",     "
		<< "Avg TRT = " << CalcAvgTRT() << endl;

	if (!ProcessesCount) return false;



	OP_Stream << "Migration%:     RTF= " << CalcRTFMigrationPercentage()
		<< "%,     MaxW= " << CalcMaxWMigrationPercentage() << "%\n"

	<< "Work Steal%: " << CalcStealPercentage() << "%\n"

	<< "Forked Process: " << CalcForkingPercentage() << "%\n"

	<< "Killed Process: " << CalcKillPercentage() << "%\n"
		
	<< "Processes Completed Before Deadline: " << CalcBeforeDeadlinePercentage() << "%\n\n";



	OP_Stream << "Processors: " << ProcessorsCount << " ["
		<< FCFSCount << " FCFS, " << SJFCount << " SJF, " 
		<< RRCount << " RR, " << EDFCount << " EDF]\n";

	OP_Stream << "Processors Load\n";

	for (size_t i = 0; i < ProcessorsCount; i++)
	{
		OP_Stream << "p" << i + 1 << "= " << 
			ProcessorsList[i]->CalcPLoad(TotalTurnAroundtime) << "%";

		if (i != ProcessorsCount)
			OP_Stream << ",     ";
	}

	OP_Stream << "\n\nProcessors Utiliz\n";

	for (size_t i = 0; i < ProcessorsCount; i++)
	{
		OP_Stream << "p" << i + 1 << "= " <<
			ProcessorsList[i]->CalcPUtil() << "%";

		if (i != ProcessorsCount)
			OP_Stream << ",     ";
	}

	OP_Stream << endl << "Avg utilization = " << CalcAvgUtilization() << "%";

	OP_Stream.close();

	return true;
}

void Scheduler::Steal()
{
	//Setting the indices of the processors with the longest and shortest finish times
	SetMinIndex();
	SetMaxIndex();

	//Calculating the steal limit
	int StealLimit = CalcStealLimit();

	while (StealLimit > 40)
	{
		Process* StolenProcess = ProcessorsList[MaxIndex]->StealProcess();

		ProcessorsList[MinIndex]->AddToReadyQueue(StolenProcess);

		StealCount++;

		//Recalculating the steal limit
		StealLimit = CalcStealLimit();
	}
}

bool Scheduler::FromRUNToBLK(Processor* ProcessorPtr)
{
	//checking if there is a processor in the RUN state or not
	if (ProcessorPtr->GetProcessorState() == IDLE)
		return false;

	//moving & updating states
	BLK_List.Enqueue(ProcessorPtr->GetRunPtr());				//adding to BLK
	ProcessorPtr->GetRunPtr()->ChangeProcessState(BLK);		//changing Process state to BLK
	ProcessorPtr->SetRunptr(nullptr);						//removing the process from Runptr
	ProcessorPtr->ChangeProcessorState(IDLE);						//changing processor state
	return true;
}

bool Scheduler::FromBLKToRDY()
{
	//checking if BLK_List is Empty
	if (BLK_List.isEmpty())
		return false;

	//checking process & processor availabilty
	Process* BLKtoRDY = nullptr;
	BLKtoRDY = BLK_List.QueueFront();
	if (!BLKtoRDY)
		return false;

	SetMinIndex();

	BLK_List.Dequeue(BLKtoRDY);

	ProcessorsList[MinIndex]->AddToReadyQueue(BLKtoRDY);

	//updating process state
	BLKtoRDY->ChangeProcessState(RDY);
	return true;
}

bool Scheduler::TerminateProcess(Process* ProcessToTerminate)
{
	//checking if process not equal nullptr
	if (!ProcessToTerminate)
		return false;

	//checking Forking 
	if (ProcessToTerminate->GetChild())
		TerminateProcess(ProcessToTerminate->GetChild());

	//moving to TRM & changing states
	ProcessToTerminate->SetTerminationTime(TimeStep);

	//If the process was completed before its expected deadline
	if (TimeStep < ProcessToTerminate->GetDeadline())
		CompletedBeforeDeadlineCount++;

	TRM_List.Enqueue(ProcessToTerminate);
	ProcessToTerminate->ChangeProcessState(TRM);
	return true;
}

void Scheduler::FromNEWtoRDY()
{
	Process* NewProcessPtr = nullptr;

	if (!NEW_List.isEmpty())
		NewProcessPtr = NEW_List.QueueFront();
	else
		return;

	while (NewProcessPtr && NewProcessPtr->GetArrivalTime() == TimeStep)
	{
		NEW_List.Dequeue(NewProcessPtr);

		//Setting the index of the processor with the smallest expected finish time
		SetMinIndex();

		//Adding the process to the RDY queue of the processor with the smallest finish time
		ProcessorsList[MinIndex]->AddToReadyQueue(NewProcessPtr);

		NewProcessPtr->ChangeProcessState(RDY);

		if (!NEW_List.isEmpty())
			NewProcessPtr = NEW_List.QueueFront();
		else
			NewProcessPtr = nullptr;
	}
}

void Scheduler::Simulate()
{
	//initializations
	ProgramUI = new UI();
	UI_Mode CrntMode;
	//Process* ProcessPtr = nullptr;

	string FileName = ProgramUI->InputFileName();
	CrntMode = ProgramUI->InputInterfaceMode();

	if (CrntMode == Silent)
		ProgramUI->PrintSilentMode(0);

	//calling Load function
	bool FileOpened = ReadInputFile(FileName);

	if (!FileOpened) return;

	int count = 0;							//acts as an index to detect which processor will be passed processes from the NEW_List
	while (TRM_List.getCount() != ProcessesCount) //program ends when all processes are in TRM list
	{
		//ProcessPtr = nullptr;

		//Moving all processes arriving at current timestep to shortest ready queues
		FromNEWtoRDY();

		//Calling ScheduleAlgo of each processor
		for (int i = 0; i < ProcessorsCount; i++)
		{
			//This should be removed when all ScheduleAlgo fns are ready
			ProcessorsList[i]->fromReadyToRun(TimeStep);
			//ProcessorsList[i]->ScheduleAlgo(TimeStep);
		}

		//Moving From RUN
		for (int i = 0; i < ProcessorsCount; i++)
		{
			if (ProcessorsList[i]->GetProcessorState() == BUSY)
			{

				int random = rand() % 100;
				if (random >= 1 && random <= 15)   //probability to terminate process
				{
					FromRUNToBLK(ProcessorsList[i]);
				}
				else if (random >= 20 && random <= 30)  //probability to move to RDY
				{
					//ToRDY(ProcessorsList[i]->GetRunPtr(), ProcessorsList[i]);
			
					//this should be removed and schedulealgos should substitute
					SetMinIndex();
					ProcessorsList[MinIndex]->AddToReadyQueue(ProcessorsList[i]->GetRunPtr());
					ProcessorsList[i]->GetRunPtr()->ChangeProcessState(RDY);
					ProcessorsList[i]->SetRunptr(nullptr);
					ProcessorsList[i]->ChangeProcessorState(IDLE);

				}
				else if (random >= 50 && random <= 60) //probability to terminate
				{
					if (TerminateProcess(ProcessorsList[i]->GetRunPtr()))
					{
						ProcessorsList[i]->ChangeProcessorState(IDLE);
						ProcessorsList[i]->SetRunptr(nullptr);
					}
				}
			}
		}

		//moving from BLK 
		int random = rand() % 100;
		if (random < 50 && !BLK_List.isEmpty())
		{
			FromBLKToRDY();
		}



		//Initiating the steal action each STL timesteps
		if (TimeStep % STL == 0)
			Steal();



		//kill test
		//random = rand() % ProcessesCount;				//randoming process ID
		//bool killed = false;							    //Detects if the process is found or not

		//for (int i = 0; i < FCFSCount && !killed; i++)
		//{
		//	FCFS_Processor* processorPtr = (FCFS_Processor*) ProcessorsList[i];		//only FCFS processors
		//	if (processorPtr)
		//	{
		//		killed = processorPtr->RandomKill(random);
		//	}
		//}

		//incrementing & printing timestep
		if (CrntMode != Silent)
			ProgramUI->TimeStepOut(BLK_List, TRM_List, ProcessorsList, FCFSCount, SJFCount, RRCount, EDFCount, TimeStep);

		TimeStep++;

		for (int i = 0; i < ProcessorsCount; i++)
		{
			ProcessorsList[i]->IncrementBusyOrIdleTime();
			ProcessorsList[i]->IncrementRunningProcess();
		}
	}

	WriteOutputFile();

	if (CrntMode == Silent)
		ProgramUI->PrintSilentMode(1);
}

double Scheduler::CalcAvgUtilization() const
{
	double Sum(0);

	for (size_t i = 0; i < ProcessorsCount; i++)
	{
		Sum += ProcessorsList[i]->CalcPUtil();
	}

	return Sum / ProcessorsCount;
}

double Scheduler::CalcAvgTRT() const
{
	return (double)TotalTurnAroundtime / ProcessesCount;
}

double Scheduler::CalcAvgWT() const
{
	return (double)TotalWaitingTime / ProcessesCount;
}

double Scheduler::CalcAvgRT() const
{
	return (double)TotalResponseTime / ProcessesCount;
}

double Scheduler::CalcRTFMigrationPercentage() const
{
	return (double)100 * RTFCount / ProcessesCount;
}

double Scheduler::CalcMaxWMigrationPercentage() const
{
	return (double)100 * MaxWCount / ProcessesCount;
}

double Scheduler::CalcStealPercentage() const
{
	return (double)100 * StealCount / ProcessesCount;
}

double Scheduler::CalcForkingPercentage() const
{
	return (double)100 * ForkCount / ProcessesCount;
}

double Scheduler::CalcKillPercentage() const
{
	return (double)100 * KillCount / ProcessesCount;
}

double Scheduler::CalcBeforeDeadlinePercentage() const
{
	return (double)100 * CompletedBeforeDeadlineCount / ProcessesCount;
}

void Scheduler::SetMinIndex() 
{
	//Initializing the index of the processor with the smallest finish time
	MinIndex = 0;

	//Checking which processor has the smallest expected finish time
	for (size_t i = 1; i < ProcessorsCount; i++)
	{
		if (ProcessorsList[i]->GetFinishTime() < ProcessorsList[MinIndex]->GetFinishTime())
			MinIndex = i;
	}
}

void Scheduler::SetMaxIndex() 
{
	//Initializing the index of the processor with the biggest finish time
	MaxIndex = 0;

	//Checking which processor has the biggest expected finish time
	for (size_t i = 1; i < ProcessorsCount; i++)
	{
		if (ProcessorsList[i]->GetFinishTime() > ProcessorsList[MaxIndex]->GetFinishTime())
			MaxIndex = i;
	}
}

int Scheduler::CalcStealLimit()
{
	SQF = ProcessorsList[MinIndex]->GetFinishTime();
	LQF = ProcessorsList[MaxIndex]->GetFinishTime();

	if (!LQF)		
		return 0;

	int StealLimit = 100 * (LQF - SQF) / LQF;

	return StealLimit;
}
