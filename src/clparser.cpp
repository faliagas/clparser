/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 2; tab-width: 2 -*-  */
/*
 * clparser_ng.cpp
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

#include "clparser.hpp"
#include <iostream>
#include <cstring>

using std::string;
using std::vector;

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
// Utility functions
// =============================================================================
static bool f_check_option(const char *option) {
  int l = (option == 0) ? 0 : strlen(option);
  if (option == 0 || l == 0 || (l == 1 && option[0] == '-'))
    return false;
  if (l == 2 && (option[0] == '-' && option[1] == '-'))
    return false;
  if (l > 0 && option[0] == '=')
    return false;
  if (l > 1 && (option[0] == '-' && option[1] == '='))
    return false;
  if (l > 2 && (option[0] == '-' && option[1] == '-' && option[2] == '='))
    return false;
  return true;
}

static string f_get_default_info_arg(int type) {
  switch (type) {
    case CMDL_BOOL:   return string("[yes|no]");
    case CMDL_INT:
    case CMDL_FP:     return string("VALUE");
    case CMDL_STR:
    case CMDL_STDSTR: return string("STRING");
    default: break;
  }
  return string("ARG");
};

static int f_find_value_sel_option(CmdLineArgs *_this, int *pi) {
  for (int i = 0; i < (int)_this->size(); ++i)
    if (_this->item(i).type == CMDL_VALUE_SEL && _this->item(i).p_int == pi)
      return i;
  return -1;
}

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

int CmdLineArgs::_set_option_id(CmdLineOption& cmdlo, int option_id) {
  if (!_initialized)
    throw string{"the method CmdLineArgs::init() must be called first"};
  if (option_id == -1)
    cmdlo.option_id = ++_curOption;
  else if (_curOption < option_id)
    cmdlo.option_id = _curOption = option_id;
  else
    cmdlo.option_id = option_id;
  return cmdlo.option_id;
}

int CmdLineArgs::option(const CmdLineOption& r) {
  _check_parsed();
  if (!(r.type < CMDL_NUM))
    throw string{"invalid option id"};
  if (r.option_id == CMDL_USER && r.option_handler == 0)
    throw string{"invalid option handler for user handled option (CMDL_USER)"};

  CmdLineOption cmdlo = r;
  const int oid = _set_option_id(cmdlo, cmdlo.option_id);
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::option(const char *option, const char *info,
                        double *fp, int option_id) {
  _check_parsed();
  if (!f_check_option(option))
    throw string{"attempt to add an empty option: "} + option;
  if (fp == 0)
    throw string{"invalid data for floating point data option"};

  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, option_id);
  cmdlo.type = CMDL_FP;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.p_fp = fp;
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::option(const char *option, const char *info,
                        bool *pb, int option_id) {
  _check_parsed();
  if (!f_check_option(option))
    throw string{"attempt to add an empty option: "} + option;
  if (pb == 0)
    throw string{"invalid data for boolean option"};

  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, option_id);
  cmdlo.type = CMDL_BOOL;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.p_bool = pb;
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::option(const char *option, const char *info,
                        int *pi, int option_id) {
  _check_parsed();
  if (!f_check_option(option))
    throw string{"attempt to add an empty option: "} + option;
  if (pi == 0)
    throw string{"invalid data for integer option"};

  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, option_id);
  cmdlo.type = CMDL_INT;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.p_int = pi;
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::option(const char *option, const char *info,
                        char **pstr, int option_id) {
  _check_parsed();
  if (!f_check_option(option))
    throw string{"attempt to add an empty option: "} + option;
  if (pstr == 0)
    throw string{"invalid data for string option"};

  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, option_id);
  cmdlo.type = CMDL_STR;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.p_const_str = pstr;
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::option(const char *option, const char *info,
                        std::string *pstr, int option_id) {
  _check_parsed();
  if (!f_check_option(option))
    throw string{"attempt to add an empty option: "} + option;

  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, option_id);
  cmdlo.type = CMDL_STDSTR;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.p_str = pstr;
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::option(const char *option, const char *info,
                        int value, int *pi, int option_id) {
  _check_parsed();
  if (!f_check_option(option))
    throw string{"attempt to add an empty option: "} + option;
  if (pi == 0)
    throw string{"invalid data for value selection (CMDL_VALUE_SEL)"};
  string name(option);
  if (name.length() != 0 && name[name.length() - 1] == _assignChar)
    throw string{"incorrect name for selection option"};

  int index = f_find_value_sel_option(this, pi);
  int optid = index == -1 ? option_id : item(index).option_id;

  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, optid);
  cmdlo.type = CMDL_VALUE_SEL;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.p_int = pi;
  cmdlo.value = value;
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::option(const char *option, const char *info,
                        CmdlOptionHandler f, void *param, int option_id) {
  _check_parsed();
  if (!f_check_option(option))
    throw string{"attempt to add an empty option: "} + option;
  if (f == 0)
    throw string{"invalid option handler for user handled option (CMDL_USER)"};

  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, option_id);
  cmdlo.type = CMDL_USER;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.param = param;
  cmdlo.option_handler = f;
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::option(const char *option, const char *info,
                        CmdlOptionLambda f, int option_id) {
  _check_parsed();
  if (!f_check_option(option))
    throw string("attempt to add an empty option: ") + option;
  if (f == 0) throw string{"invalid option handler for user handled option "
                           "(CMDL_USER_LAMBDA)"};

  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, option_id);
  cmdlo.type = CMDL_USER_LAMBDA;
  cmdlo.option = option;
  cmdlo.info = info;
  cmdlo.param = 0;
  cmdlo.option_lambda = f;
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::alias(const char *option, int option_id, int alias_id) {
  _check_parsed();

  CmdLineOption co = {alias_id};
  int idx = alias_id == -1 ? size() - 1 : index(co);
  if (idx == -1) throw string{alias_id == -1 ?
      "incorrect option alias: no options are present" :
      "incorrect option alias: option not found"};

  const CmdLineOption& src = item(idx);
  CmdLineOption cmdlo;
  const int oid = _set_option_id(cmdlo, option_id);
  cmdlo.type    = src.type;
  cmdlo.option  = option;
  cmdlo.info    = src.info;
  cmdlo.param   = src.param;
  cmdlo.value   = src.value;
  switch (src.type) {
    case CMDL_USER: cmdlo.option_handler = src.option_handler; break;
    case CMDL_USER_LAMBDA: cmdlo.option_lambda = src.option_lambda; break;
  }
  emplace_back(std::move(cmdlo));
  return oid;
}

int CmdLineArgs::parse_cmd_line(unsigned/*used by GNU argp parser*/) {
  if (_parsed)
    throw string{"command line options were previously parsed"};
  if (!_initialized)
    throw string{"the method CmdLineArgs::init() must be called first"};
  _parsed = true;

  for (int i = 1; i < _argc; ++i) {
    const string option(_argv[i]);
    string optarg("");
    string optvar(option);

    string::size_type pos = option.find('=');
    if (pos != string::npos) {
      optarg = option.substr(pos + 1);
      optvar = option.substr(0, pos + 1);
    }

    bool bFound = false;

    for (int j = 0; j < (int)size(); ++j) {
      if (optvar.compare(item(j).option) == 0) {
        if (_addedIds.index(item(j).option_id) != -1) {
          string msg = string{"option '"} + option +
                       "' conflicts with previous options";
          if (_conflictFatal) throw msg;
          _parseMessages.push_back(msg + "; this option was ignored");
        }
        else {
          _addedIds.add(item(j).option_id);
          _addedOptions.add(j);
          // Handle options
          char *after;

          switch (item(j).type) {
            case CMDL_BOOL: {
              int bval = strtol(optarg.c_str(), &after, 0);
              if (*after != 0 || after == optarg.c_str()) {
                if (optarg.compare("true") == 0 ||
                    optarg.compare("yes" ) == 0 ||
                    optarg.compare("on"  ) == 0)
                  *(item(j).p_bool) = true;
                else if (optarg.compare("false") == 0 ||
                         optarg.compare("no"   ) == 0 ||
                         optarg.compare("off"  ) == 0)
                  *(item(j).p_bool) = false;
                else if (optvar[optvar.size() - 1] != '=')
                  *(item(j).p_bool) = true;
                else
                  throw string{"incorrect bool specification '"} + option + "'";
              }
              else {
                if (bval > 1 || bval < 0)
                  throw string{"incorrect bool specification '"} + option + "'";
                *(item(j).p_bool) = (bval != 0);
              }
              break;
            }

            case CMDL_INT:
              *(item(j).p_int) = strtol(optarg.c_str(), &after, 0);
              if (*after != 0 || after == optarg.c_str())
                throw string("incorrect integer specification '") + option + "'";
              break;

            case CMDL_FP:
              *(item(j).p_fp) = strtod(optarg.c_str(), &after);
              if (*after != 0 || after == optarg.c_str())
                throw string{"incorrect floating point number "
                             "specification '"} + option + "'";
              break;

            case CMDL_STR:
              *(item(j).p_const_str) = _argv[i] + (pos + 1);
              break;

            case CMDL_STDSTR:
              *(item(j).p_str) = _argv[i] + (pos + 1);
              break;

            case CMDL_VALUE_SEL:
              // If option ends in '=', ie. it is followed by an argument...
              if (optvar[optvar.size() - 1] == _assignChar)
                throw string{"unexpected argument in selection option"};
              *(item(j).p_int) = item(j).value;
              break;

            case CMDL_USER:
            case CMDL_USER_LAMBDA: {
              // If option ends in '=', ie. it is followed by an argument...
              const char *arg = optvar[optvar.size() - 1] == _assignChar ?
                  optarg.c_str() : 0;
              bool bret = (item(j).type == CMDL_USER) ?
                  item(j).option_handler(this, item(j).param, arg) :
                  item(j).option_lambda(arg);
              if (!bret) exit(0); // stop processing cmds
              break;
            }
          }
        }

        bFound = true;
        break;
      }
    }

    if (!bFound) {
      if (option[0] != '-' && option.find('=') == string::npos) {
        _sources.push_back(option);
      }
      else if (option[0] != '-' && option.find('=') != string::npos
                                && _tolerateAssignChar) {
        _sources.push_back(option);
        _parseMessages.push_back(string{"note: source '"} + option +
                                 "' is possibly an incorrect option");
      }
      else {
        if (_extraFatal) {
          throw string{"unknown option: '"} + option + "'";
        }
        else {
          _parseMessages.push_back(string{"unknown option: '"} + option + "'");
          _mustShowHelp = true;
        }
      }
    }
  }
  return 0; // of use only in argp
}

