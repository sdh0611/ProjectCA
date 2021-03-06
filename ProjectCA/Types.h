#pragma once

#include "stdafx.h"

#define MAKE_INPUT			0b00001
#define MAKE_PHYSICS		0b00010
#define MAKE_COL_BOX		0b00100
#define MAKE_COL_CIR		0b01000

#define TYPE_ACTION


#define STATIC_POINTER_CAST(type, value)		std::tr1::static_pointer_cast<type>(value)
#define DYNAMIC_POINTER_CAST(type, value)		std::tr1::dynamic_pointer_cast<T>(value)

const unsigned int MAX_WIDTH = 1024;
const unsigned int MAX_HEIGHT = 720;
const unsigned int MAX_ACTOR_SIZE = 3000;

const unsigned int SPRITE_WIDTH = 32;
const unsigned int SPRITE_HEIGHT = 32;

const unsigned int TILE_WIDTH = 32;
const unsigned int TILE_HEIGHT = 32;

const unsigned int FONT_SIZE = 8;


namespace Types {

	typedef std::basic_string<TCHAR> tstring;

	typedef unsigned long EntityID;
	typedef unsigned long CameraID;

	////Object 타입 정의
	//enum ObjectType { OT_ACTOR, OT_UI };

	//UI 타입 정의
	enum UIType { UT_BUTTON, UT_FONT };

	//Actor 타입 정의
	//enum ActorType { AT_PLAYER, AT_ENEMY, AT_PROB, AT_PICKUP, AT_BULLET, AT_BACKGROUND };

	enum ObjectType {
		OT_PLAYER, OT_ENEMY, OT_PROB, OT_WALL, OT_GROUND,
		OT_PICKUP, OT_BULLET, OT_BLOCK, OT_PICKABLE, OT_UI
	};
#ifndef TYPE_ACTION 
	//오브젝트 상태 정의
	enum ActorState { AS_IDLE, AS_MOVE, AS_ATTACK, AS_DAMAGED, AS_DEAD };
#else
	//Objectr의 상태를 나타내기 위한 열거체 (10.13)
	enum ObjectState { OS_IDLE, OS_DAMAGED, OS_DEAD, OS_DESTROYED };

	//Actor들의 동작에 대한 열거체
	enum ActType { ACT_IDLE, ACT_MOVE, ACT_SITDOWN, ACT_LOOKUP, ACT_ATTACK, ACT_DESTROY };

#endif
	//Animation관련 
	enum AnimationMotion {
		AM_IDLE, AM_WALK, AM_RUN, AM_TURN, AM_LOOKUP, AM_SITDOWN
		, AM_JUMP, AM_FALL, AM_ATTACK, AM_DAMAGED, AM_DEAD
	};

	//방향 열거체 정의
	enum Direction { DIR_DOWN = -1, DIR_IDLE, DIR_UP, DIR_LEFT, DIR_RIGHT };

	//Scene 타입 정의
	enum SceneType { ST_TITLE, ST_GAME, ST_SELECT, ST_GAMEOVER, ST_LOADING };

	//EventType 정의
	enum EventType { EVENT_ATTACK, EVENT_DAMAGED, EVENT_ACTIVE, EVENT_INACTIVE, EVENT_DESTROY, EVENT_DEAD };

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

		bool operator !=(const Point& other) {
			if (this->x != other.x || this->y != other.y)
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
		~Rect() { }

		Rect(const Rect& other) :
			Rect(other.left, other.top, other.right, other.bottom)
		{	}

		bool operator ==(const Rect& other) {
			if ((left == other.left) && (top = other.top)
				&& (right == other.right) && (bottom = other.bottom))
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

	struct EntityData {

		UINT			m_iEntityWidth;
		UINT			m_iEntityHeight;
		Point			m_EntityPoint;
		tstring		m_strEntityName;
		EntityID		m_EntityID;

		EntityData(UINT iWidth, UINT iHeight, Point point, const tstring& strName, EntityID id)
			: m_iEntityWidth(iWidth), m_iEntityHeight(iHeight), m_EntityPoint(point), 
			m_strEntityName(strName), m_EntityID(id)
		{
		}

		EntityData& operator=(const EntityData& other)
		{
			m_iEntityWidth = other.m_iEntityWidth;
			m_iEntityHeight = other.m_iEntityHeight;
			m_EntityPoint = other.m_EntityPoint;
			m_EntityID = other.m_EntityID;
			m_strEntityName = other.m_strEntityName;
		}

	};

	struct ObjectData : public EntityData {
		class CGameScene;

		ObjectType	m_ObjectType;
		ObjectState m_ObjectState;

		ObjectData(UINT iWidth, UINT iHeight, Point point, const tstring& strName,
			EntityID id, ObjectType type, ObjectState state)
			: EntityData(iWidth, iHeight, point, strName, id),
			m_ObjectType(type), m_ObjectState(state)
		{
		}

		ObjectData& operator=(const ObjectData& other)
		{
			m_iEntityWidth = other.m_iEntityWidth;
			m_iEntityHeight = other.m_iEntityHeight;
			m_EntityPoint = other.m_EntityPoint;
			m_EntityID = other.m_EntityID;
			m_strEntityName = other.m_strEntityName;
			m_ObjectType = other.m_ObjectType;
			m_ObjectState = other.m_ObjectState;
		}

	};

	struct ActorData : public ObjectData {
		class CGameScene;

		ObjectType				m_ActorType;
		ObjectState				m_ActorState;
		ActType					m_ActType;
		Direction					m_Direction;

		ActorData(UINT iWidth, UINT iHeight, Point point, ObjectType type, ObjectState state,
			ActType act, Direction dir, EntityID id, const tstring& strName)
			:ObjectData(iWidth, iHeight, point, strName, id, type, state),
			m_ActorType(type), m_ActorState(state),
			m_ActType(act), m_Direction(dir)
		{
		}

		ActorData& operator=(const ActorData& other) {
			m_iEntityWidth = other.m_iEntityWidth;
			m_iEntityHeight = other.m_iEntityHeight;
			m_EntityPoint = other.m_EntityPoint;
			m_strEntityName = other.m_strEntityName;
			m_EntityID = other.m_EntityID;
			m_ActorType = other.m_ActorType;
			m_ActorState = other.m_ActorState;
			m_ActType = other.m_ActType;
			m_Direction = other.m_Direction;
		}
		//CWorld*					m_pOwnerWorld;
	};

}

typedef Types::tstring					TSTRING;
typedef Types::UIType				UI_TYPE;
typedef Types::Point					POSITION;
typedef Types::CameraID				CAMERA_ID;
typedef Types::EntityID				ENTITY_ID;
typedef Types::EntityData			ENTITY_DATA;
typedef Types::ObjectType			OBEJCT_TYPE;
typedef Types::ObjectType			OBJECT_TYPE;
typedef Types::ObjectData			OBJECT_DATA;
typedef Types::ObjectState			OBJECT_STATE;
typedef Types::ActorData			ACTOR_DATA;
typedef Types::ActType				ACT;
typedef Types::Direction				DIRECTION;
typedef Types::AnimationMotion	ANIM_MOTION;
typedef Types::EventType			EVENT_TYPE;