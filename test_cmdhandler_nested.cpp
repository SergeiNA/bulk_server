#define BOOST_TEST_MODULE nested_commnds

#include "command_handler.h"
#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(bulk_suite)

BOOST_AUTO_TEST_CASE(test_terminal_1)
{
    std::string ans = R"(bulk: 1
bulk: 2
bulk: 6
)";

    std::istringstream iss{R"(1
{
2
}
3)"};

    std::ostringstream oss;
    {
        auto q_command = std::make_shared<QueueCommand>(5);

        auto terminalLogThreadManager = std::make_shared<ThreadManager>();
        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss), nullptr);

        q_command->subscribe(terminalLogThreadManager);
        CommandHandler cmdHandler(q_command);
        cmdHandler.Run(iss);
        q_command->dumpRemains();
        terminalLogThreadManager->stop();
    }
    
    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_terminal_2)
{
    std::string ans = R"(bulk: 1, 2
bulk: 6
)";

   std::istringstream iss{R"(1
2
{
3
})"};

    std::ostringstream oss;
    {
        auto q_command = std::make_shared<QueueCommand>(5);

        auto terminalLogThreadManager = std::make_shared<ThreadManager>();
        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss), nullptr);

        q_command->subscribe(terminalLogThreadManager);
        CommandHandler cmdHandler(q_command);
        cmdHandler.Run(iss);
        q_command->dumpRemains();
        terminalLogThreadManager->stop();
    }

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_terminal_3)
{
    std::string ans = R"(bulk: 1, 2, 6
)";

   std::istringstream iss{R"(1
2
3
{)"};

    std::ostringstream oss;
    {
        auto q_command = std::make_shared<QueueCommand>(5);

        auto terminalLogThreadManager = std::make_shared<ThreadManager>();
        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss), nullptr);

        q_command->subscribe(terminalLogThreadManager);
        CommandHandler cmdHandler(q_command);
        cmdHandler.Run(iss);
        q_command->dumpRemains();
        terminalLogThreadManager->stop();
    }
    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_terminal_4)
{
    std::string ans = R"(bulk: 1, 2, 6
)";

   std::istringstream iss{R"(1
2
3
{
4)"};

    std::ostringstream oss;

    {
        auto q_command = std::make_shared<QueueCommand>(5);

        auto terminalLogThreadManager = std::make_shared<ThreadManager>();
        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss), nullptr);

        q_command->subscribe(terminalLogThreadManager);
        CommandHandler cmdHandler(q_command);
        cmdHandler.Run(iss);
        q_command->dumpRemains();
        terminalLogThreadManager->stop();
    }

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_terminal_5)
{
    std::string ans = R"(bulk: 1
bulk: 2, 6, 24
)";

   std::istringstream iss{R"(1
{
2
{
3
}
4
})"};

    std::ostringstream oss;
    {
        auto q_command = std::make_shared<QueueCommand>(5);

        auto terminalLogThreadManager = std::make_shared<ThreadManager>();
        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss), nullptr);

        q_command->subscribe(terminalLogThreadManager);
        CommandHandler cmdHandler(q_command);

        cmdHandler.Run(iss);
        q_command->dumpRemains();
        terminalLogThreadManager->stop();
    }

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_SUITE_END()