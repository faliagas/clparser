[ [Prev <](tutorial-3.html "Mutually Exclusive Options - Aliases") ] [ [Next >](tutorial-5.html "Customization and Advanced Techniques") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 4: Sources and Data Validation

In this lesson you will learn how to

- get access to sources specified at the command-line

- perform data validation

#### 4.1 Sources
Command line arguments that are not options are called **sources**. For example, in the command line
```bash
$ gcc -g --output=test test.c
```
`-g` and `--output=test` are options, while `test.c` is a source. Every argument starting with `-` is an option. However, **clparser** will happily accept as options arguments not starting with a `-`. Take a look at following code (`example-41.cpp`):

```c++
int num = 1;
bool displaySpecs = false;
try
{
  CmdLineArgs cl(argc, argv);
  cl.option_help("--help", "display help");
  cl.option("num=", "number of objects", &num);
  cl.option("--display-specs", "display specs", &displaySpecs);
  cl.parse();
  cl.display_messages();
  if (displaySpecs)
  {
    cout << endl << "Command line specifications:" << endl;
    cl.display_specs();
  }
}
catch(string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
```

The output of `example-41 --display-specs num=23` is
```bash
$ Command line specifications:
$ display specs: true
$ number of objects: 23
```

**Clparser** treats arguments which have been declared with an **option** statement as options. If it encounters an argument which contains `=` but was not declared by an **option** statement, it considers it an erroneous option and issues a warning message. Similarly, if it begins with a `-` but it was not previously declared, it is also considered an erroneous option. All other arguments are considered to be **sources**. For example, in `example-41 --display-specs num=23 another-num=4 a-source` the argument `a-source` is considered a source.
```bash
$ Unknown option: 'another-num=4'
$
$ Command line specifications:
$ display specs: true
$ number of objects: 23
```

**Clparser** collects all sources in string vector. To access it one uses the **CmdLineArgs::sources()** method. Here is how to modify the previous code in order to access the sources container:

```c++
int num = 1;
bool displaySpecs = false;
vector<string> sources;
try
{
  CmdLineArgs cl(argc, argv);
  cl.option_help("--help", "display help");
  cl.option("num=", "number of objects", &num);
  cl.option("--display-specs", "display specs", &displaySpecs);
  cl.parse();
  cl.display_messages();
  if (displaySpecs)
  {
    cout << endl << "Command line specifications:" << endl;
    cl.display_specs();
  }
  sources = cl.sources();
}
catch(string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "Program sources: ";
for (auto source: sources) cout << source << " ";
cout << endl;
```

The output of
```shell
$ example-41a --display-specs num=23 another-num=4 another-num abcd 1234
```
will be
```shell
$ Unknown option: 'another-num=4'
$
$ Command line specifications:
$ display specs: true
$ number of objects: 23
$ Program sources: another-num abcd 1234
```

#### 4.2 Data validation
User input can be checked after all command line options have been parsed. **Clparser** offers the possibility to check user input at the time the associated command-line argument is parsed and accordingly prompt the user for a modification or terminate the program, when the data do not qualify. To take advantage of this feature one uses _lambdas_ or _option handlers_. Assume that in our employee example the `age` must be between 18 and 70 and the `name` string cannot be longer than 50 characters. Here is a modification of `example-16` which performs the required data validation:

```c++
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
```

The last `catch` block is necessary because when the user specifies `-age=a45` STL throws a `std::invalid_argument` instead of setting `sz` to 0. The **display_specs()** method is not of much use here because it does not print anything for the options `-name` and `-age`.

[ [Prev <](tutorial-3.html "Mutually Exclusive Options - Aliases") ] [ [Next >](tutorial-5.html "Customization and Advanced Techniques") ] [ [Up](tutorial.html "Table of Contents") ]
