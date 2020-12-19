#include "app.h"
#include "app.cpp"

#include "map.h"
#include "measures.h"
#include "rightBanner.h"
#include "bottomBanner.h"
#include "player.h"
#include "camera.h"
#include "rightClickInterface.h"

int main() {
    App<
        Measures,
        RenderWindow,
        TaskManager,
        Player,
        Camera,
        Map,
        RightBanner,
        BottomBanner,
        RightClickInterface
    > app;
    app.main();
}
