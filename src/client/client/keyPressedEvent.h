#pragma once
#include "event.h"

class KeyPress {
public:
	bool shift;
};

EVENT_CLASS(LeftArrowKeyPressedEvent, : public Event)
EVENT_CLASS(RightArrowKeyPressedEvent, : public Event)
EVENT_CLASS(UpArrowKeyPressedEvent, : public Event)
EVENT_CLASS(DownArrowKeyPressedEvent, : public Event)
EVENT_CLASS(EscapeKeyPressedEvent, : public Event)
EVENT_CLASS(BackspaceKeyPressedEvent, : public Event)
EVENT_CLASS(SpaceKeyPressedEvent, : public Event)
EVENT_CLASS(EnterKeyPressedEvent, : public Event)
EVENT_CLASS(TabKeyPressedEvent, : public Event)
#undef PARAMETERS
#undef MEMBERS_SET
#define PARAMETERS char letter, bool shift
#define MEMBERS_SET MEMBER_SET(letter) MEMBER_SET(shift)
EVENT_CLASS(LetterKeyPressedEvent, : public KeyPress, char letter; , PARAMETERS, MEMBERS_SET)
