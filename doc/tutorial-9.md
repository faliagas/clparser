[ [Prev <](tutorial-8.html "Selections and Aliases (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 9: Advanced Help Topics

**Argp** has many interesting features for advanced help display, and this lesson is devoted to this topic.

#### 9.1 Help footers
**Argp** offers the possibility to add a footer to the help text. To add a help footer use the method **CmdLineArgs::set_doc()** and separate the header from the footer with a '`\v`', as in the following example (`example-91.cpp`):

```c++
int num = 1;
vector<string> sources;
try
{
  CmdLineArgs cl(argc, argv);
  cl.set_doc("Clparser argp example 9.1 -- Help footers.\v"
             "Footer text:\n"
             "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed "
             "do eiusmod tempor incididunt ut labore et dolore magna aliqua. "
             "Ut enim ad minim veniam, quis nostrud exercitation ullamco "
             "laboris nisi ut aliquip ex ea commodo consequat. Duis aute "
             "irure dolor in reprehenderit in voluptate velit esse cillum "
             "dolore eu fugiat nulla pariatur. Excepteur sint occaecat "
             "cupidatat non proident, sunt in culpa qui officia deserunt "
             "mollit anim id est laborum.\n"
             "\n"
             "Pri quas audiam virtute ut, case utamur fuisset eam ut, iisque "
             "accommodare an eam. Reque blandit qui eu, cu vix nonumy "
             "volumus. Legendos intellegam id usu, vide oporteat vix eu, "
             "id illud principes has. Nam tempor utamur gubergren no.");
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

The output of `example-91 --help` will be:

```
$ Usage: example-91 [OPTION...] ARG1 ...
$ Clparser argp example 9.1 -- Help footers.
$
$   -n, --num=VALUE            number of objects
$   -?, --help                 Give this help list
$       --usage                Give a short usage message
$
$ Mandatory or optional arguments to long options are also mandatory or optional
$ for any corresponding short options.
$
$ Footer text:
$ Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor
$ incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis
$ nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.
$ Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu
$ fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in
$ culpa qui officia deserunt mollit anim id est laborum.
$
$ Pri quas audiam virtute ut, case utamur fuisset eam ut, iisque accommodare an
$ eam. Reque blandit qui eu, cu vix nonumy volumus. Legendos intellegam id usu,
$ vide oporteat vix eu, id illud principes has. Nam tempor utamur gubergren no.
```

#### 9.2 Groups
As already said, **argp**'s groups have nothing in common with FSI groups. In FSI groups represent sets of mutually exclusive options; in **argp** groups are sets of options which are grouped together under a group header for display, when help is requested.

The simplest way to define groups of options is to separate each group with a `doc_header()` method. Let's see it in the 4<sup>th</sup> official **argp example** (`example-gnu-4.cpp`):

```c++
string arg1;
string output_file("-");
vector<string> strings;
bool silent = false, verbose = false, abort = false;
int repeat_count = 1;
try
{
  CmdLineArgs cl(argc, argv);
  cl.set_doc(
"Argp example #4 -- a program with somewhat more complicated options\
\v\
This part of the documentation comes *after* the options; \
note that the text is automatically filled, but it's possible \
to force a line-break, e.g.\n<-- here.");
  cl.set_args_doc("ARG1 [STRING...]");
  cl.option("verbose", "Produce verbose output",   &verbose, 'v');
  cl.option("quiet",   "Don't produce any output", &silent,  'q');
  cl.alias ("silent", 's');
  cl.option("output",  "Output to $FILE$ instead of standard output",
            &output_file, 'o');
  cl.doc_header("The following options should be grouped together:");
  cl.option("repeat",  "Repeat the output $COUNT$ (default 1) times",
            &repeat_count, 'r');
  cl.option("abort", "Abort before showing any output", &abort, OPT_ABORT);
  cl.option_uses_no_arg();
  cl.parse();
  if (abort)
    error(10, 0, "ABORTED");
  if (cl.sources().size() < 1)
  {
    cl.display_usage();
    return 3;
  }
  const vector<string>& sources = cl.sources();
  arg1 = sources[0];
  for (int i = 1; i < sources.size(); ++i)
    strings.push_back(sources[i]);
}
catch (std::string& msg)
{
  cout << "error: " << msg << endl;
  return 3;
}
```
In this example we use a footer for our help. The options `verbose`, `quiet`, `silent` and `output` are well-known from previous examples. The option `repeat` is associated with an `int repeat_count`, which is how many times the option-specified variables are printed on the screen. If `abort` is given, the program exits with error code 10 displaying the message "ABORTED". We want these two options grouped under the header `The following options should be grouped together`. The job is done by the method `doc_header()`. Here is the output of the `--help` option:

```
$ Usage: example-gnu-4 [OPTION...] ARG1 [STRING...]
$ Argp example #4 -- a program with somewhat more complicated options
$
$   -o, --output=FILE          Output to FILE instead of standard output
$   -q, -s, --quiet[=[yes/no]], --silent[=[yes/no]]
$                              Don't produce any output
$   -v, --verbose[=[yes/no]]   Produce verbose output
$
$  The following options should be grouped together:
$       --abort                Abort before showing any output
$   -r, --repeat=COUNT         Repeat the output COUNT (default 1) times
$
$   -?, --help                 Give this help list
$       --usage                Give a short usage message
$   -V, --version              Print program version
$
$ Mandatory or optional arguments to long options are also mandatory or optional
$ for any corresponding short options.
$
$ This part of the documentation comes *after* the options; note that the text is
$ automatically filled, but it's possible to force a line-break, e.g.
$ <-- here.
$
$ Report bugs to <bug-gnu-utils@prep.ai.mit.edu>.
```

The options ` -o file.txt prog2 arg1 arg2` produce the following output:

```
$ ARG1 = prog2
$ STRINGS = arg1, arg2
$ OUTPUT_FILE = file.txt
$ VERBOSE = no
$ SILENT = no
```

If we additionally give the option `-r3` or `--repeat=3` the output is printed three times.

Note a slight difference from the original implementation: in the original argp example the argument of `repeat` is optional and `repeat_count` has two defaults: 1, if no `repeat` option is given in the command line, and 10, if the `repeat` option is given without argument. This programming style is not supported by the **clparser** **argp** interface. Of course, at need one can implement dual-default-value variables by declaring an additional `boolean` option.

>In `cl.option("abort", ..., OPT_ABORT)` the *option identifier* `OPT_ABORT` is manually set. This practice is <u>strongly discouraged</u>, unless you know what you are doing. When the option identifier is not a printable character, let **clparser** automatically select one for you.

#### 9.3 More argp groups
Option headers is not the only way to define groups of options with **argp**. One can alternatively use the method **CmdLineArgs::begin_group()** on top of each set of options which will appear as a group under a heading, when help is dislayed. The header of the group is passed as argument to the method call. Look at the following example (`example-93.cpp`):

```c++
	int num1 = 1, num2 = 2, num3 = 3;
	int int1 = 1, int2 = 2, int3 = 3;
	double fp1 = 0., fp2 = 0., fp3 = 0.;
	string str1("group 1"), str2("group 2"), str3("group 3");
	vector<string> sources;
	try
	{
    CmdLineArgs cl(argc, argv);
    cl.set_doc("Clparser argp example 9.3 -- Help groups.");
    cl.begin_group("Group 1:");
    cl.option("num1", "number of objects",     &num1, 'n');
    cl.option("int1", "another number$INT",    &int1, 'i');
    cl.option("fp1",  "floating-point number", &fp1,  'f');
    cl.option("str1", "a string",              &str1, 's');
    cl.begin_group("Group 2:");
    cl.option("num2", "number of objects",     &num2);
    cl.option("int2", "another number$INT",    &int2);
    cl.option("fp2",  "floating-point number", &fp2 );
    cl.option("str2", "a string",              &str2);
    cl.begin_group("Group 3:");
    cl.option("num3", "number of objects",     &num3);
    cl.option("int3", "another number$INT",    &int3);
    cl.option("fp3",  "floating-point number", &fp3 );
    cl.option("str3", "a string",              &str3);
    cl.parse();
    sources = cl.sources();
  }
  catch (string& error_msg)
  {
    cout << "error: " << error_msg << endl;
    return 1;
  }
```

The output of the option `--help` is:

```
Usage: example-93 [OPTION...]
Clparser argp example 9.3 -- Help groups.

 Group 1:
  -f, --fp1=VALUE            floating-point number
  -i, --int1=INT             another number
  -n, --num1=VALUE           number of objects
  -s, --str1=STRING          a string

 Group 2:
      --fp2=VALUE            floating-point number
      --int2=INT             another number
      --num2=VALUE           number of objects
      --str2=STRING          a string

 Group 3:
      --fp3=VALUE            floating-point number
      --int3=INT             another number
      --num3=VALUE           number of objects
      --str3=STRING          a string

  -?, --help                 Give this help list
      --usage                Give a short usage message

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```

[ [Prev <](tutorial-8.html "Selections and Aliases (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]
