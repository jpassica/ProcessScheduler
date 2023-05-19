#include "scheduler.h"
#include <fstream>
#include <iomanip>

Scheduler::Scheduler()
{
	//Initializing all data members
	TimeStep = 0;

	ProgramUI = new UI();

	ProcessorsList = nullptr;

	FCFSCount = 0;
	SJFCount = 0;
	RRCount = 0;
	EDFCount = 0;
	RRtimeSlice = 0;
	ProcessorsCount = 0;
	ProcessesCount = 0;

	RTFMigrationCount = 0;
	MaxWMigrationCount = 0;
	StealCount = 0;
	ForkCount = 0;
	KillCount = 0;
	CompletedBeforeDeadlineCount = 0;

	RTF = 0;
	STL = 0;
	MaxW = 0;

	TotalResponseTime = 0;
	TotalTurnAroundtime = 0;
	TotalWaitingTime = 0;

	MaxIndex = 0;
	MinIndex = 0;

	ProcessedIO_D = 0;
}

void Scheduler::AllocateProcessors(int NF, int NS, int NR, int NE, int RRtimeSlice, int ForkProbability, int HealingTime)
{
	ProcessorsList = new Processor * [ProcessorsCount];

	for (int i = 0; i < ProcessorsCount; i++)
	{
		if (i < NF)
			ProcessorsList[i] = new FCFS_Processor(i + 1, ForkProbability, this, HealingTime);

		else if (i >= NF && i < NF + NS)
			ProcessorsList[i] = new SJF_Processor(i + 1, this, HealingTime);

		else if (i >= NF + NS && i < NF + NS + NR)
			ProcessorsList[i] = new RR_Processor(i + 1, RRtimeSlice, this, HealingTime);

		else
			ProcessorsList[i] = new EDF_Processor(i + 1, this, HealingTime);
	}
}

