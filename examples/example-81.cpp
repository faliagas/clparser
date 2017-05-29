#include <iostream>
#include "clparser.hpp"

using std::cout; using std::endl;
using std::string; using std::vector;

int main(int argc, char *argv[])
{
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
  catch (std::string& error_msg)
  {
    cout << "error: " << error_msg << endl;
    return 1;
  }

//  cout << endl;
  cout << "Selected fruit: " << fruits[selection] << endl;

  return 0;
}
