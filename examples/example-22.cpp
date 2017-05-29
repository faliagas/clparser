#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;

bool show_ver(CmdLineArgs *self, void *p, const char *)
{
   cout << "Program " << self->program_name() << " version: "
        << *static_cast<std::string *>(p) << endl;
   return true;
}

int main(int argc, char *argv[])
{
  std::string ver = "1.2.3";

  try
  {
    CmdLineArgs cl(argc, argv);
    cl.option("--version", "Display program version", show_ver, &ver);
    cl.parse();
    cl.display_messages();
    cout << "Command line specifications:" << endl;
    cl.display_specs();
  }
  catch (std::string& error_msg)
  {
    cout << "error: " << error_msg << endl;
    return 1;
  }

  return 0;
}
