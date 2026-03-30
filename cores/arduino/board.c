#include "board.h"
#include "ch32yyxx_flash.h"

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
