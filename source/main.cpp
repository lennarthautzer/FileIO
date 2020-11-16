//*

#include "FIO_UnitTests.h"
#include <locale.h>

using namespace FIOTests;

int main( int argc, char * argv[] )
{
  std::cout << "Begin Tests" << std::endl;

  FIO_UnitTests & unitTester = FIO_UnitTests::getInstance();

  unitTester.runAllUnitTests();

  std::cout << "Tests Complete." << std::endl;

  return 0;
}

//*/