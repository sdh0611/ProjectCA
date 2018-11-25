#include "..\..\stdafx.h"
#include "..\..\Include\Core\CResourceManager.h"
#include "..\..\Include\Core\Graphic\CSprite.h"


CResourceManager::CResourceManager() {

}

CResourceManager::~CResourceManager() {

	m_strongSpriteTable.clear();

}

bool CResourceManager::Init() {

	//if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/mario_big_idle_left.bmp"), TEXT("PlayerIdleLeft")))
	//	return false;

	if (!LoadPlayerSpriteInfo())
		return false;

	if (!LoadEnemySpriteInfo())
		return false;

	if (!LoadGroundTileImageInfo())
		return false;

	if (!LoadBackgroundImageInfo())
		return false;

	if (!LoadUIImageInfo())
		return false;

	if (!LoadPickupImageInfo())
		return false;

	if (!LoadBlockImageInfo())
		return false;


	return true;
}


WeakSpritePtr CResourceManager::GetWeakSprtiePtr(const Types::tstring & strName)
{
	//없는 경우 nullptr 반환
	if(m_strongSpriteTable.find(strName) == m_strongSpriteTable.end())
		return WeakSpritePtr();

	//있는 경우 해당 Sprite의 포인터 반환
	return m_strongSpriteTable[strName];
}

bool CResourceManager::AddSpritePtr(const Types::tstring & strName, CSprite * pSprite)
{
	if (pSprite == nullptr)
		return false;

	//해당 키값에 대응하는 sprite가 이미 존재하는 경우
	if(m_strongSpriteTable.find(strName) != m_strongSpriteTable.end())
		return false;

	StrongSpritePtr pStrongSprite = std::shared_ptr<CSprite>(pSprite);
	m_strongSpriteTable.insert(std::make_pair(strName, pStrongSprite));
	
	return true;
}

bool CResourceManager::DeleteSpritePtr(const Types::tstring & strName)
{
	//이미 테이블에 존재하지 않는 경우
	if (m_strongSpriteTable.find(strName) == m_strongSpriteTable.end())
		return false;

	m_strongSpriteTable.erase(strName);

	return true;
}

bool CResourceManager::AddSpriteUsePath(const Types::tstring & strPath, const Types::tstring & strName)
{
	//이미 존재하는 경우
	if (m_strongSpriteTable.find(strName) != m_strongSpriteTable.end())
		return false;

	StrongSpritePtr pSprite = StrongSpritePtr(new CSprite);

	if(!pSprite->Init(strPath))
		return false;
	
	m_strongSpriteTable.insert(std::make_pair(strName, pSprite));

	return true;

}

