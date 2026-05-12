#include "uic_main.h"
#include "zigpc_config.h"
#include "zigpc_datastore_fixt.h"
#include "zigpc_config_fixt.h"

#include <stdlib.h>

static uic_fixt_setup_step_t uic_fixt_setup_steps_list[]
  = {{&zigpc_config_fixt_setup, "ZigPC Configuration"},
     {&zigpc_datastore_fixt_setup, "ZigPC Datastore"},
     {NULL, "Terminator"}};

static uic_fixt_shutdown_step_t uic_fixt_shutdown_steps_list[]
  = {{NULL, "Terminator"}};

int main(int argc, char **argv)
{
  if (zigpc_config_init()) {
    return EXIT_FAILURE;
  }

  return uic_main(uic_fixt_setup_steps_list,
                  uic_fixt_shutdown_steps_list,
                  argc,
                  argv,
                  CMAKE_PROJECT_VERSION);
}
