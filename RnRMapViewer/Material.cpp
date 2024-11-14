#include "GameModule.h"

Material::Material()
{
	m_dwFlags = 0;
	m_dwInternalId = -1;
	m_dwPar = -1;
	m_dwColorId = 0;
	m_fTransparency = 0.0;
	m_pTexture = 0;
	m_dwInternalTexCoordsId = 0;
	m_fRotation = 0.0;
	m_fMoveY = 0.0;
	m_fMoveX = 0.0;
	env2 = 1.0;
	env1 = 1.0;
	m_dwTextureId = 0;
	m_dwMaskId = 0;
	m_fSpecular = 0.0;
	m_dwPower = 0;
	m_fReflection = 0.0;
	someColor = 0;
	m_dwEnvId = 0;
}

Material::~Material()
{

}

bool Material::Parse(char *data)
{
	char *name = strtok(data, " ");
	while (true)
	{
		char *param = strtok(0, " ");
		if (!param)
			break;

		if (!strcmp(param, "col"))
		{
			m_dwColorId = atoi(strtok(0, " ")) - 1;
		}
		else if (!strcmp(param, "tex"))
		{
			m_dwFlags |= MATERIAL_HAS_TEXTURE;
			m_dwTextureId = atoi(strtok(0, " ")) - 1;
		}
		else if (!strcmp(param, "ttx"))
		{
			m_dwFlags |= MATERIAL_HAS_TTX;
			m_dwTextureId = atoi(strtok(0, " ")) - 1;
		}
		else if (!strcmp(param, "itx"))
		{
			m_dwFlags |= MATERIAL_HAS_ITX;
			m_dwTextureId = atoi(strtok(0, " ")) - 1;
		}
		else if (!strcmp(param, "msk"))
		{
			m_dwFlags |= MATERIAL_HAS_MASK;
			m_dwMaskId = atoi(strtok(0, " ")) - 1;
		}
		else if (!strcmp(param, "transp"))
		{
			m_dwFlags |= MATERIAL_IS_TRANSPARENT;
			m_fTransparency = 1.0f - atof(strtok(0, " "));
		}
		else if (!strcmp(param, "specular"))
		{
			char *spec = strtok(0, " ");
			if (spec)
				m_fSpecular = atof(spec);
		}
		else if (!strcmp(param, "reflect"))
		{
			char *refl = strtok(0, " ");
			if (refl)
				m_fReflection = atof(refl);
			m_dwFlags |= MATERIAL_HAS_REFLECTION;
		}
		else if (!strcmp(param, "alphamirr"))
		{
			m_dwFlags |= MATERIAL_HAS_ALPHAMIR;
		}
		else if (!strcmp(param, "wave"))
		{
			m_dwFlags |= MATERIAL_HAS_WAVE;
		}
		else if (!strcmp(param, "bumpcoord"))
		{
			m_dwFlags |= MATERIAL_HAS_BUMP;
		}
		else if (!strcmp(param, "power"))
		{
			char *power = strtok(0, " ");
			if (power)
				m_dwPower = atoi(power);
		}
		else if (!strcmp(param, "usecol"))
		{
			m_dwFlags |= MATERIAL_USE_COL;
		}
		else if (!strcmp(param, "env"))
		{
			char *env1s = strtok(0, " ");
			if (env1s)
				env1 = atof(env1s);

			char *env2s = strtok(0, " ");
			if (env2s)
				env2 = atof(env2s);

			m_dwEnvId = 0;
			m_dwFlags |= MATERIAL_ENV;
		}
		else if (!strncmp(param, "env", 3))
		{
			if (param[3])
				m_dwEnvId = param[3] - '0';
			else
				m_dwEnvId = 0;

			char *env1s = strtok(0, " ");
			if (env1s)
				env1 = atof(env1s);

			char *env2s = strtok(0, " ");
			if (env2s)
				env2 = atof(env2s);

			m_dwFlags |= MATERIAL_ENV;
		}
		else if (!strcmp(param, "coord"))
		{
			char *coord = strtok(0, " ");
			if (coord)
				m_dwInternalTexCoordsId = atoi(coord) - 1;
		}
		else if (!strcmp(param, "att"))
		{
			char *att = strtok(0, " ");
			if (att)
				m_dwInternalId = atoi(att) - 1;

			m_dwFlags |= MATERIAL_ATT;
		}
		else if (!strcmp(param, "par"))
		{
			char *par = strtok(0, " ");
			if (par)
				m_dwPar = atoi(par) - 1;
		}
		else if (!strcmp(param, "move"))
		{
			m_dwFlags |= MATERIAL_MOVE;
			m_fMoveX = atof(strtok(0, " "));
			m_fMoveY = atof(strtok(0, " "));
		}
		else if (!strcmp(param, "RotPoint"))
		{
			m_fRotationPointX = atof(strtok(0, " "));
			m_fRotationPointY = atof(strtok(0, " "));
		}
		else if (!strcmp(param, "rot"))
		{
			m_dwFlags |= MATERIAL_HAS_ROTATION;
			m_fRotation = atof(strtok(0, " ")) * 0.01745329222222222;
		}
		else if (!strcmp(param, "noz"))
		{
			m_dwFlags |= MATERIAL_NOZ;
		}
		else if (!strcmp(param, "nof"))
		{
			m_dwFlags |= MATERIAL_NOF;
		}
		else if (!strcmp(param, "notile"))
		{
			m_dwFlags |= MATERIAL_NOTILE;
		}
		else if (!strcmp(param, "notileu"))
		{
			m_dwFlags |= MATERIAL_NOTILEU;
		}
		else if (!strcmp(param, "notilev"))
		{
			m_dwFlags |= MATERIAL_NOTILEV;
		}
	}
	return true;
}

void Material::InitDirect3D(GameModule *module)
{
	if (m_dwFlags & MATERIAL_HAS_TEXTURE && m_dwTextureId >= 0 && m_dwTextureId < module->m_pTextures.size())
	{
		m_pTexture = module->m_pTextures[m_dwTextureId];
		m_pTexture->InitDirect3D();
	}
	if (m_dwFlags & MATERIAL_HAS_TEXTURE && m_dwMaskId >= 0 && m_dwMaskId < module->m_pMasks.size())
	{
		m_pMask = module->m_pMasks[m_dwMaskId];
	}
	m_pMaterial3D.Diffuse = { 1.0f, 1.0f, 1.0f, m_fTransparency };
	m_pMaterial3D.Ambient = { .6f, .6f, .6f, .6f };
	m_pMaterial3D.Specular = { m_fSpecular, m_fSpecular, m_fSpecular, 1.0f };
	m_pMaterial3D.Emissive = { .0f, .0f, .0f, 1.0f };
	m_pMaterial3D.Power = 1.0f;
}