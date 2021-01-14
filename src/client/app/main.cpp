#include "main.h"

int main() {
    App<
        SFRenderWindow,
        Socket,
        Measures,
        Player,
        Camera,
        Map,
        Chat,
        Inventory,
        GameDataService
    > app;
    app.init();
    app.start();
}
