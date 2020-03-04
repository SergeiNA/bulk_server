/**
 * @file async.cpp
 * @author SergeiNA (you@domain.com)
 * @brief Definition of core async functions
 * @version 0.1
 * @date 2020-01-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "async_queue.h"

namespace asyncq {

    std::once_flag f1;
    std::unique_ptr<AsyncAdaptor> asyncAdaptor = nullptr;
    /**
     * @brief used for intilize fileLogThreadManager and terminalLogThreadManager once
     * 
     */
    void asyncInitilizer(std::size_t bsize){
        /// call once and subscribe loggers to corresponding threadManagers
        std::call_once(
            f1,
            [](std::unique_ptr<AsyncAdaptor>& asyncAdp, std::size_t bsize){
                    auto fileLogThreadManager = std::make_shared<ThreadManager>();
                    auto terminalLogThreadManager = std::make_shared<ThreadManager>();
                    fileLogThreadManager->subscribe(std::make_shared<log_observer>());
                    fileLogThreadManager->subscribe(std::make_shared<log_observer>());
                    terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>());
                    asyncAdp = std::make_unique<AsyncAdaptor>(
                        std::vector<std::shared_ptr<ThreadManager>>
                        {fileLogThreadManager,terminalLogThreadManager},
                        bsize);
                },
                asyncAdaptor,
                bsize
             );
    }

    ConnectionId connect() {
        return asyncAdaptor->openConection();
    }

    void receive(ConnectionId handle, const char *data, std::size_t size) {
        asyncAdaptor->collectData(handle, data,size);
    }

    void disconnect(ConnectionId handle) {
        asyncAdaptor->closeConnection(handle);
    }
}
