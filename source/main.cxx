//*
#include "FIO_UnitTests.hxx"
#include "FIO.hxx"

using namespace FIOTests;
using namespace FileIO;

int main( int argc, char * argv[] )
{
  FIO_UnitTests unitTester;

  unitTester.runAllUnitTests();

  return 0;
}
//*/