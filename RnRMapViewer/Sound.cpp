#include "GameModule.h"

SoundFile::SoundFile()
{
	m_pData = 0;
	m_dwSize = 0;
}

SoundFile::~SoundFile()
{
	if (m_pData && m_dwSize)
		free(m_pData);
}

bool SoundFile::ReadFromFile(FILE *f)
{
	m_sName = GameModule::ReadStringFromFile(f);
	m_dwSize = GameModule::ReadIntFromFile(f);
	m_pData = malloc(m_dwSize);
	fread(m_pData, 1, m_dwSize, f);
	return true;
}

Sound::Sound()
{
	m_dwFileId = -1;
}

Sound::~Sound()
{

}

bool Sound::ReadFromFile(FILE *f)
{
	char *name_str = GameModule::ReadStringFromFile(f);
	m_sName = strtok(name_str, " ");
	m_dwFileId = atoi(strtok(0, " "));
	return true;
}