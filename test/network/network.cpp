/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <cstdio>
#include <future>
#include <boost/format.hpp>
#include <boost/test/unit_test.hpp>
#include <bitcoin/bitcoin.hpp>

using namespace bc;
using namespace bc::network;

#define LOG_TEST "network_tests"

class log_setup_fixture
{
public:
    log_setup_fixture()
      : debug_log_(LOG_TEST ".debug.log", log_open_mode),
        error_log_(LOG_TEST ".error.log", log_open_mode)
    {
        static const auto header = "=========== " LOG_TEST " ==========";;
        initialize_logging(debug_log_, error_log_, std::cout, std::cerr);
        log::debug(LOG_TEST) << header;
        log::error(LOG_TEST) << header;
    }

    ~log_setup_fixture()
    {
        log::clear();
    }

private:
    std::ofstream debug_log_;
    std::ofstream error_log_;
};

BOOST_FIXTURE_TEST_SUITE(network_tests, log_setup_fixture)

BOOST_AUTO_TEST_CASE(p2p__start__no_connections__start_stop_okay)
{
    settings configuration = p2p::testnet;
    configuration.threads = 1;
    configuration.host_pool_capacity = 42;
    configuration.outbound_connections = 0;
    configuration.inbound_connection_limit = 0;
    configuration.seeds = { configuration.seeds[1] };
    p2p network(configuration);

    std::promise<code> started;
    auto started_future = started.get_future();
    const auto start_handler = [&started](const code& ec)
    {
        started.set_value(ec);
    };
    network.start(start_handler);
    started_future.wait();
    BOOST_REQUIRE_EQUAL(started_future.get(), code(error::success));

    std::promise<code> stopped;
    auto stop_future = stopped.get_future();
    const auto stop_handler = [&stopped](const code& ec)
    {
        stopped.set_value(ec);
    };
    network.stop(stop_handler);
    stop_future.wait();
    BOOST_REQUIRE_EQUAL(stop_future.get(), code(error::success));
}

BOOST_AUTO_TEST_CASE(p2p__start__no_connections_or_capacity__start_stop_okay)
{
    settings configuration = p2p::testnet;
    configuration.threads = 1;
    configuration.host_pool_capacity = 0;
    configuration.outbound_connections = 0;
    configuration.inbound_connection_limit = 0;
    p2p network(configuration);

    std::promise<code> started;
    const auto start_handler = [&started](const code& ec)
    {
        started.set_value(ec);
    };
    network.start(start_handler);
    const auto start_code = started.get_future().get();
    BOOST_REQUIRE_EQUAL(start_code, code(error::success));

    std::promise<code> stopped;
    const auto stop_handler = [&stopped](const code& ec)
    {
        stopped.set_value(ec);
    };
    network.stop(stop_handler);
    const auto stop_code = stopped.get_future().get();
    BOOST_REQUIRE_EQUAL(stop_code, code(error::success));
}

BOOST_AUTO_TEST_SUITE_END()
