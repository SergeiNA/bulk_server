/**
 * @file async_adaptor.cpp
 * @author SergeiNA (you@domain.com)
 * @brief AsyncAdaptor definition 
 * @version 1.0
 * @date 2020-01-15
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "async_adaptor.h"

#include <memory>
#include <iostream>
#include <sstream>

namespace asyncq{
    /**
     * @brief Construct a new Async Adaptor:: Async Adaptor object
     * 
     * Add subs to shared CommandQueue
     * 
     * @param subs subs for shared CommandQueue
     * @param bsize bulk size
     */
    AsyncAdaptor::AsyncAdaptor(std::vector<std::shared_ptr<ThreadManager>> subs,
                               std::size_t bsize):qcmd(std::make_shared<QueueCommand>(bsize)){
        for(auto&& s:subs)
           qcmd->subscribe(s);
    }
    /**
     * @brief Open a new connetion
     * If connection already exist return its id
     * overwise create a new one
     * 
     * @return ConnectionId a new connection id
     */
    ConnectionId AsyncAdaptor::openConection(){
        ConnectionId id = idManager.getConnection();
        auto connection = connections.find(id);
        if(connection!=end(connections))
            return connection->first;
        connections.emplace(id,Connection{CommandHandler(qcmd),""});
        return id;
    }
    /**
     * @brief close connection by id
     * Clear all buffers in Connection object
     * 
     * @param id closing id
     */
    void AsyncAdaptor::closeConnection(ConnectionId id){
        connections[id].buffer.clear();
        connections[id].cmdHandler.clearBuffs();
        idManager.closeConnection(id);
    }

    /**
     * @brief Collect and cut input data by id
     * If endl found send date to CommandHandler by id
     * 
     * @param id connection id
     * @param data input data 
     * @param size input date size
     */

    void AsyncAdaptor::collectData(ConnectionId id,  const char *data, std::size_t size){
        // preparation for copy and copy
        std::string buffer = connections[id].buffer;
        buffer.resize(buffer.size() + size);
        std::copy(data, data+size, std::next(buffer.begin(), connections[id].buffer.size()));

        // check for a '\n'
        std::size_t lastEndlPos =buffer.find_last_of('\n');

        if (lastEndlPos == std::string::npos){
            connections[id].buffer = buffer;
            return;
        }

        // split string to active part and reminder
        std::string buffer_to_parse = buffer.substr(0, lastEndlPos+1);
        connections[id].buffer = buffer.substr(lastEndlPos+1,std::string::npos);

        // start processing
        std::istringstream iss{buffer_to_parse.data()};

        connections[id].cmdHandler.Run(iss);
    }
    
    AsyncAdaptor::~AsyncAdaptor()
    {
        qcmd->dumpRemains();
    }
}
