/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * cmdline.hpp
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

#ifndef __CMDLINE_HPP__
#define __CMDLINE_HPP__

#include <vector>
#include <functional>
#include <cstring>
#ifdef ARGP_INTERFACE
# include <argp.h>
#endif // ARGP_INTERFACE
#include "clparser.h"
#ifdef ARGP_INTERFACE
# define CMDL_LIB_TYPE 1
#else // !ARGP_INTERFACE
# define CMDL_LIB_TYPE 0
#endif // ARGP_INTERFACE

template<typename T>
class TypeVector : public std::vector<T>
{
public:
  TypeVector() {}

  int index(const T& r) const
  {
    for (int i = 0; i < (int)std::vector<T>::size(); ++i)
    {
      if ((*this)[i] == r) return i;
    }
    return -1;
  }

  void add(const T& r) {std::vector<T>::push_back(r);}
};

class CmdLineOptionVector : public std::vector<CmdLineOption>
{
public:
  CmdLineOptionVector() {}

  const CmdLineOption& item(int i) const {return (*this)[i];}
  CmdLineOption& item(int i) {return (*this)[i];}

  int index(const CmdLineOption& r) const
  {
    for (int i = 0; i < (int)std::vector<CmdLineOption>::size(); ++i)
    {
      if ((*this)[i].option_id == r.option_id) return i;
    }
    return -1;
  }

  int index_by_name(const CmdLineOption& r) const
  {
    for (int i = 0; i < (int)std::vector<CmdLineOption>::size(); ++i)
    {
      const char *this_option = at(i).option;
      if (this_option != 0 && strcmp(this_option, r.option) == 0) return i;
    }
    return -1;
  }

  void add(const CmdLineOption& r) {std::vector<CmdLineOption>::push_back(r);}
};

#ifdef ARGP_INTERFACE
// =============================================================================
// ArgpHandler -- Interface
// =============================================================================
class CmdLineArgs;
class ArgpHandler
{
  public:
  CmdLineArgs *owner;
  ArgpHandler() {}
  // ARGP_KEY_ARG
  virtual bool on_arg(argp_state *state, char *arg, int *rc)
  { *rc = 0; return /*not handled*/ false; }
  // ARGP_KEY_END
  virtual bool on_end(argp_state *state, char *arg, int *rc)
  { *rc = 0; return /*not handled*/ false; }
  // ARGP_KEY_ARGS
  virtual bool on_args(argp_state *state, char *arg, int *rc)
  { *rc = 0; return /*not handled*/ false; }
  // ARGP_KEY_NO_ARGS
  virtual bool on_no_args(argp_state *state, char *arg, int *rc)
  { *rc = 0; return /*not handled*/ false; }
  // ARGP_KEY_INIT
  virtual bool on_init(argp_state *state, char *arg, int *rc)
  { *rc = 0; return /*not handled*/ false; }
  // ARGP_KEY_SUCCESS
  virtual bool on_success(argp_state *state, char *arg, int *rc)
  { *rc = 0; return /*not handled*/ false; }
  // ARGP_KEY_ERROR
  virtual bool on_error(argp_state *state, char *arg, int *rc)
  { *rc = 0; return /*not handled*/ false; }
  // ARGP_KEY_FINI
  virtual bool on_finished(argp_state *state, char *arg, int *rc)
  { *rc = 0; return /*not handled*/ false; }
};
#endif // ARGP_INTERFACE

// =============================================================================
// CmdLineArgs System -- Interface
// =============================================================================
class CmdLineArgs : public CmdLineOptionVector
{
  #ifdef ARGP_INTERFACE

  ArgpHandler                *_argpHandler;
  std::vector<std::string>    _parseMessages;
  std::vector<std::string>    _sources;
  std::vector<argp_option>    _argpOptions;
  int                         _curGroup;
  int                         _argc;
  char                      **_argv;
  std::string                 _pgmName;
  std::string                 _doc;
  std::string                 _args_doc;
  bool                        _initialized;
  bool                        _parsed;
  bool                        _boolStrict;
public:
  int                         indexStopped;

  #else

  TypeVector<int>             _addedIds;
  TypeVector<int>             _addedOptions;
  std::vector<std::string>    _parseMessages;
  std::vector<std::string>    _sources;
  int                         _curOption;
  int                         _argc;
  char                      **_argv;
  std::string                 _pgmName;
  std::string                 _doc;
  std::string                 _args_doc;
  bool                        _initialized;
  bool                        _parsed;
  bool                        _extraFatal;
  bool                        _conflictFatal;
  bool                        _tolerateAssignChar;
  bool                        _mustShowHelp;
  char                        _assignChar;

  #endif // ARGP_INTERFACE

public:
  #ifdef ARGP_INTERFACE

  CmdLineArgs() :
      _argpHandler        (0),
      _curGroup           (0),
      _argc               (0),
      _argv               (0),
      _pgmName            (""),
      _doc                (""),
      _args_doc           (""),
      _initialized        (false),
      _parsed             (false),
      _boolStrict         (false) {_check_lib_type(CMDL_LIB_TYPE);}
  CmdLineArgs(int argc, char *argv[]) :
      _argpHandler        (0),
      _curGroup           (0),
      _argc               (0),
      _argv               (0),
      _pgmName            (""),
      _doc                (""),
      _args_doc           (""),
      _initialized        (false),
      _parsed             (false),
      _boolStrict         (false)
  { _check_lib_type(CMDL_LIB_TYPE); init(argc, argv); }

  #else

