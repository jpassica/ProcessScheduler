//This file is for various data modules used all throughout the project

enum ProcessState { NEW, RDY, RUN, BLK, TRM, ORPH };

enum ProcessorState { IDLE, BUSY };

enum UI_Mode { Interactive, StepByStep, Silent };

struct KillSignal {
	int time;
	int PID;

	KillSignal(int t=0, int ID=0)
	{
		time = t;
		PID = ID;
	}
};

struct IO_Request {
	int IO_R;
	int IO_D;

	IO_Request(int IO_r, int IO_d) 
	{
		IO_R = IO_r;
		IO_D = IO_d;
	}
};