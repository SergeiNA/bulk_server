/**
 * @file async_adaptor.h
 * @author SergeiNA (you@domain.com)
 * @brief Async adapter class for bulk_server
 * @version 1.0
 * @date 2020-01-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once

#include "command_handler.h"
#include "connectionManager.h"

#include <shared_mutex>
#include <unordered_map>


namespace asyncq
{
    class AsyncAdaptor{
        /**
         * @brief Connection data
         * Include CommandHandler and buffer for not ended data
         * Unique for each connection
         */
        struct Connection{
            CommandHandler cmdHandler;
            std::string buffer;
        };
    public:
        AsyncAdaptor() = default;
        /**
         * @brief Construct a new Async Adaptor object
         * with vector of ThreadManager and bulk size
         * 
         * @param subs vector of subscribers
         * @param sizeb bulk size
         */
        AsyncAdaptor(std::vector<std::shared_ptr<ThreadManager>> subs, std::size_t sizeb);
        /**
         * @brief Destroy the Async Adaptor object and dump all remainds in QueueCommand
         * 
         */
        ~AsyncAdaptor();
        
        ConnectionId openConection();

        void closeConnection(ConnectionId id);

        /**
         * @brief collect data from receiver by connection id
         * 
         * Accumulate chars and send to CommandHandler by id
         * 
         * Accumulate input chars, separate by last end line
         * first part send to CommandHandler, second store in 
         * connections buffer by id
         * 
         * @param data input data 
         * @param size size of input data
         */
        void collectData(ConnectionId id, const char *data, std::size_t size);
    private:
        std::shared_ptr<QueueCommand> qcmd; ///< shared CommandQueue
        std::unordered_map<ConnectionId, Connection> connections; ///< contains remaind chars after separating
        connectionManager idManager; ///< connection id factory
    };
    
} // namespace async
