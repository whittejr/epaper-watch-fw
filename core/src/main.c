#include "app_system.h"
#include "stm32wbxx_hal.h"

int main(void) {
    app_system_init();
    app_system_loop();
    return 0;
}
