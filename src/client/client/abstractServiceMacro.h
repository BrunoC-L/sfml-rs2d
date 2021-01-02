#pragma once
#include "abstractServiceProvider.h"

#define SERVICES \
class AbstractMeasures;\
class AbstractRenderWindow;\
class AbstractTaskManager;\
class AbstractPlayer;\
class AbstractCamera;\
class AbstractMap;\
class AbstractChat;\
class AbstractInventory;\

#define SERVICE_MEMBERS \
public:\
AbstractMeasures* measures;\
AbstractRenderWindow* renderWindow;\
AbstractTaskManager* taskManager;\
AbstractPlayer* player;\
AbstractCamera* camera;\
AbstractMap* map;\
AbstractChat* chat;\
AbstractInventory* inventory;\
AbstractServiceProvider* provider;

#define REGISTER(_name) \
this->provider = provider;\
provider->set(#_name, this);\

#define ACQUIRE \
this->measures = (AbstractMeasures*)this->provider->get("Measures");\
this->renderWindow = (AbstractRenderWindow*)this->provider->get("RenderWindow");\
this->taskManager = (AbstractTaskManager*)this->provider->get("TaskManager");\
this->player = (AbstractPlayer*)this->provider->get("Player");\
this->camera = (AbstractCamera*)this->provider->get("Camera");\
this->map = (AbstractMap*)this->provider->get("Map");\
this->chat = (AbstractChat*)this->provider->get("Chat");\
this->inventory = (AbstractInventory*)this->provider->get("Inventory");\
