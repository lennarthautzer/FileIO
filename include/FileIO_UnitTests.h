#ifndef FILE_IO_UNIT_TESTS_H_
#define FILE_IO_UNIT_TESTS_H_

#include "FileIO.h"
#include "SyntaxHandler.h"
#include "dessert.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>

static int const constexpr numberOfFilesToTest = 10;
static int const constexpr numberOfLinesToTest = 10;

class FileIO_UnitTests
{
public:
  static FileIO_UnitTests & getInstance();

  FileIO_UnitTests( FileIO const & FileIO_UnitTestsToCopy ) = delete;
  FileIO_UnitTests & operator=( FileIO_UnitTests const & FileIO_UnitTestsToCopy ) = delete;

  void runAllUnitTests();
  void runResetTest();
  void runDirectoriesTest();
  void runFileSearchTest();
  void runFileReadTest();
  void runFileWriteTest();
  void runOverloadTest();
  void runWholeFileReadTest();

protected:
  explicit FileIO_UnitTests();

  static std::unique_ptr< FileIO_UnitTests > singletonInstance;

private:
  FileIO & fileIO;
};

#endif