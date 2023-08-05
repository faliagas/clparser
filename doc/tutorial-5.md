[ [Prev <](tutorial-4.html "Sources and Data Validation") ] [ [Next >](tutorial-6.html "Argp-Style Argument Handling") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 5: Customization and Advanced Techniques

In this lesson, you will learn

- how to customize **clparser**

- certain advanced techniques

#### 5.1 Customizations
We have seen in 1.3 how to make **CmdLineArgs** less tolerant of extraneous arguments by using the statement `cl.set_extra_fatal()` Here is a rundown of **CmdLineArgs** methods used to control **clparser**:

```c++
void tolerate_assign();
```

Set a flag controlling the internal algorithm used by **clparser** to decide if an unrecognized option is a source.

When this flag is set, every unrecognized option that contains '`=`' (the assignment character) is deemed by **clparser** a source; if all other prerequisite conditions are met. When this flag is not set (this is the default), the opposite behavior applies.

```c++
void set_extra_fatal();
```

When this is used, extraneous (unrecognized) options are fatal errors.

```c++
void set_conflict_fatal();
```

When this is used, conflicting options is a fatal error.

```c++
void set_assignment(const char ch);
```

> This is not fully implemented in the current version of **clparser**; do not use it.

Changes the default assignment character ('`=`').

#### 5.2 Nesting CmdLineArgs

**CmdLineArgs** is fully re-entrant, and thus, nesting **CmdLineArgs** instances within **clparser** _lambdas_ or _handlers_ is possible.

#### 5.3 Reading program arguments from files

> This feature is not available in the current version of **clparser**.

It is often convenient to have the program arguments in a file, from which the program can easily read them as if they had been specified in the command line. For example,

```
$ myprog --num=2 --@="file containing args.txt" --enable-feature1
```

When **clparser** encounters the reserved option `--@=`, it suspends command-line argument parsing and starts reading and parsing arguments from the specified file. When finished, it resumes parsing at the first command-line argument next to the `--@` option.

The format of this file is identical to that of the command line. Instead of a single long line, the user can put arguments in multiple lines; line breaks being equivalent to spaces.

[ [Prev <](tutorial-4.html "Sources and Data Validation") ] [ [Next >](tutorial-6.html "Argp-Style Argument Handling") ] [ [Up](tutorial.html "Table of Contents") ]
