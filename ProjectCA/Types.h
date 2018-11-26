#pragma once

#include "stdafx.h"

#define MAKE_INPUT			0b00001
#define MAKE_PHYSICS		0b00010
#define MAKE_COL_BOX		0b00100
#define MAKE_COL_CIR		0b01000

#define TYPE_ACTION


#define STATIC_POINTER_CAST(type, value)		std::tr1::static_pointer_cast<type>(value)
#define DYNAMIC_POINTER_CAST(type, value)		std::tr1::dynamic_pointer_cast<T>(value)

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

	////Object 타입 정의
	//enum ObjectType { OT_ACTOR, OT_UI };

	//UI 타입 정의
	enum UIType { UT_BUTTON, UT_FONT };

	//Actor 타입 정의
	//enum ActorType { AT_PLAYER, AT_ENEMY, AT_PROB, AT_PICKUP, AT_BULLET, AT_BACKGROUND };

	enum ObjectType { OT_PLAYER, OT_ENEMY, OT_PROB, OT_PICKUP, OT_BULLET, OT_BACKGROUND, OT_BLOCK, OT_UI };
#ifndef TYPE_ACTION 
	//오브젝트 상태 정의
	enum ActorState { AS_IDLE, AS_MOVE, AS_ATTACK, AS_DAMAGED, AS_DEAD };
#else
	//Actor의 상태를 나타내기 위한 열거체 (10.13)
	enum ObjectState { OS_IDLE, OS_MOVE, OS_SITDOWN, OS_LOOKUP, OS_ATTACK, OS_DAMAGED, OS_PROTECTED, OS_DEAD };

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
	enum SceneType { ST_TITLE, ST_GAME, ST_SELECT, ST_GAMEOVER, ST_LOADING };

	//EventType 정의
	enum EventType { ET_ATTACK, ET_DAMAGE };

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

	struct ObjectData {
		class CGameScene;

		UINT			m_iObjectWidth;
		UINT			m_iObjectHeight;
		Point			m_ObjectPoint;
		tstring		m_strObjectName;
		ObjectType	m_ObjectType;
		ObjectState m_ObjectState;

		ObjectData(UINT iWidth, UINT iHeight, Point point, const tstring& strName, 
			ObjectType type,ObjectState state)
			: m_iObjectWidth(iWidth), m_iObjectHeight(iHeight), m_ObjectPoint(point),
			m_strObjectName(strName), m_ObjectType(type), m_ObjectState(state)
		{
		}

		ObjectData& operator=(const ObjectData& other)
		{
			m_iObjectWidth		= other.m_iObjectWidth;
			m_iObjectHeight		= other.m_iObjectHeight;
			m_ObjectPoint			= other.m_ObjectPoint;
			m_strObjectName		= other.m_strObjectName;
			m_ObjectType			= other.m_ObjectType;
			m_ObjectState			= other.m_ObjectState;
		}


	};

	struct ActorData : public ObjectData {
		class CGameScene;

		ObjectType				m_ActorType;
		ObjectState				m_ActorState;
		VerticalState			m_VerticalState;
		HorizonalState			m_HorizonalState;
		Direction					m_Direction;
		ActorID					m_iActorID;

		ActorData(UINT iWidth, UINT iHeight, Point point, ObjectType type, ObjectState state,
			VerticalState vertical, HorizonalState horizonal, Direction dir,
			ActorID id, const tstring& strName)
			:ObjectData(iWidth, iHeight, point, strName, type, state),
			m_ActorType(type), m_ActorState(state), m_VerticalState(vertical), m_HorizonalState(horizonal),
			m_Direction(dir), m_iActorID(id)
		{			
		}

		ActorData& operator=(const ActorData& other) {
			m_iObjectWidth		= other.m_iObjectWidth;
			m_iObjectHeight		= other.m_iObjectHeight;
			m_ObjectPoint			= other.m_ObjectPoint;
			m_strObjectName		= other.m_strObjectName;
			m_ActorType			= other.m_ActorType;
			m_ActorState			= other.m_ActorState;
			m_VerticalState			= other.m_VerticalState;
			m_HorizonalState		= other.m_HorizonalState;
			m_Direction				= other.m_Direction;
			m_iActorID				= other.m_iActorID;
		}
		//CWorld*					m_pOwnerWorld;
	};
	

}

typedef Types::tstring					TSTRING;
typedef Types::ObjectType			OBEJCT_TYPE;
typedef Types::UIType				UI_TYPE;
typedef Types::ObjectType			OBJECT_TYPE;
typedef Types::Point					POSITION;
typedef Types::ObjectData			OBJECT_DATA;
typedef Types::ActorData			ACTOR_DATA;
typedef Types::ObjectState			OBJECT_STATE;
typedef Types::VerticalState			VER_STATE;
typedef Types::HorizonalState		HOR_STATE;
typedef Types::Direction				DIRECTION;
typedef Types::AnimationMotion	ANIM_MOTION;