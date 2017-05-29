[ [Prev <](tutorial-6.html "Argp-Style Argument Handling") ] [ [Next >](tutorial-8.html "Selections and Aliases (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 7: Displaying Information and Executing Commands

If the command line specification of basic data types is enough for your application (as it is for example the case with programs performing scientific calculations), after section 7.1 you can terminate this series of tutorials. In this lesson you will learn how to

- display help and usage information

- direct the program to execute specific commands (_callbacks_ or _lambdas_) during command line parsing, and

- handle internal **argp** parse events.

#### 7.1 Displaying information
In certain situations it is necessary to manually display help or program usage to the user and possibly stop the program.

Assume that in `example-62` our program additionally expects at least one source. Code `example-71.cpp` contains the necessary changes:

```c++
int num = 1;
vector<string> sources;
try
{
  CmdLineArgs cl(argc, argv);
  cl.set_doc("Clparser argp example 7.1 -- Displaying information.");
  cl.set_args_doc("ARG1 ...");
  cl.option("num", "number of objects", &num, 'n');
  cl.parse();
  if (cl.sources().size() < 1)
  {
    cl.display_usage();
    return 3;
  }
  sources = cl.sources();
}
catch (string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "num = " << num << endl;
cout << "sources:" << endl;
for (auto source: sources)
  cout << source << " ";
cout << endl;
```

If we run the compiled program with the arguments `--num=3 source-A source-B source-C source-D` we will get following output:

```
$ num = 3
$ sources:
$ source-A source-B source-C source-D
```

In place of `cl.display_usage()` we can use `cl.display_help()`, which displays a summary of the options. The output of both methods can be customized by supplying an `unsigned` [flags](https://www.gnu.org/software/libc/manual/html_node/Argp-Help-Flags.html) argument.

>Note that no **display_specs()** method is available in the **argp** interface.

#### 7.2 Executing commands
Let us assume that in `example-62` `num` must be at least 1 and not greater than 16. Of course, this can be checked after all command line options have been parsed. **Clparser** provides a more powerful method: _command line argument handlers_ and _lambdas_.

The code of the next program is in `example-72.cpp`.

```c++
int num = 1;
try
{
  CmdLineArgs cl(argc, argv);
  cl.set_doc("Clparser argp example 7.2 -- Argument handling.");
  cl.option("num", "number of objects, where $INTEGER$ is from 1 to 16",
    [&num](const char *arg)
    {
      size_t sz;
      num = std::stoi(arg, &sz);
      if (sz < string(arg).length())
        throw string("An invalid integer was supplied for option num");
      if (num < 1 || num > 16)
        throw string("num is out of range 1 to 16");
      return false;
    });
  cl.parse();
}
catch (string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
catch (std::invalid_argument& a)
{
  cout << "error: " << "An invalid integer was supplied for option num" << endl;
  return 1;
}
cout << "num = " << num << endl;
```

Instead of _lambdas_, _argument handlers_ can be alternatively used (see Tutorial 2.2). The `return false` statement signals that program execution must be terminated after the lambda has returned. If the lambda (or function) returns `true` the program keeps on running.

The output, when the program is given the argument `--num=23` is

```
$ error: An invalid integer was supplied for option num
```

And if we give it an incorrect argument `--num=2s3`

```
$ error: An invalid integer was supplied for option num
```

> Lambdas and handlers are run when `parse` is called.

The next less trivial example (`example-72a.cpp`) collects employee data (see example-16) from the command line and adds them into a database. Employee records are `;`-separated and employee data are `,`-separated.

```c++
vector<Employee> db;
try
{
  CmdLineArgs cl(argc, argv);
  cl.set_doc("example-72a -- add employee records to database.\n"
             "Employee records are supplied as name,age,salary,married"
             " separated by ';'.");
  cl.option("employees", "Employee data separated by ';'$REC1;REC2...",
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
}
catch (string& error_msg)
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
```

If we run the program with the argument `--employees="John Doe,45,3500.75,yes;;Jack Smith,52,32800,yes;Jim Last,62,40000,no"` we obtain:

```
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

We can take advantage of the fact that **argp** allows multiple specifications of the same option (FSI does not by default) to simplify somewhat the parsing process as follows (`example-72b.cpp`):

```c++
vector<Employee> db;
try
{
  CmdLineArgs cl(argc, argv);
  cl.set_doc("example-72b -- add employee records to database.\n"
             "Each employee record is supplied as name,age,salary,married.");
  cl.option("employee", "Employee data in the form 'name,age,salary,married'",
    [&db](const char *arg) {
      Employee employee = parse_employee(string(arg));
      db.push_back(employee);
      return true;
    });
  cl.parse();
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
```

The database printing statements are as previously. If we run the program with the argument `--employee="John Doe,45,3500.75,yes" --employee="" --employee="Jack Smith,52,32800,yes" --employee="Jim Last,62,40000,no"` we obtain exactly the same output.

[ [Prev <](tutorial-6.html "Argp-Style Argument Handling") ] [ [Next >](tutorial-8.html "Selections and Aliases (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]
