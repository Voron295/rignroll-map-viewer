#include "CBlockObserver.h"

int CBlockObserver::ReadFromFile(FILE *f)
{
	char buf[32];
	fread(buf, 32, 1, f);
	fread(buf, 32, 1, f);
	return 1;
}

void CBlockObserver::WriteToFile(FILE *f)
{

}