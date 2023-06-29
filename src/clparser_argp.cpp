/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * clparser_argp.cpp
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

#include "clparser.hpp"
#include <cstring>

using std::string;
using std::vector;

#define NULL_OPTION {0,0,0,0,0,0}

// =============================================================================
// CmdLineOptionVector
// =============================================================================
int CmdLineOptionVector::index(const CmdLineOption& r) const {
  for (size_t i = 0; i < std::vector<CmdLineOption>::size(); ++i)
    if ((*this)[i].option_id == r.option_id) return i;
  return -1;
}

int CmdLineOptionVector::index_by_name(const CmdLineOption& r) const {
  for (size_t i = 0; i < std::vector<CmdLineOption>::size(); ++i) {
    const char *this_option = at(i).option;
    if (this_option != 0 && strcmp(this_option, r.option) == 0) return i;
  }
  return -1;
}

// =============================================================================
// Argp interface
// =============================================================================
static const argp_option nullopt = NULL_OPTION;

// Used by CmdLineArgs::::parse_cmd_line() to communicate with parse_opt.
struct CmdLineArgsData {
  CmdLineArgs *self;
  ArgpHandler *argp_handler;
};

static int f_find_argp_option(argp_option *a, int key) {
  for (size_t i = 0; a[i].name != 0 || a[i].key != 0 ||
                  memcmp(&a[i], &nullopt, sizeof(argp_option)) != 0; ++i)
    if (key == a[i].key) return i;
  return -1;
}

static error_t f_options_parser(int key, char *arg, argp_state *state) {
  // Get the input argument from argp_parse, which
  // is a pointer to our CmdLineArgsData structure.
  CmdLineArgsData *cml_data = static_cast<CmdLineArgsData*>(state->input);
  CmdLineArgs* _this = cml_data->self;
  ArgpHandler* argp_handler = cml_data->argp_handler;
  int rc = 0;
  switch (key) {
    case ARGP_KEY_ARG:
      if (argp_handler == 0 || !argp_handler->on_arg(state, arg, &rc))
        _this->sources().push_back(arg);
      return rc;
    case ARGP_KEY_NO_ARGS:  // This is called just before ARGP_KEY_END
      if (argp_handler != 0) argp_handler->on_args(state, arg, &rc);
      return rc;
    case ARGP_KEY_INIT:     // This is passed in before any parsing is done
      if (argp_handler != 0) argp_handler->on_init(state, arg, &rc);
      return rc;
    case ARGP_KEY_END:
      if (argp_handler != 0) argp_handler->on_end(state, arg, &rc);
      return rc;
    case ARGP_KEY_SUCCESS:  // When parsing has successfully been completed
      if (argp_handler != 0) argp_handler->on_success(state, arg, &rc);
      return rc;
    case ARGP_KEY_ERROR:    // When an error occurred & parsing is terminated
      if (argp_handler != 0) argp_handler->on_error(state, arg, &rc);
      return rc;
    case ARGP_KEY_FINI:     // The final key ever seen by a parser
      if (argp_handler != 0) argp_handler->on_finished(state, arg, &rc);
      return rc;
    default:
      return _this->options_parser(key, arg);
  }
  return 0;
}

// =============================================================================
// Utility functions
// =============================================================================
static int f_find_option_id(CmdLineArgs *clargs, int id) {
  CmdLineOption clopt = {id};
  return clargs->index(clopt);
}

static int f_find_option(CmdLineArgs *clargs, const std::string& name) {
  CmdLineOption clopt = {0, 0, name.c_str()};
  return clargs->index_by_name(clopt);
}

static int f_find_last_non_alias(CmdLineArgs *_this) {
  for (int i = (int)_this->size() - 1; i > -1; --i)
    if (_this->item(i).type != CMDL_ALIAS) return i;
  return -1;
};

static int f_get_default_option_id(CmdLineArgs *clargs) {
  for (int i = 10000; i < 100000; ++i)
    if (f_find_option_id(clargs, i) == -1)
      return i;
  throw string{"too many command line options"};
  return -1;
}

static string f_get_string_for_reserved_key(int key) {
  if (key == ARGP_KEY_ARG    ) return std::string("ARGP_KEY_ARG");
  if (key == ARGP_KEY_ARGS   ) return std::string("ARGP_KEY_ARGS");
  if (key == ARGP_KEY_NO_ARGS) return std::string("ARGP_KEY_NO_ARGS");
  if (key == ARGP_KEY_INIT   ) return std::string("ARGP_KEY_INIT");
  if (key == ARGP_KEY_END    ) return std::string("ARGP_KEY_END");
  if (key == ARGP_KEY_SUCCESS) return std::string("ARGP_KEY_SUCCESS");
  return std::string{""};
}

