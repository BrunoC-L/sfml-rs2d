#include "app.h"

#include "map.h"
#include "measures.h"
#include "rightBanner.h"
#include "bottomBanner.h"
#include "player.h"
#include "camera.h"
#include "rightClickInterface.h"
#include "chat.h"
#include "inventory.h"
#include "sfRenderWindow.h"
#include "gameDataService.h"
#include "socket.h"

int main() {
    App<
        Measures,
        SFRenderWindow,
        TaskManager,
        Player,
        Camera,
        Map,
        Chat,
        Inventory,
        Socket,
        GameDataService
    > app;
    app.init();
    app.start();
}
