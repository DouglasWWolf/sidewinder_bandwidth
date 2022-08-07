//=================================================================================================
// measure_bw - Measures PCI and RAM bandwidth on a Fidus Sidewinder
//
// Author: Doug Wolf
//=================================================================================================
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include "PciDevice.h"

// Extract the high and low 32-bits of a 64-bit word
#define HI32(x) ((x >> 32) & 0xFFFFFFFF)
#define LO32(x) (x & 0xFFFFFFFF)

// This maps PCI resources into user-space
PciDevice PCI;

// This defines which PCI resource (i.e., BAR) has the AXI slave registers mapped
const int AXIREG_RESOURCE = 0;

// This is defined in FindContig.cpp
uint64_t findContig();

// These are the base addresses of the "Measure Bandwidth" AXI slaves
const int MBW_PCI = 0x1000;
const int MBW_DDR = 0x2000;

// The clock speeds (in MHz) at which the "Measure Bandwidth" AXI slaves are being driven
const double PCI_CLOCK_SPEED = 250.0;
const double DDR_CLOCK_SPEED = 266.5;

// Register map for the "measure_bw" RTL core
enum 
{
   REG_RADDR_H   = 0,
   REG_RADDR_L   = 1,
   REG_WADDR_H   = 2,
   REG_WADDR_L   = 3,
   REG_BLK_SIZE  = 4,
   REG_COUNT     = 5,
   REG_RRESULT_H = 6,
   REG_RRESULT_L = 7,
   REG_WRESULT_H = 8,
   REG_WRESULT_L = 9,
   REG_CTL_STAT  = 10
};

// These are the constants to write to the CTL_STAT register 
const int START_READ  = 1;
const int START_WRITE = 2;



//=================================================================================================
// measureReadBandwidth() - Returns the number of clock-cycles it took to perform the requested
//                          bandwidth measurement
//
// Passed: deviceAddress = The AXI address of the bandwidth measurement core
//         axiAddress    = The first address to read or write from
//         blockSize     = The number of bytes in one AXI burst
//         blockCount    = The total number of bursts to perform
//=================================================================================================
uint64_t measureReadBandwidth(uint32_t deviceAddress, uint64_t axiAddress, uint32_t blockSize,
                              uint32_t blockCount)
{

   // Get a pointer to our measurement engine's AXI registers
   volatile uint32_t* engine = (uint32_t*) (PCI.resourceList()[AXIREG_RESOURCE].baseAddr + deviceAddress);

   // Configure the bandwith measurement core
   engine[REG_RADDR_H ] = HI32(axiAddress);
   engine[REG_RADDR_L ] = LO32(axiAddress);
   engine[REG_BLK_SIZE] = blockSize;
   engine[REG_COUNT   ] = blockCount;

   // Start the bandwidth measurement
   engine[REG_CTL_STAT] = START_READ;

   // Wait for the measurement to complete
   while (engine[REG_CTL_STAT]) usleep(10000);

   // Fetch the number of clock cycles the measurement took
   uint64_t result_hi = engine[REG_RRESULT_H];
   uint64_t result_lo = engine[REG_RRESULT_L];

   // And return the elapsed number of clock cycles to the caller
   return (result_hi << 32) | result_lo;
}
//=================================================================================================


//=================================================================================================
// measureWriteBandwidth() - Returns the number of clock-cycles it took to perform the requested
//                           bandwidth measurement
//
// Passed: deviceAddress = The AXI address of the bandwidth measurement core
//         axiAddress    = The first address to read or write from
//         blockSize     = The number of bytes in one AXI burst
//         blockCount    = The total number of bursts to perform
//=================================================================================================
uint64_t measureWriteBandwidth(uint32_t deviceAddress, uint64_t axiAddress, uint32_t blockSize,
                               uint32_t blockCount)
{
   // Get a pointer to our measurement engine's AXI registers
   volatile uint32_t* engine = (uint32_t*) (PCI.resourceList()[AXIREG_RESOURCE].baseAddr + deviceAddress);

   // Configure the bandwith measurement core
   engine[REG_WADDR_H ] = HI32(axiAddress);
   engine[REG_WADDR_L ] = LO32(axiAddress);
   engine[REG_BLK_SIZE] = blockSize;
   engine[REG_COUNT   ] = blockCount;

   // Start the bandwidth measurement
   engine[REG_CTL_STAT] = START_WRITE;

   // Wait for the measurement to complete
   while (engine[REG_CTL_STAT]) usleep(10000);

   // Fetch the number of clock cycles the measurement took
   uint64_t result_hi = engine[REG_WRESULT_H];
   uint64_t result_lo = engine[REG_WRESULT_L];

   // And return the elapsed number of clock cycles to the caller
   return (result_hi << 32) | result_lo;
}
//=================================================================================================




