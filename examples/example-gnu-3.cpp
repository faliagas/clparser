/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * example-gnu-3.cpp
 * Copyright (C) 2017 Apostol Faliagas <apostol.faliagas@gmail.com>
 *
 * clparser is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * clparser is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// =============================================================================
// This is GNU's Example 3: A simple program with user options
// https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html
// -----------------------------------------------------------------------------

#include <cstdlib>
#include <cstdio>
#include "clparser.hpp"

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

/* Program documentation --> see SetDoc in main()
static char doc[] =
  "Argp example #3 -- a program with options and arguments using argp"; */
/* A description of the arguments we accept --> see SetArgsDoc in main()
static char args_doc[] = "ARG1 ARG2"; */

#if 0
/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"quiet",    'q', 0,      0,  "Don't produce any output" },
  {"silent",   's', 0,      OPTION_ALIAS },
  {"output",   'o', "FILE", 0, "Output to FILE instead of standard output" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *args[2];                /* arg1 & arg2 */
  int silent, verbose;
  char *output_file;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key) {
    case 'q': case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        /* Too many arguments. */
        argp_usage (state);

      arguments->args[state->arg_num] = arg;

      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };
#endif // 0

int main (int argc, char **argv) {
  // These are the variables left to the user's control.
  // No need for struct arguments -- use variables directly with default values.
  char *args[2]; // arg1 & arg2
  int   silent = 0, verbose = 0;
  char *output_file = (char*)"-";

  CmdLineArgs cl(argc, argv);
  try {
    // Program documentation
    cl.set_doc("Argp example #3 "
        "-- a program with options and arguments using argp");
    // A description of the arguments we accept
    cl.set_args_doc("ARG1 ARG2");
    // The options we understand
    cl.option("verbose", "Produce verbose output",   &verbose, 'v');
    cl.option("quiet",   "Don't produce any output", &silent,  'q');
    cl.alias ("silent", 's');
    cl.option("output",  "Output to $FILE$ instead of standard output",
        &output_file, 'o');
    // Parse our arguments; every option seen by Parse
    // will be reflected in the associated variables.
    cl.parse();
    // The user must supply exactly two arguments
    if (cl.sources().size() != 2) {
      cl.display_usage();
      return 3;
    }
    args[0] = const_cast<char*>(cl.sources().at(0).c_str());
    args[1] = const_cast<char*>(cl.sources().at(1).c_str());
  }
  catch (std::string& msg) {
    printf("error: %s\n", msg.c_str());
    return 3;
  }

  printf ("ARG1 = %s\nARG2 = %s\nOUTPUT_FILE = %s\n"
          "VERBOSE = %s\nSILENT = %s\n",
          args[0], args[1],
          output_file,
          verbose ? "yes" : "no",
          silent ? "yes" : "no");

  return 0;
}
