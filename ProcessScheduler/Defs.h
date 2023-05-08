//This file is for various data modules used all throughout the project

enum ProcessorState { IDLE, BUSY, STOP };

enum UI_Mode { Interactive, StepByStep, Silent };

struct KillSignal {
	int Time;
	int PID;

	KillSignal(int t, int ID)
	{
		Time = t;
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