/**
 * @file command_handler.cpp
 * @author SergeiNA (you@domain.com)
 * @brief Method definitoin of QueueCommand and CommandHandler
 * @version 1.0
 * @date 2020-01-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "command_handler.h"

/**
 * @brief Add a new command to a queue
 * If queue is empty add timestamp
 * If queue size equal to bulk size -> notify all subs
 * 
 * @param cmd input commnad
 */
void QueueCommand::addCommand(std::string cmd)
{
    std::lock_guard<std::mutex> guard(lock);
    if(pack.first.empty())
        pack.second = getUnixTime();
    pack.first.emplace_back(std::move(cmd));
    if (pack.first.size() == block_size_)
    {
        notify();
    }
}
 /**
  * @brief Add block of commands {....}
  * Create timestamp and notify all subs
  * 
  * @param comm_block input block of commands
  */
void QueueCommand::addBlock(std::vector<std::string> comm_block){
    std::lock_guard<std::mutex> guard(lock);
    if(!pack.first.empty())
         notify();
    pack.second = getUnixTime();
    pack.first = std::move(comm_block);
    notify();
}

/**
 * @brief Add a new sub
 * 
 * @param tmanager 
 */
void QueueCommand::subscribe(std::shared_ptr<ThreadManager> tmanager)
{
    subs.emplace_back(tmanager);
}
/**
 * @brief Sent cmd message to all subscribers
 * 
 * Send all commands as a packege with timestamp to all subs
 * If metrics_ exist collect package information
 * Clear pack of commands (delete commands)
 * 
 * Note: If vector of commands is empty 
 * "queuery is empty" - will be send as command
 * and "empty" as a timestamp
 * 
 */
void QueueCommand::notify()
{
    if(empty())
        return;
    for (size_t i = 0; i < subs.size(); i++)
        subs[i]->push(pack);
    
    if(metrics_)
       metrics_->collect(pack.first);

    pack.first.clear();
    pack.first.resize(0);
}

void QueueCommand::dumpRemains(){
    notify();
}

rawTimestamp QueueCommand::getUnixTime()
{
    using namespace std::chrono;
    return system_clock::now().time_since_epoch();
}

/**
 * @brief handle all type of input commands
 * 
 * If process get block type command ('{' or '}')
 * depends on current state It will operate differently
 * 
 * - Current state is 'regular':
 *      get usual command -> call addCommand for current command
 *      get '{' block start indicator -> switch state to 
 *          'nested' increment braces_count
 *      get '}' block end indicator -> throw exception
 * 
 * - Current state is 'nested':
 *      get usual command -> call accumulate cmds inside blockCmd
 *      get '{' block start indicator -> increment braces_count
 *      get '}' block end indicator -> decrement braces_count
 *          if braces_count == 0 -> switch state to regular and call addBlock 
 *          for QueueCommand
 * 
 * Also accumulate number of handled lines
 * 
 * @param cmd input command
 */

void CommandHandler::process(std::string &&cmd)
{
    ++nstrings;
    if (cmd == "{")
    {
        if (!isNested())
        {
            nasted = true;
        }
        ++braces_count;
        return;
    }
    if (cmd == "}")
    {
        if (!braces_count)
            throw std::invalid_argument("} can not be first bracket");
        --braces_count;
        if (!braces_count)
        {
            nasted = false;
            queueCmd_->addBlock(std::move(blockCmd));
        }
        return;
    }

    if(isNested())
        blockCmd.emplace_back(cmd);
    else
        queueCmd_->addCommand(cmd);
}

/**
 * @brief Read all commands from stdin and call process on them
 * 
 * Read form is input stream while is not empty
 * and if metrics_ exist collect metrics
 * 
 * @param is input stdin stream
 */
void CommandHandler::Run(std::istream &is = std::cin)
{
    while (is && !is.eof())
    {
        std::string temp;
        std::getline(is, temp);
        if(temp.empty()&&is.eof())
            break;
        process(std::move(temp));
    }
}

CommandHandler::~CommandHandler(){
}

void CommandHandler::operator=(CommandHandler&& lhs){
    queueCmd_ = std::move(lhs.queueCmd_);
    metrics_ = std::move(lhs.metrics_);
    nasted = lhs.nasted;
    braces_count = lhs.braces_count;
    nstrings = lhs.nstrings;
}

void CommandHandler::clearBuffs(){
    blockCmd.clear();       
	nasted = false;                             
    braces_count = 0;                     
    nstrings = 0;
}

