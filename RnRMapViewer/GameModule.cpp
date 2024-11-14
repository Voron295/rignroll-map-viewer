//#include "GameModule.h"
#include <string>
#include "CBlockContainer0x2.h"
#include "CBlockContainer0x3.h"
#include "CBlockContainerStatic.h"
#include "CBlockContainer0x5.h"
#include "CBlockContainer0x6.h"
#include "CBlockContainer0x7.h"
#include "CBlockModelElement.h"
#include "CBlock0xD.h"
#include "CBlockAnmObj.h"
#include "CBlockRef.h"
#include "CBlock0x19.h"
#include "CBlockContainer0x1A.h"
#include "CBlockSprite.h"
#include "CBlock0x1C.h"
#include "CBlockContainer0x1D.h"
#include "CBlockRoomDesc.h"
#include "CBlock0x1F.h"
#include "CBlockContainerLamp.h"
#include "CBlockCollision.h"
#include "CBlockContainerEnv.h"
#include "CBlockContainer.h"
#include "CBlockObserver.h"
#include "CBlockGenerator.h"
#include "ResourceManager.h"

// Helper functions
DWORD GameModule::ReadIntFromFile(FILE *f)
{
	int buf;
	fread(&buf, 4, 1, f);
	return buf;
}

float GameModule::ReadFloatFromFile(FILE *f)
{
	float buf;
	fread(&buf, 4, 1, f);
	return buf;
}

void GameModule::ReadVectorFromFile(FILE *f, CVector *v)
{
	fread(v, 4, 3, f);
}

void GameModule::ReadVector4FromFile(FILE *f, CVector4 *v)
{
	fread(v, 16, 1, f);
}

void GameModule::ReadMatrixFromFile(FILE *f, CMatrix *m)
{
	fread(m, 4, 12, f);
}

void GameModule::ReadVertexFromFile(FILE *f, Vertex *v)
{
	v->pos.x = ReadFloatFromFile(f);
	v->pos.y = ReadFloatFromFile(f);
	v->pos.z = ReadFloatFromFile(f);
	v->u = ReadFloatFromFile(f);
	v->v = ReadFloatFromFile(f);
}

char *GameModule::ReadStringFromFile(FILE *f, int size)
{
	static char buf[4096];
	char *pBuf = buf;
	buf[0] = 0;

	if (size > 0)
	{
		fread(buf, size, 1, f);
		return buf;
	}

	while (fread(pBuf, 1, 1, f))
	{
		if (!*pBuf)
			return buf;

		pBuf++;
	}

	return 0;
}

void GameModule::WriteIntToFile(FILE *f, DWORD val)
{
	fwrite(&val, 4, 1, f);
}

void GameModule::WriteFloatToFile(FILE *f, float val)
{
	fwrite(&val, 4, 1, f);
}

void GameModule::WriteVectorToFile(FILE *f, CVector *v)
{
	fwrite(v, 4, 3, f);
}

void GameModule::WriteVector4ToFile(FILE *f, CVector4 *v)
{
	fwrite(v, 4, 4, f);
}

void GameModule::WriteMatrixToFile(FILE *f, CMatrix *m)
{
	fwrite(m, 4, 12, f);
}

void GameModule::WriteVertexToFile(FILE *f, Vertex *v)
{
	WriteFloatToFile(f, v->pos.x);
	WriteFloatToFile(f, v->pos.y);
	WriteFloatToFile(f, v->pos.z);
	WriteFloatToFile(f, v->u);
	WriteFloatToFile(f, v->v);
}

void GameModule::WriteStringToFile(FILE *f, char *str, int size)
{
	if (!size)
		size = strlen(str) + 1;

	for (int i = 0; i < size; i++)
	{
		fwrite(str, 1, 1, f);
		if (str)
			str++;
	}
}

// Constructor
GameModule::GameModule()
{
	m_pBlocks = NULL;
	m_pRooms = NULL;
}

