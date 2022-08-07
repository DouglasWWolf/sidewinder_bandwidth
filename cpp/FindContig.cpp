
//=================================================================================================
// FindContig.cpp - Finds the reserved contiguous buffer assigned at Linux boot time
//=================================================================================================
#include <unistd.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/mman.h>
#include <sys/fcntl.h>
#include <string.h>
#include <string>
#include <fstream>
using namespace std;



//=================================================================================================
// throwRuntime() - Throws a runtime exception
//=================================================================================================
static void throwRuntime(const char* fmt, ...)
{
    char buffer[1024];
    va_list ap;
    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);

    throw runtime_error(buffer);
}
//=================================================================================================




//=================================================================================================
// parseKMG() - Examines a string for a delimeter, and parses the integer immediately after 
//              the delimeter.  Looks for the character after the digits, expect to find a K, M,
//              or G (meaning Kilo, Mega, or Giga) and returns the parsed value.
//
//              Example:  4G = 0x1_0000_0000
//                        2K = 0x400
//                        3M = 0x30_0000
//
// If the delimieter is not found or the string is malformed in some way, returns 0;
//=================================================================================================
static uint64_t parseKMG(const char delimeter, const char* ptr)
{
    // Look for the delimeter in the string the user gave us
    ptr = strchr(ptr, delimeter);

    // If the delimeter didn't exist, tell the caller
    if (ptr == nullptr) return 0;

    // Point to the character after the delimeter
    ++ptr;

    // Convert the ASCII digits that follow the delimeter to an integer
    int64_t value = strtol(ptr, 0, 0);

    // Skip over all of the ASCII digits
    while (*ptr >= '0' && *ptr <= '9') ++ptr;

    // Return the appropriate scaled integer value
    if (*ptr == 'K') return value * 1024;
    if (*ptr == 'M') return value * 1024 * 1024;
    if (*ptr == 'G') return value * 1024 * 1024 * 1024;

    // If we get here, there wasn't a K, M, or G after the numeric value
    return 0;
}
//=================================================================================================





//=================================================================================================
// findContig() - Finds the physical address of a reserved contiguous buffer
//=================================================================================================
uint64_t findContig()
{
    string line;
    const uint64_t ONE_GIG = 1024 * 1024 * 1024;
    const char* filename = "/proc/cmdline";

    // Open the specified file.  It will contain a line of ASCII data
    ifstream file(filename);

    // If we couldn't open the file, tell the caller
    if (!file.is_open()) throwRuntime("Can't open %s", filename);
    
    // Fetch the first line of the file
    getline(file, line);

    // Look for "memmap=" in the command line
    const char* p = ::strstr(line.c_str(), "memmap=");

    // If we can't find "memmap=", something is awry
    if (p == nullptr) throwRuntime("Malformed %s", filename);

    // Fetch the value after the '='
    auto size = parseKMG('=', p);

    // Fetch the value after the '$'
    auto physAddr = parseKMG('$', p);

    // Warn the user if there's no reserved buffer
    if (physAddr == 0) throwRuntime("No reserved contiguous buffer found!");

    // If we couldn't parse one of those values, /proc/cmdline is malformed
    if (size < ONE_GIG) throwRuntime("Reserved buffer size of 0x%lx is too small!", size);

    // Return the physical address of the reserved contiguous buffer
    return physAddr;
}
//=================================================================================================

