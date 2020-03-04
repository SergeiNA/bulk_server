#define BOOST_TEST_MODULE async_single_thread

#include "async_adaptor.h"
#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(bulk_suite)

BOOST_AUTO_TEST_CASE(test_regular)
{
    std::string ans =
        R"(bulk: 1
bulk: 2
bulk: 6
)";

    std::ostringstream oss;
    {
        auto terminalLogThreadManager = std::make_shared<ThreadManager>();

        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss));

        asyncq::AsyncAdaptor asyncAdaptor({terminalLogThreadManager}, 1);
        ConnectionId id = asyncAdaptor.openConection();

        asyncAdaptor.collectData(id, "1", 1); 
        asyncAdaptor.collectData(id, "\n2\n", 3);
        asyncAdaptor.collectData(id, "3", 1);
        asyncAdaptor.collectData(id, "\n", 1);
        asyncAdaptor.closeConnection(id);
    }

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_regular_2)
{
    std::string ans = R"(bulk: 1, 2
bulk: 6
)";

    std::ostringstream oss;

    {
        auto terminalLogThreadManager = std::make_shared<ThreadManager>();

        std::shared_ptr<ThreadManager> terminal;
        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss));
        asyncq::AsyncAdaptor asyncAdaptor({terminalLogThreadManager}, 2);
        ConnectionId id = asyncAdaptor.openConection();

        asyncAdaptor.collectData(id, "1", 1);
        asyncAdaptor.collectData(id, "\n2\n", 3);
        asyncAdaptor.collectData(id, "3", 1);
        asyncAdaptor.collectData(id, "\n", 1);

        asyncAdaptor.closeConnection(id);

    }

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_nested_1)
{
    std::string ans = R"(bulk: 1
bulk: 2
bulk: 6
)";

    // 1
    // {
    // 2
    // }
    // 3

    std::ostringstream oss;

    {
        auto terminalLogThreadManager = std::make_shared<ThreadManager>();

        std::shared_ptr<ThreadManager> terminal;
        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss));
        asyncq::AsyncAdaptor asyncAdaptor({terminalLogThreadManager}, 5);
        ConnectionId id = asyncAdaptor.openConection();

        asyncAdaptor.collectData(id, "1", 1);
        asyncAdaptor.collectData(id, "\n{\n", 3);
        asyncAdaptor.collectData(id, "2\n}", 3);
        asyncAdaptor.collectData(id, "\n3", 2);
        asyncAdaptor.collectData(id, "\n", 1);

        asyncAdaptor.closeConnection(id);
    }
    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_CASE(test_nested_2)
{
    std::string ans = R"(bulk: 1, 2
bulk: 6
)";

// 1
// 2
// {
// 3
// }

    std::ostringstream oss;

    {
        auto terminalLogThreadManager = std::make_shared<ThreadManager>();

        std::shared_ptr<ThreadManager> terminal;
        terminalLogThreadManager->subscribe(std::make_shared<terminal_observer>(oss));
        asyncq::AsyncAdaptor asyncAdaptor({terminalLogThreadManager}, 5);
        ConnectionId id = asyncAdaptor.openConection();

        asyncAdaptor.collectData(id, "1", 1);
        asyncAdaptor.collectData(id, "\n2\n{", 4);
        asyncAdaptor.collectData(id, "\n3", 2);
        asyncAdaptor.collectData(id, "\n", 1);
        asyncAdaptor.collectData(id, "}\n", 2);

        asyncAdaptor.closeConnection(id);

    }

    BOOST_CHECK_EQUAL(ans, oss.str());
}

BOOST_AUTO_TEST_SUITE_END()