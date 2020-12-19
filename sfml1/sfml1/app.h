#pragma once
#include <thread>

#include "units.h"
#include "pathfinder.h"
#include "taskManager.h"
#include "getRenderWindow.h"
#include "movingPredicate.h"
#include "gameTick.h"
#include "mouseWheelEvent.h"
#include "eventObserver.cpp"
#include "VPixelToVTileConverter.h"
#include "mouseLeftClickEvent.h"
#include "mouseRightClickEvent.h"
#include "mouseMoveEvent.h"

template <
    typename Measures,
    typename RenderWindow,
    typename TaskManager,
    typename Player,
    typename Camera,
    typename Map,
    typename RightBanner,
    typename BottomBanner,
    typename RightClickInterface
>
class App {
public:
	void main();
};