bool CResourceManager::LoadPlayerSpriteInfo()
{
	//Small state image
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_idle_right.bmp"), TEXT("PlayerSmallIdleRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_idle_left.bmp"), TEXT("PlayerSmallIdleLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_walk_right.bmp"), TEXT("PlayerSmallWalkRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_walk_left.bmp"), TEXT("PlayerSmallWalkLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_lookup_right.bmp"), TEXT("PlayerSmallLookupRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_lookup_left.bmp"), TEXT("PlayerSmallLookupLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_sitdown_right.bmp"), TEXT("PlayerSmallSitdownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_sitdown_left.bmp"), TEXT("PlayerSmallSitdownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_run_right.bmp"), TEXT("PlayerSmallRunRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_run_left.bmp"), TEXT("PlayerSmallRunLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_jump_right.bmp"), TEXT("PlayerSmallJumpRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_jump_left.bmp"), TEXT("PlayerSmallJumpLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_falldown_right.bmp"), TEXT("PlayerSmallFalldownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_falldown_left.bmp"), TEXT("PlayerSmallFalldownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_run_jump_right.bmp"), TEXT("PlayerSmallRunJumpRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_run_jump_left.bmp"), TEXT("PlayerSmallRunJumpLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_turn_right.bmp"), TEXT("PlayerSmallTurnRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_small_turn_left.bmp"), TEXT("PlayerSmallTurnLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_dead_image.bmp"), TEXT("PlayerDeadImage")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Small/mario_dead_animation.bmp"), TEXT("PlayerDeadAnimation")))
		return false;


	//Big state image
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_idle_right.bmp"), TEXT("PlayerBigIdleRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_idle_left.bmp"), TEXT("PlayerBigIdleLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_walk_right.bmp"), TEXT("PlayerBigWalkRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_walk_left.bmp"), TEXT("PlayerBigWalkLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_lookup_right.bmp"), TEXT("PlayerBigLookupRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_lookup_left.bmp"), TEXT("PlayerBigLookupLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_sitdown_right.bmp"), TEXT("PlayerBigSitdownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_sitdown_left.bmp"), TEXT("PlayerBigSitdownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_run_right.bmp"), TEXT("PlayerBigRunRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_run_left.bmp"), TEXT("PlayerBigRunLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_jump_right.bmp"), TEXT("PlayerBigJumpRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_jump_left.bmp"), TEXT("PlayerBigJumpLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_falldown_right.bmp"), TEXT("PlayerBigFalldownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_falldown_left.bmp"), TEXT("PlayerBigFalldownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_run_jump_right.bmp"), TEXT("PlayerBigRunJumpRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_run_jump_left.bmp"), TEXT("PlayerBigRunJumpLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_turn_right.bmp"), TEXT("PlayerBigTurnRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Big/mario_big_turn_left.bmp"), TEXT("PlayerBigTurnLeft")))
		return false;


	//Flower state image
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_idle_right.bmp"), TEXT("PlayerFlowerIdleRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_idle_left.bmp"), TEXT("PlayerFlowerIdleLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_walk_right.bmp"), TEXT("PlayerFlowerWalkRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_walk_left.bmp"), TEXT("PlayerFlowerWalkLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_lookup_right.bmp"), TEXT("PlayerFlowerLookupRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_lookup_left.bmp"), TEXT("PlayerFlowerLookupLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_sitdown_right.bmp"), TEXT("PlayerFlowerSitdownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_sitdown_left.bmp"), TEXT("PlayerFlowerSitdownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_run_right.bmp"), TEXT("PlayerFlowerRunRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_run_left.bmp"), TEXT("PlayerFlowerRunLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_jump_right.bmp"), TEXT("PlayerFlowerJumpRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_jump_left.bmp"), TEXT("PlayerFlowerJumpLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_falldown_right.bmp"), TEXT("PlayerFlowerFalldownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_falldown_left.bmp"), TEXT("PlayerFlowerFalldownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_run_jump_right.bmp"), TEXT("PlayerFlowerRunJumpRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_run_jump_left.bmp"), TEXT("PlayerFlowerRunJumpLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_turn_right.bmp"), TEXT("PlayerFlowerTurnRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_turn_left.bmp"), TEXT("PlayerFlowerTurnLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_attack_right.bmp"), TEXT("PlayerFlowerAttackRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_attack_left.bmp"), TEXT("PlayerFlowerAttackLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_jump_attack_right.bmp"), TEXT("PlayerFlowerJumpAttackRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/New/Flower/mario_flower_jump_attack_left.bmp"), TEXT("PlayerFlowerJumpAttackLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/fireball_right.bmp"), TEXT("FireballRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Player/fireball_Left.bmp"), TEXT("FireballLeft")))
		return false;


	return true;
}

bool CResourceManager::LoadEnemySpriteInfo()
{
	//Koopa
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Enemy/Koopa/koopa_green_walk_right.bmp"), TEXT("KoopaGreenWalkRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Enemy/Koopa/koopa_green_walk_left.bmp"), TEXT("KoopaGreenWalkLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Enemy/Koopa/koopa_green_thrown_right.bmp"), TEXT("KoopaGreenThrownRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Enemy/Koopa/koopa_green_thrown_left.bmp"), TEXT("KoopaGreenThrownLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Enemy/Koopa/koopa_green_shell.bmp"), TEXT("KoopaGreenShell")))
		return false;


	//Goomba
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Enemy/Goomba/goomba_walk_right.bmp"), TEXT("GoombaWalkRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Enemy/Goomba/goomba_walk_left.bmp"), TEXT("GoombaWalkLeft")))
		return false;


	return true;
}

