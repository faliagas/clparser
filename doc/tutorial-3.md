[ [Prev <](tutorial-2.html "Displaying Reports and Executing Commands") ] [ [Next >](tutorial-4.html "Sources and Data Validation") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 3: Selections - Mutually Exclusive Options - Aliases

In this lesson, you will learn how to use some utilities offered by **clparser** to make your code simpler and more efficient. Specifically, you will learn how to

- efficiently program options for indexes into vectors and arrays (_selections_)

- define mutually exclusive groups of options, and

- declare aliases, i.e. different options producing the same effect.


#### 3.1 Selections

Assume we have a string array,

```c++
const char* fruits[] {"apple", "orange", "peach", "melon"};
```

and we'd like to let the user select an entry of this array. We can implement this by declaring an integer `selection` so that the selected entry will be `fruits[selection]`. To do this safely, we must check if the user-supplied value of `selection` is in the expected range. A more tedious task is to document this option; the program's user does not know which fruit each number from 0 to 3 is associated with.

**Clparser** offers an alternative, more efficient way to handle situations like this. Instead of using an integer command-line option (which implies involved documentation frequently consulted by the user), we can treat each array entry as a standalone command-line option, mapping all these options to the same integer `selection`. This approach requires less documentation and is better maintained. The program invocation will look something like this:

```bash
$ prog -orange
```

**CmdLineArgs** will take care of the rest. Let's see it in action (`example-31.cpp`):

```c++
vector<string> fruits = {"apple", "orange", "peach", "melon"};
int selection = -1;
bool displaySpecs = false;
try {
  CmdLineArgs cl(argc, argv);
  cl.set_args_doc("-[apple|orange|peach|melon]");
  cl.option_help("--help", "display help");
  cl.option("-apple", "apple", 0, &selection);
  cl.option("-orange", "orange", 1, &selection);
  cl.option("-peach", "peach", 2, &selection);
  cl.option("-melon", "melon", 3, &selection);
  cl.option("-display-specs=", "display specs", &displaySpecs);
  cl.parse();
  cl.display_messages();
  if (selection == -1)
    throw string("One of -apple -orange -peach -melon "
                 "must be selected");
  if (displaySpecs) {
    cout << endl << "Command line specifications:" << endl;
    cl.display_specs();
  }
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << endl << "Selected fruit: " << fruits[selection] << endl;
```

The first line declares a list of objects, and `selection` in the following line is an object selector. The default value `-1` denotes that nothing has been selected. We handle this condition by throwing an error message. One could give `selection` a valid default value and throw no error message if the user does not select anything at the command line.

In the following line we declare a boolean to control the display of command-line specifications. The three lines that follow are clear. The method **set_args_doc()** tells the parser how to format help (see program output below). In the line below them we take advantage of **option_help()**-method's automated support for command line help, replacing the manual coding of the previous examples. The first argument to `option_help()` is the command-line argument, and the second is the help information.

The following four statements declare the items of our `fruits` list, which are selectable by the user (in our case: all). The first argument is the option's name as it appears in the command line; the second is its documentation; the third is the index of the selected object in the list; and finally, the last argument is a pointer to the `int` that will receive the user's selection.

If we run the program with the options `example-31 -orange -display-specs=on`, we'll get:

```bash
$ Command line specifications:
$ orange [1]
$ display specs: true
$
$ Selected fruit: orange
```

The user can specify only one of the options `-apple`, `-orange`, `-peach`, and `-melon` on the command line. If more options are specified, a warning message will be displayed. For example `example-31 -apple -peach -display-specs=on` produces

```bash
$ Option '-peach' conflicts with previous options. This option was ignored.
$
$ Command line specifications:
$ apple [0]
$ display specs: true
$
$ Selected fruit: apple
```

To make this warning a fatal error we use the **CmdLineArgs::set_conflict_fatal()** method:

```c++
  cl.set_conflict_fatal();
```

The usual placement of this method call is below **CmdLineArgs**'s declaration. In this case the result is:

```bash
$ error: Option '-peach' conflicts with previous options.
```

Finally, let us take a look at the output of `example-13 --help`:

```bash
$ Usage: example-31 [option1 option2 ...] -[apple|orange|peach|melon]
$
$ Available options:
$ --help                        display help
$ -apple                        apple
$ -orange                       orange
$ -peach                        peach
$ -melon                        melon
$ -display-specs=[yes|no]       display specs
```

#### 3.2 Mutually exclusive options (Groups)

Often the user may select only one out of a specific subset of options, while other options are freely selectable. Such _subsets of mutually exclusive options_ are called **groups** in the free-style version of **clparser**. They are not to be confused with the groups of the **argp** interface; **argp** uses groups for documentation purposes only.
We have already seen a particular type of group: _selections_. In general, to specify a group of options, we declare the first option of the group as usual with one of the **CmdLineArgs::option** methods and all other options of the group with a **CmdLineArgs::group** method. Assume we'd like to have a boolean option `--option=` for our program along with a shortcut version `-o=`, and additionally a no-arguments version of the same option, `-o`, whose task is to turn on and off the features associated with that option. The outcome is three options that do the same job. To avoid usage errors, we decide to restrict the user's access to these three options so that only one of the three options is selectable at the command line. Here is how to do that (`example-32.cpp`):

```c++
bool opt = false;
bool displaySpecs = false;
try {
  CmdLineArgs cl(argc, argv);
  cl.set_conflict_fatal();
  cl.option_help("--help", "display help");
  cl.option("-o", "enable option", &opt);
  cl.group ("-o=", "enable/disable option", &opt);
  cl.group ("--option=", "enable/disable option", &opt);
  cl.option("--display-specs", "display specs", &displaySpecs);
  cl.parse();
  cl.display_messages();
  if (displaySpecs) {
    cout << endl << "Command line specifications:" << endl;
    cl.display_specs();
  }
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << endl;
cout << "option: " << string(opt ? "en" : "dis") + "abled" << endl;
```

If we run the program with the arguments `-o --option=on` we get the following output:

```bash
Option '--option=on' conflicts with previous options. This option was ignored.

option: enabled
```

As usually, we insert the statement `cl.set_conflict_fatal();` below `cl`'s declaration to make option conflicts fatal. In this case we get the following message:

```bash
error: Option '--option=on' conflicts with previous options.
```

There is no difference if we use **groups** instead of **options** to handle selections. For example if we write

```c++
cl.option("-apple", "apple", 0, &selection);
cl.group ("-orange", "orange", 1, &selection);
cl.group ("-peach", "peach", 2, &selection);
cl.group ("-melon", "melon", 3, &selection);
```

in `example-31` leaving all other statements unchanged, the result will be the same (`example-32a.cpp`).

#### 3.3 Aliases

In the above examples, we used the option `--help` to display the program's usage information. However, the options `-r` and `-?` are commonly used for the same purpose. To declare several options producing the same effect, we use **aliases**. To declare an option along with some aliases, declare the option as usual, and then use the method **CmdLineArgs::alias()** to declare the aliases. For example, to add multiple _help_ options use **CmdLineArgs::option_help()**, which is essentially equivalent to an `option()` invocation, and then use **alias**, as in the following code snippet (`example-33.cpp`):

```c++
bool opt = false;
bool displaySpecs = false;
try {
  CmdLineArgs cl(argc, argv);
  cl.option_help("--help", "display help");
  cl.alias ("-h");
  cl.alias ("-?");
  cl.option("-o", "enable option", &opt);
  cl.group ("-o=", "enable/disable option", &opt);
  cl.group ("--option=", "enable/disable option", &opt);
  cl.option("-display-specs", "display specs", &displaySpecs);
  cl.parse();
  cl.display_messages();
  if (displaySpecs) {
    cout << endl << "Command line specifications:" << endl;
    cl.display_specs();
  }
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}
cout << "option: " << string(opt ? "en" : "dis") + "abled" << endl;
```

Any one of the options `--help`, `-h` or `-?` produces the output:

```bash
Usage: example-33 [option1 option2 ...]

Available options:
--help                        display help
-h                            display help
-?                            display help
-o                            enable option
-o=[yes|no]                   enable/disable option
--option=[yes|no]             enable/disable option
-display-specs                display specs
```

[ [Prev <](tutorial-2.html "Displaying Reports and Executing Commands") ] [ [Next >](tutorial-4.html "Sources and Data Validation") ] [ [Up](tutorial.html "Table of Contents") ]