// Loading the game module
// It means it will load the B3D and RES file at once into class instance
void GameModule::Load(const char *path, const char *name)
{
	std::string filePath;
	if (!path || !name)
		return;

	m_sName = name;

	filePath = path;
	filePath += name;
	filePath += ".b3d";

	FILE *f = fopen(filePath.c_str(), "rb");
	if (!f)
		return;

	LoadB3D(f);
	fclose(f);

	filePath = path;
	filePath += name;
	filePath += ".res";

	f = fopen(filePath.c_str(), "rb");
	if (f)
	{
		LoadRES(f);
		fclose(f);
	}

	InitPrimitiveData();
}

// Loading the B3D file using FILE pointer
// This function is almost a copy of game reading B3D file function
void GameModule::LoadB3D(FILE *f)
{
	char textBuf[4];
	fread(&textBuf, 1, 4, f); // b3d
	if (strncmp(textBuf, "b3d", 3))
		return; // not signed as b3d file

	ReadIntFromFile(f); // Unused value
	int materialsOffset = ReadIntFromFile(f) * 4;
	ReadIntFromFile(f); // Unused value
	int blocksOffset = ReadIntFromFile(f) * 4;
	ReadIntFromFile(f); // Unused value

	if (materialsOffset)
	{
		fseek(f, materialsOffset, SEEK_SET);
		int matCount = ReadIntFromFile(f);
		
		if (matCount)
		{
			for (int i = 0; i < matCount; i++)
			{
				Material *mat = new Material();
				mat->m_sName = ReadStringFromFile(f, 32);
				m_pMaterials.push_back(mat);
			}
		}
	}

	if (blocksOffset)
	{
		fseek(f, blocksOffset, SEEK_SET);

		if (ReadIntFromFile(f) != 111) // start of blocks
			return;

		while (true)
		{
			int flag;
			do
			{
				flag = ReadIntFromFile(f);
			} while (flag == 444); // skip flag
			
			if (flag == 222)
				break; // end of file

			if (flag != 333)
				return; // something gone wrong

			int blockType;
			CSimpleBlock *block = LoadBlock(f, this, blockType, 0, 0);
			if (block)
			{
				AddToPool(block);
				if (ReadIntFromFile(f) != 555)
					return;
			}
			else
			{
				if (blockType)
				{
					if (blockType != 1)
						return;

					CBlockObserver *m_pObserver = new CBlockObserver();
					m_pObserver->ReadFromFile(f);
				}
				else
				{
					fseek(f, 0x2C, SEEK_CUR);
				}
				if (ReadIntFromFile(f) != 555)
					return;
			}
		}
	}
}

CSimpleBlock *GameModule::AddToPool(CSimpleBlock *block)
{
	CSimpleBlock *result;
	if (block->m_dwType == 6)
	{
		result = m_pBlocks;
		if (m_pBlocks)
			result = (CSimpleBlock *)block->InsertAfter(m_pBlocks);
		else
			m_pBlocks = block;
	}
	else
	{
		block->m_pModule = this;
		result = m_pRooms;
		if (m_pRooms)
			result = (CSimpleBlock *)block->InsertAfter(m_pRooms);
		else
			m_pRooms = block;
	}
	return result;
}

void GameModule::InitPrimitiveData()
{
	for (auto it = m_pMaterials.begin(); it != m_pMaterials.end(); it++)
	{
		(*it)->InitDirect3D(this);
	}


	CSimpleBlock *block = m_pRooms;
	if (block)
	{
		do
		{
			block->ScanFunc(InitPrimitiveDataCb);
			block = (CSimpleBlock*)block->m_pNext;
		} while (block);
	}
	block = m_pBlocks;
	if (block)
	{
		do
		{
			block->ScanFunc(InitPrimitiveDataCb);
			block = (CSimpleBlock*)block->m_pNext;
		} while (block);
	}
}

int GameModule::InitPrimitiveDataCb(CSimpleBlock *block)
{
	switch (block->m_dwType)
	{
	case 1:
		((CBlockModelElement *)block)->InitPrimitiveData();
		break;

	case 2:
		((CBlockContainer0x6 *)block)->InitPrimitiveData();
		break;

	case 3:
		((CBlockContainer0x7 *)block)->InitPrimitiveData();
		break;

	case 0x16:
		((CBlockCollision *)block)->InitPrimitiveData();
		break;
	}
	return -1;
}

