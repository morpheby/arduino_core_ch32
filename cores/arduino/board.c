#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif


WEAK __attribute__((used)) void pre_init(void)
{
  hw_config_init();
}


#ifdef __cplusplus
}
#endif
