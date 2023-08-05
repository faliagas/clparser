#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;

struct Employee {
  string  name;
  int     age;
  double  salary;
  bool    married;
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
  }
  catch (string& error_msg) {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  cout << "Employee data:" << endl
       << "  name:    " << employee.name   << endl
       << "  age:     " << employee.age    << endl
       << "  salary:  " << employee.salary << endl
       << "  married: " << (employee.married ? "yes" : "no") << endl;

  return 0;
}
