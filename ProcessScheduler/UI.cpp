#include "UI.h"
using namespace std;

UI::UI(scheduler* s)
{
	pSch = s;
}

string UI::ReadLine()
{
	string s;
	getline(cin, s);
	return  s;
}

void UI::WriteLine(string s)
{
	cout << s;
}

void UI::TimeStepOut()
{
	cout << "Current Timestep: " << pSch->getTimeStep() << endl;
	cout << "--------------------   RDY Processes --------------------------------" << endl;
	for (int i = 0; i < pSch->getFCFSCount(); i++)
	{
	//	cout<<"Processor "<<i<<"[FCFS]"<<": "<< 
	}
	//To be continued
}