//This file is for various data modules used all throughout the project

enum ProcessState { NEW, RDY, RUN, BLK, TRM, ORPH };

enum ProcessorState { IDLE, BUSY };

struct KillSignal {
	int time;
	int PID;

	KillSignal(int t, int ID)
	{
		time = t;
		PID = ID;
	}
};