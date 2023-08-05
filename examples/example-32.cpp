#include <iostream>
#include "clparser.hpp"

using std::cout; using std::endl;
using std::string; using std::vector;

int main(int argc, char *argv[]) {
  bool opt = false;
  bool displaySpecs = false;
  try {
    CmdLineArgs cl(argc, argv);
//    cl.set_conflict_fatal();
    cl.option_help("--help", "display help");
    cl.option("-o", "enable option", &opt);
    cl.group ("-o=", "enable/disable option", &opt);
    cl.group ("--option=", "enable/disable option", &opt);
    cl.option("-display-specs", "display specs", &displaySpecs);
    cl.parse();
    cl.display_messages();
    if (displaySpecs) {
      cout << endl << "Command line specifications:" << endl;
      cl.display_specs();
    }
  }
  catch (string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  cout << endl;
  cout << "option: " << string(opt ? "en" : "dis") + "abled" << endl;

  return 0;
}
