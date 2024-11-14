#ifndef SOUND_CLASS_H
#define SOUND_CLASS_H
#include <string>

class SoundFile
{
public:
	std::string m_sName;
	void *m_pData;
	unsigned int m_dwSize;

	SoundFile();
	~SoundFile();

	bool ReadFromFile(FILE *f);
};

class Sound
{
public:
	std::string m_sName;
	int m_dwFileId;

	Sound();
	~Sound();

	bool ReadFromFile(FILE *f);
};

#endif