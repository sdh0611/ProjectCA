#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CTile.h"
#include "..\..\Include\Core\CResourceManager.h"


CTile::CTile()
{
}

CTile::~CTile()
{
}

bool CTile::PostInit(const Types::tstring& strImage)
{
	SetTileImage(strImage);

	m_iTileWidth = TILE_WIDTH;
	m_iTileHeight = TILE_HEIGHT;

	return true;
}

void CTile::Init()
{
	m_iTileWidth = TILE_WIDTH;
	m_iTileHeight = TILE_HEIGHT;

}

void CTile::SetTileWidth(UINT iWidth)
{
	m_iTileWidth = iWidth;
}

void CTile::SetTileHeight(UINT iHeight)
{
	m_iTileHeight = iHeight;
}

void CTile::SetTileSize(UINT iWidth, UINT iHeight)
{
	m_iTileWidth = iWidth;
	m_iTileHeight = iHeight;
}

bool CTile::SetTileImage(const Types::tstring & strImage)
{
	auto pImage = CResourceManager::GetInstance()->GetWeakSprtiePtr(strImage);

	if (pImage.expired())
		return false;

	m_pTileImage = pImage.lock();

	return true;
}

UINT CTile::GetTileWidth()
{
	return m_iTileWidth;
}

UINT CTile::GetTileHeight()
{
	return m_iTileHeight;
}
