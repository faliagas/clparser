/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * example-gnu-2.cpp
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
// This is GNU's Example 2, a minimal program using argp
// https://www.gnu.org/software/libc/manual/html_node/Argp-Example-2.html
// -----------------------------------------------------------------------------
// This program doesn’t use any options or arguments, but uses
// argp to be compliant with the GNU standard command line
// format.
//
// In addition to making sure no arguments are given, and
// implementing a –help option, this example will have a
// –version option, and will put the given documentation string
// and bug address in the –help output, as per GNU standards.
//
// The variable ARGP contains the argument parser specification;
// adding fields to this structure is the way most parameters are
// passed to argp_parse (the first three fields are usually used,
// but not in this small program).  There are also two global
// variables that argp knows about defined here,
// ARGP_PROGRAM_VERSION and ARGP_PROGRAM_BUG_ADDRESS (they are
// global variables because they will almost always be constant
// for a given program, even if it uses different argument
// parsers for various tasks).
// =============================================================================

#include "clparser.hpp"

const char *argp_program_version = "t2-argp 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

int main(int argc, char **argv)
{
  /* Our argument parser.  The options, parser, and
     args_doc fields are zero because we have neither options or
     arguments; doc and argp_program_bug_address will be
     used in the output for ‘--help’, and the ‘--version’
     option will print out argp_program_version. */
  CmdLineArgs cl(argc, argv);
  /* Program documentation. */
  cl.set_doc("Argp example #2 -- a pretty minimal program using argp");
  cl.parse();
  return 0;
}
