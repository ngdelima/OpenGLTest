#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include<unordered_map>

#include<Enums.h>
#include<EventManager.h>

typedef std::unordered_map<unsigned char, Keys> NormalKeyMap;
typedef std::unordered_map<int, Keys> 			SpecialKeyMap;

typedef std::unordered_map<int, bool> 			KeyMap;

class InputManager
{

public:
	InputManager(EventManager& _eventManager)
	: eventManager(&_eventManager)
	{
		initializeInputManager();
	}

	void setKey(unsigned char key, bool state)
	{
		if( normalKeyMap.find(key) != normalKeyMap.end() )
		{
			registerInputEvent( (normalKeyMap.find(key)->second ), state);
		}
	}

	void setKey(int key, bool state)
	{
		if( specialKeyMap.find(key) != specialKeyMap.end() )
		{
			registerInputEvent( (specialKeyMap.find(key)->second ), state);
		}
	}

private:
	
	void initializeInputManager()
	{
		//Initialize normal key map
		normalKeyMap['c']	= KEY_CHAR_C;
		normalKeyMap[27]	= KEY_ESC;
		//keyMap[''] = false;

		//Initialize special key map
		specialKeyMap[GLUT_KEY_UP]		= KEY_UP;
		specialKeyMap[GLUT_KEY_DOWN]	= KEY_DOWN;
		specialKeyMap[GLUT_KEY_RIGHT]	= KEY_RIGHT;
		specialKeyMap[GLUT_KEY_LEFT]	= KEY_LEFT;
		//specialKeyMap[]= false;

		//Initialize key map
		keyMap[KEY_UP] 			= false;
		keyMap[KEY_DOWN] 		= false;
		keyMap[KEY_RIGHT] 		= false;
		keyMap[KEY_LEFT] 		= false;
		keyMap[KEY_ESC] 		= false;
		keyMap[KEY_CHAR_C] 		= false;
		keyMap[KEY_CHAR_CAP_C] 	= false;
	}

	void registerInputEvent(Keys key, bool state)
	{
		if(keyMap[key] != state)
		{
			keyMap[key] = state;
			EventType eventType = (state)? EVENT_TYPE_INPUT_KEY_DOWN : EVENT_TYPE_INPUT_KEY_UP;
			InputEventInfo* inputEventInfo = new InputEventInfo(key);
			Event inputEvent(eventType, inputEventInfo);
			eventManager->registerEvent(inputEvent);

			std::cout 	<< "Entered to registerInputEvent()" 
					 	<< "Event of type " << eventType
						<< ", key " 		<< key
						<< " registered."  	<< std::endl;
		}
	}

	EventManager* eventManager;
	KeyMap keyMap;
	NormalKeyMap normalKeyMap;
	SpecialKeyMap specialKeyMap;
};


#endif