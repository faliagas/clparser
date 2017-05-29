[ [Prev <](tutorial-5.html "Sources and Data Validation") ] [ [Next >](tutorial-7.html "Displaying Information and Executing Commands") ] [ [Up](tutorial.html "Table of Contents") ]

## PART II: Argp-Style Argument Handling

**Argp** is a mature interface for parsing program arguments, which has evolved over a long time. It is a fully-featured argument parser, its most prominent feature being **GNU conformity**. Programs using **argp** automatically produce output in response to the `--help` and `--version` command-line options, as described in the **GNU coding standards**; no call to **CmdLineArgs::option_help()** is necessary.

There are serious downsides to this, however. The programmer is required to write an _argument handling routine_ containing a long switch statement, and communicate the _main()_ function variables, which are exposed to command-line modification, through data structures; pointers of these structures are passed to the argument handling functions. This too much effort, no code reusability, programming style of the past. Let us see an example of standard argp programming ([Example 3](https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html) of **argp**'s documentation):

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
struct arguments
{
  char *args[2];                /* arg1 & arg2 */
  int silent, verbose;
  char *output_file;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
  struct arguments *arguments = state->input;

  switch (key)
  {
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

int main (int argc, char **argv)
{
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

**Clparser** is here to relieve the programmer of this coding burden by providing a high level interface to **argp**, quite similar to the free-style argument handling in Part I. Here is the **clparser** version of the previous example:

<p id='Listing_2'>Listing 2:</p>

```c++
#include <cstdlib>
#include <cstdio>
#include "clparser.hpp"

const char *argp_program_version = "argp-ex3 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

int main (int argc, char **argv)
{
  // These are the variables exposed to command line modification.
  // No need for struct arguments; use variables directly with default values.
  char *args[2]; // arg1 & arg2
  int   silent = 0, verbose = 0;
  char *output_file = (char*)"-";

  CmdLineArgs cl(argc, argv);
  try
  {
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
    if (cl.sources().size() != 2)
    {
      cl.display_usage();
      return 3;
    }
    args[0] = const_cast<char*>(cl.sources().at(0).c_str());
    args[1] = const_cast<char*>(cl.sources().at(1).c_str());
  }
  catch (std::string& msg)
  {
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

In the final version of this file we use `string`s and c++'s `iostream`s, which simplifies further the code.

The following differences from the free-style interface (**FSI**) must be born in mind when programming argument parsing with the argp interface:

- Option names in **argp** are _plain names_, i.e. without preceding `-` marks or trailing assignment characters (`=`). Option names in **argp** can be `null`; this is not allowed in FSI.
- Option identifiers, although optional, are important in **argp**; when they are printable characters, they form the _short version_ of the option. In FSI there are no such short versions available. If a short version of an option is desirable, it must be manually included as an **alias** of the main option. Option identifiers are usually determined internally by FSI.
- The user may specify _abbreviated versions of options_ in the command line; for example, instead of `--version`, abbreviations like `--vers` or `--ver` can be specified. This feature is not available in FSI.
- FSI by default tolerates undeclared options; the **argp** interface does not.
- FSI <u>never</u> accesses the screen to print errors or messages. Everything that needs to be printed is collected as a message or thrown as a string error, when it is fatal. **Argp** doesn't; instead, it tries to take care of all possible printing needs of the program during argument parsing. Thus, there is no need to **display_messages()** in **argp**.
- Arguments not preceded by `-` are not available in **argp**. You have to collect them as sources, manually recognize them, and, of course, parse and handle them.
- Groups have an entirely different meaning in **argp**: they are for documentation purposes only. Mutually exclusive groups of options must be manually implemented using lambdas or option handlers.

The following lessons are similar to the preceding five lessons, up to a few differences originating in the different nature of the two interfaces. The reader that has followed the details of the previous lessons may pass them faster taking care to notice the differences from the FSI.

### Lesson 6: Command Line Specification of Basic Types

In this lesson you will learn how to program command line argument specifications for

- integers
- floating point numbers
- booleans, and
- strings

#### 6.1 The first steps

The [first example](https://www.gnu.org/software/libc/manual/html_node/Argp-Example-1.html) found in the official **argp** documentation is a minimal program that uses argp.  It won’t do much except print an error message and exit when there are any arguments, and print a (rather pointless) message for the arguments `-–help`, `-?`. It also prints a message for `--usage` which informs the user how to invoke the program. This is the basic behavior, common to all programs using the argp interface. Here is the transposition of this program to **clparser**:

```c++
#include "clparser.hpp"
int main(int argc, char **argv)
{
  CmdLineArgs cl(argc, argv);
  cl.parse();
  return 0;
}
```

The [second official](https://www.gnu.org/software/libc/manual/html_node/Argp-Example-2.html) **argp** example uses argp in order to be compliant with the **GNU standard command line format**, without using any options or arguments. In addition to the previous example, this will have a `--version` option, and will put a documentation string and bug address in the `--help` output, as per **GNU standards**. The program version and bug address are specified by simply declaring the global variables `argp_program_version` and `argp_program_bug_address`. For the program documentation string one uses the `CmdLineArgs::set_doc()` method, as in the following code snippet:

```c++
#include "clparser.hpp"

const char *argp_program_version = "t2-argp 1.0";
const char *argp_program_bug_address = "<bug-gnu-utils@gnu.org>";

int main(int argc, char **argv)
{
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
Before going on to the third argp example, let us recall our very first example

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

in which we declare an integer `num`, and we would like to make it modifiable at the command line. As in the FSI (see Lesson 1), we create an instance of **CmdLineArgs**, and use the method **CmdLineArgs::option()** to declare a command-line argument `-num`:

```c++
int num = 1;
try
{
  CmdLineArgs cl(argc, argv);
  cl.option("num", "number of objects", &num, 'n');
  cl.parse();
}
catch (string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "num = " << num << endl;
```

Take notice of the way options are declared in the **argp** interface: <u>no leading dashes</u> and <u>no trailing assignments</u>. And if you want your option to also have a short version, the option's identifier (last argument of **option()**) must be set to a printable character. Otherwise, if you, for example, write `cl.option("num", "number of objects", &num)`, **clparser** automatically generates a valid non-printable identifier for your option. The second argument is informative and is used in formatting help massages. The third argument is a pointer to the variable that will receive the value specified at the command line.

The absence of any calls to message displaying functions is not due to negligence or carelessness; recall that **argp** takes care of all message processing.

If we run the program with the argument `--num=2` or, which is the same for **argp**, `--num 2`, or `-n 2` (but **not** `-n=2`!), the output is: `num = 2`. The argument `--help` produces the output:

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

The `VALUE` argument is by default supplied by **clparser**. To have `--num=XXX` displayed, change the option declaration as in

```bash
  cl.option("num", "$XXX$: number of objects", &num, 'n');
```
for
```shell
$   -n, --num=XXX              XXX: number of objects
```
or
```shell
  cl.option("num", "number of objects$INTEGER", &num, 'n');
```
for
```
$   -n, --num=INTEGER          number of objects
```

The code of this example is in `example-62.cpp`. To create the executable you need to also compile `clparser-arpg.cpp` and link them together, or link to the `clparser-argp` library. The header file `clparser.hpp` must be included in the main source file. For convenience, one may use the supplied `codeblocks` project file `example-62.cbp`. For direct compilation with debugging information use the following commands in the `examples` subdirectory:

```shell
$ g++ -Wall -g -DARGP_INTERFACE -I../include -c ../src/clparser-argp.cpp -o clparser-argp.o
$ g++ -Wall -g -DARGP_INTERFACE -I../include -c example-62.cpp -o example-62.o
$ g++ -o example-62 example-62.o clparser-argp.o
```

The `-Wall` and `-g` switches are of course optional. The `-DARGP_INTERFACE` definition is needed to select the **argp** interface. Alternatively, the convenience shell script `compile-clparser-examples --argp` can be used. You can use it either from the _clparser root directory_ or from the _examples_ directory. The executable will be placed in the installation directory.

What is the use of the `try-catch` block if **argp** takes care of message processing? If we run the program with an incorrect argument, say `-num=12y`, we will obtain an unhandled exception message:

```sh
$ terminate called after throwing an instance of 'std::string'
```

**Argp** does all the work required for the argument parsing; it does no attempt to interpret the user's specifications. This is done by **clparser** on behalf of the programmer. As a consequence, when **clparser** encounters an incorrect integer, it throws a string error. With the try-catch block the result is:

```sh
$ error: Incorrect integer specification 'num=12y'
```

#### 6.3 Unexpected arguments
Unexpected arguments are handled by the underlying **argp** code. If, for example, we use `example-62` with the additional (unexpected) argument, say `--abc=3`, the program terminates issuing the following message:

```sh
$ example-62a: unrecognized option '--abc'
$ Try `example-62a --help' or `example-62a --usage' for more information.
```

No error message is received if we specify the argument `num=12` (without `--`), and num has its default value (1). The reason is that **argp** has taken this for a source. Note that there are some differences in the algorithm **argp** and the FSI apply to decide if an argument is a source.

In the FSI this behavior is controlled by `CmdLineArgs::set_extra_fatal()`. **Argp** uses a number of [flags](https://www.gnu.org/software/libc/manual/html_node/Argp-Flags.html) to control the argument parsing process, which can be passed to `CmdLineArgs::parse_cmd_line()`. For example, to make the program tolerant of unknown options use

```c++
  cl.parse_cmd_line(ARGP_NO_ERRS);
```

instead of `cl.parse()`, which is identical to `cl.parse_cmd_line(0)`.

#### 6.4 Floating point and string (character) options
The specification of floating-point and string data in the command line is much the same as the specification of integers. Assume, we would like to let the user specify employee data, as for example name, age and salary, in the command line. Here is how to program this case:

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
  cl.set_doc("Cmdline argp example 6.4 -- Integer, string and FP options.");
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

>To specify a string containing _quotation marks_ in the command line, escape them by prepending `\`, as for example

```sh
$ ./example-64 --name="Johnny \"the fox\"" --salary=32500 --age=29
```

> In the **argp** interface the `=` between the option and its value can be replaced by spaces.

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

This behavior is typical of **argp**: it allows the specification of an option more than once. The value of the variable is determined by its last specification. If we compiled the program with FSI, we would obtain the output:

```
Option '--age=32' conflicts with previous options. This option was ignored.
Employee data:
  name:   John Doe
$   age:    43
$   salary: 32500.8
```

#### 6.5 Booleans
Note that in order to specify integer, floating-point or string data, an assignment argument is always necessary. If we leave the assignment empty, as for example `--salary= --age=32`, an error string will be thrown. If we specify `--salary --age=32`, argp considers `--age=32` to be the argument of `--salary`; again, an error condition will be thrown, as the argument is not a valid number.

By contrast to the FSI, in the **argp** interface the declaration of a boolean option specifies <u>always</u> a command-line option with argument. Recall that FSI decides if an option expects an argument from the trailing assignment sign in its declaration. In **argp** all option declarations are without trailing assignment signs. For example, in the code of the previous section add a member `bool married` to the Employee structure and modify its instantiation

```c++
Employee employee = {"noname", 0, 0., false};
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

> Note that the command line specification of a boolean switch _without assignment_ always turns the connected boolean variable **on**. As we cannot know if the user will give an argument or not, the variable's default value must be `false`.

This is the standard **argp**'s behavior. We can prohibit the user from supplying an argument to a boolean argument by using the method `CmdLineArgs::option_uses_no_arg()`. A call to this method must be placed immediately below the option's declaration. It has no effect on integer, string and floating-point number options. For example, if we write

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

Assigned booleans can also be specified by the keywords `yes` and `on`. A false boolean value is specified by the keywords `false`, `no`, `off`.

[ [Prev <](tutorial-5.html "Sources and Data Validation") ] [ [Next >](tutorial-7.html "Displaying Information and Executing Commands") ] [ [Up](tutorial.html "Table of Contents") ]