static void f_check_option_key(const char *option, int key) {
  int l = (option == 0) ? 0 : strlen(option);
  if (l == 0 && key == 0)
    throw string{"attempt to add an empty option"};
  if (key == ARGP_KEY_ARG     ||
      key == ARGP_KEY_ARGS    ||
      key == ARGP_KEY_NO_ARGS ||
      key == ARGP_KEY_INIT    ||
      key == ARGP_KEY_END     ||
      key == ARGP_KEY_SUCCESS)
    throw string{"option with reserved key "}+
        f_get_string_for_reserved_key(key);
}

// =============================================================================
// String manipulation
// =============================================================================
// Find $ skipping $$
size_t f_find_dol(const string& info, size_t l, size_t p) {
  while ((p = info.find('$', p)) != string::npos && p < l-1 && info[p+1] == '$')
    p += 2;
  return p;
};

// Replace $$ with $
void f_pack_dol(string& s) {
  size_t p = 0;
  while ((p = s.find("$$", p)) != string::npos)
    s.replace(p++, 2, "$");
};

string f_get_default_info_arg(int type) {
  switch (type) {
    case CMDL_BOOL:   return string("[yes/no]");
    case CMDL_INT:
    case CMDL_FP:     return string("VALUE");
    case CMDL_STR:
    case CMDL_STDSTR: return string("STRING");
    default: break;
  }
  return string{"ARG"};
};

// =============================================================================
// CmdLineArgs implementation
// =============================================================================

void CmdLineArgs::init(int argc, char *argv[]) {
  if (_initialized)
    throw string{"the command line parser was previously initialized"};
#ifdef __windows__
  char file[1024];
  _splitpath(argv[0], 0, 0, file, 0);
#else
  char *file = basename(argv[0]);
#endif // __windows__
  _pgmName = file;
  _argc = argc;
  _argv = argv;
  _initialized = true;
};

// =============================================================================
// Option definition
// =============================================================================
void CmdLineArgs::_set_option_id(CmdLineOption& co, int option_id) {
  if (!_initialized)
    throw string{"the method CmdLineArgs::init() must be called first"};
  co.option_id = option_id == -1 ? f_get_default_option_id(this) : option_id;
}

void CmdLineArgs::begin_group(const char *header) {
  _check_parsed();
  // Start from 1. Options with group_id 0 are not groupped.
  ++_curGroup;
  if (header == 0) return;

  CmdLineOption cmdlo;
  _set_option_id(cmdlo, 0);
  cmdlo.type   = CMDL_GROUP_HEADER;
  cmdlo.option = 0;
  cmdlo.info   = header;
  cmdlo.group  = _curGroup;
  cmdlo.p_int  = 0;
  add(cmdlo);
}

int CmdLineArgs::option(const CmdLineOption& r) {
  _check_parsed();

  CmdLineOption cmdlo = r;

  if (!(cmdlo.type < CMDL_NUM)) throw string{"invalid option type"};

  _set_option_id(cmdlo, cmdlo.option_id);
  cmdlo.group = _curGroup;
  add(cmdlo);
  return cmdlo.option_id;
}

int CmdLineArgs::option(const char *option, const char *info,
                        double *fp, int option_id) {
  _check_parsed();
  if (!fp) throw string{"invalid data for floating point data option"};

  CmdLineOption cmdlo;

  _set_option_id(cmdlo, option_id);
  _check_option(option, cmdlo.option_id);
  cmdlo.type = CMDL_FP;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.group = _curGroup;
  cmdlo.p_fp = fp;

  add(cmdlo);

  return cmdlo.option_id;
}

int CmdLineArgs::option(const char *option, const char *info,
                        bool *pb, int option_id) {
  _check_parsed();
  if (pb == 0) throw string{"invalid data for boolean option"};

  CmdLineOption cmdlo;

  _set_option_id(cmdlo, option_id);
  _check_option(option, cmdlo.option_id);
  cmdlo.type = CMDL_BOOL;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.group = _curGroup;
  cmdlo.p_bool = pb;

  add(cmdlo);

  return cmdlo.option_id;
}

int CmdLineArgs::option(const char *option, const char *info,
                        int *pi, int option_id) {
  _check_parsed();
  if (pi == 0) throw string{"invalid data for integer option"};

  CmdLineOption cmdlo;

  _set_option_id(cmdlo, option_id);
  _check_option(option, cmdlo.option_id);
  cmdlo.type = CMDL_INT;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.group = _curGroup;
  cmdlo.p_int = pi;

  add(cmdlo);

  return cmdlo.option_id;
}