// Loading a sub-block of B3D file
CSimpleBlock *GameModule::LoadBlock(FILE *f, GameModule *module, int &blockType, void *data1, void *data2)
{
	char blockName[32];
	fread(blockName, 32, 1, f);
	blockType = ReadIntFromFile(f);
	CSimpleBlock *block = NULL;
	switch (blockType)
	{
	case 0:
	case 1:
		return 0;

	case 2:
		block = new CBlockContainer0x2(blockName);
		break;
		
	case 3:
		block = new CBlockContainer0x3(blockName);
		break;

	case 4:
		block = new CBlockContainerStatic(blockName);
		break;

	case 5:
		block = new CBlockContainer0x5(blockName);
		break;

	case 6:
		block = new CBlockContainer0x6(blockName);
		break;

	case 7:
		block = new CBlockContainer0x7(blockName);
		break;

	case 8:
		block = new CBlockModelElement(blockName);
		break;

	case 9:
	case 0x16:
		block = new CBlockContainerLOD(blockName);
		((CBlockContainerLOD *)block)->data.type = 5;
		break;

	case 0xA:
		block = new CBlockContainerLOD(blockName);
		((CBlockContainerLOD *)block)->data.type = 1;
		break;

	case 0xB:
		block = new CBlockContainerLOD(blockName);
		((CBlockContainerLOD *)block)->data.type = 3;
		break;

	case 0xC:
		block = new CBlock0xD(blockName);
		((CBlock0xD *)block)->data.type = 5;
		break;

	case 0xD:
		block = new CBlock0xD(blockName);
		((CBlock0xD *)block)->data.type = 0;
		break;

	case 0xE:
		block = new CBlock0xD(blockName);
		((CBlock0xD *)block)->data.type = 1;
		break;

	case 0xF:
		block = new CBlock0xD(blockName);
		((CBlock0xD *)block)->data.type = 2;
		break;

	case 0x10:
		block = new CBlock0xD(blockName);
		((CBlock0xD *)block)->data.type = 3;
		break;

	case 0x11:
		block = new CBlock0xD(blockName);
		((CBlock0xD *)block)->data.type = 4;
		break;

	case 0x12:
		block = new CBlockAnmObj(blockName);
		break;

	case 0x13:
		block = new CBlockRoom(blockName);
		break;

	case 0x14:
		block = new CBlock0xD(blockName);
		((CBlock0xD *)block)->data.type = 7;
		break;

	case 0x15:
		block = new CBlockRef(blockName);
		break;

	case 0x17:
		block = new CBlock0xD(blockName);
		((CBlock0xD *)block)->data.type = 6;
		break;

	case 0x18:
		block = new CBlockSpace(blockName);
		break;

	case 0x19:
		block = new CBlock0x19(blockName);
		break;

	case 0x1A:
		block = new CBlockContainer0x1A(blockName);
		break;

	case 0x1B:
		block = new CBlockSprite(blockName);
		break;

	case 0x1C:
		block = new CBlock0x1C(blockName);
		break;

	case 0x1D:
		block = new CBlockContainer0x1D(blockName);
		break;

	case 0x1E:
		block = new CBlockRoomDesc(blockName);
		break;

	case 0x1F:
		block = new CBlock0x1F(blockName);
		break;

	case 0x21:
		block = new CBlockContainerLamp(blockName);
		break;

	case 0x22:
		block = new CBlockCollision(blockName);
		break;

	case 0x23:
		block = new CBlockModelElement(blockName);
		((CBlockModelElement*)block)->m_wFlag = 1;
		break;

	case 0x24:
		block = new CBlockContainer0x6(blockName);
		((CBlockContainer0x6 *)block)->flag_0x6_or_0x24 = 1;
		break;

	case 0x25:
		block = new CBlockContainer0x7(blockName);
		((CBlockContainer0x7 *)block)->flag_0x7_or_0x25 = 1;
		break;

	case 0x27:
		block = new CBlockContainerEnv(blockName);
		break;

	case 0x28:
		block = new CBlockGenerator(blockName);
		break;

	case 0x29:
		block = new CBlockRoom(blockName);
		break;
	}

	if (block)
	{
		block->m_pModule = module;

		if (blockName[0])
			ResourceManager::AddBlockToPool(blockName, block);
		if (block->ReadFromFile(f, data1, data2))
			return block;
	}
	return 0;
}