//=================================================================================================
// process() - Take the bandwidth measurements and report the results
//
// Passed: contigAddress = physical address of a reserved contiguous buffer on this computer
//                         that is at least 1 GB in size
//=================================================================================================
void process(uint64_t contigAddress)
{
   double nanoseconds, gbPerSec;
   uint64_t cycles;

   // We're going to transfer 1 GB of data
   uint64_t xferSize  = 1024 * 1024 * 1024;
   
   // Define the size of each AXI burst (in bytes)
   uint32_t burstSize = 2048;

   //-----------------------------------------------------------------------
   // >>>>>>>>>>>>>>>>>>>>  Measure PCI write bandwidth  <<<<<<<<<<<<<<<<<<<<
   //-----------------------------------------------------------------------

   // Measure the number of clock cycles required to read the data from the PCI bus
   cycles = measureWriteBandwidth(MBW_PCI, contigAddress, burstSize, xferSize / burstSize);

   // Translate the measured number of clock cycles into nanoseconds
   nanoseconds = cycles * 1000 / PCI_CLOCK_SPEED;
   
   // Compute the bandwidth in GB/sec
   gbPerSec = xferSize / nanoseconds;

   // Tell the user the bandwidth for writing to the PCI bus
   printf("%5.1lf Mhz PCI write time = %9lu cycles (%4.1lf GB/sec)\n", PCI_CLOCK_SPEED, cycles, gbPerSec);

   //-----------------------------------------------------------------------
   // >>>>>>>>>>>>>>>>>>>>  Measure DDR write bandwidth  <<<<<<<<<<<<<<<<<<<<
   //-----------------------------------------------------------------------

   // Measure the number of clock cycles required to read the data from the PCI bus
   cycles = measureWriteBandwidth(MBW_DDR, 0, burstSize, xferSize / burstSize);

   // Translate the measured number of clock cycles into nanoseconds
   nanoseconds = cycles * 1000 / DDR_CLOCK_SPEED;
   
   // Compute the bandwidth in GB/sec
   gbPerSec = xferSize / nanoseconds;

   // Tell the user the bandwidth for writing to DDR RAM
   printf("%5.1lf Mhz DDR write time = %9lu cycles (%4.1lf GB/sec)\n", DDR_CLOCK_SPEED, cycles, gbPerSec);

   //-----------------------------------------------------------------------
   // >>>>>>>>>>>>>>>>>>>>  Measure PCI read bandwidth  <<<<<<<<<<<<<<<<<<<<
   //-----------------------------------------------------------------------

   // Measure the number of clock cycles required to read the data from the PCI bus
   cycles = measureReadBandwidth(MBW_PCI, contigAddress, burstSize, xferSize / burstSize);

   // Translate the measured number of clock cycles into nanoseconds
   nanoseconds = cycles * 1000 / PCI_CLOCK_SPEED;
   
   // Compute the bandwidth in GB/sec
   gbPerSec = xferSize / nanoseconds;

   // Tell the user the bandwidth for reading from the PCI bus
   printf("%5.1lf Mhz PCI read time  = %9lu cycles (%4.1lf GB/sec)\n", PCI_CLOCK_SPEED, cycles, gbPerSec);

   //-----------------------------------------------------------------------
   // >>>>>>>>>>>>>>>>>>>>  Measure DDR read bandwidth  <<<<<<<<<<<<<<<<<<<<
   //-----------------------------------------------------------------------
   
   // Measure the number of clock cycles required to read the data from the PCI bus
   cycles = measureReadBandwidth(MBW_DDR, 0, burstSize, xferSize / burstSize);

   // Translate the measured number of clock cycles into nanoseconds
   nanoseconds = cycles * 1000 / DDR_CLOCK_SPEED;
   
   // Compute the bandwidth in GB/sec
   gbPerSec = xferSize / nanoseconds;

   // Tell the user the bandwidth for reading from DDR RAM
   printf("%5.1lf Mhz DDR read time  = %9lu cycles (%4.1lf GB/sec)\n", DDR_CLOCK_SPEED, cycles, gbPerSec);

}
//=================================================================================================



//=================================================================================================
// main() - Execution begins here
//=================================================================================================
int main()
{
   try
   {
      // Map the Sidewinder's PCI resources into userspace
      PCI.open(0x10ee, 0x903f);

      // Find the address of the reserved contiguous buffer
      uint64_t contigAddress = findContig();

      // And go measure and report our bandwidth 
      process(contigAddress);
   }

   catch(const std::exception& e)
   {
      printf("%s\n", e.what());
   }
}
//=================================================================================================
