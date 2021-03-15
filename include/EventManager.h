#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include<iostream>
#include<unordered_map>

#include<Enums.h>
#include<InputManager.h>

class EventInfo
{
public:
	virtual ~EventInfo() = default;
};

class InputEventInfo : public EventInfo
{
public:
	InputEventInfo(){}
	InputEventInfo(Keys _key)
	: key(_key)
	{}

	Keys key;
};

class Event
{

public:

	Event()
	: eventType(EVENT_TYPE_FIRST)
	{

	}

	Event(EventType _eventType, EventInfo* _eventInfo)
	: eventType(_eventType)
	, eventInfo(_eventInfo)
	{
		//TODO: _eventInfo needs to be copied into eventInfo
	}

	EventType getEventType() const { return eventType; }
	void setEventType(EventType _eventType) { eventType = _eventType; }

	EventInfo* getEventInfo() { return eventInfo; }
	void setEventInfo(EventInfo* _eventInfo) { eventInfo = _eventInfo; }

private:
	
	EventType eventType;
	EventInfo* eventInfo;

};

class EventListener
{
public:
	virtual void sendEvent(Event& event) = 0;
};

typedef std::vector<EventListener*>					EventListenerList;
typedef std::unordered_map<int, EventListenerList> 	EventListenerMap;

class EventManager
{
public:

	void registerListener(EventListener& newEventListener, EventType eventType)
	{
		eventListenersMap[eventType].push_back(&newEventListener);
	}

	void registerEvent(Event& event)
	{
		EventType eventType = event.getEventType();
		auto listenerMapEntry = eventListenersMap.find(eventType);

		if(listenerMapEntry != eventListenersMap.end())
		{
			std::vector<EventListener*> listenerList = listenerMapEntry->second;	

			for(auto it = listenerList.begin(); it != listenerList.end(); ++it)
			{
				(*it)->sendEvent(event);
			}
		}

	}

private:

	EventListenerMap eventListenersMap;
};


#endif