#include "uic_main.h"
#include "attribute_store_fixt.h"
#include "dotdot_mqtt.h"
#include "unify_dotdot_attribute_store.h"
#include "zigpc_config.h"
#include "datastore_fixt.h"
#include "zigpc_datastore_fixt.h"
#include "zigpc_attrmgmt.h"
#include "zigpc_config_fixt.h"
#include "zigpc_ncp_fixt.h"

#include <stdlib.h>

static uic_fixt_setup_step_t uic_fixt_setup_steps_list[]
  = {{&zigpc_config_fixt_setup, "ZigPC Configuration"},
     {&zigpc_datastore_fixt_setup, "ZigPC Datastore"},
     {&attribute_store_init, "Attribute store"},
     {&unify_dotdot_attribute_store_init, "Unify DotDot Attribute Store"},
     {&uic_mqtt_dotdot_init, "DotDot MQTT"},
     {&zigpc_attrmgmt_init, "ZigPC Attribute Management"},
     {&zigpc_ncp_fixt_setup, "ZigPC NCP"},
     {NULL, "Terminator"}};

static uic_fixt_shutdown_step_t uic_fixt_shutdown_steps_list[]
  = {{&zigpc_ncp_fixt_teardown, "ZigPC NCP"},
     {&attribute_store_teardown, "Attribute store"},
     {&datastore_fixt_teardown, "Datastore"},
     {NULL, "Terminator"}};

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
