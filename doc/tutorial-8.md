[ [Prev <](tutorial-7.html "Displaying Information and Executing Commands (Argp)") ] [ [Next >](tutorial-9.html "Advanced Help (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 8: Selections and Aliases

In this lesson you will learn how to use certain utilities offered by **clparser** to make your code simpler and more efficient. Specifically, you will learn how to

- efficiently program options for indexes into vectors and arrays (selections), and

- declare aliases, i.e. different options producing the same effect.

Recall that mutually exclusive options are not available in the **argp** interface. Due to the close similarity of this Lesson with 3, we present only the differences in the codes.


#### 8.1 Selections
Read Lesson 3.1 if you have not done it already.

Let's see example-81 which is the counterpart of example-31:

```c++
vector<string> fruits = {"apple", "orange", "peach", "melon"};
int selection = -1;
try
{
  CmdLineArgs cl(argc, argv);
  cl.set_doc("example-81 -- selections.");
  cl.set_args_doc("--[apple|orange|peach|melon]");
  cl.option("apple", "apple", 0, &selection, '0');
  cl.option("orange", "orange", 1, &selection, '1');
  cl.option("peach", "peach", 2, &selection, '2');
  cl.option("melon", "melon", 3, &selection, '3');
  cl.parse();
  if (selection == -1)
    throw string("One of --apple --orange --peach --melon must be selected");
}
catch (string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "Selected fruit: " << fruits[selection] << endl;
```

If we run the program with the options `example-81 --orange` we obtain:

```
$ Selected fruit: orange
```

As it is always the case with **argp**, and contrary to FSI, the user can specify more than once the options `--apple`, `--orange`, `--peach`, `--melon`. Of course, the last option will be in effect.

Finally, let us take a look at the output of `example-81 --help`:

```
Usage: example-81 [OPTION...] --[apple|orange|peach|melon]
example-81 -- selections.

  -0, --apple                apple
  -1, --orange               orange
  -2, --peach                peach
  -3, --melon                melon
  -?, --help                 Give this help list
      --usage                Give a short usage message
```

#### 8.2 Aliases
It is often desirable to use a command-line argument with a different option name. To declare different options producing the same effect we use **aliases**. To declare an option along with a number of aliases, declare the option in the usual fashion, and then, immediately following this declaration, place a call to the method **CmdLineArgs::alias()** for each equivalent option. Look at the following example (`example-82.cpp`):

```c++
bool opt = false;
try
{
  CmdLineArgs cl(argc, argv);
  cl.option("option", "enable feature 'option'", &opt);
  cl.alias ("enable-option");
  cl.parse();
}
catch (std::string& error_msg)
{
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "option: " << string(opt ? "en" : "dis") + "abled" << endl;
```

The options `--option` or `--enable-option` produce `option: enabled` and `--help` or `-?` produce the output:

```
Usage: example-82 [OPTION...]
example-82 -- aliases.

  -o, --option[=[yes/no]], --enable-option[=[yes/no]]
                             enable feature 'option'
  -?, --help                 Give this help list
      --usage                Give a short usage message

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```

#### 8.3 More GNU argp examples
Carrying on with our presentation of official **argp** examples, in the [third official argp example](https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html#Argp-Example-3) we have, in addition to the second example, two options `verbose` and `quiet` turning on certain features, a third option `silent` having the same effect as `quiet`, and finally an option `output` with a string argument. Beside these options, the program expects exactly two arguments (sources). The original source of this example is in [Listing 1](#Listing_1) and its **clparser** version in [Listing 2](#Listing_2).

We slightly modify the code to take advantage of **clparser**'s added features. In the first place, `verbose`, `quiet`, and `silent` need not be integers. They turn on some feature, which by default is off; so they'd better be booleans. Further, we make `output_file` to a `string` and `args` to a `vector<string>`. It is recommended that sources be declared as string vectors. With these modifications the code inside `main()` looks like this:

```c++
vector<string> args; // arg1 & arg2
bool silent = false, verbose = false;
string output_file("-");

CmdLineArgs cl(argc, argv);
try
{
  cl.set_doc("Argp example #3 "
      "-- a program with options and arguments using argp");
  cl.set_args_doc("ARG1 ARG2");
  cl.option("verbose", "Produce verbose output",   &verbose, 'v');
  cl.option("quiet",   "Don't produce any output", &silent,  'q');
  cl.alias ("silent", 's');
  cl.option("output",  "Output to $FILE$ instead of standard output",
      &output_file, 'o');
  cl.parse();
  if (cl.sources().size() != 2)
  {
    cl.display_usage();
    return 3;
  }
  args = cl.sources();
}
catch (string& msg)
{
  printf("error: %s\n", msg.c_str());
  return 3;
}
cout << "ARG1 = " << args[0] << endl
     << "ARG2 = " << args[1] << endl
     << "OUTPUT_FILE = " << output_file << endl
     << "VERBOSE = " << (verbose ? "yes" : "no") << endl
     << "SILENT = "  << (silent ? "yes" : "no") << endl;
```

The output of `example-gnu-3a --output=my-file --quiet=yes mysrc1 mysrc2` is

```
$ ARG1 = mysrc1
$ ARG2 = mysrc2
$ OUTPUT_FILE = my-file
$ VERBOSE = no
$ SILENT = yes
```

Exactly the same output is obtained by `example-gnu-3a --output my-file --quiet=yes mysrc1 mysrc2`. The command line `example-gnu-3a --output=my-file --quiet yes mysrc1 mysrc2` gives, however:

```
$ Usage: example-gnu-3a [OPTION...]
$ Try `example-gnu-3a --help' or `example-gnu-3a --usage' for more information.
```

The reason is that `quiet` is an option with **optional argument**. The arguments `--help` or `--usage` say

```
Usage: example-gnu-3a [OPTION...] ARG1 ARG2
Argp example #3 -- a program with options and arguments using argp

  -o, --output=FILE          Output to FILE instead of standard output
  -q, -s, --quiet[=[yes/no]], --silent[=[yes/no]]
                             Don't produce any output
  -v, --verbose[=[yes/no]]   Produce verbose output
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
...
```

We can change this behavior by using **CmdLineArgs::set_bool_strict()**:

```c++
  cl.set_bool_strict();
```

This time the program works, and `--help` gives:

```
Usage: example-gnu-3a [OPTION...] ARG1 ARG2
Argp example #3 -- a program with options and arguments using argp

  -o, --output=FILE          Output to FILE instead of standard output
  -q, -s, --quiet=[yes/no], --silent=[yes/no]
                             Don't produce any output
  -v, --verbose=[yes/no]     Produce verbose output
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```

It is possible to check the number of supplied sources while the parser is picking them up. To apply this method we have to override the default handling of certain internal options used by **argp**. Sources are picked up when the special option `ARGP_KEY_ARG` is handled. To override its default action we subclass `ArgpHandler`:

```c++
class MyArgpHandler : public ArgpHandler
{
  public:
  MyArgpHandler() {}
  // ARGP_KEY_ARG handler
  virtual bool on_arg(argp_state *state, char *arg, int *rc)
  {
    if (state->arg_num >= 2) // Too many arguments
    {
      cout << arg << " is an extra source." << endl;
      argp_usage(state);
    }
    *rc = 0;
    return /* pass control to default handler */ false;
  }
  // ARGP_KEY_END handler
  virtual bool on_end(argp_state *state, char *arg, int *rc)
  {
    if (state->arg_num < 2) // Not enough arguments
    {
      cout << "too few arguments" << endl;
      argp_usage(state);
    }
    *rc = 0;
    return /* ignored */ false;
  }
};
```

When `argp_usage()` is called it forces the program to terminate with an error exit code. The `on_end()` handler does about the same job as the `if (cl.sources().size() != 2)` code block. All other handlers stay with their default implementation (which is `return false`). After `MyArgpHandler` has been coded, we can tell our argument parser to use this handler instead of the default by calling `CmdLineArgs::set_argp_handler(ArgpHandler *)`:

```c++
  vector<string> args; // arg1 & arg2
  bool silent = false, verbose = false;
  string output_file("-");
  try
  {
    CmdLineArgs cl(argc, argv);
    MyArgpHandler argp_handler;
    cl.set_argp_handler(&argp_handler);
    cl.set_doc("Argp example #3 "
        "-- a program with options and arguments using argp");
    cl.set_args_doc("ARG1 ARG2");
    cl.option("verbose", "Produce verbose output",   &verbose, 'v');
    cl.option("quiet",   "Don't produce any output", &silent,  'q');
    cl.alias ("silent", 's');
    cl.option("output",  "Output to $FILE$ instead of standard output",
        &output_file, 'o');
    cl.parse();
    args = cl.sources();
  }
  catch (string& msg)
  {
    printf("error: %s\n", msg.c_str());
    return 3;
  }
```

[ [Prev <](tutorial-7.html "Displaying Information and Executing Commands (Argp)") ] [ [Next >](tutorial-9.html "Advanced Help (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]
