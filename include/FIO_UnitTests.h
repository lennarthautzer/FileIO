#ifndef FILE_IO_UNIT_TESTS_H_
#define FILE_IO_UNIT_TESTS_H_

#include "FIO.h"
#include "SyntaxHandler.h"
#include "dessert.hpp"

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <iostream>

static int const constexpr numberOfFilesToTest = 10;
static int const constexpr numberOfLinesToTest = 10;

namespace FIOTests
{
  using namespace NSFIO;

  class FIO_UnitTests
  {
  public:
    static FIO_UnitTests & getInstance();

    FIO_UnitTests( FIO const & FIO_UnitTests ) = delete;
    FIO_UnitTests & operator=( FIO_UnitTests const & FIO_UnitTests ) = delete;

    void runAllUnitTests();
    void runPathTest();
    void runStreamTest();
    void runStringConversionTest();
    void runResetTest();
    void runDirectoriesTest();
    void runFileSearchTest();
    void runFileReadTest();
    void runFileWriteTest();
    void runOverloadTest();
    void runWholeFileReadTest();
    void runCSVTest();

  protected:
    explicit FIO_UnitTests();

    static std::unique_ptr< FIO_UnitTests > instance;

  private:
    FIO & fio;
  };
} // namespace FIOTests

#endif