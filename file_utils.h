#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <cstring>

namespace fs = std::filesystem;

using namespace std;

void createDirectory(char *directoryName);
void createFile(char *fileName);
void deleteFile(char *fileName);
char* readFromFile(char *fileName);
void eraseLastLine(char *fileName);
char* readLastLine(char *fileName);
void removeBreakLine(char* text);
void writeToFile(char *fileName, char *text);