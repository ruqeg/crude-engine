module;

export module crude.core.filesystem;

export import crude.core.std_containers_heap;
export import crude.core.std_containers_stack;

export namespace crude::core
{

class Filesystem 
{
public:
  enum Permission
  {
    PERMISSION_WRITE,
    PERMISSION_READ,
    PERMISSION_WRITE_READ
  };
  enum Result
  {
    RESULT_SUCCESS,
    RESULT_OUT_OF_RANGE,
    RESULT_FAILED_OPTN,
  };
public:
  static Filesystem& getInstance();

public:
  Result read(const char* filename, span<char>& buffer);
  Result read(const char* filename, vector<char>& buffer);
  //void readAsync(char const *fileName);

  bool fileAccess(const char* filename);

  static inline constexpr char separator();

private:
  Filesystem() = default;
};

}