// =============================================================================
//   Info and documentation methods
// =============================================================================
void CmdLineArgs::set_doc(const string& s) {_doc = s;}
void CmdLineArgs::set_args_doc(const string& s) {_args_doc = s;}
void CmdLineArgs::set_footer(const string&) {}

static bool f_defhelp_handler(CmdLineArgs *_this, void*, const char*) {
  _this->display_help();
  return false;
}

int CmdLineArgs::option_help(const char* optname, const char* info) {
  string name(optname);
  if (name.empty() || name[name.length() - 1] == _assignChar)
    throw string{"incorrect default help option specification '" + name + "'"};
  return option(optname, info, f_defhelp_handler, 0);
}

int CmdLineArgs::group_help(const char* optname, const char* info) {
  string name(optname);
  if (name.empty() || name[name.length() - 1] == _assignChar)
    throw string{"incorrect default help option specification '" + name + "'"};
  return option(optname, info, f_defhelp_handler, 0, _curOption);
}

void CmdLineArgs::display_help(unsigned tab_pos) const {
  using std::cout; using std::endl;
  string header, footer;
  string::size_type pos = _doc.find('\v');
  header = _doc.substr(0, pos);
  if (pos != string::npos)
    footer = _doc.substr(pos + 1);
  string pgm = _pgmName.length() != 0 ? _pgmName : "Program";
  string args = _args_doc.length() == 0 ? "" : " " + _args_doc;
  cout << "Usage: " << _pgmName
       << " [option1 option2 ...]" << args << endl;
  if (header.length() != 0)
    cout << header << endl;
  cout << endl;
  cout << "Available options:" << endl;
  if (tab_pos == 0) tab_pos = 30;
  const int tp = (tab_pos > 60) ? 60 : (int)(unsigned short)tab_pos;
  char buf[512];
  for (int k = 0; k < (int)size(); ++k) {
    string name(item(k).option);
    bool no_arg = name.length() == 0 || name.back() != _assignChar;
    string arg = no_arg ? "" : f_get_default_info_arg(item(k).type);
    sprintf(buf, "%-*s%s\n", tp, (name + arg).c_str(), item(k).info);
    cout << buf;
  }
  if (footer.length() != 0)
    cout << endl << footer << endl;
}

