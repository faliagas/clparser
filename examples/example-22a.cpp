#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
  std::string ver = "1.2.3";

  try {
    CmdLineArgs cl(argc, argv);
    cl.option("--version", "Display program version", [ver, &cl](const char*) {
        cout << "Program " << cl.program_name() << " version: "
             << ver << endl;
        return true;
      });
    cl.parse();
    cl.display_messages();
    cout << "Command line specifications:" << endl;
    cl.display_specs();
  }
  catch (std::string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  return 0;
}
