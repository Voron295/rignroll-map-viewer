#ifndef GAMEMODULE_CLASS_H
#define GAMEMODULE_CLASS_H

#include <Windows.h>
#include <iostream>
#include "Material.h"
#include "Palette.h"
#include "Sound.h"
#include "Ovl.h"
#include "Color.h"
#include "Effect.h"
//#include "CBlockRoom.h"
//#include "CBlockSpace.h"
#include "CVector.h"
#include "CMatrix.h"
#include "Vertex.h"
#include "CVector4.h"
#include <vector>

class GameModule
{
public:
	std::string m_sName;

	// Props
	std::vector<Material *>m_pMaterials;
	std::vector<Texture *>m_pTextures;
	std::vector<Palette *>m_pPalettes;
	std::vector<Texture *>m_pBackFiles;
	std::vector<Sound *>m_pSounds;
	std::vector<SoundFile *>m_pSoundFiles;
	//std::vector<Color *>m_pColors;
	std::vector<Mask *>m_pMasks;
	std::vector<Effect *>m_pEffects;


	// B3D Props
	class CSimpleBlock *m_pBlocks;
	class CSimpleBlock *m_pRooms;
	class CBlockObserver *m_pObserver;


	// Functions
	void LoadB3D(FILE *f);
	static CSimpleBlock *LoadBlock(FILE *f, GameModule *module, int &blockType, void *data1, void *data2);
	void LoadRES(FILE *f);
	void Load(const char *path, const char *name);
	class CSimpleBlock *AddToPool(class CSimpleBlock *block);
	void InitPrimitiveData();
	static int InitPrimitiveDataCb(CSimpleBlock *block);

	void Render();

	static DWORD ReadIntFromFile(FILE *f);
	static float ReadFloatFromFile(FILE *f);
	static void ReadVectorFromFile(FILE *f, CVector *v);
	static void ReadVector4FromFile(FILE *f, CVector4 *v);
	static void ReadMatrixFromFile(FILE *f, CMatrix *m);
	static void ReadVertexFromFile(FILE *f, Vertex *v);
	static char *ReadStringFromFile(FILE *f, int size = 0);

	static void WriteIntToFile(FILE *f, DWORD val);
	static void WriteFloatToFile(FILE *f, float val);
	static void WriteVectorToFile(FILE *f, CVector *v);
	static void WriteVector4ToFile(FILE *f, CVector4 *v);
	static void WriteMatrixToFile(FILE *f, CMatrix *m);
	static void WriteVertexToFile(FILE *f, Vertex *v);
	static void WriteStringToFile(FILE *f, char *str, int size = 0);

	GameModule();
	~GameModule();
};

#endif