void CmdLineArgs::display_specs() const {
  int i;
  for (i = 0; i < (int)_addedOptions.size(); ++i)
     _display_spec(_addedOptions[i]);

  TypeVector<int> a;
  bool bDefaultValueFound = false;

  for (i = 0; i < (int)size(); ++i) {
    if (_addedOptions.index(i) == -1 &&
        _addedIds.index(item(i).option_id) == -1 &&
        a.index(item(i).option_id) == -1 &&
        item(i).type != CMDL_USER &&
        item(i).type != CMDL_USER_LAMBDA &&
        item(i).type != CMDL_VALUE_SEL) {
      if (!bDefaultValueFound) {
         std::cout << "Default values:" << std::endl;
         bDefaultValueFound = true;
      }
      a.add(item(i).option_id);
      _display_spec(i);
    }
  }
}

void CmdLineArgs::_display_spec(int j) const {
  const char *info = (item(j).info == 0) ? item(j).option : item(j).info;

  switch (item(j).type) {
    case CMDL_BOOL:
      std::cout << info << ": "
                << (*(item(j).p_bool) ? "true" : "false") << std::endl;
      break;
    case CMDL_INT:
      std::cout << info << ": " << *(item(j).p_int) << std::endl;
      break;
    case CMDL_FP:
      std::cout << info << ": " << *(item(j).p_fp) << std::endl;
      break;
    case CMDL_STR:
      std::cout << info << ": \"" << *(item(j).p_const_str) << "\"" << std::endl;
      break;
    case CMDL_STDSTR:
      std::cout << info << ": \"" << *(item(j).p_str) << "\"" << std::endl;
      break;
    case CMDL_VALUE_SEL:
      std::cout << info << " [" << item(j).value << "]" << std::endl;
      break;
    case CMDL_USER:
    case CMDL_USER_LAMBDA:
      std::cout << info << std::endl;
      break;
  }
}

void CmdLineArgs::display_messages() const {
  for (int i = 0; i < (int)_parseMessages.size(); ++i)
    std::cout << _parseMessages[i] << std::endl;
}

// =============================================================================
// Consistency checks
// =============================================================================

void CmdLineArgs::_check_parsed() const {
  if (_parsed)
    throw string{"command line options cannot be added because the "
                 "command line was parsed"};
}

bool CmdLineArgs::_check_lib_type(int lib_type) {
  if (lib_type != 0)
    throw string{"the program was compiled for "} +
      (lib_type == 1 ? "the argp " : "an unknown ") +
      "interface but linked to the free-style library";
  return true;
}
