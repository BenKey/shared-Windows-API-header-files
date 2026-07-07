/*
Copyright © 2019 - 2026 by Benilda Key

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

#include <windows.h>
#include <string>
#include <type_traits>

#include <boost/format.hpp>

namespace Utility {

// Source: https://stackoverflow.com/questions/27610994/how-should-i-use-getdc-as-a-local-object-with-autocleanup-c
class DCObject
{
private:
    HWND my_handle{nullptr};
    HDC my_hdc{nullptr};
public:
    DCObject() = default;
    explicit DCObject(HWND handle): my_handle(handle), my_hdc(::GetDC(handle))
    {
    }
    DCObject(DCObject const&) = delete;
    DCObject(DCObject&& other) noexcept
    {
        my_handle = std::exchange(other.my_handle, nullptr);
        my_hdc = std::exchange(other.my_hdc, nullptr);
    }
    ~DCObject()
    {
        if (my_hdc != nullptr)
        {
            ::ReleaseDC(my_handle, my_hdc);
        }
    }
    DCObject& operator=(const DCObject&) = delete;
    DCObject& operator=(DCObject&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }
        if (my_hdc != nullptr)
        {
            ::ReleaseDC(my_handle, my_hdc);
        }
        my_handle = std::exchange(other.my_handle, nullptr);
        my_hdc = std::exchange(other.my_hdc, nullptr);
        return *this;
    }
    [[nodiscard]]
    HDC handle() const
    {
        return my_hdc;
    }
    [[nodiscard]]
    HDC get() const
    {
        return my_hdc;
    }
    bool operator!()
    {
        return (my_hdc == nullptr);
    }
};

std::wstring DescribeError(IN DWORD error = ::GetLastError());
void SetResouceInstance(HINSTANCE hInstance);
HINSTANCE GetResouceInstance();
std::wstring loadString(HINSTANCE hInstance, UINT uID);
std::wstring loadString(UINT uID);
HANDLE loadImage(LPCWSTR name, UINT type, int cx, int cy, UINT fuLoad);
RECT GetMonitorRectForWindow(HWND hwndWindow);
bool CenterWindow(HWND hwndWindow);
std::wstring getWindowText(HWND window);
std::wstring getClassName(HWND window);
std::wstring GetDialogBoxText(HWND dialog);
bool CopyToClipboard(HWND hwndOwner, const std::wstring& text);
bool CopyToDialogBoxTextClipboard(HWND dialog);
int messageBox(HWND hWnd, UINT textID, UINT captionID, UINT uType);
bool getTextExtentPoint32(HWND window, const std::wstring& str, SIZE& size);
HWND CreateDialogWithAccelerators(_In_opt_ HINSTANCE hInstance, _In_ LPCWSTR dialogTemplateName, _In_opt_ HWND parent,
    _In_opt_ DLGPROC dialogFunc, _In_ LPARAM initParam, _In_ LPCWSTR acceleratorTableName, _Out_ HACCEL& acceleratorTable);
ULONGLONG GetDllVersion(LPCWSTR dllName);
HWND GetTaskbarWindow();
std::wstring ReplaceXMLCharacterEntities(const std::wstring& str);
std::wstring ReplaceHTMLCharacterEntities(const std::wstring& str);
// Source: https://stackoverflow.com/questions/8357240/how-to-automatically-convert-strongly-typed-enum-into-int
template <typename E>
inline constexpr auto to_underlying(E e) noexcept
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}

template<typename... Arguments>
inline std::wstring FormatArgs(const std::wstring& fmt, const Arguments&... args)
{
    return boost::str((boost::wformat(fmt) % ... % args));
}

template<typename... Arguments>
inline std::wstring FormatArgs(UINT fmt, const Arguments&... args)
{
    using namespace std::string_literals;
    const auto formatStr{loadString(fmt)};
    if (formatStr.empty())
    {
        return L""s;
    }
    return boost::str((boost::wformat(formatStr) % ... % args));
}

} // namespace Utility
