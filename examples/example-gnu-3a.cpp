/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * example-gnu-3a.cpp
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

#include <iostream>
#include "clparser.hpp"

using std::cout; using std::endl;
using std::string; using std::vector;

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

int main (int argc, char **argv)
{
  vector<string> args; // arg1 & arg2
  bool silent = false, verbose = false;
  string output_file("-");

  try
  {
    CmdLineArgs cl(argc, argv);
    cl.set_doc("Argp example #3 "
        "-- a program with options and arguments using argp");
    cl.set_args_doc("ARG1 ARG2");
    cl.set_bool_strict();
    cl.option("verbose", "Produce verbose output",   &verbose, 'v');
    cl.option("quiet",   "Don't produce any output", &silent,  'q');
    cl.alias ("silent", 's');
    cl.option("output",  "Output to $FILE$ instead of standard output",
        &output_file, 'o');
    cl.parse();
    if (cl.sources().size() != 2)
    {
      cl.display_usage();
      return 3;
    }
    args = cl.sources();
  }
  catch (string& msg)
  {
    printf("error: %s\n", msg.c_str());
    return 3;
  }

  cout << "ARG1 = " << args[0] << endl
       << "ARG2 = " << args[1] << endl
       << "OUTPUT_FILE = " << output_file << endl
       << "VERBOSE = " << (verbose ? "yes" : "no") << endl
       << "SILENT = "  << (silent ? "yes" : "no") << endl;

  return 0;
}
