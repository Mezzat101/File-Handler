#include "fh.hpp"

#include <iostream>
#include <fstream>
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

namespace FileHandler {
    void handleFileRequests()
    {
        logging::add_file_log("run_logs.txt");

        src::logger logger;
        logger.add_attribute("LineID", logging::attributes::counter<unsigned int>(1));
        logger.add_attribute("TimeStamp", logging::attributes::local_clock());

        message_queue mq(open_or_create, "file_handler_queue", 100, sizeof(std::string));

        while (true)
        {
            std::string request;
            message_queue::size_type recv_size;
            unsigned int priority;
            mq.receive(&request, sizeof(std::string), recv_size, priority);

            if (request == "list_files")
            {
                BOOST_LOG(logger) << "Received request: list_files";

                std::vector<std::string> file_list = getListOfFiles("path_to_directory");

                managed_shared_memory segment(open_or_create, "file_list_memory", 65536);
                typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
                typedef basic_string<char, std::char_traits<char>, CharAllocator> SharedString;
                typedef allocator<SharedString, managed_shared_memory::segment_manager> StringAllocator;
                typedef vector<SharedString, StringAllocator> SharedStringVector;

                segment.destroy<SharedStringVector>("FileList");
                segment.construct<SharedStringVector>("FileList")(segment.get_segment_manager());

                SharedStringVector* shared_file_list = segment.find_or_construct<SharedStringVector>("FileList")(segment.get_segment_manager());
                for (const auto& file : file_list)
                {
                    shared_file_list->emplace_back(file.c_str(), segment.get_segment_manager());
                }

                BOOST_LOG(logger) << "List of files sent to the requester";
            }
            else if (request.substr(0, 10) == "read_file:")
            {
                BOOST_LOG(logger) << "Received request: " << request;

                std::string filename = request.substr(10);

                std::string file_content = readFileContent(filename);

                managed_shared_memory segment(open_or_create, "file_content_memory", 65536);
                typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
                typedef basic_string<char, std::char_traits<char>, CharAllocator> SharedString;

                segment.destroy<SharedString>("FileContent");
                segment.construct<SharedString>("FileContent")(file_content.c_str(), segment.get_segment_manager());

                BOOST_LOG(logger) << "File content sent to the requester";
            }
        }
    }

    std::vector<std::string> getListOfFiles(const std::string& directory)
    {
        std::vector<std::string> file_list;
        // Add code to retrieve the list of files in the directory
        // ...

        return file_list;
    }

    std::string readFileContent(const std::string& filename)
    {
        std::string file_content;
        // Add code to read the content of the file
        // ...

        return file_content;
    }
}