bool Scheduler::ReadInputFile(string& FileName)
{
	//Creating input stream object and opening file
	ifstream IP_Stream;
	FileName += ".txt";
	IP_Stream.open(FileName);

	while (!IP_Stream.is_open())							//keep reading file names until we successfully open file
	{
		FileName = ProgramUI->ReadInputFileName(1);
		FileName += ".txt";
		IP_Stream.open(FileName);
	}

	if (!IP_Stream.good())									//if file is not good or corrupted, abort
		return false;

	//now we can safely read from file

	int ForkProb(0), HealingTime(0);

	//reading the main parameters
	IP_Stream >> FCFSCount >> SJFCount >> RRCount >> EDFCount;
	IP_Stream >> RRtimeSlice;
	IP_Stream >> RTF >> MaxW >> STL >> ForkProb >> HealingTime;
	IP_Stream >> ProcessesCount;

	//setting ProcessorsCount
	ProcessorsCount = FCFSCount + SJFCount + RRCount + EDFCount;

	//creating the processors according to the read data
	AllocateProcessors(FCFSCount, SJFCount, RRCount, EDFCount, RRtimeSlice, ForkProb, HealingTime);

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

		newProcess = new Process(PID, AT, CT, DL);

		//iterator for reading the IO requests string
		int StIndex(1);

		for (size_t j = 0; j < IO_N; j++)
		{
			//Start from 1st digit

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

	//Reading SIGKILLs

	//Reading each kill signal's data until there is no more data
	int Time(0), ID(0);
	
	while (!IP_Stream.eof())
	{
		IP_Stream >> Time >> PID;

		FCFS_Processor::AddKillSignal(Time, PID);
	}

	IP_Stream.close();

	//after successfully reading all data
	return true;
}

void Scheduler::WriteOutputFile(string FileName)
{
	//Creating output stream object and opening file for writing
	ofstream OP_Stream;
	char FileCode = FileName[FileName.length() - 5];
	FileName = "OutputFile";
	FileName +=  FileCode;
	FileName += ".txt";
	OP_Stream.open(FileName);

	//If there is any problem with the file, abort
	if (!OP_Stream || !OP_Stream.good())
		return;

	//Starting to write the first line, setting a specific width for each field
	OP_Stream << left << setprecision(1) << fixed
		<< setw(7) << "TT" << setw(7) << "PID"
		<< setw(7) << "AT" << setw(7) << "CT"
		<< setw(9) << "IO_D" << setw(8) << "WT"
		<< setw(7) << "RT" << setw(7) << "TRT" << endl;

	//Pointer used to dequeue from TRM, write all needed info, and then deallocate process object
	Process* deletePtr(nullptr);

	for (size_t i = 0; i < ProcessesCount && !TRM_List.isEmpty(); i++)
	{
		TRM_List.Dequeue(deletePtr);

		//Writing all needed info for each process
		OP_Stream << setw(7) << deletePtr->GetTerminationTime()
			<< setw(7) << deletePtr->GetID()
			<< setw(7) << deletePtr->GetArrivalTime()
			<< setw(8) << deletePtr->GetCPUTime()
			<< setw(8) << deletePtr->GetTotalIO_D()
			<< setw(8) << deletePtr->GetWaitingTime()
			<< setw(7) << deletePtr->GetResponseTime()
			<< setw(7) << deletePtr->GetTurnAroundTime()
			<< endl;

		TotalWaitingTime += deletePtr->GetWaitingTime();
		TotalResponseTime += deletePtr->GetResponseTime();
		TotalTurnAroundtime += deletePtr->GetTurnAroundTime();

		//Deallocating process since we don't need it anymore
		delete deletePtr;
	}


	//--- Process statistics ---
	OP_Stream << "\nProcesses: " << ProcessesCount << endl
		<< "Avg WT = " << CalcAvgWT() << ",     "
		<< "Avg RT = " << CalcAvgRT() << ",     "
		<< "Avg TRT = " << CalcAvgTRT() << endl;

	//To prevent any instance of division by zero
	if (!ProcessesCount) return;

	OP_Stream << "Migration%:     RTF= " << CalcRTFMigrationPercentage()
		<< "%,     MaxW= " << CalcMaxWMigrationPercentage() << "%\n"
		<< "Work Steal%: " << CalcStealPercentage() << "%\n"
		<< "Forked Processes: " << CalcForkingPercentage() << "%\n"
		<< "Killed Processes: " << CalcKillPercentage() << "%\n"
		<< "Processes Completed Before Deadline: " << CalcBeforeDeadlinePercentage() << "%\n\n";


	//--- Processor Statistics ---
	OP_Stream << "Processors: " << ProcessorsCount << " ["
		<< FCFSCount << " FCFS, " << SJFCount << " SJF, "
		<< RRCount << " RR, " << EDFCount << " EDF]\n";

	OP_Stream << "Processors Load\n";

	for (size_t i = 0; i < ProcessorsCount; i++)
	{
		OP_Stream << "p" << i + 1 << " = " << 
			ProcessorsList[i]->CalcPLoad(TotalTurnAroundtime) << "%";

		if (i != ProcessorsCount)
			OP_Stream << ",     ";
	}

	OP_Stream << "\n\nProcessors Utiliz\n";

	for (size_t i = 0; i < ProcessorsCount; i++)
	{
		OP_Stream << "p" << i + 1 << " = " <<
			ProcessorsList[i]->CalcPUtil() << "%";

		if (i != ProcessorsCount)
			OP_Stream << ",     ";
	}

	OP_Stream << endl << "Avg utilization = " << CalcAvgUtilization() << "%";

	OP_Stream.close();
}

void Scheduler::HandleIODuration() 
{
	if (!BLK_List.isEmpty())
	{
		if (ProcessedIO_D == BLK_List.QueueFront()->GetIO_D())
		{
			BLK_List.QueueFront()->DeleteIO_Request();
			UnBlockProcess();
			ProcessedIO_D = 0;
		}
		ProcessedIO_D++;
	}
}

bool Scheduler::MigrateFromFCFStoRR(Process* MigratingProcess)
{
	// If there is no process to migrate 
    // Or if there are no processors to migrate to
	if (!MigratingProcess || !RRCount)
		return false;

	MigratingProcess->UpdateWaitingTime(TimeStep);

	if (!MigratingProcess->IsChild() && MigratingProcess->GetWaitingTime() > MaxW)
	{
		if (!SetMinIndex(3))				// Set MinIndex to index of RR processor with shortest ready queue
			return false;

		ProcessorsList[MinIndex]->AddToReadyQueue(MigratingProcess);

		MaxWMigrationCount++;

		return true;
	}
	return false;
}

bool Scheduler::MigrateFromRRtoSJF(Process* MigratingProcess) 
{
    // If there is no process to migrate 
	// Or if there are no processors to migrate to
	if (!MigratingProcess || !SJFCount)
		return false;

	if (MigratingProcess->GetRemainingCPUTime() < RTF) 
	{
		if (!SetMinIndex(2))				// Set MinIndex to index of SJF processor with shortest ready queue		
			return false;

		ProcessorsList[MinIndex]->AddToReadyQueue(MigratingProcess);

		RTFMigrationCount++;

		return true;
	}
	return false;
}

void Scheduler::Steal()
{
	//Setting the indices of the processors with the longest and shortest finish times
	if (!SetMinIndex())  return;
	if (!SetMaxIndex())  return;

	//Calculating the steal limit
	int StealLimit = CalcStealLimit();

	while (StealLimit > 40)
	{
		Process* StolenProcess = ProcessorsList[MaxIndex]->StealProcess();

		//If the longest queue is comprised entirely of forked processes, we cannot steal any 
		if (!StolenProcess)
			return;

		ProcessorsList[MinIndex]->AddToReadyQueue(StolenProcess);
	
		StealCount++;

		//Recalculating the steal limit
		StealLimit = CalcStealLimit();
	}
}

void Scheduler::IncrementKillCount()
{
	KillCount++;
}

void Scheduler::CheckForOverheating()
{
	bool TurnedOff = 0;

	for (size_t i = 0; i < ProcessorsCount && !TurnedOff; i++)
	{
		if (rand() % 1000 < 3)
		{
			ProcessorsList[i]->ClearOverheatedProcessor();
			TurnedOff = 1;
		}
	}
}

void Scheduler::BlockProcess(Process* ProcessPtr)
{
	//Moving process to BLK_List
	BLK_List.Enqueue(ProcessPtr);							
}

void Scheduler::Fork(Process* ParentProcess)
{
	//Checking if the process can't fork
	if (!ParentProcess->CanFork())
		return;

	//Initializing the child Process's data members
	int ChildID = ++ProcessesCount;
	int ChildAT = TimeStep;
	int ChildCT = ParentProcess->GetRemainingCPUTime();
	int ChildDL = ParentProcess->GetDeadline();

	//Creating child process
	Process* Child = new Process(ChildID, ChildAT, ChildCT, ChildDL);

	ParentProcess->AddChild(Child);

	// We are sure that there will be at least 1 functional processor
	SetMinIndex(1);

	ProcessorsList[MinIndex]->AddToReadyQueue(Child);

	ForkCount++;
}

void Scheduler::KillOrphan(Process* OrphanProcess)
{
	if (!OrphanProcess)
		return;

	bool Found = 0;
	FCFS_Processor* FCFSPtr = nullptr;

	for (size_t i = 0; i < FCFSCount && !Found; i++)
	{
		//Force casting to access FCFS_Processor exclusive member function
		FCFSPtr = (FCFS_Processor*)ProcessorsList[i];

		Found = FCFSPtr->KillOrphan(OrphanProcess->GetID());
	}
}

void Scheduler::UnBlockProcess()
{
	//Checking if BLK_List is Empty
	if (BLK_List.isEmpty())
		return;

	//Dequeueing the process to be unblocked
	Process* UnBlockedProcess = nullptr;
	BLK_List.Dequeue(UnBlockedProcess);

	//Choosing the processor with the shortest ready queue
	if (SetMinIndex())
		ProcessorsList[MinIndex]->AddToReadyQueue(UnBlockedProcess);

	else
		TMP_List.Enqueue(UnBlockedProcess);
}

void Scheduler::TerminateProcess(Process* ProcessToTerminate)
{
	//Checking if the terminated process has children 
	if (ProcessToTerminate->IsParent())
	{
		KillOrphan(ProcessToTerminate->GetLeftChild());

		KillOrphan(ProcessToTerminate->GetRightChild());
	}

	//Checking if the terminated process is a child
	if (ProcessToTerminate->IsChild())
		ProcessToTerminate->SeparateFromParent();

	//Moving to TRM & changing states
	ProcessToTerminate->SetTerminationTime(TimeStep);

	//If the process was completed before its expected deadline
	if (TimeStep < ProcessToTerminate->GetDeadline())
		CompletedBeforeDeadlineCount++;

	TRM_List.Enqueue(ProcessToTerminate);
}

void Scheduler::MoveNEWtoRDY()
{
	Process* NewProcessPtr = nullptr;

	if (!NEW_List.isEmpty())
		NewProcessPtr = NEW_List.QueueFront();
	else
		return;

	while (NewProcessPtr && NewProcessPtr->GetArrivalTime() == TimeStep)
	{
		NEW_List.Dequeue(NewProcessPtr);

		//Setting the index of the processor with the smallest expected finish Time
		if (SetMinIndex())
			//Adding the process to the RDY queue of the processor with the smallest finish Time
			ProcessorsList[MinIndex]->AddToReadyQueue(NewProcessPtr);
		else
			TMP_List.Enqueue(NewProcessPtr);


		if (!NEW_List.isEmpty())
			NewProcessPtr = NEW_List.QueueFront();
		else
			NewProcessPtr = nullptr;
	}

	Process* ReturningProcess = nullptr;

	//Returning all processes that had to wait for overheated processors to recover
	while (!TMP_List.isEmpty())
	{
		if (!SetMinIndex())
			break;

		ProcessorsList[MinIndex]->AddToReadyQueue(ReturningProcess);

		if (!TMP_List.isEmpty())
			ReturningProcess = NEW_List.QueueFront();
		else
			ReturningProcess = nullptr;
	}
}

void Scheduler::MovetoRDY(Process* ProcessToMove) 
{
	//Setting the index of the processor with the smallest expected finish Time
	if (SetMinIndex())
		//Adding the process to the RDY queue of the processor with the smallest finish Time
		ProcessorsList[MinIndex]->AddToReadyQueue(ProcessToMove);
	else
		TMP_List.Enqueue(ProcessToMove);
}

void Scheduler::Simulate()
{
	UI_Mode CrntMode;

	//User chooses what input file to use
	string FileName = ProgramUI->ReadInputFileName();

	//Reading the input file
	if (!ReadInputFile(FileName)) return;

	//User chooses what UI mode to run on
	CrntMode = ProgramUI->InputInterfaceMode();

	if (CrntMode == Silent)
		ProgramUI->PrintSilentMode(0);


	while (TRM_List.getCount() != ProcessesCount)	//program ends when all processes are in TRM list
	{
		MoveNEWtoRDY();

		for (size_t i = 0; i < ProcessorsCount; i++)
		{
			//Calling ScheduleAlgo of each processor   
			ProcessorsList[i]->ScheduleAlgo(TimeStep);

			if (i == FCFSCount)
				FCFS_Processor::RemoveIgnoredKillSignals(TimeStep);
		}

		//Initiating the steal action each STL timesteps
		if (TimeStep % STL == 0 && TimeStep)
			Steal();

		//Handle IO_Duration in BLK list each Time step
		HandleIODuration();

		//Overheating
		CheckForOverheating();

		//Incrementing & printing timestep
		if (CrntMode != Silent)
			ProgramUI->PrintTimeStep(BLK_List, TRM_List, ProcessorsList, FCFSCount, SJFCount, RRCount, EDFCount, TimeStep);

		TimeStep++;
	}

	//Generating the output file
	WriteOutputFile(FileName);

	if (CrntMode == Silent)
		ProgramUI->PrintSilentMode(1);
}

bool Scheduler::SetMinIndex(int RangeSelect)
{
	//If RangeSelect = 0 (default), the fn will search for the shortest queue within all procssors
	//If RangeSelect = 1, it will search within the range of FCFS processors only
	//If RangeSelect = 2, it will search within the range of SJF processors only
	//If RangeSelect = 3, it will search within the range of RR processors only

	//Default values for start, end & initializing MinIndex
	int Start, End;

	//while loop to handle that if all of the processors are stopped 
	if (RangeSelect == 1)			//The range is FCFS processors only
	{
		MinIndex = 0;
		End = FCFSCount;

		while (ProcessorsList[MinIndex]->IsStopped() && MinIndex < End)
			MinIndex++;

		if (MinIndex == End)
			return false;
	}

	else if (RangeSelect == 2)		//The range is SJF processors only
	{
		MinIndex = FCFSCount;
		End = FCFSCount + SJFCount;

		while (ProcessorsList[MinIndex]->IsStopped() && MinIndex < End) 
			MinIndex++;
	
		if (MinIndex == End)
			return false;
	}

	else if (RangeSelect == 3)		//The range is RR processors only
	{
		MinIndex = FCFSCount + SJFCount;
		End = FCFSCount + SJFCount + RRCount;

		while (ProcessorsList[MinIndex]->IsStopped() && MinIndex < End) 
			MinIndex++;

		if (MinIndex == End)
			return false;
	}

	else if (!RangeSelect)
	{
		MinIndex = 0;
		End = ProcessorsCount;

		while (ProcessorsList[MinIndex]->IsStopped() && MinIndex < End)
			MinIndex++;

		if (MinIndex == End)
			return false;
	}

	Start = MinIndex + 1;

	//Checking which processor has the smallest expected finish Time (within given range)
	for (size_t i = Start; i < End; i++)
	{
		if (ProcessorsList[i]->GetFinishTime() < ProcessorsList[MinIndex]->GetFinishTime() && !ProcessorsList[i]->IsStopped())
			MinIndex = i;
	}

	if (ProcessorsList[MinIndex]->IsStopped())
		return false;
	else
		return true;
}

bool Scheduler::SetMaxIndex(int RangeSelect)
{
	MaxIndex = 0;

	//Checking which processor has the biggest expected finish Time
	for (size_t i = 1; i < ProcessorsCount; i++)
	{
		if (ProcessorsList[i]->GetFinishTime() > ProcessorsList[MaxIndex]->GetFinishTime() && !ProcessorsList[i]->IsStopped())
			MaxIndex = i;
	}

	if (ProcessorsList[MinIndex]->IsStopped())
		return false;
	else
		return true;
}

int Scheduler::CalcStealLimit() const
{
	int SQF = ProcessorsList[MinIndex]->GetFinishTime();
	int LQF = ProcessorsList[MaxIndex]->GetFinishTime();

	if (!LQF)
		return 0;

	int StealLimit = 100 * (LQF - SQF) / LQF;

	return StealLimit;
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
	return (double)100 * RTFMigrationCount / ProcessesCount;
}

double Scheduler::CalcMaxWMigrationPercentage() const
{
	return (double)100 * MaxWMigrationCount / ProcessesCount;
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

Scheduler::~Scheduler()
{
	//Deallocating dynamically allocated objects
	for (size_t i = 0; i < ProcessorsCount; i++)
	{
		delete ProcessorsList[i];
	}
	delete[] ProcessorsList;

	delete ProgramUI;

	FCFS_Processor::ClearKillSignalQ();
}