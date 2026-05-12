// Includes from the component being tested
#include "zigpc_config.h"
#include "zigpc_config_fixt.h"

// Test includes
#include "unity.h"

// Unify library
#include "config.h"

// Generic includes
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define TEST_CONFIG_FILE "_test_config.ini"

static void remove_test_config_file()
{
  if (access(TEST_CONFIG_FILE, F_OK) != -1) {
    remove(TEST_CONFIG_FILE);
  }
}

void setUp()
{
  remove_test_config_file();
  config_reset();
}

void tearDown()
{
  remove_test_config_file();
}

static bool create_file_with_content(const char *filename, const char *content)
{
  FILE *fpth = fopen(filename, "w");
  if (fpth == NULL) {
    return false;
  }

  const int result = fputs(content, fpth);
  fclose(fpth);

  return (result > 0);
}

void test_config_from_file_populates_datastore_and_cpc_instance()
{
  char *argv_inject[3]    = {"zigpc_config_test", "--conf", TEST_CONFIG_FILE};
  const char *ini_content = "zigpc:\n"
                            "    datastore_file: zigpc-test.db\n"
                            "    cpc_instance: 12\n"
                            "mqtt:\n"
                            "    host: localhost\n"
                            "    port: 2000\n";

  TEST_ASSERT_TRUE_MESSAGE(
    create_file_with_content(TEST_CONFIG_FILE, ini_content),
    "Failed to create config file");
  TEST_ASSERT_EQUAL_MESSAGE(0,
                            zigpc_config_init(),
                            "zigpc_config_init failed");
  TEST_ASSERT_EQUAL_MESSAGE(CONFIG_STATUS_OK,
                            config_parse(sizeof(argv_inject) / sizeof(char *),
                                         argv_inject,
                                         "test version"),
                            "config_parse failed");
  TEST_ASSERT_EQUAL_MESSAGE(SL_STATUS_OK,
                            zigpc_config_fixt_setup(),
                            "zigpc_config_fixt_setup failed");

  TEST_ASSERT_EQUAL_STRING("zigpc-test.db", zigpc_get_config()->datastore_file);
  TEST_ASSERT_EQUAL(12, zigpc_get_config()->cpc_instance);
  TEST_ASSERT_EQUAL_STRING("localhost", zigpc_get_config()->mqtt_host);
  TEST_ASSERT_EQUAL(2000, zigpc_get_config()->mqtt_port);
}
