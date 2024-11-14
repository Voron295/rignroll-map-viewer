#include <iostream>
#include "RGB.h"
#include <string>

class Palette
{
public:
	Palette();
	~Palette();

	bool ReadFromFile(FILE *f);

	std::string m_sName;
	RGB m_pPalette[256];

};