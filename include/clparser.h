/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * cmdline.h
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

#ifndef __CLPARSER_H__
#define __CLPARSER_H__

enum
{
  CMDL_BOOL,
  CMDL_BOOL_NO_ARG,
  CMDL_INT,
  CMDL_FP,
  CMDL_STR,
  CMDL_STDSTR,
  CMDL_VALUE_SEL,
  CMDL_USER,
  CMDL_USER_WITH_ARG,
  CMDL_USER_LAMBDA,
  CMDL_USER_LAMBDA_WITH_ARG,
  CMDL_ALIAS,
  CMDL_GROUP_HEADER,
  CMDL_EXTRA_DOC,
  CMDL_NUM
};

class CmdLineArgs;
typedef bool (*CmdlOptionHandler)(CmdLineArgs *, void *param, const char *arg);
typedef std::function<bool(const char*)> CmdlOptionLambda;

struct CmdLineOption
{
  int                 option_id;
  unsigned            type;
  const char         *option;
  const char         *info;
  int                 group;
  union {
    double           *p_fp;
    int              *p_int;
    bool             *p_bool;
    char            **p_const_str;
    std::string      *p_str;
    void             *param;            // for CMDL_USER, CMDL_USER_WITH_ARG
  };
  union {
    int               value;            // for CMDL_VALUE_SEL, CMDL_ALIAS
    CmdlOptionHandler option_handler;   // for CMDL_USER, CMDL_USER_WITH_ARG
  };
  CmdlOptionLambda    option_lambda;    // for CMDL_USER_LAMBDA,
                                        //     CMDL_USER_LAMBDA_WITH_ARG
};

#endif // __CLPARSER_H__
