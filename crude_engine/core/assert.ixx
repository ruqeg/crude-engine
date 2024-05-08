module;

export module crude_engine.core.assert;

import crude_engine.core.debug;

export namespace crude_engine
{

template<class T>
void assert(T&& assertion) noexcept requires(gDebug);
template<class T>
void staticAssert(T&& assertion);

}