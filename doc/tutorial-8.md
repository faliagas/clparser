[ [Prev <](tutorial-7.html "Displaying Information and Executing Commands (Argp)") ] [ [Next >](tutorial-9.html "Advanced Help (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 8: Selections and Aliases

In this lesson, you will discover how to utilize specific tools provided by **clparser** to simplify and optimize your code. You will gain knowledge on how to:

- Program selections for indexes into vectors and arrays efficiently

- Declare aliases, which are different options that produce the same effect

- Override default Argp handlers

It's important to note that mutually exclusive options are not available in the **argp** interface. As this lesson is closely related to Lesson 3, we will only focus on the differences in the code.

#### 8.1 Selections

Read Lesson 3.1 if you have not done it already.

Let's see example-81 which is the counterpart of example-31:

```c++
vector<string> fruits = {"apple", "orange", "peach", "melon"};
int selection = -1;
try {
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
catch (string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "Selected fruit: " << fruits[selection] << endl;
```

If we run the program with the options `example-81 --orange` we obtain:

```
$ Selected fruit: orange
```

As is always the case with **argp**, and contrary to FSI, the user can specify more than once the options `--apple`, `--orange`, `--peach`, `--melon`. Of course, the last option will be in effect.

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
try {
  CmdLineArgs cl(argc, argv);
  cl.option("option", "enable feature 'option'", &opt);
  cl.alias ("enable-option");
  cl.parse();
}
catch (std::string& error_msg) {
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

Continuing with our presentation of official **argp** examples, this [third example](https://www.gnu.org/software/libc/manual/html_node/Argp-Example-3.html#Argp-Example-3) includes two options in addition to those in the second example: `verbose` and `quiet`, which turn on certain features; and `silent`, which has the same effect as `quiet`. Additionally, there is an option `output` that requires a string argument. The program expects exactly two arguments (sources). The original source code for this example is in [Listing 1](5a.-PART-II.-The-Argp-Interface#Listing_1), and its **Clparser** version is in [Listing 2](5a.-PART-II.-The-Argp-Interface#Listing_2).

We made some modifications to the code to take advantage of **clparser**'s added features. Firstly, `verbose`, `quiet`, and `silent` don't need to be integers. They turn on a feature that is off by default, so they should be booleans instead. Secondly, we changed `output_file` to a `string` and `args` to a `vector<string>`. It's recommended that sources be declared as string vectors. With these modifications, the code inside `main()` looks like this:

```c++
vector<string> args; // arg1 & arg2
bool silent = false, verbose = false;
string output_file("-");

CmdLineArgs cl(argc, argv);
try {
  cl.set_doc("Argp example #3 "
      "-- a program with options and arguments using argp");
  cl.set_args_doc("ARG1 ARG2");
  cl.option("verbose", "Produce verbose output",   &verbose, 'v');
  cl.option("quiet",   "Don't produce any output", &silent,  'q');
  cl.alias ("silent", 's');
  cl.option("output",  "Output to $FILE$ instead of standard output",
      &output_file, 'o');
  cl.parse();
  if (cl.sources().size() != 2) {
    cl.display_usage();
    return 3;
  }
  args = cl.sources();
}
catch (const string& msg) {
  cout "error: " << msg << endl;
  return 3;
}
cout << "ARG1 = " << args[0] << endl
     << "ARG2 = " << args[1] << endl
     << "OUTPUT_FILE = " << output_file << endl
     << "VERBOSE = " << (verbose ? "yes" : "no") << endl
     << "SILENT = "  << (silent ? "yes" : "no") << endl;
```

The output of `example-gnu-3a --output=my-file --quiet=yes mysrc1 mysrc2` is:

```
$ ARG1 = mysrc1
$ ARG2 = mysrc2
$ OUTPUT_FILE = my-file
$ VERBOSE = no
$ SILENT = yes
```

You get the same output by using the command `example-gnu-3a --output my-file --quiet=true mysrc1 mysrc2`. However, if you use the command `example-gnu-3a --output=my-file --quiet yes mysrc1 mysrc2`, the output will be different.

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

To keep track of the number of sources being picked up by the parser, we can use a method that involves overriding certain internal options used by the **argp** library. The sources are picked up when the `ARGP_KEY_ARG` option is handled. We can redefine the `on_arg` handler for `ARGP_KEY_ARG` to change its default action:

```c++
  vector<string> args; // arg1 & arg2
  bool silent = false, verbose = false;
  string output_file("-");
  try {
    CmdLineArgs cl(argc, argv);
    // ARGP_KEY_ARG handler
    cl.set_on_arg([](argp_state *state, char *arg, int *rc) -> bool {
      if (state->arg_num >= 2) { // Too many arguments
        cout << arg << " is an extra source." << endl;
        argp_usage(state);
      }
      *rc = 0;
      return /* pass control to default handler? */ false;
    });
    // ARGP_KEY_END handler
    cl.set_on_end([](argp_state *state, char *arg, int *rc) -> bool {
      if (state->arg_num < 2) { // Not enough arguments
        cout << "too few arguments" << endl;
        argp_usage(state);
      }
      *rc = 0;
      return /* ignored */ false;
    });
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
  catch (string& msg) {
    cout << "error: " << msg << endl;
    return 3;
  }
```

When `argp_usage()` is called, the program terminates with an error exit code. The `on_end()` handler has a similar function to the `if (cl.sources().size() != 2)` code block. The remaining handlers retain their default implementation.

[ [Prev <](tutorial-7.html "Displaying Information and Executing Commands (Argp)") ] [ [Next >](tutorial-9.html "Advanced Help (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]
