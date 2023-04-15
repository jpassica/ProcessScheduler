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

void scheduler::ReadInputFile()
{
	fstream IP_File_Stream;
	string filename("IP_File");
	IP_File_Stream.open(filename);

	if (IP_File_Stream)
	{   //read from file

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

		int StIndex(0);

		for (size_t i(0); i < ProcessesCount; i++)
		{
			IP_File_Stream >> AT >> PID >> CT >> IO_N;
			IP_File_Stream >> IO_st;

			newProcess = new Process(PID, AT, CT);
			newProcess->InitialIO(IO_N);

			for (size_t j = 0; j < IO_N; j++)
			{
				//start from 1st digit

				while (IO_st[StIndex] != ',')
				{
					IO_R_st += IO_st[StIndex]; i++;
				}

				i++;			//skip comma between numbers

				while (IO_st[StIndex] != ')')
				{
					IO_D_st += IO_st[StIndex]; i++;
				}

				i += 3;			//skip closing bracket, intermediary comma, and opening bracket  "),("

				IO_R = stoi(IO_R_st);
				IO_D = stoi(IO_D_st);

				IO_R_st.clear();
				IO_D_st.clear();

				newProcess->SetIO(j, IO_R, IO_D);
			}
		}
	}
	else
	{
		cerr << "Error. Couldn't open file!\n";
	}
}

scheduler::~scheduler()
{
	//for(int )
}
