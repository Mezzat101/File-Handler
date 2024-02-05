#include "requester.hpp"

#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

using namespace boost::interprocess;
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;

namespace Requester {
    void requestFileList()
    {
        logging::add_file_log("run_logs.txt");

        src::logger logger;
        logger.add_attribute("LineID", logging::attributes::counter<unsigned int>(1));
        logger.add_attribute("TimeStamp", logging::attributes::local_clock());

        message_queue mq(open_or_create, "file_handler_queue", 100, sizeof(std::string));

        std::string request = "list_files";
        mq.send(request.c_str(), request.size(), 0);

        BOOST_LOG(logger) << "Request sent: list_files";
    }

    void requestFileContent(const std::string& filename)
    {
        logging::add_file_log("run_logs.txt");

        src::logger logger;
        logger.add_attribute("LineID", logging::attributes::counter<unsigned int>(1));
        logger.add_attribute("TimeStamp", logging::attributes::local_clock());

        message_queue mq(open_or_create, "file_handler_queue", 100, sizeof(std::string));

        std::string request = "read_file:" + filename;
        mq.send(request.c_str(), request.size(), 0);

        BOOST_LOG(logger) << "Request sent: " << request;
    }
}