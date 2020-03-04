#pragma once
/*
    - connection manager -> what to do here???
    - general command handler
    - container with ptr to command_handlers
    - check input sstream from each context 
        for blocks
    - create new command_handler obj and link to its context
*/
#include <cstddef>
#include "async_adaptor.h"

namespace asyncq {

    void asyncInitilizer(std::size_t bsize);
    ConnectionId connect();
    void receive(ConnectionId handle, const char *data, std::size_t size);
    void disconnect(ConnectionId handle);

}
