#include <iostream>
#include "clparser.hpp"

using std::cout; using std::endl;
using std::string; using std::vector;

int main(int argc, char *argv[]) {
  int i = -1;
  string text;
  string sourceFile;
  vector<string> extraArgs;

  try {
    CmdLineArgs cl(argc, argv);
    cl.set_doc("A not so trivial example for argp");
    cl.set_args_doc("[SOURCE [its_options]]");
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

  return 0;
}
