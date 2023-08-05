[ [Prev <](tutorial-5.html "Sources and Data Validation") ] [ [Next >](tutorial-7.html "Displaying Information and Executing Commands") ] [ [Up](tutorial.html "Table of Contents") ]

## PART II: Argp-Style Argument Handling

The **argp** interface is a well-established tool for parsing program arguments, developed over a long period. It is a comprehensive argument parser, notably conforming to **GNU standards**. Programs utilizing **argp** automatically generate output in response to `--help` and `--version` command-line options, as outlined in the **GNU coding standards**. It is unnecessary to call `CmdLineArgs::option_help()`.

However, there are significant drawbacks to this approach. The programmer must create an _argument handling routine_, including a lengthy switch statement, and communicate any `main()` function variables exposed to command-line modification through data structures. This requires too much effort, lacks code reusability, and follows an outdated programming style. Let's take a look at an example of standard **argp** programming ([Example 3](https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html) of **argp**'s documentation):

<p id='Listing_1'>Listing 1:</p>

```c
#include <stdlib.h>
#include <argp.h>

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";
/* Program documentation. */
static char doc[] =
  "Argp example #3 -- a program with options and arguments using argp";
/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 ARG2";

/* The options we understand. */
static struct argp_option options[] = {
  {"verbose",  'v', 0,      0,  "Produce verbose output" },
  {"quiet",    'q', 0,      0,  "Don't produce any output" },
  {"silent",   's', 0,      OPTION_ALIAS },
  {"output",   'o', "FILE", 0, "Output to FILE instead of standard output" },
  { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments {
  char *args[2];                /* arg1 & arg2 */
  int silent, verbose;
  char *output_file;
};

/* Parse a single option. */
static error_t parse_opt (int key, char* arg, struct argp_state* state) {
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key) {
    case 'q': case 's':
      arguments->silent = 1;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'o':
      arguments->output_file = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        /* Too many arguments. */
        argp_usage (state);
      arguments->args[state->arg_num] = arg;
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc };

int main (int argc, char **argv) {
  struct arguments arguments;
  /* Default values. */
  arguments.silent = 0;
  arguments.verbose = 0;
  arguments.output_file = "-";
  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  printf ("ARG1 = %s\nARG2 = %s\nOUTPUT_FILE = %s\n"
          "VERBOSE = %s\nSILENT = %s\n",
          arguments.args[0], arguments.args[1],
          arguments.output_file,
          arguments.verbose ? "yes" : "no",
          arguments.silent ? "yes" : "no");

  exit (0);
}
```

The objective of **clparser** is to relieve the programmer of this coding burden by offering a high-level interface to **argp**, similar to the free-style argument handling in Part I. Here is the **clparser** version of the previous example:

<p id='Listing_2'>Listing 2:</p>

```c++
#include <cstdlib>
#include <cstdio>
#include "clparser.hpp"

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

int main (int argc, char **argv) {
  // These are the variables exposed to command line modification.
  // No need for struct arguments; use variables directly with default values.
  char* args[2]; // arg1 & arg2
  int   silent = 0, verbose = 0;
  char* output_file = (char*)"-";

  CmdLineArgs cl(argc, argv);
  try {
    // Program documentation
    cl.set_doc("Argp example #3 "
    	"-- a program with options and arguments using argp");
    cl.set_args_doc("ARG1 ARG2");
    // The options we understand
    cl.option("verbose", "Produce verbose output",   &verbose, 'v');
    cl.option("quiet",   "Don't produce any output", &silent,  'q');
    cl.alias ("silent",  's');
    cl.option("output",  "Output to $FILE$ instead of standard output",
        &output_file, 'o');
    cl.parse();
    // We are expecting exactly two arguments
    if (cl.sources().size() != 2) {
      cl.display_usage();
      return 3;
    }
    args[0] = const_cast<char*>(cl.sources().at(0).c_str());
    args[1] = const_cast<char*>(cl.sources().at(1).c_str());
  }
  catch (const std::string& msg) {
    printf("error: %s\n", msg.c_str());
    return 3;
  }

  printf ("ARG1 = %s\nARG2 = %s\nOUTPUT_FILE = %s\n"
          "VERBOSE = %s\nSILENT = %s\n",
          args[0], args[1],
          output_file,
          verbose ? "yes" : "no",
          silent ? "yes" : "no");

  return 0;
}
```

