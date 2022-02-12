#pragma once
#include "event.h"

struct LeftArrowKeyPressedEventData {}; using LeftArrowKeyPressedEvent = Event<LeftArrowKeyPressedEventData>;
struct RightArrowKeyPressedEventData {}; using RightArrowKeyPressedEvent = Event<RightArrowKeyPressedEventData>;
struct UpArrowKeyPressedEventData {}; using UpArrowKeyPressedEvent = Event<UpArrowKeyPressedEventData>;
struct DownArrowKeyPressedEventData {}; using DownArrowKeyPressedEvent = Event<DownArrowKeyPressedEventData>;
struct EscapeKeyPressedEventData {}; using EscapeKeyPressedEvent = Event<EscapeKeyPressedEventData>;
struct BackspaceKeyPressedEventData {}; using BackspaceKeyPressedEvent = Event<BackspaceKeyPressedEventData>;
struct SpaceKeyPressedEventData {}; using SpaceKeyPressedEvent = Event<SpaceKeyPressedEventData>;
struct EnterKeyPressedEventData {}; using EnterKeyPressedEvent = Event<EnterKeyPressedEventData>;
struct TabKeyPressedEventData {}; using TabKeyPressedEvent = Event<TabKeyPressedEventData>;

struct LetterKeyPressedEventData {
    char letter;
    bool shift;
};
using LetterKeyPressedEvent = Event<LetterKeyPressedEventData>;
