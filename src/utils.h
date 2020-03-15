#include <ctime>
#include <cmath>
#include <vector>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <exception>

enum MY_EXCEPTIONS {FILE_NOT_FOUND_EXCEPTION, USAGE_EXCEPTION};

std::string readFile(char*);
void checkCliArguments(int, char*[], unsigned int &, unsigned int &, std::string&);
