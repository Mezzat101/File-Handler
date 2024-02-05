#ifndef REQUESTER_HPP
#define REQUESTER_HPP

#include <string>

namespace Requester {
    void requestFileList();
    void requestFileContent(const std::string& filename);
}

#endif