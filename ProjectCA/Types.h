#pragma once

#include "stdafx.h"

#define MAKE_INPUT			0b00001
#define MAKE_PHYSICS		0b00010
#define MAKE_COL_BOX		0b00100
#define MAKE_COL_CIR		0b01000

#define TYPE_ACTION 1

const unsigned int MAX_WIDTH			= 1024;
const unsigned int MAX_HEIGHT			= 720;
const unsigned int MAX_ACTOR_SIZE	= 3000;

const unsigned int SPRITE_WIDTH		= 32;
const unsigned int SPRITE_HEIGHT		= 48;


namespace Types {

	typedef std::basic_string<TCHAR> tstring;
	typedef unsigned long ActorID;

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

		Rect(float _left, float _top, float _right, float _bottom) {
			left = _left;
			top = _top;
			right = _right;
			bottom = _bottom;
		}
		~Rect(){ }

		Rect(const Rect& other) :
			Rect(other.left, other.top, other.right, other.bottom)
		{	}

		//Rect(Rect&& other) :
		//	Rect(other.left, other.top, other.right, other.bottom)
		//{	}


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

	//struct ActorData {
	//	//ObjectData(ObjectType type, Point objectPoint, Direction dir,
	//	//	ObjectState state, UINT width, UINT height, ActorID id, 
	//	//	class CWorld* pWorld, class CGameScene* pScene, tstring strTag)
	//	//	: objectType(type), fObjectPoint(objectPoint), objectDirection(dir), 
	//	//	objectState(state), iWidth(width), iHeight(height),actorID(id),
	//	//	pOwnerWorld(pWorld), pOwnerScene(pScene)
	//	//{

	//	//}

	//	ActorData(ObjectType type, Point objectPoint, Direction dir,
	//		ObjectState state, UINT width, UINT height, ActorID id,
	//		tstring strName)
	//		: objectType(type), fObjectPoint(objectPoint), objectDirection(dir),
	//		objectState(state), iWidth(width), iHeight(height), actorID(id),
	//		//pOwnerScene(pScene), 
	//		strActorName(strName)
	//	{

	//	}

	//	ActorData(const ActorData& other)
	//	{
	//		objectType = other.objectType;
	//		fObjectPoint = other.fObjectPoint;
	//		objectDirection = other.objectDirection;
	//		objectState = other.objectState;
	//		iWidth = other.iWidth;
	//		iHeight = other.iHeight;
	//		//pOwnerScene = other.pOwnerScene;
	//		strActorName = other.strActorName;
	//	}
	//
	//	ActorData(ActorData&& other)
	//		: objectType(other.objectType), fObjectPoint(other.fObjectPoint), objectDirection(other.objectDirection),
	//		objectState(other.objectState), iWidth(other.iWidth), iHeight(other.iHeight), actorID(other.actorID),
	//		//pOwnerScene(other.pOwnerScene), 
	//		strActorName(std::move(other.strActorName))
	//	{
	//		//other.pOwnerScene = nullptr;
	//	}



	//	ObjectType				objectType;
	//	Point						fObjectPoint;
	//	Direction					objectDirection;
	//	ObjectState				objectState;
	//	UINT						iWidth;
	//	UINT						iHeight;
	//	ActorID					actorID;
	//	//CWorld*					pOwnerWorld;
	//	//CGameScene*			pOwnerScene;
	//	tstring					strActorName;
	//};
	

	struct ActorData {
		class CGameScene;

		ActorData(UINT iWidth, UINT iHeight, Point point, ActorType type, ActorState state, 
			VerticalState vertical, HorizonalState horizonal, Direction dir, Point _vector,
			ActorID id, const tstring& strTag, bool _bActive)
			:iActorWidth(iWidth), iActorHeight(iHeight), actorPoint(point), actorType(type), actorState(state),
			verticalState(vertical), horizonalState(horizonal), direction(dir), vector(_vector),
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
			vector = other.vector;
			strActorTag = other.strActorTag;
			//pOwnerScene = other.pOwnerScene;
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
		Point						vector;			//방향을 표시하기 위한 방법으로써 direction 상수대신 vector로 대체(09.25)
		ActorID					actorID;
		tstring					strActorTag;
		//CWorld*					m_pOwnerWorld;
		//CGameScene*			pOwnerScene;
		bool						bActive;


	};
	
	typedef Types::tstring TSTRING;

}