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
    fileIO.getBaseDirectory() == std::string(std::filesystem::current_path()), 
    "ERROR: getBaseDirectory() - EXPECTED: " 
    + std::string(std::filesystem::current_path())
    + " RECEIVED: " + fileIO.getBaseDirectory());

    fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");

    BOOST_REQUIRE_MESSAGE(
    fileIO.getDirectoryPath("data") == 
      std::string(std::filesystem::current_path()) + "/data", 
    "ERROR: getDirectoryPath() - EXPECTED: " 
    + std::string(std::filesystem::current_path()) + "/data"
    + " RECEIVED: " + fileIO.getDirectoryPath("data"));
}

//Check FileIO can find files recursively
BOOST_AUTO_TEST_CASE(FileSearchTest)
{
  FileIO fileIO;
  fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");

  auto filePaths = fileIO.findAllFiles( fileIO.getDirectoryPath("data"), 
  ".txt", RECURSIVE_SEARCH_TRUE);

  for(auto & file : filePaths)
  {
    std::cout << file << std::endl;
  }

  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/" + "fileIOTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDirectoryPath("data") + "/" + "fileIOTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/testSubFolder/fileIOSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDirectoryPath("data") + "/testSubFolder/fileIOSubTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/anotherTestSubFolder/fileIOAnotherSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDirectoryPath("data") + "/anotherTestSubFolder/fileIOAnotherSubTestData.txt");
  
  BOOST_REQUIRE_MESSAGE(
    std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt") != filePaths.end(), 
    "ERROR: findAllFiles() - EXPECTED: " 
    + fileIO.getDirectoryPath("data") + "/testSubFolder/yetAnotherTestSubFolder/fileIOYetAnotherSubTestData.txt");
}

//Check FileIO can read a file
BOOST_AUTO_TEST_CASE(FileReadTest)
{
  FileIO fileIO;
  fileIO.setDirectoryPath("data", fileIO.getBaseDirectory() + "/data");

  SyntaxHandler syntax;

  auto filePaths = fileIO.findAllFiles( fileIO.getDirectoryPath("data"), 
  ".txt", RECURSIVE_SEARCH_TRUE);

  std::string const fileToRead = *std::find(filePaths.begin(), filePaths.end(), 
    fileIO.getDirectoryPath("data") + "/" + "fileIOTestData.txt");

  std::string line;
  std::string line2;
  auto & fileStream = fileIO.openInputStream("testFile", fileIO.getDirectoryPath("data") + "/fileIOTestData.txt");

  getline(fileStream, line);
  syntax.stripChar(line, '\n');
  syntax.stripChar(line, '\r');
  
  BOOST_REQUIRE_MESSAGE(line == "This is a test file.", 
    "ERROR: openInputStream() - EXPECTED: This is a test file - RECEIVED: " + line);
  
  fileIO.resetInputStreamToFileStart("testFile");

  getline(fileStream, line2);
  syntax.stripChar(line2, '\n');
  syntax.stripChar(line2, '\r');
  
  BOOST_REQUIRE_MESSAGE(line == line2, 
    "ERROR: resetInputStreamToFileStart() - EXPECTED: This is a test file - RECEIVED: " + line2);

  fileIO.resetInputStreamToFileStart("testFile");

  fileIO.readLine("testFile", line2);
  syntax.stripChar(line2, '\n');
  syntax.stripChar(line2, '\r');
  
  BOOST_REQUIRE_MESSAGE(line == line2, 
    "ERROR: readLine(streamIDIn, lineToWriteTo) - EXPECTED: This is a test file - RECEIVED: " + line2);

  fileIO.resetInputStreamToFileStart("testFile");

  line2 = fileIO.readLine("testFile");
  syntax.stripChar(line2, '\n');
  syntax.stripChar(line2, '\r');
  
  BOOST_REQUIRE_MESSAGE(line == line2, 
    "ERROR: readLine(streamIDIn) - EXPECTED: This is a test file - RECEIVED: " + line2);
}

//*/