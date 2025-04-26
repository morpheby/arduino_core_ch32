#include "board.h"
#include "ch32v20x_flash.h"

#ifdef __cplusplus
extern "C" {
#endif


WEAK __attribute__((used)) void pre_init(void)
{
  hw_config_init();
  FLASH_Enhance_Mode(ENABLE);
}


#ifdef __cplusplus
}
#endif
