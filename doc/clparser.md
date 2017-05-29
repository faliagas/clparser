# Clparser #

Command Line Parser (**_clparser_**) makes it easy to write user-friendly command-line interfaces for applications and scientific or computational programs. It comes in two flavors: **argp** and **free-style**. The **argp** interface is a front-end to **argp**.

### Description ###
The program declares an instance of the C++ class **CmdLineArgs** and defines what arguments the program requires. Then a call to the method **CmdLineArgs::parse**() parses the user supplied arguments, which are passed as arguments to the **main**() function, taking care to place user supplied values to program variables and execute any specified actions. **CmdLineArgs** also automatically generates help and usage messages, and issues warnings and errors when necessary.

**CmdLineArgs** is an alternative to [getopt](https://www.gnu.org/software/libc/manual/html_node/Getopt.html) and [argp](https://www.gnu.org/software/libc/manual/html_node/Argp.html), but much easier, faster and more powerful.

### Features ###
The user can specify:

* _Integer_, _floating-point_, _string_ and _boolean_ data.   
>For example, assuming our program is `myprog`, the command line
```
$ myprog -max_num_iter=32 -temperature=+1.e-2 -where="right boundary"
```
>specifies an integer (`-max_num_iter`), a floating point number (`-temperature`) and a string. The following command line specifies boolean values:
```
$ myprog -use_emulation -extra-precision=no
```
>
>**Clparser** will take care to collect all these values and place them automatically to the appropriate program variables.
* _Selectors_ or _offsets_ in arrays  
>Assume in our program we have the following array  
```c++
const char *fruits[] = {"orange", "apple", "grape", "peach", "watermelon"};  
int ofs = 1; // default selection is apple
```
>and we'd like to have the user select a fruit. This is specified on the command line as follows:
```
$ myprog -peach
```
>**Clparser** will automatically set the value of `ofs` to 3.

* _Mutually exclusive groups of options_.

* _Actions_. They are command line specifications which trigger the execution of code in the program.

* _Sources_ and _targets_. They are files, directories and other string arguments. The program is responsible for their interpretation. **Clparser** collects them automatically into a `vector`.

### Documentation ###
**_CmdLineArgs_** was designed with minimal learning requirements in mind. A tutorial is available, and many examples are in the _examples_ folder.

### Usage ###
Compile your program and link it to the appropriate **clparser** library. For more details look at the tutorial.

Sun 28 May 2017 23:01:36 EEST
