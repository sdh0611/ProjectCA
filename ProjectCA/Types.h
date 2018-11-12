#pragma once

#include "stdafx.h"

#define MAKE_INPUT			0b00001
#define MAKE_PHYSICS		0b00010
#define MAKE_COL_BOX		0b00100
#define MAKE_COL_CIR		0b01000

#define TYPE_ACTION

const unsigned int MAX_WIDTH			= 1024;
const unsigned int MAX_HEIGHT			= 720;
const unsigned int MAX_ACTOR_SIZE	= 3000;

const unsigned int SPRITE_WIDTH		= 32;
const unsigned int SPRITE_HEIGHT		= 32;

const unsigned int TILE_WIDTH = 32;
const unsigned int TILE_HEIGHT = 32;


namespace Types {

	typedef std::basic_string<TCHAR> tstring;

	typedef unsigned long ActorID;
	typedef unsigned long CameraID;

	//오브젝트 타입 정의
	enum ActorType { AT_PLAYER, AT_ENEMY, AT_PROB, AT_PICKUP, AT_MAP, AT_BACKGROUND };
#ifndef TYPE_ACTION 
	//오브젝트 상태 정의
	enum ActorState { AS_IDLE, AS_MOVE, AS_ATTACK, OS_DAMAGED, OS_DEAD };
#else
	//Actor의 상태를 나타내기 위한 열거체 (10.13)
	enum ActorState { AS_IDLE, AS_MOVE, AS_SITDOWN, AS_LOOKUP, AS_ATTACK, AS_DAMAGED, AS_DEAD };

	//수직상의 이동 상태를 구분하기 위한 열거체 (10.13)
	enum VerticalState { VS_IDLE, VS_JUMP, VS_FALL };

	//수평상의 이동 상태를 구분하기 위한 열거체 (10.13)
	enum HorizonalState{ HS_IDLE, HS_WALK, HS_RUN, HS_TURN };
#endif
	//Animation관련 
	enum AnimationMotion { AM_IDLE, AM_WALK, AM_RUN, AM_TURN, AM_LOOKUP, AM_SITDOWN 
		,AM_JUMP, AM_FALL, AM_ATTACK, AM_DAMAGED, AM_DEAD};

	//방향 열거체 정의
	enum Direction { DIR_DOWN = -1, DIR_IDLE, DIR_UP, DIR_LEFT, DIR_RIGHT };

	//Scene 타입 정의
	enum SceneType { ST_TITLE, ST_GAME, ST_SELECT };

	//EventType 정의
	enum EventType { ET_ATTACK, ET_DAMAGE };

	//KeyType 정의
	//enum KeyType { KT_LEFT, KT_RIGHT, KT_UP, KT_DOWN, KT_JUMP, KT_FIRE1, KT_FIRE2 };

	struct KeyInfo {
		KeyInfo(const Types::tstring& strKeyname, SHORT iKeyCode, bool bPressed = false)
			:m_strKeyName(strKeyname), m_iKeyCode(iKeyCode), m_bPressed(bPressed)
		{
		}

		KeyInfo(KeyInfo&& other)
			:m_strKeyName(std::move(other.m_strKeyName)), m_iKeyCode(other.m_iKeyCode), m_bPressed(other.m_bPressed)
		{
		}

		bool operator ==(const KeyInfo& other)
		{
			if (m_strKeyName == other.m_strKeyName && m_iKeyCode == other.m_iKeyCode
				&& m_bPressed == other.m_bPressed)
				return true;

			return false;
		}

		KeyInfo& operator =(const KeyInfo& other) 
		{
			if (*this == other)
				return *this;

			this->m_strKeyName = other.m_strKeyName;
			this->m_iKeyCode = other.m_iKeyCode;
			this->m_bPressed = other.m_bPressed;

			return *this;
		}

		Types::tstring	m_strKeyName;
		SHORT			m_iKeyCode;
		bool				m_bPressed;
	};


	//Point구조체 정의
	struct Point {

		float x, y;

		Point(float nx = 0.f, float ny = 0.f) :x(nx), y(ny) {		}

		Point(const Point& other) = default;

		~Point() { };

		bool operator ==(const Point& other) {
			if (this->x == other.x && this->y == other.y)
				return true;

			return false;
		}

		Point operator +(const Point& other) {
			return Point(this->x + other.x, this->y + other.y);
		}

		Point operator -(const Point& other) {
			return Point(this->x - other.x, this->y - other.y);
		}

		Point operator *(const Point& other) {
			return Point(this->x * other.x, this->y * other.y);
		}

		Point& operator =(const Point& other) {
			if (*this == other)
				return *this;

			this->x = other.x;
			this->y = other.y;

			return *this;
		}

	};
	   
	

	//Rect구조체 정의
	struct Rect {
		float left;
		float top;
		float right;
		float bottom;

		Rect(float _left, float _top, float _right, float _bottom) 
		{
			left = _left;
			top = _top;
			right = _right;
			bottom = _bottom;
		}
		~Rect(){ }

		Rect(const Rect& other) :
			Rect(other.left, other.top, other.right, other.bottom)
		{	}

		bool operator ==(const Rect& other) {
			if ( (left == other.left) && (top = other.top) 
				&& (right == other.right) && (bottom = other.bottom) )
				return true;

			return false;
		}

		Rect operator +(const Rect& other) {
			return Rect(other.left + left, other.top + top, other.right + right, other.bottom + bottom);
		}

		Rect operator *(const Rect& other) {
			return Rect(other.left * left, other.top * top, other.right * right, other.bottom * bottom);
		}

		Rect operator -(const Rect& other) {
			return Rect(left - other.left, top - other.top, right - other.right, bottom - other.bottom);
		}

		Rect& operator =(const Rect& other) {
			if (*this == other)
				return *this;

			left = other.left;
			top = other.top;
			right = other.right;
			bottom = other.bottom;

			return *this;
		}

	};

	struct ActorData {
		class CGameScene;

		ActorData(UINT iWidth, UINT iHeight, Point point, ActorType type, ActorState state, 
			VerticalState vertical, HorizonalState horizonal, Direction dir, 
			ActorID id, const tstring& strTag, bool _bActive)
			:iActorWidth(iWidth), iActorHeight(iHeight), actorPoint(point), actorType(type), actorState(state),
			verticalState(vertical), horizonalState(horizonal), direction(dir), 
			actorID(id), strActorTag(strTag),  bActive(_bActive)
		{			
		}

		ActorData& operator=(const ActorData& other) {

			iActorWidth = other.iActorWidth;
			iActorHeight = other.iActorHeight;
			actorPoint = other.actorPoint;
			actorType = other.actorType;
			actorState = other.actorState;
			verticalState = other.verticalState;
			horizonalState = other.horizonalState;
			direction = other.direction;
			strActorTag = other.strActorTag;
			bActive = other.bActive;

		}

		UINT						iActorWidth;
		UINT						iActorHeight;
		Point						actorPoint;
		ActorType				actorType;
		ActorState				actorState;
		VerticalState			verticalState;
		HorizonalState			horizonalState;
		Direction					direction;
		ActorID					actorID;
		tstring					strActorTag;
		//CWorld*					m_pOwnerWorld;
		bool						bActive;


	};
	

}

typedef Types::tstring TSTRING;
typedef Types::Point POSITION;
typedef Types::ActorData ACTORDATA;
