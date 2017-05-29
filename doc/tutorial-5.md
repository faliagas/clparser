[ [Prev <](tutorial-4.html "Sources and Data Validation") ] [ [Next >](tutorial-6.html "Argp-Style Argument Handling") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 5: Customization and Advanced Techniques

In this lesson you will learn

- how to customize **clparser**

- certain advanced techniques

#### 5.1 Customizations
We have seen in 1.3 how to make **CmdLineArgs** less tolerant of extra arguments by using the statement `cl.set_extra_fatal()` Here is a full list of **CmdLineArgs** methods which can be used to control **clparser**:

```c++
void tolerate_assign();
```
Sets a flag which controls the internal algorithm according to which **clparser** decides if an unrecognized option is a source or not.

When this flag is set, every unrecognized option that contains '`=`' (the assignment character) is deemed by **clparser** to be a source; if all other prerequisite conditions are met. When this flag is not set (default), the opposite behavior is enforced.

```c++
void set_extra_fatal();
```
When this is used, extra (unrecognized) options are considered to be fatal errors.

```c++
void set_conflict_fatal();
```
When this is used, the specification of conflicting options is considered to be a fatal error.

```c++
void set_assignment(const char ch);
```
> This is not fully implemented in the current version of **clparser**, and should not be used yet.

Changes the default assignment character ('`=`').

#### 5.2 Nesting CmdLineArgs
**CmdLineArgs** is fully re-entrant, so nesting **CmdLineArgs** instances within **clparser** _lambdas_ or _handlers_ is possible.

#### 5.3 Reading program arguments from files

> This feature is not available in the current version of **clparser**.

It is often convenient to have program arguments stored in a file, from which the program can easily read the specified arguments as if they had been specified in the command line. For example,

```
$ myprog --num=2 --@="file containing args.txt" --enable-feature1
```

When **clparser** encounters the reserved option `--@=`, it suspends command-line argument parsing, and starts reading and parsing arguments from the specified file. When finished, it resumes parsing at the first command-line argument next to the `--@` option.

The format of this file is identical to the format of the command line. Instead of a single very long line, multiple lines can be used; line breaks are equivalent to spaces.

[ [Prev <](tutorial-4.html "Sources and Data Validation") ] [ [Next >](tutorial-6.html "Argp-Style Argument Handling") ] [ [Up](tutorial.html "Table of Contents") ]
