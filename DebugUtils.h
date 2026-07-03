/*
Copyright © 2023 - 2026 by Benilda Key

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#pragma once

#include <string>

#include <debugapi.h>

#include <boost/format.hpp>

namespace debug {

template<typename... Arguments>
inline std::wstring FormatArgs(const std::wstring& fmt, const Arguments&... args)
{
    return boost::str((boost::wformat(fmt) % ... % args));
}

template<typename... Arguments>
inline std::string FormatArgs(const std::string& fmt, const Arguments&... args)
{
    return boost::str((boost::format(fmt) % ... % args));
}

inline void out(LPCWSTR outputString)
{
    OutputDebugStringW(outputString);
}

inline void out(const std::wstring& outputString)
{
    OutputDebugStringW(outputString.c_str());
}

template<typename... Arguments>
inline void out(const std::wstring& format, const Arguments&... args)
{
    const auto outputString{FormatArgs(format, args...)};
    OutputDebugStringW(outputString.c_str());
}

inline void out(LPCSTR outputString)
{
    OutputDebugStringA(outputString);
}

inline void out(const std::string& outputString)
{
    OutputDebugStringA(outputString.c_str());
}

template<typename... Arguments>
inline void out(const std::string& format, const Arguments&... args)
{
    const auto outputString{FormatArgs(format, args...)};
    OutputDebugStringA(outputString.c_str());
}

} // namespace debug
