//*

#include "FileIO_UnitTests.h"

int main( int argc, char * argv[] )
{
  FileIO_UnitTests & unitTester = FileIO_UnitTests::getInstance();

  unitTester.runAllUnitTests();

  return 0;
}

//*/