#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[]) {
  int num = 1;

  try {
    CmdLineArgs cl(argc, argv);
    cl.option("-num=", "number of objects", &num);
    cl.parse();
  }
  catch (string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  cout << "num = " << num << endl;

  return 0;
}
