//*

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE FileIOTest

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <chrono>
#include <filesystem>
#include <algorithm>
#include "FileIO.h"
#include "SyntaxHandler.h"

//Check FileIO constructs
BOOST_AUTO_TEST_CASE(ConstructionAndDestruction)
{
  FileIO fileIO;
}

//Check FileIO can find directories
BOOST_AUTO_TEST_CASE(DirectoriesTest)
{
  FileIO fileIO;

  BOOST_REQUIRE_MESSAGE(
    fileIO.getWorkingDir() == std::string(std::filesystem::current_path()), 
    "ERROR: getWorkingDir() - EXPECTED: " 
    + std::string(std::filesystem::current_path())
    + " RECEIVED: " + fileIO.getWorkingDir());
  
  BOOST_REQUIRE_MESSAGE(
    fileIO.getDataDir() == std::string(std::filesystem::current_path()) +
    "/data", 
    "ERROR: getDataDir() - EXPECTED: " 
    + std::string(std::filesystem::current_path()) +
    "/data"
    + " RECEIVED: " + fileIO.getDataDir());
}

//Check FileIO can find files recursively
BOOST_AUTO_TEST_CASE(FileSearchTest)
{
  FileIO fileIO;

  auto filePaths = fileIO.findAllFiles( fileIO.getDataDir(), 
  ".txt", RECURSIVE_SEARCH_TRUE);

  for(auto & file : filePaths)
  {
    std::cout << file << std::endl;
  }

  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDataDir() + "/" + "fileIOTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDataDir() + "/" + "fileIOTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDataDir() + "/testSubFolder/fileIOSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDataDir() + "/testSubFolder/fileIOSubTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDataDir() + "/anotherTestSubFolder/fileIOAnotherSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDataDir() + "/anotherTestSubFolder/fileIOAnotherSubTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDataDir() + "/testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDataDir() + "/testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt");
}

//Check FileIO can read a file
BOOST_AUTO_TEST_CASE(FileReadTest)
{
  FileIO fileIO;
  SyntaxHandler syntax;

  auto filePaths = fileIO.findAllFiles( fileIO.getDataDir(), 
  ".txt", RECURSIVE_SEARCH_TRUE);

  std::string const fileToRead = *std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDataDir() + "/" + "fileIOTestData.txt");

  std::string line;
  auto & fileStream = fileIO.openInputStream("testFile", fileIO.getDataDir() + "\\fileIOTestData.txt");

  getline(fileStream, line);
  syntax.stripChar(line, '\n');
  syntax.stripChar(line, '\r');
  
  BOOST_REQUIRE_MESSAGE(line == "This is a test file.", 
    "ERROR: openInputStream() - EXPECTED: This is a test file - RECEIVED: " + line);
  
  
}

//*/