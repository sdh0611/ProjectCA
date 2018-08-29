#pragma once

#include "stdafx.h"

#define MAKE_INPUT			0b00001
#define MAKE_PHYSICS		0b00010
#define MAKE_COL_BOX		0b00100
#define MAKE_COL_CIR		0b01000


namespace Types {

	typedef std::basic_string<TCHAR> tstring;

	//Point구조체 정의
	struct Point {

		float x, y;

		Point(float nx = 0.f, float ny = 0.f) :x(nx), y(ny) {		}
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

	struct ObjectData {
		ObjectData(ObjectType type, Point objectPoint, Direction dir, 
			ObjectState state, ActorID id)
			: objectType(type), fObjectPoint(objectPoint), objectDirection(dir), 
			objectState(state), actorID(id)
		{

		}

		ObjectType	objectType;
		Point	fObjectPoint;
		Direction objectDirection;
		ObjectState	objectState;
		ActorID		actorID;

	};
	
	
	//오브젝트 타입 정의
	enum ObjectType { OT_PLAYER, OT_ENEMY, OT_PROBS, OT_PICKUP, OT_MAP };

	//오브젝트 상태 정의
	enum ObjectState { OS_IDLE, OS_MOVE, OS_JUMP, OS_ATTACK, OS_DAMAGED, OS_FALL };

	//방향 열거체 정의
	enum Direction { DIR_DOWN = -1, DIR_IDLE, DIR_UP, DIR_LEFT, DIR_RIGHT };

	//Scene 타입 정의
	enum SceneType { ST_TITLE, ST_GAME, ST_SELECT };

	//EventType 정의
	enum EventType { ET_ATTACK, ET_DAMAGE };

}