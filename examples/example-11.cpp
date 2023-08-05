/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * clparser_argp.cpp
 * Copyright (C) 2017 apostol <apostol.faliagas@gmail.com>
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

#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  int num = 1;
  CmdLineArgs cl(argc, argv);
  cl.option("-num=", "number of objects", &num);
  cl.parse();
  cout << "num = " << num << endl;

  return 0;
}
