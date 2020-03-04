/**
 * @file command_handler.h
 * @author SergeiNA (you@domain.com)
 * @brief Declaration of commands container and commands handler
 * QueueCommand is able to bulk commands if condition reached
 * CommandHandler handle on blocks {...}
 * @version 1.0
 * @date 2020-01-05
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#pragma once
#include "observer.h"
#include "threadManager.h"
#include <vector>
#include <utility>


/**
 * @brief Contains commands, if limit is reached notify subscribers
 * Subscribers have to be inhareted from Observer
 */
class QueueCommand{
public:
    QueueCommand():metrics_{nullptr}, block_size_{100} {}
    QueueCommand(size_t block_size, std::shared_ptr<Metrics> metrics = nullptr):
        metrics_(metrics),block_size_{block_size} {}

    QueueCommand(const QueueCommand&)   = default;
    QueueCommand(QueueCommand&&)        = default;
    ~QueueCommand()                     = default;

    /**
     * @brief Add pair of command and timestamp in unixtime to container
     * 
     * @param cmd added command
     */
    void addCommand(std::string cmd);
    /**
     * @brief Add block of cmds
     * 
     */
    void addBlock(std::vector<std::string>);
    /**
     * @brief Add subscriber as observer (threadManager)
     * 
     * @param obs 
     */

    void subscribe(std::shared_ptr<ThreadManager> obs);
    /**
     * @brief Notify all subscribers to handle package of commands
     * 
     */
    void notify();

    /**
     * @brief Dump all remaind commands in queueCmd_
     * Also collect metric data
     */
    void dumpRemains();

    inline bool empty() const{
        return pack.first.empty();
    }
    
    inline size_t GetNumOfSubs() const noexcept{
        return subs.size();
    }
private:
/**
 * @brief Return timestamp of each command in unixtime
 * 
 * @return rawTimestamp returned timestamp
 */
	rawTimestamp getUnixTime();

private:
    std::vector<std::shared_ptr<ThreadManager>> subs;   ///< contains subscribers
    std::shared_ptr<Metrics> metrics_;                  ///< metrics handler
    Packet pack;                                        ///< contsains  package of commands with timestamp
    size_t block_size_;                                 ///< max free block size (if not in block)
    std::mutex lock;
};

/**
 * @brief Handle on all input commands
 * 
 * Able to handle with blocks:
 * {
 * cmd1
 * cmd2
 * {
 * cmd3
 * }
 * }
 * 
 * Throw exception if no '{' matches for '}'
 */
class CommandHandler{
public:
    CommandHandler():queueCmd_(nullptr),
                     metrics_(nullptr),
                     nasted(false),
                     braces_count{0},
                     nstrings{0}
                     {}

	CommandHandler(std::shared_ptr<QueueCommand> qCmd, 
                   std::shared_ptr<Metrics> metrics = nullptr):
                   queueCmd_(std::move(qCmd)),
                   metrics_(metrics),
                   nasted(false),
                   braces_count{0},
                   nstrings{0}
                   {}

    CommandHandler(CommandHandler&&) = default;
    void operator= (CommandHandler&& lhs);
	~CommandHandler();
    /**
     * @brief handle on input command
     * 
     * @param cmd input command
     */
	void process(std::string&& cmd);
    /**
     * @brief Start to read commands form input
     * 
     * @param is input stream
     */
	void Run(std::istream& is);

    /**
     * @brief Check if current state Nested
     * 
     * @return true 
     * @return false 
     */
    bool isNested() {
    	return nasted;
    }
    void clearBuffs();
private:
	std::shared_ptr<QueueCommand> queueCmd_; ///< pointer to QueueCommand object
    std::vector<std::string> blockCmd;       ///< brace {...} block accumulator 
    std::shared_ptr<Metrics> metrics_;       ///< metrics handler
	bool nasted;                             ///< contain current state
    size_t braces_count;                     ///< contain current degree of nesting
    size_t nstrings;                         ///< accumulate the number of all handled strings (metric requirement)
};

