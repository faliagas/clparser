/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * example-gnu-1.cpp
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
// This is GNU's Example 1, a minimal program using argp
// https://www.gnu.org/software/libc/manual/html_node/Argp-Example-1.html
// -----------------------------------------------------------------------------
//  This is (probably) the smallest possible program that
//  uses argp.  It won’t do much except give an error
//  messages and exit when there are any arguments, and print
//  a (rather pointless) messages for –help.
// =============================================================================

#include "clparser.hpp"

int main(int argc, char **argv)
{
  CmdLineArgs cl(argc, argv);
  cl.parse();
  return 0;
}
