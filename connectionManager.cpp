#include "connectionManager.h"
/**
 * @brief Return a connection ID
 * If awaiting queue is not empty return last active id
 * overwise create a new one
 * 
 * @return ConnectionId 
 */
ConnectionId connectionManager::getConnection(){
    if(awaiting.empty())
        return createConnection();
    ConnectionId used = awaiting.top();
    awaiting.pop();
    return used;
}
/**
 * @brief Move connection id to not active 
 * 
 * @param connId closing id
 */
void connectionManager::closeConnection(ConnectionId connId){
    awaiting.push(connId);
}
/**
 * @brief Increment id counter and create a new connection id
 * 
 * @return ConnectionId 
 */
ConnectionId connectionManager::createConnection(){
    return counter++;
}