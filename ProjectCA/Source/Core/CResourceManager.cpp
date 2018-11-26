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

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Title/startgame.bmp"), TEXT("StartButton")))
		return false;

	if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Title/exitgame.bmp"), TEXT("ExitButton")))
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


	//Fonts
	{
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


		//UI Alphabet font(lower)
		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_a.bmp"), TEXT("UIFontLowerA")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_b.bmp"), TEXT("UIFontLowerB")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_c.bmp"), TEXT("UIFontLowerC")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_d.bmp"), TEXT("UIFontLowerD")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_e.bmp"), TEXT("UIFontLowerE")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_f.bmp"), TEXT("UIFontLowerF")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_g.bmp"), TEXT("UIFontLowerG")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_h.bmp"), TEXT("UIFontLowerH")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_i.bmp"), TEXT("UIFontLowerI")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_j.bmp"), TEXT("UIFontLowerJ")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_k.bmp"), TEXT("UIFontLowerK")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_l.bmp"), TEXT("UIFontLowerL")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_m.bmp"), TEXT("UIFontLowerM")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_n.bmp"), TEXT("UIFontLowerN")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_o.bmp"), TEXT("UIFontLowerO")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_p.bmp"), TEXT("UIFontLowerP")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_q.bmp"), TEXT("UIFontLowerQ")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_r.bmp"), TEXT("UIFontLowerR")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_s.bmp"), TEXT("UIFontLowerS")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_t.bmp"), TEXT("UIFontLowerT")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_u.bmp"), TEXT("UIFontLowerU")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_v.bmp"), TEXT("UIFontLowerV")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_w.bmp"), TEXT("UIFontLowerW")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_x.bmp"), TEXT("UIFontLowerX")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_y.bmp"), TEXT("UIFontLowerY")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_lower_z.bmp"), TEXT("UIFontLowerZ")))
			return false;

		
		//UI Alphabet font(upper)
		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_a.bmp"), TEXT("UIFontUpperA")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_b.bmp"), TEXT("UIFontUpperB")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_c.bmp"), TEXT("UIFontUpperC")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_d.bmp"), TEXT("UIFontUpperD")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_e.bmp"), TEXT("UIFontUpperE")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_f.bmp"), TEXT("UIFontUpperF")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_g.bmp"), TEXT("UIFontUpperG")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_h.bmp"), TEXT("UIFontUpperH")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_i.bmp"), TEXT("UIFontUpperI")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_j.bmp"), TEXT("UIFontUpperJ")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_k.bmp"), TEXT("UIFontUpperK")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_l.bmp"), TEXT("UIFontUpperL")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_m.bmp"), TEXT("UIFontUpperM")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_n.bmp"), TEXT("UIFontUpperN")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_o.bmp"), TEXT("UIFontUpperO")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_p.bmp"), TEXT("UIFontUpperP")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_q.bmp"), TEXT("UIFontUpperQ")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_r.bmp"), TEXT("UIFontUpperR")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_s.bmp"), TEXT("UIFontUpperS")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_t.bmp"), TEXT("UIFontUpperT")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_u.bmp"), TEXT("UIFontUpperU")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_v.bmp"), TEXT("UIFontUpperV")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_w.bmp"), TEXT("UIFontUpperW")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_x.bmp"), TEXT("UIFontUpperX")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_y.bmp"), TEXT("UIFontUpperY")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_upper_z.bmp"), TEXT("UIFontUpperZ")))
			return false;

		//UI Outline font(uppercase only)
		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_a.bmp"), TEXT("UIFontOutlineA")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_b.bmp"), TEXT("UIFontOutlineB")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_c.bmp"), TEXT("UIFontOutlineC")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_d.bmp"), TEXT("UIFontOutlineD")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_e.bmp"), TEXT("UIFontOutlineE")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_f.bmp"), TEXT("UIFontOutlineF")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_g.bmp"), TEXT("UIFontOutlineG")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_h.bmp"), TEXT("UIFontOutlineH")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_i.bmp"), TEXT("UIFontOutlineI")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_j.bmp"), TEXT("UIFontOutlineJ")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_k.bmp"), TEXT("UIFontOutlineK")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_l.bmp"), TEXT("UIFontOutlineL")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_m.bmp"), TEXT("UIFontOutlineM")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_n.bmp"), TEXT("UIFontOutlineN")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_o.bmp"), TEXT("UIFontOutlineO")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_p.bmp"), TEXT("UIFontOutlineP")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_q.bmp"), TEXT("UIFontOutlineQ")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_r.bmp"), TEXT("UIFontOutlineR")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_s.bmp"), TEXT("UIFontOutlineS")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_t.bmp"), TEXT("UIFontOutlineT")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_u.bmp"), TEXT("UIFontOutlineU")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_v.bmp"), TEXT("UIFontOutlineV")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_w.bmp"), TEXT("UIFontOutlineW")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_x.bmp"), TEXT("UIFontOutlineX")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_y.bmp"), TEXT("UIFontOutlineY")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_outline_z.bmp"), TEXT("UIFontOutlineZ")))
			return false;


		//UI Font etc
		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_em.bmp"), TEXT("UIFontEM")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_qm.bmp"), TEXT("UIFontQM")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_sharp.bmp"), TEXT("UIFontSharp")))
			return false;

		if (!AddSpriteUsePath(TEXT("./Resources/Sprite/UI/Font/ui_font_line.bmp"), TEXT("UIFontLine")))
			return false;
				
	}

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