int CmdLineArgs::option(const char *option, const char *info,
                        char **pstr, int option_id) {
  _check_parsed();
  if (pstr == 0) throw string{"invalid data for string option"};

  CmdLineOption cmdlo;

  _set_option_id(cmdlo, option_id);
  _check_option(option, cmdlo.option_id);
  cmdlo.type = CMDL_STR;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.group = _curGroup;
  cmdlo.p_const_str = pstr;

  add(cmdlo);

  return cmdlo.option_id;
}

int CmdLineArgs::option(const char *option, const char *info,
                        std::string *pstr, int option_id) {
  _check_parsed();
  if (pstr == 0) throw string{"invalid data for string option"};

  CmdLineOption cmdlo;
  _set_option_id(cmdlo, option_id);
  _check_option(option, cmdlo.option_id);
  cmdlo.type = CMDL_STDSTR;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.group = _curGroup;
  cmdlo.p_str = pstr;

  add(cmdlo);

  return cmdlo.option_id;
}

int CmdLineArgs::option(const char *option, const char *info,
                        int value, int *pi, int option_id) {
  _check_parsed();
  if (pi == 0) throw string{"invalid data for value selection (CMDL_VALUE_SEL)"};

  CmdLineOption cmdlo;

  _set_option_id(cmdlo, option_id);
  _check_option(option, cmdlo.option_id);
  cmdlo.type = CMDL_VALUE_SEL;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.group = _curGroup;
  cmdlo.p_int = pi;
  cmdlo.value = value;

  add(cmdlo);

  return cmdlo.option_id;
}

int CmdLineArgs::option(const char *option, const char *info,
                        CmdlOptionHandler f, void *param, int option_id) {
  _check_parsed();
  if (f == 0) throw string{"invalid option handler for user handled option "
                           "(CMDL_USER_WITH_ARG)"};

  CmdLineOption cmdlo;

  _set_option_id(cmdlo, option_id);
  _check_option(option, cmdlo.option_id);
  cmdlo.type = CMDL_USER_WITH_ARG;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.group = _curGroup;
  cmdlo.param = param;
  cmdlo.option_handler = f;

  add(cmdlo);

  return cmdlo.option_id;
}

int CmdLineArgs::option(const char *option, const char *info,
                        CmdlOptionLambda f, int option_id) {
  _check_parsed();
  if (f == 0) throw string{"invalid option handler for user handled option "
                           "(CMDL_USER_LAMBDA_WITH_ARG)"};

  CmdLineOption cmdlo;

  _set_option_id(cmdlo, option_id);
  _check_option(option, cmdlo.option_id);
  cmdlo.type = CMDL_USER_LAMBDA_WITH_ARG;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.group = _curGroup;
  cmdlo.param = 0;
  cmdlo.option_lambda = f;

  add(cmdlo);

  return cmdlo.option_id;
}

int CmdLineArgs::alias(const char *option, int option_id, int alias_id) {
  _check_parsed();

  int index = alias_id == -1 ? f_find_last_non_alias(this) :
                               f_find_option_id(this, alias_id);
  if (index == -1) throw string{alias_id == -1 ?
        "incorrect option alias: no options are present" :
        "incorrect option alias: option not found"};
  if (item(index).type == CMDL_ALIAS)
    throw string{"option aliases cannot be aliased"};

  CmdLineOption cmdlo;
  _set_option_id(cmdlo, option_id);
  f_check_option_key(option, cmdlo.option_id);
  cmdlo.type    = CMDL_ALIAS;
  cmdlo.option  = option;
  cmdlo.info    = 0;
  cmdlo.group   = item(index).group;
  cmdlo.param   = 0;
  cmdlo.value   = index;

  // Skip previous aliases to the same option
  CmdLineArgs::iterator it = begin() + (index + 1);
  while (it != end() && it->type == CMDL_ALIAS) ++it;

  insert(it, cmdlo);

  return cmdlo.option_id;
}

void CmdLineArgs::doc_header(const char *doc) {
  _check_parsed();
  if (doc == 0) throw string{"documentation string is not valid"};

  CmdLineOption cmdlo;
  _set_option_id(cmdlo, 0/*NOT! -1*/);
  cmdlo.type = CMDL_EXTRA_DOC;
  cmdlo.option = 0;
  cmdlo.info = doc;
  cmdlo.group = _curGroup;
  cmdlo.p_int = 0;
  add(cmdlo);
}

