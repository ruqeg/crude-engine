module;

export module crude_engine.core.byte_utils;
import crude_engine.core.alias;

export namespace crude_engine
{

template<class T, size_t TSize>
class Byte_Swapper;

class Byte_Utils
{
public:
  static uint16 byteSwap2(uint16 inData)
  {
    return (inData >> 8) | (inData << 8);
  }

  static uint32 byteSwap4(uint32 inData)
  {
    return ((inData >> 24) & 0x000000FF) |
      ((inData >> 8) & 0x0000FF00) |
      ((inData << 8) & 0x00FF0000) |
      ((inData << 24) & 0xFF000000);
  }

  static uint64 byteSwap8(uint64 inData)
  {
    return ((inData >> 56) & 0x00000000000000FF) |
      ((inData >> 40) & 0x000000000000FF00) |
      ((inData >> 24) & 0x0000000000FF0000) |
      ((inData >> 8) & 0x00000000FF000000) |
      ((inData << 8) & 0x000000FF00000000) |
      ((inData << 24) & 0x0000FF0000000000) |
      ((inData << 40) & 0x00FF000000000000) |
      ((inData << 56) & 0xFF00000000000000);
  }

  template<class T>
  static T byteSwap(T inData)
  {
    return Byte_Swapper<T, sizeof(T)>().swap(inData);
  }
};

template <class TFrom, class TTo>
class Type_Aliaser
{
public:
  Type_Aliaser(TFrom inFromValue)
    : m_asFromType(inFromValue) {};
  TTo& get() {return m_asToType;}
private:
  union
  {
    TFrom m_asFromType;
    TTo   m_asToType;
  };
};

template<class T>
class Byte_Swapper<T, 2>
{
public:
  T swap(T inData) const
  {
    uint16 result = Byte_Utils::byteSwap2(Type_Aliaser<T, uint16>(inData).get());
    return Type_Aliaser<uint16, T>(result).get();
  }
};

template<class T>
class Byte_Swapper<T, 4>
{
public:
  T swap(T inData) const
  {
    uint32 result = Byte_Utils::byteSwap2(Type_Aliaser<T, uint32>(inData).get());
    return Type_Aliaser<uint32, T>(result).get();
  }
};

template<class T>
class Byte_Swapper<T, 8>
{
public:
  T swap(T inData) const
  {
    uint64 result = Byte_Utils::byteSwap2(Type_Aliaser<T, uint64>(inData).get());
    return Type_Aliaser<uint64, T>(result).get();
  }
};

}