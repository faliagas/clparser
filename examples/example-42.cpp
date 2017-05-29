#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;

struct Employee
{
  string      name;
  int         age;
  double      salary;
  bool        married;
};

int main(int argc, char *argv[])
{
  Employee employee = {"noname", 0, 0., false};
  try
  {
    CmdLineArgs cl(argc, argv);
    cl.option_help("--help", "display help");
    cl.option("-name=", "employee name", [&employee](const char *arg) {
        if (string(arg).length() > 50)
        {
          cout << "employee name too long" << endl;
          return false; // this causes program termination
        }
        employee.name = arg;
        return true;
      });
    cl.option("-age=", "employee age", [&employee](const char *arg) {
        size_t sz;
        int age = std::stoi(string(arg), &sz);
        if (sz < string(arg).length())
        {
          cout << "incorrect number for employee age" << endl;
          return false; // this causes program termination
        }
        if (age < 18 || age > 70)
        {
          cout << "employee age out of range" << endl;
          return false; // this causes program termination
        }
        employee.age = age;
        return true;
      });
    cl.option("-salary=", "employee salary", &employee.salary);
    cl.option("-married", "marital status", &employee.married);
    cl.parse();
    cl.display_messages();
  }
  catch (std::string& error_msg)
  {
    cout << "error: " << error_msg << endl;
    return 1;
  }
  catch (std::invalid_argument& e)
  {
    cout << "incorrect number for employee age" << endl;
    return 1;
  }

  cout << "Employee data:" << endl
       << "  name:    " << employee.name   << endl
       << "  age:     " << employee.age    << endl
       << "  salary:  " << employee.salary << endl
       << "  married: " << (employee.married ? "yes" : "no") << endl;

  return 0;
}
