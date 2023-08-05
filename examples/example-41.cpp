#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {
  int num = 1;
  bool displaySpecs = false;
  try {
    CmdLineArgs cl(argc, argv);
    cl.option_help("--help", "display help");
    cl.option("num=", "number of objects", &num);
    cl.option("--display-specs", "display specs", &displaySpecs);
    cl.parse();
    cl.display_messages();
    if (displaySpecs) {
      cout << endl << "Command line specifications:" << endl;
      cl.display_specs();
    }
  }
  catch(string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  return 0;
}
