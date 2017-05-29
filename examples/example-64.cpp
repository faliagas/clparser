#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;

struct Employee
{
  string name;
  int    age;
  double salary;
};

const char *argp_program_version = "1.0";
const char *argp_program_bug_address = "<apostol.faliagas@gmail.com>";

int main(int argc, char *argv[])
{
  Employee employee = {"noname", 0, 0.};

  try
  {
    CmdLineArgs cl(argc, argv);
    cl.set_doc("Clparser argp example 6.4 -- Integer, string and FP options.");
    cl.option("name", "employee name", &employee.name);
    cl.option("age", "employee age", &employee.age);
    cl.option("salary", "employee salary", &employee.salary);
    cl.parse();
  }
  catch (string& error_msg)
  {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  cout << "Employee data:" << endl
       << "  name:   " << employee.name   << endl
       << "  age:    " << employee.age    << endl
       << "  salary: " << employee.salary << endl;

  return 0;
}
