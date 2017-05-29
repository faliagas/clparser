[ [Next >](tutorial-2.html "Displaying Reports and Executing Commands") ] [ [Up](tutorial.html "Table of Contents") ]

# Command Line Parser Tutorial

_Document Version_: 1.0 (Sun 28 May 2017 23:01:36 EEST)

Command Line Parser (**clparser**) comes in two flavors: **argp** and **free-style**. The **argp** interface is a front-end to **argp**, which is compliant with the **GNU standard command line format**. The free-style interface (FSI) concentrates on simplicity and efficiency, without caring about GNU compliance. Conversion from one style to the other is not hard. We start our tutorials with the FSI.

## PART I: Free-Style Argument Handling

### Lesson 1: Command Line Specification of Basic Types

In this lesson you will learn how to program command line argument specifications for

- integers
- floating point numbers
- booleans, and
- strings

#### 1.1 The first steps

Assume we have the following program

```c++
#include <iostream>
using namespace std;
int main(int argc, char *argv[])
{
  int num = 1;
  cout << "num = " << num << endl;
  return 0;
}
```

and we would like to make the variable `num` accessible to the program's user, so that its default value 1 can be changed from the command line.

To do this we create an instance of **CmdLineArgs**, and tell it our intension to handle a command-line argument `-num` as follows (we are displaying the main function's body only without the `return` statement):

```c++
int num = 1;
CmdLineArgs cl(argc, argv);
cl.option("-num=", "number of objects", &num);
cl.parse();
cout << "num = " << num << endl;
```

On line 2 we create an instance of **CmdLineArgs**, and in the next line we use the **CmdLineArgs::option()** method to declare that our program (in its binary executable form) will be expecting an argument `-num=xxx`, where `xxx` is an integer, as in the following example:

```shell
$ prog -num=12
```

Of course, the user may run the program without the argument. The second argument of the **option** method is informative and is used in formatting error massages and specification reports. We'll get to talk about this later. The last argument is a pointer to the variable that will receive the value specified at the command line. The  **option** method has an additional optional argument, which is a suggested `id` for the argument. The returned value is the `id` that was actually assigned by the system. The recommended use of the **option** method, in the free-style interface, is with the default (no argument) `id` value.

In the following line we ask `cl` to _parse the command line_, and, if the user has specified an argument `-num=xxx`, change the value of `num` to the the supplied value `xxx`. Finally, we print the value of `num`.

The code of this example is found in the file `example-11.cpp`. To create the executable program you need to also compile `clparser.cpp` and link them together, or link to the `clparser` library. The header file `clparser.hpp` must be included in the main source file. For convenience, one may use the supplied `codeblocks` project file `example-11.cbp`. For direct compilation with debugging information use the following commands in the `examples` subdirectory:

```shell
$ g++ -Wall -g -I../include -c ../src/clparser.cpp -o clparser.o
$ g++ -Wall -g -I../include -c example-11.cpp -o example-11.o
$ g++ -o example-11 example-11.o clparser.o
```

The `-Wall` and `-g` switches are of course optional. Alternatively, the convenience script `compile-clparser-examples`, found in the `bin` subdirectory of the installation directory, can be used to compile and link with the `clparser` library:
```shell
$ compile-clparser-examples example-11
```
> `compile-clparser-examples --help` displays the options available to compile an example or your program. `compile-clparser-examples` without any arguments compiles with default options and installs all examples.

To use the program, issue at the command prompt, for example,

```shell
$ ./example-11 -num=12
```

The output is

```shell
$ num = 12
```

#### 1.2 Adding exception handling
Let us experiment a little with our program and use it with an incorrect argument, say `-num=12y`. We obtain a message like this:

```sh
$ terminate called after throwing an instance of 'std::string'
```

As it is usually the case with C++ programming, we have to `catch` the unexpected condition and handle it. Look at the following modification of our code (see `example-12.cpp`):

```c++
int num = 1;
try
{
  CmdLineArgs cl(argc, argv);
  cl.option("-num=", "number of objects", &num);
  cl.parse();
}
catch (string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "num = " << num << endl;
```

All errors in **CmdLineArgs** are thrown as `std::string`s, so that their handling is quite simple (see code in the `catch` block). Compile as previously and run with the argument `-num=12y`. The result is now:

```sh
$ error: Incorrect integer specification '-num=12y'
```

#### 1.3 Handling unexpected arguments
Continuing with our experimentation, let us use it with an additional (unexpected) argument besides `-num=12`, say `-a=3`. We see that nothing particular happens; we get the same output as previously:

```sh
$ num = 12
```

and no error message is printed. The same no-error response we obtain if we specify the argument `num=12` (without `-`), this time with the default value of `num`:

```sh
$ num = 1
```

The reason is that the free-style (non-argp) version of **CmdLineArgs** by default tolerates extra (unexpected) arguments, issuing only a warning message. The program must handle this message (we will see later how to do this), otherwise its behavior will be unusual and unexpected to the user, as we have seen.

We can make **CmdLineArgs** less tolerant of extra arguments by adding the statement `cl.set_extra_fatal()` below `cl`'s declaration, so that now our code looks like this (see `example-13.cpp`):

```c++
int num = 1;
try
{
  CmdLineArgs cl(argc, argv);
  cl.set_extra_fatal();
  cl.option("-num=", "number of objects", &num);
  cl.parse();
}
catch(std::string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "num = " << num << endl;
```

Now, if we compile and run with the arguments `-num=12 -a=5` or `num=3` we get an error message:

```sh
$ error: Unknown option: '-a=6'
$ error: Unknown option: 'num=6'
```

#### 1.4 Displaying warning messages
Assume that we would like to have our program tolerate extra arguments. This means that the program does not interrupt its execution but issues a message informing the user of what is going on. Here is how simple it is to do this with **CmdLineArgs**: remove the statement `cl.set_extra_fatal()` below the `cl` declaration, and insert at the end of the `try` block the statement:

```c++
cl.display_messages();
```

Compile and run with the arguments `-num=12 -a=5`. Here is the output:

```sh
$ Unknown option: '-a=6'
$ num = 12
```

Of course, nothing is printed if there are no messages. To check the presence of messages use the `messages` method. For example, you can make all warning messages fatal errors as follows:

```c++
if (cl.messages())
{
  cout << "The following errors were encountered:" << endl;
  cl.display_messages();
  return 1;
}
```

If we run the program with the argument `num=12` we obtain:

```sh
$ Unknown option: 'num=12'
$ num = 1
```

However, if we run the program with the argument `num` no error message is obtained:

```sh
$ num = 1
```

This happens because **CmdLineArgs** considers the argument `num` a **source** instead of a program option. We will get to speak about sources later on.

The code of this section is `example-14.cpp`.

#### 1.5 Floating point and string (character) data
The specification of floating-point and string data in the command line is much the same as the specification of integers. Assume, we have employee data, as for example name, age and salary, and we would like to make them accessible from the command line.

```c++
struct Employee
{
  string name;
  int    age;
  double salary;
};

Employee employee = {"noname", 0, 0.};

try
{
  CmdLineArgs cl(argc, argv);
  cl.option("-name=", "employee name", &employee.name);
  cl.option("-age=", "employee age", &employee.age);
  cl.option("-salary=", "employee salary", &employee.salary);
  cl.parse();
  cl.display_messages();
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

```

The code of this example is in `example-15.cpp`.
>To specify a string containing _spaces_ in the command line, enclose it in quotation marks.

Let's see it in action:

```sh
$ ./example-15 -name="John Doe" -salary=32500.75 -age=32
```

The output is

```sh
$ Employee data:
$   name:   John Doe
$   age:    32
$   salary: 32500.8
```

>To specify a string containing _quotation marks_ in the command line, escape them by prepending `\`, as for example

```sh
$ ./example-15 -name="Johnny \"the fox\"" -salary=32500 -age=29
```

#### 1.6 Booleans
Note that in order to specify integer, floating-point or string data, an assignment is necessary. If we leave the assignment empty, as for example `-salary= -age=32`, an error condition is thrown. Also, if we omit the assignment sign (`=` by default), we get again an error.

Command line arguments without assignment are used to specify booleans (between other interesting thinks). Let us see this with an example. In the code of the previous section add a member `bool married` to the `Employee` structure and modify its instantiation as follows:

```c++
Employee employee = {"noname", 0, 0., false};
```

Below the line `cl.option("-salary=", ...` add

```c++
  cl.option("-married", "married", &employee.married);
```

and modify the employee data printing statement to display the employee's marital status. Compile and run

```sh
$ ./example-16 -name="John Doe" -salary=32500.75 -age=32 -married
```

> Note that the command line specification of a boolean switch _without assignment_ always turns the connected boolean variable **on**. Consequently, the variable's default value must be `false`.

If this is a limitation, there is a second way offered by **CmdLineArgs** to specify booleans, which is similar to string specification. Change the previous code line to

```c++
  cl.option("-married=", "married", &employee.married);
```

and recompile. The command line is now

```sh
$ ./example-16 -name="John Doe" -salary=32500.75 -age=32 -married=true
```

>Notice the difference: `-married=true` instead of `-married`.

Assigned booleans can alternatively be specified by the keywords `yes` and `on`. A false boolean value is specified by the keywords `false`, `no`, `off`.

> The command line specification of a boolean switch _with assignment_ can turn the connected boolean variable **on** or **off**. The variable's default value can be `true` or `false`.

[ [Next >](tutorial-2.html "Displaying Reports and Executing Commands") ] [ [Up](tutorial.html "Table of Contents") ]
