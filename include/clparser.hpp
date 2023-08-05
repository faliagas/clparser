/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * cmdline.hpp
 * Copyright (C) 2017-23 Apostol Faliagas <apostol.faliagas@gmail.com>
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

#include <string>
#include <functional>
#include <vector>
//#include <cstring>
#ifdef ARGP_INTERFACE
# include <argp.h>
#endif // ARGP_INTERFACE
//#include "clparser.h"
#ifdef ARGP_INTERFACE
# define CMDL_LIB_TYPE 1
#else // !ARGP_INTERFACE
# define CMDL_LIB_TYPE 0
#endif // ARGP_INTERFACE

enum {
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
typedef bool (*CmdlOptionHandler)(CmdLineArgs*, void *param, const char *arg);
typedef std::function<bool(const char*)> CmdlOptionLambda;

struct CmdLineOption {
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

#ifdef ARGP_INTERFACE
#else
template<typename T>
class TypeVector : public std::vector<T> {
public:
  TypeVector() {}
  int index(const T& r) const {
    for (int i = 0; i < (int)std::vector<T>::size(); ++i)
      if ((*this)[i] == r) return i;
    return -1;
  }
  void add(const T& r) {std::vector<T>::push_back(r);}
};
#endif // ARGP_INTERFACE

class CmdLineOptionVector : public std::vector<CmdLineOption> {
public:
  CmdLineOptionVector() {}
  const CmdLineOption& item(int i) const {return (*this)[i];}
  CmdLineOption& item(int i) {return (*this)[i];}
  int index(const CmdLineOption& r) const;
  int index_by_name(const CmdLineOption& r) const;
};

#ifdef ARGP_INTERFACE
// =============================================================================
// ArgpHandler -- Interface
// =============================================================================
class CmdLineArgs;
using SpecialKeyParserFunction = std::function<bool(argp_state*, char*, int*)>;
using ArgpParserFunction = std::function<bool(int, argp_state*, char*, int*)>;
class ArgpHandler {
public:
  CmdLineArgs *owner;
  ArgpHandler() {}
  // ARGP_KEY_ARG
  SpecialKeyParserFunction on_arg;
  // ARGP_KEY_END
  SpecialKeyParserFunction on_end;
  // ARGP_KEY_ARGS
  SpecialKeyParserFunction on_args;
  // ARGP_KEY_NO_ARGS
  SpecialKeyParserFunction on_no_args;
  // ARGP_KEY_INIT
  SpecialKeyParserFunction on_init;
  // ARGP_KEY_SUCCESS
  SpecialKeyParserFunction on_success;
  // ARGP_KEY_ERROR
  SpecialKeyParserFunction on_error;
  // ARGP_KEY_FINI
  SpecialKeyParserFunction on_finished;
  // User key parser
  ArgpParserFunction on_key;
};
#endif // ARGP_INTERFACE

// =============================================================================
// CmdLineArgs System -- Interface
// =============================================================================
class CmdLineArgs : public CmdLineOptionVector {
#ifdef ARGP_INTERFACE

  ArgpHandler                 _argpHandler;
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
      _curGroup           (0),
      _argc               (0),
      _argv               (0),
      _pgmName            (""),
      _doc                (""),
      _args_doc           (""),
      _initialized        (false),
      _parsed             (false),
      _boolStrict         (false)
  { _check_lib_type(CMDL_LIB_TYPE); _argpHandler.owner = this; }
  CmdLineArgs(int argc, char *argv[]) :
      _curGroup           (0),
      _argc               (0),
      _argv               (0),
      _pgmName            (""),
      _doc                (""),
      _args_doc           (""),
      _initialized        (false),
      _parsed             (false),
      _boolStrict         (false)
  { _check_lib_type(CMDL_LIB_TYPE); init(argc, argv); _argpHandler.owner = this; }

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
  // Set group documentation
         void begin_group         (const char *group_header = 0);
         void doc_header          (const char *doc);
#endif // !ARGP_INTERFACE
  // Set documentation
         void set_doc             (const std::string&);
         void set_args_doc        (const std::string&);
         void set_footer          (const std::string&);

  // Parse cmd line and execute commands
         int  parse_cmd_line      (unsigned flags = 0);
  inline void parse               () {parse_cmd_line();}
         int  options_parser      (int key, char *arg);
  // Display help
         void display_help        (unsigned flags = 0) const;

#ifdef ARGP_INTERFACE
  inline void set_bool_strict     (bool f = true) {_boolStrict = f;}
  // Configure parser
  using SKPF = SpecialKeyParserFunction;
  using SAPF = ArgpParserFunction;
  inline void set_on_arg          (SKPF f) {_argpHandler.on_arg = f;}
  inline void set_on_end          (SKPF f) {_argpHandler.on_end = f;}
  inline void set_on_args         (SKPF f) {_argpHandler.on_args = f;}
  inline void set_on_no_args      (SKPF f) {_argpHandler.on_no_args = f;}
  inline void set_on_init         (SKPF f) {_argpHandler.on_init = f;}
  inline void set_on_success      (SKPF f) {_argpHandler.on_success = f;}
  inline void set_on_error        (SKPF f) {_argpHandler.on_error = f;}
  inline void set_on_finished     (SKPF f) {_argpHandler.on_finished = f;}
  inline void set_on_key          (SAPF f) {_argpHandler.on_key = f;}
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
         int  _set_option_id      (CmdLineOption& cmdlo, int option_id);
         bool _check_lib_type     (int lib_type);
#ifdef ARGP_INTERFACE
  static error_t _options_parser  (int key, char* arg, argp_state* state);
         int  _build_info_arg     (const std::string& info, unsigned type);
         bool _check_option       (const char* option, int key);
#else // ! ARGP_INTERFACE
         void _display_spec       (int j) const;
#endif // ARGP_INTERFACE
         void _check_parsed       () const;
};

#endif // __CMDLINE_HPP__