// Loading the RES file using FILE pointer
void GameModule::LoadRES(FILE *f)
{
	while (true)
	{
		char section[128];
		int count = 0;
		char *buf = ReadStringFromFile(f);
		if (!buf)
			break;

		strcpy(section, strtok(buf, " "));
		count = atoi(strtok(0, " "));
		if (count > 0)
		{
			if (!strcmp(section, "PALETTEFILES"))
			{
				for (int i = 0; i < count; i++)
				{
					Palette *plt = new Palette();
					plt->ReadFromFile(f);
					m_pPalettes.push_back(plt);
				}
			}
			else if (!strcmp(section, "SOUNDFILES"))
			{
				for (int i = 0; i < count; i++)
				{
					SoundFile *sound = new SoundFile();
					sound->ReadFromFile(f);
					m_pSoundFiles.push_back(sound);
				}
			}
			else if (!strcmp(section, "BACKFILES"))
			{
				for (int i = 0; i < count; i++)
				{
					Texture *tex = new Texture();
					tex->ReadFromFile(f);
					m_pBackFiles.push_back(tex);
				}
			}
			else if (!strcmp(section, "MASKFILES"))
			{
				for (int i = 0; i < count; i++)
				{
					Mask *mask = new Mask();
					mask->ReadFromFile(f);
					m_pMasks.push_back(mask);
				}
			}
			else if (!strcmp(section, "TEXTUREFILES"))
			{
				for (int i = 0; i < count; i++)
				{
					Texture *tex = new Texture();
					tex->ReadFromFile(f);
					m_pTextures.push_back(tex);
				}
			}
			else if (!strcmp(section, "COLORS"))
			{
				for (int i = 0; i < count; i++)
					ReadStringFromFile(f);
			}
			else if (!strcmp(section, "EFFECTS")) // there is no such type but game try to load
			{
				for (int i = 0; i < count; i++)
				{
					ReadStringFromFile(f);
				}
			}
			else if (!strcmp(section, "SOUNDS"))
			{
				for (int i = 0; i < count; i++)
				{
					Sound *sound = new Sound();
					sound->ReadFromFile(f);
					m_pSounds.push_back(sound);
				}
			}
			else if (!strcmp(section, "MATERIALS"))
			{
				if (m_pMaterials.size() == count)
				{
					// valid
				}
				for (int i = 0; i < count; i++)
				{
					char data[128];
					char *line = ReadStringFromFile(f);
					strcpy(data, line);
					
					char *name = strtok(line, " ");
					auto it = m_pMaterials.begin();
					while (it != m_pMaterials.end())
					{
						if ((*it)->m_sName == name)
						{
							(*it)->Parse(data);
							break;
						}
						it++;
					}
				}
			}
		}
	}
}

void GameModule::Render()
{
	CSimpleBlock *room = m_pRooms;
	CSimpleBlock *block = m_pBlocks;
	char format_str[512];
	int i = 2;
	if (room)
	{
		do
		{
			room->Render();
			sprintf(format_str, "(%.1f, %.1f, %.1f, %.1f) %s", room->m_vPos.pos.x, room->m_vPos.pos.y, room->m_vPos.pos.z, room->m_vPos.len, room->m_sName);
			//ResourceManager::DrawTextString(i / 30 * 250, (i % 30) * 15, D3DCOLOR_XRGB(255, 255, 255), format_str);
			room = (CSimpleBlock *)room->m_pNext;
			i++;
		} while (room);
	}
	if (block)
	{
		do
		{
			sprintf(format_str, "(%.1f, %.1f, %.1f, %.1f) %s", block->m_vPos.pos.x, block->m_vPos.pos.y, block->m_vPos.pos.z, block->m_vPos.len, block->m_sName);
			//ResourceManager::DrawTextString(i / 30 * 250, (i % 30) * 15, D3DCOLOR_XRGB(255, 255, 255), format_str);
			block = (CSimpleBlock *)block->m_pNext;
			//i++;
		} while (block);
	}
}