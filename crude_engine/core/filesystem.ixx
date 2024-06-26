module;

export module crude.core.filesystem;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::core
{

enum Filesystem_Permission
{
  FILESYSTEM_PERMISSION_WRITE,
  FILESYSTEM_PERMISSION_READ,
  FILESYSTEM_PERMISSION_WRITE_READ
};
enum Filesystem_Result
{
  FILESYSTEM_RESULT_SUCCESS,
  FILESYSTEM_RESULT_OUT_OF_RANGE,
  FILESYSTEM_RESULT_FAILED_OPEN,
};

Filesystem_Result readFile(const char* filename, span<char>& buffer);
Filesystem_Result readFile(const char* filename, vector<char>& buffer);
bool checkFileAccess(const char* filename);
constexpr char getFileSeparator();

}