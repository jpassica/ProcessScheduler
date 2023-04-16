#include "scheduler.h"
#include <fstream>

void scheduler::setProcessors(int NF, int NS, int NR , int RRtimeSlice)
{
	Processors_List = new Processor * [NF + NS + NR];
	RUN = new Process* [NF + NS + NR];
	for (int i = 0; i < NF; i++)
	{
		Processors_List[i] = new FCFS_Processor(i+1);
		RUN[i] = nullptr;
	}

	for (int i = NF; i < NS; i++)
	{
		Processors_List[i] = new SJF_Processor(i+NF+1);
		RUN[i] = nullptr;
	}

	for (int i = NS+NR; i < NR; i++)
	{
		Processors_List[i] = new RR_Processor(i+NF+NR+1 ,RRtimeSlice);
		RUN[i] = nullptr;
	}
}

scheduler::scheduler()	
{
	Timestep = 0;

	Processors_List = nullptr;

	FCFSCount=0;
	SJFCount=0;
	RRCount=0;
	ProcessorsCount=0;

	RUN = nullptr;

	RTFCount = 0;
	MaxWCount = 0;
	STLCount = 0;
	Forkcount = 0;
	KillCount = 0;
}

int scheduler::getTimeStep()
{
	return Timestep;
}

Processor** scheduler::getProcessors_List()
{
	return Processors_List;
}

const Queue<Process*>& scheduler::getBLK()
{
	return BLK;
}

const Queue<Process*>& scheduler::getTRM()
{
	return TRM;
}

Process** scheduler::getRUN()
{
	return RUN;
}	

int scheduler::getFCFSCount()
{
	return 	FCFSCount;
}

int scheduler::getSJFCount()
{
	return SJFCount;
}

int scheduler::getRRCount()
{
	return RRCount;
}

int scheduler::getProcessorsCount()
{
	return ProcessorsCount;
}

bool scheduler::ReadInputFile(string filename)
{
	//creating input stream object and opening file
	fstream IP_File_Stream;
	IP_File_Stream.open(filename);

	if (!IP_File_Stream.is_open())			//if file open is not successful, abort
		return false;

	if (!IP_File_Stream.good())		//if file is not good or corrupted, abort
		return false;

	//now we can safely read from file

	//reading the main parameters
	IP_File_Stream >> FCFSCount >> SJFCount >> RRCount;
	IP_File_Stream >> RRtimeSlice;
	IP_File_Stream >> RTF >> MaxW >> STL >> ForkProb;
	IP_File_Stream >> ProcessesCount;

	//variables to be read for each process and sent to ctor
	int AT(0), PID(0), CT(0), IO_N(0), IO_R(0), IO_D(0);

	//pointer to be used for creating and allocating all the processes
	Process* newProcess(nullptr);

	//used for reading the string of IO pairs
	string IO_st, IO_R_st, IO_D_st;

	//reading data of all processes
	for (size_t i(0); i < ProcessesCount; i++)
	{
		IP_File_Stream >> AT >> PID >> CT >> IO_N;

		//read string only if there are IO requests
		if (IO_N)
			IP_File_Stream >> IO_st;

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
		this->NEW.Enqueue(newProcess);
	}

	//reading SIGKILLs

	//skipping the phrase 'SIGKILL Times'
	string SIGKILL_st, TIMES_st;
	IP_File_Stream >> SIGKILL_st >> TIMES_st;

	//making sure the file's format is as expected
	if (SIGKILL_st != "SIGKILL" || TIMES_st != "Times")
		return false;

	//reading each kill signal's data until there is no more data
	int time(0), ID(0);
	while (!IP_File_Stream.eof())
	{
		IP_File_Stream >> time >> PID;

		KillSignal* newKillSignal = new KillSignal(time, PID);

		KillSignalQ.Enqueue(newKillSignal);
	}
	
	//after successfully reading all data
	return true;
}

scheduler::~scheduler()  {}
