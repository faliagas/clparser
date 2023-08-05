#include <iostream>
#include "clparser.hpp"

using std::cout; using std::endl;
using std::string; using std::vector;

int main(int argc, char *argv[]) {
  vector<string> fruits = {"apple", "orange", "peach", "melon"};
  int selection = -1;
  bool displaySpecs = false;
  try {
    CmdLineArgs cl(argc, argv);
    //cl.set_conflict_fatal();
    cl.set_args_doc("-[apple|orange|peach|melon]");
    cl.option_help("--help", "display help");
    cl.option("-apple", "apple", 0, &selection);
    cl.option("-orange", "orange", 1, &selection);
    cl.option("-peach", "peach", 2, &selection);
    cl.option("-melon", "melon", 3, &selection);
    cl.option("-display-specs=", "display specs", &displaySpecs);
    cl.parse();
    cl.display_messages();
    if (selection == -1)
      throw string("One of -apple -orange -peach -melon must be selected");
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
  cout << "Selected fruit: " << fruits[selection] << endl;

  return 0;
}