bool CmdLineArgs::option_uses_no_arg(int id) /*id = -1: most recent*/ {
  if (_parsed) throw string{"command line options are already parsed"};
  int index = id == -1 ? static_cast<int>(size()) - 1 :
                         f_find_option_id(this, id);
  if (index == -1)
    throw string{"CmdLineArgs::option_uses_no_arg: incorrect option key"};
  CmdLineOption& clo = item(index);
  switch (clo.type) {
    case CMDL_BOOL:
      clo.type = CMDL_BOOL_NO_ARG;
      break;
    case CMDL_USER_WITH_ARG:
      clo.type = CMDL_USER;
      break;
    case CMDL_USER_LAMBDA_WITH_ARG:
      clo.type = CMDL_USER_LAMBDA;
      break;
    default:
      return false;
  }
  return true;
}

// =============================================================================
// Parse options
// =============================================================================
int CmdLineArgs::parse_cmd_line(unsigned flags) {
  if (_parsed)
    throw string{"Command line options were previously parsed"};
  if (!_initialized)
    throw string{"The method CmdLineArgs::init() must be called first"};
  _parsed = true;

  vector<string>& info = _parseMessages;
  info.reserve(2*size());

  // Build Argp option list
  for (int i = 0; i < (int)size(); ++i) {
    CmdLineOption& clo = item(i);
    bool no_arg  = clo.type == CMDL_ALIAS ||
                   clo.type == CMDL_GROUP_HEADER ||
                   clo.type == CMDL_EXTRA_DOC ||
                   clo.type == CMDL_BOOL_NO_ARG ||
                   clo.type == CMDL_VALUE_SEL ||
                   clo.type == CMDL_USER ||
                   clo.type == CMDL_USER_LAMBDA;
    int c = no_arg ? -1 : _build_info_arg((string) clo.info, clo.type);
    argp_option option;
    option.name  = clo.option;
    option.key   = clo.option_id;
    option.flags = (clo.type == CMDL_BOOL && !_boolStrict) ?
                   OPTION_ARG_OPTIONAL : 0;
    option.arg   = no_arg ? 0 : info[c + 1].c_str();
    option.doc   = no_arg ? clo.info : info[c].c_str();
    option.group = clo.group;
    option.flags = clo.type == CMDL_ALIAS ? OPTION_ALIAS : (
                   clo.type == CMDL_EXTRA_DOC ? OPTION_DOC : option.flags);
    _argpOptions.push_back(option);
  }
  _argpOptions.push_back(nullopt);

  // Build parser data structure
  argp parser_data;
  parser_data.options     = _argpOptions.data();
  parser_data.parser      = f_options_parser;
  parser_data.doc         = _doc.empty() ? 0 : _doc.c_str();
  parser_data.args_doc    = _args_doc.empty() ? 0 : _args_doc.c_str();
  parser_data.children    = 0;
  parser_data.help_filter = 0;
  parser_data.argp_domain = 0;

  // Build data structure to communicate CmdLineArgs data to the parser
  CmdLineArgsData cml_data;
  cml_data.self = this;
  cml_data.argp_handler = _argpHandler;

  // Parse arguments
  return argp_parse(&parser_data, _argc, _argv, flags, &indexStopped,
      static_cast<void*>(&cml_data));
}

// =============================================================================
// The parser
// =============================================================================
int CmdLineArgs::options_parser(int key, char *arg) {
  auto* const opt_array = _argpOptions.data();
  int option_index = key == 0 ? -1 : f_find_argp_option(opt_array, key);
  if (option_index == -1)
    return ARGP_ERR_UNKNOWN;
  if (item(option_index).type == CMDL_ALIAS)
    option_index = item(option_index).value;
  string optarg{arg ? arg : ""};
  string errmsg;
  char *after;

  auto& cmdlo = item(option_index);

  switch (cmdlo.type) {
    case CMDL_BOOL: {
      int bval = strtol(optarg.c_str(), &after, 0);
      if (*after || after == optarg.c_str()) {
        if (optarg.size() == 0)
          *(cmdlo.p_bool) = true;
        else if (optarg.compare("true") == 0  ||
            optarg.compare("yes" ) == 0       ||
            optarg.compare("on"  ) == 0)
          *(cmdlo.p_bool) = true;
        else if (optarg.compare("false") == 0 ||
            optarg.compare("no"   ) == 0      ||
            optarg.compare("off"  ) == 0)
          *(cmdlo.p_bool) = false;
        else
          errmsg = "incorrect boolean specification ";
      }
      else {
        if (bval > 1 || bval < 0)
          errmsg = "incorrect boolean specification ";
        else
          *cmdlo.p_bool = (bval != 0);
      }
      break;
    }

    case CMDL_BOOL_NO_ARG:
      *cmdlo.p_bool = true;
      break;

    case CMDL_INT:
      *cmdlo.p_int = strtol(optarg.c_str(), &after, 0);
      if (*after || after == optarg.c_str())
        errmsg = "incorrect integer specification ";
      break;

    case CMDL_FP:
      *cmdlo.p_fp = strtod(optarg.c_str(), &after);
      if (*after || after == optarg.c_str())
        errmsg = "incorrect floating point specification ";
      break;

    case CMDL_STR:
      *cmdlo.p_const_str = arg;
      break;

    case CMDL_STDSTR:
      *cmdlo.p_str = arg;
      break;

    case CMDL_VALUE_SEL:
      *cmdlo.p_int = cmdlo.value;
      break;

    case CMDL_USER:
    case CMDL_USER_WITH_ARG:
      if (!cmdlo.option_handler(this, cmdlo.param, arg))
        exit(0);
      break;
    case CMDL_USER_LAMBDA:
    case CMDL_USER_LAMBDA_WITH_ARG:
      if (!cmdlo.option_lambda(arg))
        exit(0);
      break;
  }
  if (errmsg.size() != 0) {
    if (string(cmdlo.option).length() != 0)
      errmsg += string(cmdlo.option);
    else if (isprint(key))
      errmsg += string(1, key);
    if (optarg.size() != 0)
      errmsg += "=" + optarg;
    throw errmsg;
  }
  return 0;
}

