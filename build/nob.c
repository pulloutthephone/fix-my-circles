#define NOB_IMPLEMENTATION
#include "nob.h"

#define SRC_FOLDER "../src/"
#define BIN_FOLDER "../bin/"
int
main (int argc, char **argv)
{
  NOB_GO_REBUILD_URSELF (argc, argv);

  if (!nob_mkdir_if_not_exists (BIN_FOLDER))
    return 1;

  Nob_Cmd cmd = { 0 };

  // Build solve
  nob_cmd_append (&cmd, "gcc", "-Wall", "-Wextra", "-std=c11", "-O2", "-o",
                  BIN_FOLDER "solve", SRC_FOLDER "solve.c",
                  SRC_FOLDER "tests.c", "-lcriterion", "-lm");

  if (!nob_cmd_run_sync (cmd))
    {
      nob_log (NOB_ERROR, "Failed to build solve");
      return 1;
    }
  cmd = (Nob_Cmd){ 0 };

  // Build fail
  nob_cmd_append (&cmd, "gcc", "-Wall", "-Wextra", "-std=c11", "-O2", "-o",
                  BIN_FOLDER "fail", SRC_FOLDER "fail.c", SRC_FOLDER "tests.c",
                  "-lcriterion", "-lm");

  if (!nob_cmd_run_sync (cmd))
    {
      nob_log (NOB_ERROR, "Failed to build fail");
      return 1;
    }

  return 0;
}