In the final version of this file we use `string`s and C++ `stream`s, which further simplify the code.

The following differences from the free-style interface (**FSI**) must be born in mind when programming argument parsing with the argp interface:

- Option names in **argp** are _plain names_, i.e. without preceding `-` marks or trailing assignment characters (`=`). Option names in **argp** can be `null`; this is not allowed in FSI.
- Option identifiers, although optional, are vital to **argp**; when they are printable characters, they form the _short version_ of the option. In the FSI, short versions, if desirable, are coded as **aliases**, and option identifiers are internally determined.
- The user may specify _abbreviated versions of options_ in the command line; for example, instead of `--version`, the abbreviations `--vers` or `--ver` can be specified. This feature is not available in the FSI.
- The FSI by default tolerates undeclared options; the **argp** interface does not.
- The FSI <u>never</u> accesses the screen to print errors or messages. Everything that needs to reach the screen is collected as a message or thrown as a string error when it is fatal. **Argp**, though, tries to take care of all possible printing needs of the program during argument parsing. Thus, there is no need to **display_messages()** in **argp**.
- Arguments not preceded by `-` are not available in **argp**. You must collect them as sources, manually recognize them and then parse and handle them.
- Groups have an entirely different meaning in **argp**: they are for documentation purposes only. In the current library version, the programmer must manually implement mutually exclusive groups of options using lambdas or option handlers.

The following lessons are similar to the preceding five, up to a few differences originating in the different nature of the two interfaces. The reader that has followed the details of the previous lessons may pass them faster, only taking care to notice the differences from the FSI.

### Lesson 6: Command Line Specification of Basic Types

In this lesson, you will learn how to program command-line argument specifications for

- integers
- floating point numbers
- booleans, and
- strings

#### 6.1 The first steps

