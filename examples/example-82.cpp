#include <iostream>
#include "clparser.hpp"

using std::cout; using std::endl; using std::string;

int main(int argc, char *argv[]) {
  bool opt = false;
  try {
    CmdLineArgs cl(argc, argv);
    cl.set_doc("example-82 -- aliases.");
    cl.option("option", "enable feature 'option'", &opt, 'o');
    cl.alias ("enable-option");
    cl.parse();
  }
  catch (string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  cout << "option: " << string(opt ? "en" : "dis") + "abled" << endl;

  return 0;
}