// =============================================================================
// Info and documentation routines
// =============================================================================
void CmdLineArgs::set_doc(const std::string& s) {_doc = s;}
void CmdLineArgs::set_args_doc(const std::string& s) {_args_doc = s;}

void CmdLineArgs::display_usage(unsigned flags) const {
  if (!_parsed)
    throw string{"CmdLineArgs::display_help and _usage() can only "
                 "be called after program arguments have been parsed"};
  // Build parser data structure
  argp parser_data;
  parser_data.options     = _argpOptions.data();
  parser_data.parser      = f_options_parser;
  parser_data.doc         = _doc.empty() ? _doc.c_str() : 0;
  parser_data.args_doc    = _args_doc.empty() ? _args_doc.c_str() : 0;
  parser_data.children    = 0;
  parser_data.help_filter = 0;
  parser_data.argp_domain = 0;
  // Call std usage function
  flags = flags == 0 ? ARGP_HELP_SHORT_USAGE|ARGP_HELP_SEE : flags;
  argp_help(&parser_data, stdout, flags, const_cast<char*>(_pgmName.c_str()));
}

void CmdLineArgs::display_help(unsigned flags) const {
  flags = flags == 0 ? ARGP_HELP_LONG: flags;
  display_usage(flags);
}

int CmdLineArgs::_build_info_arg(const string& info, unsigned type) {
  const auto npos = string::npos;
  size_t l = info.length();
  size_t p = f_find_dol(info, l, 0);
  size_t e = p == npos ? npos : f_find_dol(info, l, p + 1);
  bool no_trail = (e == npos);
  string lead = info.substr(0, p);
  string arg  = p == npos ? string("") : info.substr(p + 1,
      no_trail ? npos : e - p - 1);
  string trail = no_trail ? string("") : info.substr(e + 1);
  if (lead.length()  > 1) f_pack_dol(lead);
  if (arg.length()   > 1) f_pack_dol(arg);
  if (trail.length() > 1) f_pack_dol(trail);

  if (arg.empty()) arg = f_get_default_info_arg(type);

  int where = (int)_parseMessages.size();

  _parseMessages.push_back(no_trail ? lead : lead + arg + trail);
  size_t lb = arg.find_first_not_of(" \t");
  if (lb != 0 && lb != npos) arg = arg.substr(lb);
  _parseMessages.push_back(arg);
  return where;
}

// =============================================================================
// Consistency checks
// =============================================================================
bool CmdLineArgs::_check_option(const char *option, int key) {
  f_check_option_key(option, key);
  if (key != 0 && f_find_option_id(this, key) != -1) {
    string strkey = isprint(key) ? string(1, key) : std::to_string(key);
    throw string{"duplicate option key: "} + strkey;
  }
  if (option != 0 && f_find_option(this, string(option)) != -1)
    throw string{"duplicate option name: "} + option;
  return true;
}

void CmdLineArgs::_check_parsed() const {
  if (_parsed)
    throw string{"command line options cannot be added because the "
                 "command line was parsed"};
}

bool CmdLineArgs::_check_lib_type(int lib_type) {
  if (lib_type != 1)
    throw string{"the program was compiled for "} +
      (lib_type == 0 ? "the free-style " : "an unknown ") +
      "interface but linked to the argp library";
  return true;
}
