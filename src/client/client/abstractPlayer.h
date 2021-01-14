#pragma once
#include <functional>
#include "../../common/common/abstractService.h"

class AbstractPlayer : public AbstractService {
public:
    int id = 0;
    VTile position;
};
