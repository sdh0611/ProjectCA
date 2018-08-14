#pragma once

/*
	NOTE:
		컴포넌트간 통신을 위한 Message객체의 인터페이스 정의.
		각 컴포넌트 특성에 따라 이 Class를 상속한 후 새로운 Message Class 작성.
*/

#include "..\..\..\..\stdafx.h"


class ComponentMessage {
public:
	enum MessageType { MT_COL, MT_PHYSICS, MT_INPUT };


public:
	ComponentMessage(MessageType type);
	virtual ~ComponentMessage() { };


public:
	virtual void GetComponentMessage() { };
	virtual void SetMessageType(MessageType msg) { m_msgType = msg; }


public:
	MessageType GetMessageType() const { return m_msgType; }


private:
	MessageType		m_msgType;


};