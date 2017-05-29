#include <iostream>
#include <sstream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

struct Employee
{
  std::string name;
  int         age;
  double      salary;
  bool        married;
};

static bool get_bool(string& bstr)
{
  bool rv = bstr == "yes" || bstr == "true" || bstr == "on";
  if (rv) return true;
  rv = !(bstr == "no" || bstr == "false" || bstr == "off");
  if (rv) throw string("Incorrect boolean specification in record");
  return false;
}

static Employee parse_employee(const string& record)
{
  Employee employee = {"noname", 0, 0., false};
  std::stringstream ssr(record);
  string field;
  for (int i = 0; std::getline(ssr, field, ','); ++i)
    switch (i)
    {
      case 0: employee.name = field; break;
      case 1: employee.age = std::stoi(field); break;
      case 2: employee.salary = std::stod(field); break;
      case 3: employee.married = get_bool(field); break;
      default: throw string("Too many fields for record");
    }
  return employee;
}

int main(int argc, char *argv[])
{
  vector<Employee> db;
  try
  {
    CmdLineArgs cl(argc, argv);
    cl.set_doc("example-22c -- add employee records to database.\n"
               "Employee records are supplied as name,age,salary,married"
               " separated by ';'.");
    cl.option("--help", "Display help", [&cl](const char*) {
        cl.display_help();
        return false;
      });
    cl.option("-employees=", "Employee data separated by ';'",
      [&db](const char *arg) {
        std::stringstream ss(arg);
        string record;
        while (std::getline(ss, record, ';'))
        {
          Employee employee = parse_employee(record);
          db.push_back(employee);
        }
        return true;
      });
    cl.parse();
    cl.display_messages();
    cout << "Command line specifications:" << endl;
    cl.display_specs();
  }
  catch (std::string& error_msg)
  {
    cout << "error: " << error_msg << endl;
    return 1;
  }
  catch (std::invalid_argument& a)
  {
    cout << "error: " << "An invalid argument was supplied" << endl;
    return 1;
  }

  cout << "Employee printout:" << endl;
  for (auto employee: db)
    cout << "name   : " << employee.name   << endl
         << "age    : " << employee.age    << endl
         << "salary : " << employee.salary << endl
         << "married: " << string(employee.married ? "yes" : "no") << endl;

  return 0;
}
