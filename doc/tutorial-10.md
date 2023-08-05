[ [Prev <](tutorial-9.html "Advanced Help (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]

### Lesson 10: A More Challenging Example

In this lesson, we demonstrate how to use the **argp** interface to treat a task often encountered in real-world programming situations. More precisely, we have a program that handles a number of options and a source that expects its own set of arguments. The source's arguments can be several options, possibly the same as those of the program, and some sources. The source and its arguments are optional. The code of this example is in `example-gnu-5.cpp`.

```c++
int i = -1;
string text;
string sourceFile;
vector<string> extraArgs;

try {
  CmdLineArgs cl(argc, argv);
  cl.set_args_doc("[SOURCE [its_options]]");
  cl.set_doc("A not so trivial example for argp");
  cl.option("iopt", "integer option", &i, 'i');
  cl.option("text", "text option", &text, 't');
  cl.set_on_arg([&extraArgs](argp_state* state, char* arg, int* rc) -> bool {
    if (state->arg_num != 0)
      throw string{"internal error, unexpected source argument"};
    for (size_t i = state->next; i < state->argc; ++i)
      extraArgs.emplace_back(string{state->argv[i]});
    state->next = state->argc;
    *rc = 0;
    return /* handled? */ false; // it passes control to default handler
  });
  cl.parse_cmd_line(ARGP_IN_ORDER);
  if (!cl.sources().empty())
    sourceFile = cl.sources().front();
}
catch (const string& error_msg) {
  cout << "error: " << error_msg << endl;
  return 1;
}

if (i != -1)
  cout << "i: " << i << endl;
if (!text.empty())
  cout << "text: " << text << endl;
if (!sourceFile.empty())
  cout << "source file: " << sourceFile << endl;
if (!extraArgs.empty()) {
  cout << "extra args: ";
  for (const auto& item : extraArgs)
    cout << item << " ";
  cout << endl;
}
```

The first noteworthy difference is the statement `cl.parse_cmd_line(ARGP_IN_ORDER)` instead of the familiar `cl.parse()`. When **argp** parses the command line, it usually reorders the program's arguments. Since our task requires that arguments be parsed in order, the flag `ARGP_IN_ORDER` must be used. The `parse` method maps to `parse_cmd_line(0)`.

To collect the arguments that follow a source (note that in the GNU terminology, the sources are called _arguments_) we must handle the `arg` (`ARGP_KEY_ARG`) event, which occurs when the parser comes across a source. This is done by using the `set_on_arg` method to specify a lambda which takes as arguments a pointer to an [argp_state structure](https://www.gnu.org/software/libc/manual/html_node/Argp-Parsing-State.html "Argp Parsing State"), the argument, and a pointer to an integer, the handler's return code. The `next` member of that structure is the index of the argument next to the current one.

After having finished collecting the source's arguments, we set `next` to point at the end of the argument list to indicate to the parser that there are no more arguments. Finally, we return `false` to tell the parser to invoke the default handler immediately after the lambda has returned. Recall that the default handler's action is to store the source's name. An alternative option is to store the source by using the `arg` field of the `argp_state` structure and return `true` to skip the default handling.

[ [Prev <](tutorial-9.html "Advanced Help (Argp)") ] [ [Up](tutorial.html "Table of Contents") ]

