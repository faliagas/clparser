#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {
  string ver = "1.2.3";
  int num = 0;

  try {
    CmdLineArgs cl(argc, argv);
    cl.set_doc("example-22b -- my first program using help.");
    cl.option("--help", "Display help", [&cl](const char*) {
        cl.display_help();
        return false;
      });
    cl.option("--version", "Display program version", [ver, &cl](const char*) {
        cout << cl.program_name() << " version: " << ver << endl;
        return false;
      });
    cl.option("--num=", "Number of objects", &num);
    cl.parse();
    cl.display_messages();
    cout << "Command line specification:" << endl;
    cl.display_specs();
  }
  catch (string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  return 0;
}
