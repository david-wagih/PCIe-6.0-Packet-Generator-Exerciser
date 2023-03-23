#include "../memory_mapper/memory_mapper.h"
#include "../../utils/tlp.h"
#include "../endpoint_app/memory_request_handler.h"


class MemoryWriteHandler : public MemoryRequestHandler {
public:
    void setMemoryMap(MemoryMap* memoryMap) override {
        memoryMap_ = memoryMap;
    }
    TLP handleTlp(TLP* packet) override {
        // implementation for handling Memory Write TLPs
        TLP response;
        return response;
    }
private:
    MemoryMap* memoryMap_;
};