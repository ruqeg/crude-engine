module;

export module crude_engine.assert;

import crude_engine.debug;

export namespace crude_engine
{

template<class T>
void assert(T&& assertion) noexcept requires(gDebug);
template<class T>
void staticAssert(T&& assertion);
template<class T>
void staticAssert(T&& assertion, const char* str);

}