  CmdLineArgs() :
      _curOption          (0),
      _argc               (0),
      _argv               (0),
      _pgmName            (""),
      _doc                (""),
      _args_doc           (""),
      _initialized        (false),
      _parsed             (false),
      _extraFatal         (false),
      _conflictFatal      (false),
      _tolerateAssignChar (false),
      _mustShowHelp       (false),
      _assignChar         ('=') {_check_lib_type(CMDL_LIB_TYPE);}
  CmdLineArgs(int argc, char *argv[]) :
      _curOption          (0),
      _argc               (0),
      _argv               (0),
      _pgmName            (""),
      _doc                (""),
      _args_doc           (""),
      _initialized        (false),
      _parsed             (false),
      _extraFatal         (false),
      _conflictFatal      (false),
      _tolerateAssignChar (false),
      _mustShowHelp       (false),
      _assignChar         ('=')
  { _check_lib_type(CMDL_LIB_TYPE); init(argc, argv); }

  #endif // ARGP_INTERFACE

  // Set executable file name etc
  void init  (int argc, char *argv[]);
  // CMDL_FP
  int option (const char *option, const char *info,
              double *fp, int option_id = -1);
  // CMDL_BOOL
  int option (const char *option, const char *info,
              bool *pb, int option_id = -1);
  // CMDL_INT
  int option (const char *option, const char *info,
              int *pi, int option_id = -1);
  // CMDL_STR
  int option (const char *option, const char *info,
              char **pstr, int option_id = -1);
  int option (const char *option, const char *info,
              std::string *pstr, int option_id = -1);
  // CMDL_VALUE_SEL
  int option (const char *option, const char *info,
              int value, int *pi, int option_id = -1);
  // CMDL_USER_WITH_ARG, CMDL_USER (use option_uses_no_arg)
  int option (const char *option, const char *info,
              CmdlOptionHandler f, void *param, int option_id = -1);
  // CMDL_USER_LAMBDA_WITH_ARG, CMDL_USER_LAMBDA (use option_uses_no_arg)
  int option (const char *option, const char *info,
              CmdlOptionLambda f, int option_id = -1);
  // General usage
  int option (const CmdLineOption& cmdlo);
  // Alias
  int alias (const char *option, int option_id = -1, int alias_id = -1);
  #ifndef ARGP_INTERFACE
  // CMDL_FP
  int group (const char *opt, const char *info, double *fp)
  { return option(opt, info, fp, _curOption); }
  // CMDL_BOOL
  int group (const char *opt, const char *info, bool *pb)
  { return option(opt, info, pb, _curOption); }
  // CMDL_INT
  int group (const char *opt, const char *info, int *pi)
  { return option(opt, info, pi, _curOption); }
  // CMDL_STR
  int group (const char *opt, const char *info, char **p)
  { return option(opt, info, p, _curOption); }
  int group (const char *opt, const char *info, std::string *p)
  { return option(opt, info, p, _curOption); }
  // CMDL_VALUE_SEL
  int group (const char *opt, const char *info, int value, int *pi)
  { return option(opt, info, value, pi, _curOption); }
  // CMDL_USER
  int group (const char *opt, const char *info, CmdlOptionHandler f, void *p)
  { return option(opt, info, f, p, _curOption); }
  // CMDL_USER_LAMBDA
  int group (const char *opt, const char *info, CmdlOptionLambda f)
  { return option(opt, info, f, _curOption); }
  // Help
  int option_help (const char *option, const char *info);
  int group_help  (const char *option, const char *info);
  #endif // !ARGP_INTERFACE

  #ifdef ARGP_INTERFACE
         bool option_uses_no_arg  (int which = -1/*most recent*/);
         void set_argp_handler    (ArgpHandler *h) { _argpHandler = h;
                                      if (h != 0) h->owner = this; }
  // Set group documentation
         void begin_group         (const char *group_header = 0);
         void doc_header          (const char *doc);
  #endif // !ARGP_INTERFACE
  // Set documentation
         void set_doc             (const std::string&);
         void set_args_doc        (const std::string&);

  // Parse cmd line and execute commands
         int  parse_cmd_line      (unsigned flags = 0);
  inline void parse               () {parse_cmd_line();}
  // Display help
         void display_help        (unsigned flags = 0) const;

  #ifdef ARGP_INTERFACE
  inline void set_bool_strict     (bool f = true) {_boolStrict = f;}
  #else
  // Display user specs
         void display_specs       () const;
  // Handle parse messages
         void display_messages    () const;
  inline bool must_show_help      () const {return _mustShowHelp;}
  inline bool messages_exist      () const {return !_parseMessages.empty();}
  inline bool messages            () const {return messages_exist ();}
  inline void tolerate_assign     () {_tolerateAssignChar = true;}
  inline void set_extra_fatal     () {_extraFatal = true;}
  inline void set_conflict_fatal  () {_conflictFatal = true;}
  inline void set_assignment      (const char ch) {_assignChar = ch;}
  #endif // !ARGP_INTERFACE
  inline const std::string
              program_name        () const {return _pgmName;}
  inline std::vector<std::string>&
              sources             () {return _sources;}
  void        display_usage       (unsigned flags = 0) const;

protected:
         void _set_option_id      (CmdLineOption& cmdlo, int option_id);
         bool _check_lib_type     (int lib_type);
  #ifdef ARGP_INTERFACE
         int  _build_info_arg     (const std::string& info, unsigned type);
  #else // ! ARGP_INTERFACE
         void _display_spec       (int j) const;
  #endif // ARGP_INTERFACE
         void _check_parsed       () const;
  #ifdef ARGP_INTERFACE
         bool _check_option       (const char *option, int key);
  #endif // ARGP_INTERFACE
};

#endif // __CMDLINE_HPP__
