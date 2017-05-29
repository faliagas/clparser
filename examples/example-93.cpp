#include <iostream>
#include "clparser.hpp"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char *argv[])
{
	int num1 = 1, num2 = 2, num3 = 3;
	int int1 = 1, int2 = 2, int3 = 3;
	double fp1 = 0., fp2 = 0., fp3 = 0.;
	string str1("group 1"), str2("group 2"), str3("group 3");
	vector<string> sources;
	try
	{
    CmdLineArgs cl(argc, argv);
    cl.set_doc("Clparser argp example 9.3 -- Help groups.");
    cl.begin_group("Group 1:");
    cl.option("num1", "number of objects",     &num1, 'n');
    cl.option("int1", "another number$INT",    &int1, 'i');
    cl.option("fp1",  "floating-point number", &fp1,  'f');
    cl.option("str1", "a string",              &str1, 's');
    cl.begin_group("Group 2:");
    cl.option("num2", "number of objects",     &num2);
    cl.option("int2", "another number$INT",    &int2);
    cl.option("fp2",  "floating-point number", &fp2 );
    cl.option("str2", "a string",              &str2);
    cl.begin_group("Group 3:");
    cl.option("num3", "number of objects",     &num3);
    cl.option("int3", "another number$INT",    &int3);
    cl.option("fp3",  "floating-point number", &fp3 );
    cl.option("str3", "a string",              &str3);
    cl.parse();
    sources = cl.sources();
  }
  catch (string& error_msg)
  {
    cout << "error: " << error_msg << endl;
    return 1;
  }
	cout << "num1 = " << num1 << endl;
	cout << "int1 = " << int1 << endl;
	cout << "fp1  = " << fp1  << endl;
	cout << "str1 = " << str1 << endl;
	cout << "num2 = " << num2 << endl;
	cout << "int2 = " << int2 << endl;
	cout << "fp2  = " << fp2  << endl;
	cout << "str2 = " << str2 << endl;
	cout << "num3 = " << num3 << endl;
	cout << "int3 = " << int3 << endl;
	cout << "fp3  = " << fp3  << endl;
	cout << "str3 = " << str3 << endl;
	if (sources.size() != 0)
    cout << "sources:" << endl;
	for (auto source: sources)
    cout << source << " ";
	if (sources.size() != 0)
    cout << endl;
  return 0;
}
