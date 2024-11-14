#include "StreamManager.h"

StreamManager::StreamManager()
{
	m_pBufEnd = 0;
}

int StreamManager::Open(LPSTR filename)
{
	return ((int(__thiscall *)(StreamManager*, LPSTR))0x5E6BF0)(this, filename);
}

int StreamManager::Close()
{
	return ((int(__thiscall *)(StreamManager*))0x5E6B30)(this);
}

int StreamManager::Skip(DWORD startOffset, DWORD size)
{
	return ((int(__thiscall *)(StreamManager*, DWORD, DWORD))0x5E6B60)(this, startOffset, size);
}

int StreamManager::Read(DWORD startOffset, char *dest, DWORD size)
{
	return ((int(__thiscall *)(StreamManager*, DWORD, char*, DWORD))0x5E6C10)(this, startOffset, dest, size);
}