
 #include "Requister/requester.hpp"
 #include "File handler/fh.hpp"

int main() {
  Requester::requestFileList();
  managed_shared_memory segment(open_only, "file_list_memory");
  typedef allocator<char, managed_shared_memory::segment_manager> CharAllocator;
  typedef basic_string<char, std::char_traits<char>, CharAllocator>
      SharedString;
  typedef allocator<SharedString, managed_shared_memory::segment_manager>
      StringAllocator;
  typedef vector <SharedString, StringAllocator> SharedStringVector;

  SharedStringVector *shared_file_list = segment.find<SharedStringVector>("FileList").first;
  if (shared_file_list) {
    std::cout << "File List:\n";
    for (const auto &file : *shared_file_list) {
      std::cout << file.c_str() << std::endl;
    }
  }

  std::string filename;
  std::cout << "Enter the filename to read: ";
  std::cin >> filename;

  Requester::requestFileContent(filename);

  managed_shared_memory content_segment(open_only, "file_content_memory");
  SharedString *shared_file_content =
      content_segment.find<SharedString>("FileContent").first;
  if (shared_file_content) {
    std::cout << "File Content:\n";
    std::cout << shared_file_content->c_str() << std::endl;
  }

  return 0;
}