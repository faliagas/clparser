#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;

struct Employee {
  std::string name;
  int         age;
  double      salary;
  bool        married;
};

int main(int argc, char *argv[]) {
  Employee employee = {"noname", 0, 0., false};

  try {
    CmdLineArgs cl(argc, argv);
    cl.option("-name=", "employee name", &employee.name);
    cl.option("-age=", "employee age", &employee.age);
    cl.option("-salary=", "employee salary", &employee.salary);
    cl.option("-married", "marital status", &employee.married);
    cl.parse();
    cl.display_messages();
    cout << "Command line specifications:" << endl;
    cl.display_specs();
  }
  catch (std::string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  while (true) {
    char ch;
    cout << "Continue (y/n) ? "; std::cin >> ch;
    if (ch == 'y' || ch == 'Y') break;
    if (ch == 'n' || ch == 'N') return 0;
    cout << "Incorrect specification: '" << ch << "'" << endl;
  }

  return 0;
}
