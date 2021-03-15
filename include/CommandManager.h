#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include<iostream>

#include<Enums.h>
#include<EventManager.h>
#include<Widgets.h>

class CommandManager : public EventListener
{
public:

	CommandManager(RotatingCircleWidget* _upperCircle, RotatingCircleWidget* _lowerCircle, MultipointLineWidget* _multipointLineWidget)
	: upperCircle(_upperCircle)
	, lowerCircle(_lowerCircle)
	, multipointLineWidget(_multipointLineWidget)
	{

	}

	void sendEvent(Event& event)
	{
		EventType eventType = event.getEventType();
		if(isInputType(eventType))
		{
			InputEventInfo* inputEventInfo = dynamic_cast<InputEventInfo*>(event.getEventInfo());
			if(inputEventInfo == NULL) return;
			if(eventType != EVENT_TYPE_INPUT_KEY_DOWN) return;

			switch(inputEventInfo->key)
			{
				case KEY_UP:
					upperCircle->addAngularVelocity(0.5);
					break;
				case KEY_DOWN:
					upperCircle->addAngularVelocity(-0.5);
					break;
				case KEY_RIGHT:
					lowerCircle->addAngularVelocity(0.5);
					break;
				case KEY_LEFT:
					lowerCircle->addAngularVelocity(-0.5);
					break;
				case KEY_CHAR_C:
					multipointLineWidget->clear();
					break;
				case KEY_ESC:
					exit(0);
					break;
				default:
					break;
			}
		}
	}

private:

	RotatingCircleWidget* upperCircle;
	RotatingCircleWidget* lowerCircle;
	MultipointLineWidget* multipointLineWidget;
};


#endif