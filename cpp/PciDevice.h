//=================================================================================================
// PciDevice.h - Defines a generic class for mapping PCIe devices into user-space
//=================================================================================================
#pragma once
#include <string>
#include <vector>

class PciDevice
{
public:
   
    // Default constructor
    PciDevice() {};

    // Destructor
    ~PciDevice() {close();}

    // No copy or assignment constructor - objects of this class can't be copied
    PciDevice (const PciDevice&) = delete;
    PciDevice& operator= (const PciDevice&) = delete;

    // These each describe a memory mapped resource from a PCI device
    struct resource_t {uint8_t* baseAddr; size_t size; off_t physAddr;};

    // Opens a connection to a PCIe device
    void    open(int vendorID, int deviceID, std::string deviceDir = "");

    // Fetches the list of memory mappable resources
    std::vector<resource_t>& resourceList() {return resource_;}
    
    // Stop access to the PCI device
    void    close();

protected:

    // Fetches the list of memory-mappable resources
    std::vector<resource_t> getResourceList(std::string deviceDir);

    // Memory maps the resources whose definitions are in resource_
    void mapResources();

    // Contains one entry for each resource (i.e, BAR) that is configured in the PCI device
    std::vector<resource_t> resource_;
};
