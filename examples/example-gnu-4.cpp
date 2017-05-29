/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * example-gnu-4.cpp
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
// This is GNU's Example 4: A Program Using Multiple Combined Argp Parsers
// https://www.gnu.org/software/libc/manual/html_node/Argp-Example-4.html
// -----------------------------------------------------------------------------

#include <iostream>
#include <error.h>
#include "clparser.hpp"

const char *argp_program_version = "argp-ex4 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@prep.ai.mit.edu>";

using std::cout; using std::endl; using std::string; using std::vector;

// Keys for options without short-options
#define OPT_ABORT  1            // –abort

string btos(bool b) {return string(b ? "yes" : "no");}

int main (int argc, char **argv)
{

  string arg1;
  string output_file("-");
  vector<string> strings;
  bool silent = false, verbose = false, abort = false;
  int repeat_count = 1;

  try
  {
    CmdLineArgs cl(argc, argv);
    // Program documentation
    cl.set_doc(
"Argp example #4 -- a program with somewhat more complicated options\
\v\
This part of the documentation comes *after* the options; \
note that the text is automatically filled, but it's possible \
to force a line-break, e.g.\n<-- here.");
    cl.set_args_doc("ARG1 [STRING...]");
    cl.option("verbose", "Produce verbose output",   &verbose, 'v');
    cl.option("quiet",   "Don't produce any output", &silent,  'q');
    cl.alias ("silent", 's');
    cl.option("output",  "Output to $FILE$ instead of standard output",
              &output_file, 'o');
    cl.doc_header("The following options should be grouped together:");
    cl.option("repeat",  "Repeat the output $COUNT$ (default 1) times",
              &repeat_count, 'r');
    cl.option("abort", "Abort before showing any output", &abort, OPT_ABORT);
    cl.option_uses_no_arg();
    cl.parse();
    if (abort)
      error (10, 0, "ABORTED");
    if (cl.sources().size() < 1)
    {
      cl.display_usage();
      return 3;
    }
    const vector<string>& sources = cl.sources();
    arg1 = sources[0];
    for (int i = 1; i < sources.size(); ++i)
      strings.push_back(sources[i]);
  }
  catch (std::string& msg)
  {
    cout << "error: " << msg << endl;
    return 3;
  }

  for (int i = 0; i < repeat_count; ++i)
  {
    cout << "ARG1 = " << arg1 << endl
        << "STRINGS = ";
    for (int j = 0; j < strings.size(); ++j)
      cout << string(j == 0 ? "" : ", ") + strings[j];
    cout << endl;
    cout << "OUTPUT_FILE = " << output_file << endl
        << "VERBOSE = " << btos(verbose) << endl
        << "SILENT = " << btos(silent) << endl;
  }

  return 0;
}
