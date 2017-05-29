#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char *argv[])
{
	int num = 1;
	vector<string> sources;
	try
	{
    CmdLineArgs cl(argc, argv);
    cl.set_doc("Clparser argp example 7.1 -- Displaying information.");
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
  return 0;
}
