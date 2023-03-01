#include "non_header_base.h"

boost::dynamic_bitset<> AddressRouting32Bit::getBitRep() const {
    boost::dynamic_bitset<(nonBase->headerSizeInBytes * 8)-32> result;
    result |= (boost::dynamic_bitset<>((nonBase->headerSizeInBytes * 8) - 32, address));
    result |= (boost::dynamic_bitset<>((nonBase->headerSizeInBytes * 8) - 32, tag) << 32);
    result |= (boost::dynamic_bitset<>((nonBase->headerSizeInBytes * 8) - 32, requestID) << 48);
    return result;
}

boost::dynamic_bitset<> AddressRouting64Bit::getBitRep() const {
    boost::dynamic_bitset<(nonBase->headerSizeInBytes * 8) - 32> result;
    result |= (boost::dynamic_bitset<>((nonBase->headerSizeInBytes * 8) - 32, address));
    result |= (boost::dynamic_bitset<>((nonBase->headerSizeInBytes * 8) - 32, tag) << 64);
    result |= (boost::dynamic_bitset<>((nonBase->headerSizeInBytes * 8) - 32, requestID) << 80);
    return result;
}

string MessageNonHeaderBase::getBitRep() const {
    bitset<96> result;
    //Add the messageCode
    result |= (messageCode << 64);
    //Add the requestID
    result |= (requestID << 80);

    return result.to_string();
}

string ConfigNonHeaderBase::getBitRep() const {
    bitset<64> result;
    //Add the registerNumber
    result |= (registerNumber << 2);
    //Add the functionNumber
    result |= (functionNumber << 16);
    //Add the deviceNumber
    result |= (deviceNumber << 19);
    //Add the busNumber
    result |= (busNumber << 24);
    //Add the tag
    result |= (tag << 32);
    //Add the requestID
    result |= (requestID << 48);
    return result.to_string();
}

string CompletionNonHeaderBase::getBitRep() const {
    std::bitset<6> myBitset(lowerAddress);  // convert integer to bitset
    std::bitset<1> lastBit;  // bitset to hold last 1 bits

    lastBit[0] = myBitset[5];
    bitset<64> result;
    //Add the byteCount
    result |= byteCount;
    //Add the lowerAddress
    for (int i = 0; i < 5; i++) {
        result |= (myBitset[i] << (12 + i));

    }
    //Add the functionNumber
    result |= (functionNumber << 16);
    //Add the deviceNumber
    result |= (deviceNumber << 19);
    //Add the busNumber
    result |= (busNumber << 24);
    //Add the tag
    result |= (tag << 32);
    //Add the lowerAddress 6th bit
    result |= (lastBit[0] << 46);
    //Add the completerID
    result |= (completerID << 48);


    return result.to_string();
}