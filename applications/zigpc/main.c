#include "uic_main.h"

#include <stdlib.h>

static uic_fixt_setup_step_t uic_fixt_setup_steps_list[]
  = {{NULL, "Terminator"}};

static uic_fixt_shutdown_step_t uic_fixt_shutdown_steps_list[]
  = {{NULL, "Terminator"}};

int main(int argc, char **argv)
{
  return uic_main(uic_fixt_setup_steps_list,
                  uic_fixt_shutdown_steps_list,
                  argc,
                  argv,
                  CMAKE_PROJECT_VERSION);
}
