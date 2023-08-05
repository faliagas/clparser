#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {
  int num = 1;
  vector<string> sources;
  try {
    CmdLineArgs cl(argc, argv);
    cl.set_doc("Clparser argp example 9.1 -- Help footers.");
    cl.set_args_doc("ARG1 ...");
    cl.set_footer(
               "Footer text:\n"
               "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed "
               "do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
               "Ut enim ad minim veniam, quis nostrud exercitation ullamco "
               "laboris nisi ut aliquip ex ea commodo consequat. Duis aute "
               "irure dolor in reprehenderit in voluptate velit esse cillum "
               "dolore eu fugiat nulla pariatur. Excepteur sint occaecat "
               "cupidatat non proident, sunt in culpa qui officia deserunt "
               "mollit anim id est laborum.\n"
               "\n"
               "Pri quas audiam virtute ut, case utamur fuisset eam ut, iisque "
               "accommodare an eam. Reque blandit qui eu, cu vix nonumy "
               "volumus. Legendos intellegam id usu, vide oporteat vix eu, "
               "id illud principes has. Nam tempor utamur gubergren no.");
    cl.option("num", "number of objects", &num, 'n');
    cl.parse();
    if (cl.sources().size() < 1) {
      cl.display_usage();
      return 3;
    }
    sources = cl.sources();
  }
  catch (const string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }
  cout << "num = " << num << endl;
  cout << "sources:" << endl;
  for (auto source: sources)
    cout << source << " ";
  cout << endl;
  return 0;
}
