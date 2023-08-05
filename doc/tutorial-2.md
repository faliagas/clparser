[ [Prev <](tutorial-1.html "Basic types") ] [ [Next >](tutorial-3.html "Mutually Exclusive Options - Aliases") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 2: Displaying Reports And Executing Commands

If the command line specification of basic data types is enough for your application (as it is for example the case with programs intended for scientific calculations), you can stop this tutorial after Section 2.1 or proceed to Part II. In this second lesson of the tutorial you will learn how to

- automatically display a report of command line specifications

- automatically display help, and

- direct the program to execute specific commands (_callbacks_) during command line parsing.


#### 2.1 Displaying reports

As we have seen, communication of the program's parameters to the user is often necessary after processing the program's arguments. **Clparser** offers an automated method to do this.

Carrying on with example-16, we replace the `cout << ...` statement, which we used to display a report of the user's specification, with the **CmdLineArgs**-provided method **display_specs()** (`example-21.cpp`):

```c++
cout << "Command line specifications:" << endl;
cl.display_specs();
```

Place these two statements below the `cl.display_messages()` statement.
Run the compiled program with the arguments `-name="John Doe" -salary=35000` to get the following output:

```bash
$ Command line specifications:
$ employee name: "John Doe"
$ employee salary: 35000
$ Default values:
$ employee age: 0
$ marital status: false
```

The standard **CmdLineArgs** method **display_specs()** prints the user's specifications using the information provided by the second argument to the `option` method calls or, if null, the option's name (first argument of `option`). Parameters with default values, i.e., those not changed by the user, come under _Default values_.

#### 2.2 Executing commands

Let us assume we would like to have our program display its version upon the user's request. We could easily handle it by defining a boolean and inserting an `if` statement to check the user's specification. **Clparser** provides a more elegant method requiring less coding effort: _command line argument handlers_. Let's see it in action with the following example (example-22):

```c++
string ver = "1.2.3";
try {
  CmdLineArgs cl(argc, argv);
  cl.option("--version", "Display program version", show_ver, &ver);
  cl.parse();
  cl.display_messages();
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
```

Here is the code for the `show_ver` function:

```c++
bool show_ver(CmdLineArgs* self, void* p) {
  string *pstr = static_cast<string *>(p);
  cout << "Program " << self->program_name() << " version: " << *pstr << endl;
  return false;
}
```

Alternatively, one might use _lambdas_ for a more elegant and compact code (example-22a):

```c++
string ver = "1.2.3";
try {
  CmdLineArgs cl(argc, argv);
  cl.option("--version", "Display program version",
    [ver, &cl](const char*) {
      cout << "Program " << cl.program_name() << " version: " << ver << endl;
      return false;
    });
  cl.parse();
  cl.display_messages();
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
```

The `return false` statement signals that the program must be terminated after the lambda has returned. If the lambda (or function) returns `true` the program continues running uninterruptedly.

The output, when the program is given the argument `--version`, is

```bash
$ Program example-22a version: 1.2.3
```

The next example (example-22b.cpp) shows how to add support for command line help.

```c++
string ver = "1.2.3";
int num = 0;
try {
  CmdLineArgs cl(argc, argv);
  cl.set_doc("example-22b -- my first program using help.");
  cl.option("--help", "Display help", [&cl](const char*) {
      cl.display_help();
      return false;
    });
  cl.option("--version", "Display program version", [ver, &cl](const char*) {
      cout << cl.program_name() << " version: " << ver << endl;
      return false;
    });
  cl.option("--num=", "Number of objects", &num);
  cl.parse();
  cl.display_messages();
  cout << "Command line specification:" << endl;
  cl.display_specs();
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
```

> Lambdas and handlers are run when `parse` is called.

Run the program with the argument `--help`. The result will be:

```bash
$ Usage: example-22b [option1 option2 ...]
$ example-22b -- my first program using help.
$
$ Available options:
$ --help              Display help
$ --version           Display program version
$ --num=VALUE         Number of objects
```

The next less trivial example (example-22c.cpp) collects employee data (see example-16) from the command line and adds them into a database. Employee records are '`;`'-separated and employee data are '`,`'-separated.

```c++
vector<Employee> db;
try {
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
      while (std::getline(ss, record, ';')) {
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
catch (const std::string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
catch (std::invalid_argument& a) {
  cout << "error: " << "An invalid argument was supplied" << endl;
  return 1;
}
```

Run the program with the argument `-employees="John Doe,45,3500.75,yes;;Jack Smith,52,32800,yes;Jim Last,62,40000,no"` to get:

```bash
$ Command line specifications:
$ Employee data separated by ';'
$ Employee printout:
$ name   : John Doe
$ age    : 45
$ salary : 3500.75
$ married: yes
$ name   : noname
$ age    : 0
$ salary : 0
$ married: no
$ name   : Jack Smith
$ age    : 52
$ salary : 32800
$ married: yes
$ name   : Jim Last
$ age    : 62
$ salary : 40000
$ married: no
```
> Enclosing the argument in quotation marks is necessary because it contains spaces.

[ [Prev <](tutorial-1.html "Basic types") ] [ [Next >](tutorial-3.html "Mutually Exclusive Options - Aliases") ] [ [Up](tutorial.html "Table of Contents") ]