bool CResourceManager::LoadGroundTileImageInfo()
{
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_edge_left_bot.bmp"), TEXT("GroundEdgeLeftBot")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_edge_left_top.bmp"), TEXT("GroundEdgeLeftTop")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_edge_left_inner.bmp"), TEXT("GroundEdgeLeftInner")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_edge_right_bot.bmp"), TEXT("GroundEdgeRightBot")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_edge_right_top.bmp"), TEXT("GroundEdgeRightTop")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_edge_right_inner.bmp"), TEXT("GroundEdgeRightInner")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_line_bot.bmp"), TEXT("GroundLineBot")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_line_right.bmp"), TEXT("GroundLineRight")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_line_top.bmp"), TEXT("GroundLineTop")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_line_left.bmp"), TEXT("GroundLineLeft")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Tiles/ground_inner.bmp"), TEXT("GroundInner")))
		return false;


	return true;
}

bool CResourceManager::LoadBackgroundImageInfo()
{
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Background/background_mountain1.bmp"), TEXT("BackgroundMountain1")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Background/background_mountain2.bmp"), TEXT("BackgroundMountain2")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Title/title.bmp"), TEXT("Title")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Title/logo.bmp"), TEXT("Logo")))
		return false;


	return true;
}

bool CResourceManager::LoadUIImageInfo()
{
	//UI TestButton
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Title/button.bmp"), TEXT("TestButton")))
		return false;


	//UI Item info
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_item_empty.bmp"), TEXT("UIItemInfoEmpty")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_item_mushroom.bmp"), TEXT("UIItemInfoMushroom")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_item_flower.bmp"), TEXT("UIItemInfoFlower")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_item_star.bmp"), TEXT("UIItemInfoStar")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_item_feather.bmp"), TEXT("UIItemInfoFeather")))
		return false;



	//UI Number font
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_0.bmp"), TEXT("UINumberWhite0")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_1.bmp"), TEXT("UINumberWhite1")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_2.bmp"), TEXT("UINumberWhite2")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_3.bmp"), TEXT("UINumberWhite3")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_4.bmp"), TEXT("UINumberWhite4")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_5.bmp"), TEXT("UINumberWhite5")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_6.bmp"), TEXT("UINumberWhite6")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_7.bmp"), TEXT("UINumberWhite7")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_8.bmp"), TEXT("UINumberWhite8")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_white_9.bmp"), TEXT("UINumberWhite9")))
		return false;



	//UI Time number font
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_0.bmp"), TEXT("UINumberYellow0")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_1.bmp"), TEXT("UINumberYellow1")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_2.bmp"), TEXT("UINumberYellow2")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_3.bmp"), TEXT("UINumberYellow3")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_4.bmp"), TEXT("UINumberYellow4")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_5.bmp"), TEXT("UINumberYellow5")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_6.bmp"), TEXT("UINumberYellow6")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_7.bmp"), TEXT("UINumberYellow7")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_8.bmp"), TEXT("UINumberYellow8")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_number_yellow_9.bmp"), TEXT("UINumberYellow9")))
		return false;



	//UI Etc
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_mario.bmp"), TEXT("UIMario")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_time.bmp"), TEXT("UITime")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_stars.bmp"), TEXT("UIStars")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_x.bmp"), TEXT("UIX")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Game/ui_coin.bmp"), TEXT("UICoin")))
		return false;

	return true;
}

bool CResourceManager::LoadPickupImageInfo()
{
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Pickup/mushroom.bmp"), TEXT("Mushroom")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Pickup/mushroom_green.bmp"), TEXT("MushroomGreen")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Pickup/flower_animation.bmp"), TEXT("FlowerAnimation")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Pickup/flower_image.bmp"), TEXT("FlowerImage")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Pickup/coin_animation.bmp"), TEXT("CoinAnimation")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Pickup/coin_image.bmp"), TEXT("CoinImage")))
		return false;

	return true;
}

bool CResourceManager::LoadBlockImageInfo()
{
	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Block/block_random_image.bmp"), TEXT("RandomBlockImage")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Block/block_random_animation.bmp"), TEXT("RandomBlockAnimation")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Block/block_random_hit_animation.bmp"), TEXT("RandomBlockHit")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/Block/block_random_dead_image.bmp"), TEXT("RandomBlockDead")))
		return false;



	return true;
}

bool CResourceManager::LoadEffectImageInfo()
{
	return true;
}