The official **argp** documentation provides a minimal program as its [first example](https://www.gnu.org/software/libc/manual/html_node/Argp-Example-1.html). This program does not perform any significant task, except for displaying an error message and exiting when given any options. It also displays a relatively meaningless message when `-–help` or `-?` is specified. Additionally, it provides a message informing the user how to invoke the program when called with the option `--usage`. This basic behavior is shared by all programs that use the **argp** interface. 

```c++
#include "clparser.hpp"
int main(int argc, char** argv) {
  CmdLineArgs cl(argc, argv);
  cl.parse();
  return 0;
}
```

The [second example](https://www.gnu.org/software/libc/manual/html_node/Argp-Example-2.html) of **argp** is more in line with the **GNU standard command line format**. It includes a `--version` option, a short description of the program, and a bug-report address in the `--help` output, as per **GNU standards**. To specify the program's version and bug address, declare the global variables `argp_program_version` and `argp_program_bug_address`. The program description is taken care of by the `CmdLineArgs::set_doc()` method, as shown in the code snippet below:

```c++
#include "clparser.hpp"

const char *argp_program_version = "t2-argp 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

int main(int argc, char** argv) {
  CmdLineArgs cl(argc, argv);
  cl.set_doc("Argp example #2 -- a pretty minimal program using argp");
  cl.parse();
  return 0;
}
```

The command line `$ example-gnu-2 --help` produces the output:

```shell
Usage: example-gnu-2 [OPTION...]
Argp example #2 -- a pretty minimal program using argp

  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Report bugs to <bug-gnu-utils@gnu.org>.
```

#### 6.2 Integer options

Before going on to the third **argp example**, let us recall our very first example

```c++
#include <iostream>
using namespace std;
int main(int argc, char* argv[]) {
  int num = 1;
  cout << "num = " << num << endl;
  return 0;
}
```

where we declared an integer `num` and made it command-line adjustable. As in the FSI case (see Lesson 1), we create an instance of **CmdLineArgs** and use the method **CmdLineArgs::option()** to declare a command-line argument `-num`:

```c++
int num = 1;
try {
  CmdLineArgs cl(argc, argv);
  cl.option("num", "number of objects", &num, 'n');
  cl.parse();
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "num = " << num << endl;
```

Take notice of the difference from the FSI: the **argp** interface declares options with _no_ leading dashes and trailing assignments. And if you want your option also has a short version, set the option's identifier (last argument of **option()**) to a printable character. Otherwise, if you, for example, write `cl.option("num", "number of objects", &num)`, **clparser** will automatically generate a valid non-printable identifier for your option. The second argument is informative; its utility lies in formatting help messages. The third argument is a pointer to the variable that will receive the value specified at the command line.

The absence of any calls to message displaying functions is not due to negligence or carelessness; recall that **argp** takes care of all message processing.

If we run the program with the argument `--num=2` or, which is the same for **argp**, `--num 2` or `-n 2` (but **not** `-n=2`!), the output we'll get is: `num = 2`. The argument `--help` produces the output:

```
$ Usage: example-62 [OPTION...]
$
$   -n, --num=VALUE            number of objects
$   -?, --help                 Give this help list
$       --usage                Give a short usage message
$
$ Mandatory or optional arguments to long options are also mandatory or optional
$ for any corresponding short options.
```

The `VALUE` string is by default supplied by **clparser**. To have `--num=XXX` displayed, change the option declaration to:

```bash
  cl.option("num", "$XXX$: number of objects", &num, 'n');
```

The help output will be:

```shell
$   -n, --num=XXX              XXX: number of objects
```

Another option is

```shell
  cl.option("num", "number of objects$INTEGER", &num, 'n');
```

producing the output:

```
$   -n, --num=INTEGER          number of objects
```

The code of this example is in `example-62.cpp`. To create the executable you'd also compile `clparser-arpg.cpp` and link them all together or to the `clparser-argp` library. The header file `clparser.hpp` must be included in the main source file. In a more convenient setting, you would use the supplied `codeblocks` project file `example-62.cbp`. For plain shell compilation with debugging information use the following commands in the `examples` subdirectory:

```shell
$ g++ -Wall -g -DARGP_INTERFACE -I../include -c ../src/clparser-argp.cpp -o clparser-argp.o
$ g++ -Wall -g -DARGP_INTERFACE -I../include -c example-62.cpp -o example-62.o
$ g++ -o example-62 example-62.o clparser-argp.o
```

The `-Wall` and `-g` switches are optional. The `-DARGP_INTERFACE` definition is needed to select the **argp** interface. Alternatively, the convenience shell script `compile-clparser-examples --argp` can be used. You can use it either from the _clparser root directory_ or from the _examples_ directory. The executable will be placed in the installation directory.

What's the use of the `try-catch` block if **argp** takes care of all message processing? Let's run the program with an incorrect argument, say `-num=12y`. Here is the output:

```sh
$ terminate called after throwing an instance of 'std::string'
```

**Argp** does all the work required for the argument parsing but does no attempt to interpret the user's specifications; **clparser** does it on behalf of the programmer. Thus, when **clparser** comes across an incorrect integer, it throws a string error. With the try-catch block the result is:

```sh
$ error: Incorrect integer specification 'num=12y'
```

#### 6.3 Unexpected arguments

Unexpected arguments are handled by the underlying **argp** code. If, for example, we use `example-62` with the additional (unexpected) argument, say `--abc=3`, the program terminates issuing the following message:

```sh
$ example-62a: unrecognized option '--abc'
$ Try `example-62a --help' or `example-62a --usage' for more information.
```

No error message is received if we specify the argument `num=12` (without `--`), and `num` has its default value (1). The reason is that **argp** has taken this for a source. Note that there are some differences in the algorithm **argp** and the FSI apply to decide if an argument is a source.

In the FSI, this behavior is controlled by `CmdLineArgs::set_extra_fatal()`. On the other hand, **argp** employs several [flags](https://www.gnu.org/software/libc/manual/html_node/Argp-Flags.html) to control the process of parsing arguments. These flags are passed as argument to the `CmdLineArgs::parse_cmd_line()` function. For example, to make the program tolerant of unknown options use

```c++
  cl.parse_cmd_line(ARGP_NO_ERRS);
```

instead of `cl.parse()`. Note that `cl.parse()` is identical to `cl.parse_cmd_line(0)`.

#### 6.4 Floating point and string (character) options

When it comes to specifying floating-point and string data in the command line, the process is similar to that of integers. As an example, let's say we want to allow the user to input employee information in the command line, such as their name, age, and salary. Here is how to program this case:

```c++
struct Employee {
  string name;
  int    age;
  double salary;
};

Employee employee {"noname", 0, 0.};

try {
  CmdLineArgs cl(argc, argv);
  cl.set_doc("Cmdline argp example 6.4 -- Integer, string and FP options.");
  cl.option("name", "employee name", &employee.name);
  cl.option("age", "employee age", &employee.age);
  cl.option("salary", "employee salary", &employee.salary);
  cl.parse();
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}

cout << "Employee data:" << endl
     << "  name:   " << employee.name   << endl
     << "  age:    " << employee.age    << endl
     << "  salary: " << employee.salary << endl;

```

The code of this example is in `example-64.cpp`. Let's see it in action:

```bash
$ ./example-64 --name="John Doe" --salary=32500.75 --age=32
```

>To specify a string containing _spaces_ in the command line, enclose it in quotation marks.

The output is

```
$ Employee data:
$   name:   John Doe
$   age:    32
$   salary: 32500.8
```

>To specify a string containing _quotation marks_ in the command line, escape them by prepending `\`, for example:

```sh
$ ./example-64 --name="Johnny \"the fox\"" --salary=32500 --age=29
```

> In the **argp** interface the `=` between the option and its value can be replaced with spaces.

Let's try the following command line specification:

```sh
$ ./example-64 --name="John Doe" --age=43 --salary=32500.75 --age=32
```

The output is:

```
$ Employee data:
$   name:   John Doe
$   age:    32
$   salary: 32500.8
```

It is common for **argp** to enable multiple specifications of an option. In such cases, the value of the variable is determined by the last specification made. If the program were compiled with FSI, the resulting output would be as follows:

```
Option '--age=32' conflicts with previous options. This option was ignored.
Employee data:
  name:   John Doe
$   age:    43
$   salary: 32500.8
```

#### 6.5 Booleans

Please note that when specifying integer, floating-point or string data, an assignment argument is _always_ necessary. Leaving the assignment empty, as in `--salary= --age=32`, will result in an error being thrown. If we specify `--salary --age=32`, **argp** will consider `--age=32` to be the argument of `--salary`; again, which is not a valid number and will result in an error being thrown.

Unlike the FSI, the **argp** interface _always_ specifies a command-line option with argument when declaring a boolean option. It is important to remember that in **argp**, all option declarations are without trailing assignment signs. For example, in the code of the previous section, you can add a member `bool married` to the Employee structure and modify its instantiation to look like this:

```c++
Employee employee {"noname", 0, 0., false};
```

Below the line `cl.option("salary", ...` add

```c++
  cl.option("married", "married", &employee.married);
```

and modify the employee data printing statement to display the employee's marital status. Compile and run

```
$ ./example-65 --name="John Doe" --salary=32500.75 --age=32 --married
```

The program will happily change the marital status to `true`. If we say `$ ./example-65 --name="John Doe" --salary=32500.75 --age=32 --married=yes`, it will be happy again.

> Keep in mind that when a boolean switch _without assignment_ is specified on the command line, it will always turn the associated boolean variable **on**. Since it is impossible to predict whether the user will provide an argument or not, the variable's default value must be `false`.

This is the standard behavior of **argp**. To prevent the user from supplying an argument to a boolean command-line option, we can use the `CmdLineArgs::option_uses_no_arg()` method. A call to this method should be placed directly below the option's declaration and has no effect on integer, string, or floating-point number options. For instance, if we write

```c++
  ...
  cl.option("married", "married", &employee.married);
  cl.option_uses_no_arg();
  ...
```

and recompile, the command line

```
$ ./example-65a --name="John Doe" --salary=32500.75 --age=32 --married=true
```

produces

```
$ ./example-65a: option '--married' doesn't allow an argument
$ Try `example-65a --help' or `example-65a --usage' for more information.
```

You can also specify a true _boolean with assignment_ using the keywords `yes` and `on`. To indicate a false value, use the keywords `false`, `no`, or `off`.

[ [Prev <](tutorial-5.html "Sources and Data Validation") ] [ [Next >](tutorial-7.html "Displaying Information and Executing Commands") ] [ [Up](tutorial.html "Table of Contents") ]
