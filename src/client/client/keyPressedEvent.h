#pragma once
#include "event.h"

using namespace std;

class KeyPress {
public:
	bool shift;
};

EVENT_CLASS(LeftArrowKeyPressedEvent)
EVENT_CLASS(RightArrowKeyPressedEvent)
EVENT_CLASS(UpArrowKeyPressedEvent)
EVENT_CLASS(DownArrowKeyPressedEvent)
EVENT_CLASS(EscapeKeyPressedEvent)
EVENT_CLASS(SpaceKeyPressedEvent)
EVENT_CLASS(EnterKeyPressedEvent)
EVENT_CLASS(TabKeyPressedEvent)
#undef PARAMETERS
#undef MEMBERS_SET
#define PARAMETERS char letter, bool shift
#define MEMBERS_SET MEMBER_SET(letter) MEMBER_SET(shift)
EVENT_CLASS(LetterKeyPressedEvent, : public KeyPress, char letter; , PARAMETERS)
