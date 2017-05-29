#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
	int num = 1;
	try
	{
    CmdLineArgs cl(argc, argv);
    cl.set_doc("Clparser argp example 7.2 -- Argument handling.");
    cl.option("num", "number of objects, where $INTEGER$ is from 1 to 16",
      [&num](const char *arg)
      {
        size_t sz;
        num = std::stoi(arg, &sz);
        if (sz < string(arg).length())
          throw string("An invalid integer was supplied for option num");
        if (num < 1 || num > 16)
          throw string("num is out of range 1 to 16");
        return false;
      });
    cl.option("skata", "skata", &num);
    cl.parse();
  }
  catch (string& error_msg)
  {
    cout << "error: " << error_msg << endl;
    return 1;
  }
  catch (std::invalid_argument& a)
  {
    cout << "error: " << "An invalid integer was supplied for option num" << endl;
    return 1;
  }
	cout << "num = " << num << endl;
  return 0;
}
