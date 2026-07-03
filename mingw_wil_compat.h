#pragma once

#include <windows.h>
#include <memory>
#include <oleauto.h>
#include <string>

namespace wil_compat {

template<typename T, auto Deleter, T InvalidValue = nullptr>
struct Win32Deleter
{
    using pointer = T;
    void operator()(T handle) const
    {
        if (handle != InvalidValue)
        {
            Deleter(handle);
        }
    }
};

using unique_environstrings_ptr = std::unique_ptr<wchar_t, Win32Deleter<LPWCH, FreeEnvironmentStringsW>>;
using unique_event_log = std::unique_ptr<HANDLE, Win32Deleter<HANDLE, CloseEventLog>>;
using unique_hglobal = std::unique_ptr<HGLOBAL, Win32Deleter<HGLOBAL, GlobalFree>>;
using unique_hglobal_locked = std::unique_ptr<void, Win32Deleter<void*, GlobalUnlock>>;
using unique_hkey = std::unique_ptr<HKEY, Win32Deleter<HKEY, RegCloseKey>>;
using unique_hmenu = std::unique_ptr<HMENU, Win32Deleter<HMENU, DestroyMenu>>;
using unique_hmodule = std::unique_ptr<HMODULE, Win32Deleter<HMODULE, FreeLibrary>>;

using unique_hbrush = std::unique_ptr<HBRUSH, Win32Deleter<HBRUSH, DeleteObject>>;
using unique_hfont = std::unique_ptr<HFONT, Win32Deleter<HFONT, DeleteObject>>;
using unique_hicon = std::unique_ptr<HICON, Win32Deleter<HICON, DestroyIcon>>;
using unique_hpen = std::unique_ptr<HPEN, Win32Deleter<HPEN, DeleteObject>>;

struct variant_deleter
{
    void operator()(VARIANT* v) const
    {
        if (v)
        {
            VariantClear(v);
            delete v;
        }
    }
};
using unique_variant = std::unique_ptr<VARIANT, variant_deleter>;

struct stack_variant: public VARIANT
{
    stack_variant()
    {
        VariantInit(this);
    }
    ~stack_variant()
    {
        VariantClear(this);
    }
    stack_variant(const stack_variant&) = delete;
    stack_variant(stack_variant&&) = delete;
    stack_variant& operator=(const stack_variant&) = delete;
    stack_variant& operator=(stack_variant&&) = delete;
};

class BeginPaint_context
{
    HWND m_hwnd{nullptr};
    PAINTSTRUCT m_ps{};
    HDC m_hdc{nullptr};
public:
    BeginPaint_context() = delete;
    BeginPaint_context(HWND hwnd):
        m_hwnd(hwnd)
    {
        m_hdc = ::BeginPaint(m_hwnd, &m_ps);
    }
    BeginPaint_context(const BeginPaint_context&) = delete;
    BeginPaint_context(BeginPaint_context&&) = delete;
    BeginPaint_context& operator=(const BeginPaint_context&) = delete;
    BeginPaint_context& operator=(BeginPaint_context&&) = delete;
    ~BeginPaint_context()
    {
        ::EndPaint(m_hwnd, &m_ps);
    }
    HDC get() const
    {
        return m_hdc;
    }
    const PAINTSTRUCT& ps() const
    {
        return m_ps;
    }
    operator HDC() const
    {
        return m_hdc;
    }
};

class GetDC_context
{
    HWND m_hwnd{nullptr};
    HDC m_hdc{nullptr};
public:
    GetDC_context() = delete;
    GetDC_context(HWND hwnd):
        m_hwnd(hwnd)
    {
        m_hdc = ::GetDC(m_hwnd);
    }
    GetDC_context(const GetDC_context&) = delete;
    GetDC_context(GetDC_context&&) = delete;
    GetDC_context& operator=(const GetDC_context&) = delete;
    GetDC_context& operator=(GetDC_context&&) = delete;
    ~GetDC_context()
    {
        if (m_hdc)
        {
            ::ReleaseDC(m_hwnd, m_hdc);
        }
    }
    HDC get() const
    {
        return m_hdc;
    }
    operator HDC() const
    {
        return m_hdc;
    }
};

class SelectObject_context
{
    HDC m_hdc{nullptr};
    HGDIOBJ m_old{nullptr};
public:
    SelectObject_context() = delete;
    SelectObject_context(HDC hdc, HGDIOBJ obj):
        m_hdc(hdc)
    {
        if (!m_hdc || !obj)
        {
            return;
        }
        m_old = ::SelectObject(m_hdc, obj);
        if (m_old == nullptr || m_old == reinterpret_cast<HGDIOBJ>(-1))
        {
            m_old = nullptr;
        }
    }
    SelectObject_context(const SelectObject_context&) = delete;
    SelectObject_context(SelectObject_context&&) = delete;
    SelectObject_context& operator=(const SelectObject_context&) = delete;
    SelectObject_context& operator=(SelectObject_context&&) = delete;
    ~SelectObject_context()
    {
        if (m_hdc && m_old)
        {
            ::SelectObject(m_hdc, m_old);
        }
    }
    explicit operator bool() const
    {
        return m_old != nullptr;
    }
    HGDIOBJ get() const
    {
        return m_old;
    }
};

inline SelectObject_context SelectObject(HDC hdc, HGDIOBJ obj)
{
    return SelectObject_context{hdc, obj};
}

inline std::wstring GetCurrentDirectoryW()
{
    DWORD bufferSize{::GetCurrentDirectoryW(0, nullptr)};
    if (bufferSize == 0)
    {
        return {};
    }
    std::wstring buffer(bufferSize, L'\0');
    DWORD result{::GetCurrentDirectoryW(static_cast<DWORD>(buffer.size()), buffer.data())};
    if (result > 0 && result < buffer.size())
    {
        buffer.resize(result);
        return buffer;
    }
    return {};
}

inline std::wstring GetSystemDirectoryW()
{
    UINT bufferSize{::GetSystemDirectoryW(nullptr, 0)};
    if (bufferSize == 0)
    {
        return {};
    }
    std::wstring buffer(bufferSize, L'\0');
    UINT result{::GetSystemDirectoryW(buffer.data(), static_cast<UINT>(buffer.size()))};
    if (result > 0 && result < buffer.size())
    {
        buffer.resize(result);
        return buffer;
    }
    return {};
}

inline HRESULT GetEnvironmentVariableW(LPCWSTR name, std::wstring& value)
{
    value.clear();
    DWORD bufferSize{::GetEnvironmentVariableW(name, nullptr, 0)};
    if (bufferSize == 0)
    {
        return HRESULT_FROM_WIN32(::GetLastError());
    }
    std::wstring buffer(bufferSize, L'\0');
    for (;;)
    {
        const DWORD result{::GetEnvironmentVariableW(name, buffer.data(), static_cast<DWORD>(buffer.size()))};
        if (result == 0)
        {
            return HRESULT_FROM_WIN32(::GetLastError());
        }
        if (result < buffer.size())
        {
            buffer.resize(result);
            value = buffer;
            return S_OK;
        }
        buffer.resize(result + 1U);
    }
}

inline HRESULT GetModuleFileNameW(HMODULE module, std::wstring& path)
{
    path.clear();
    constexpr DWORD maxBufferSize{32768U};
    DWORD bufferSize{MAX_PATH};
    std::wstring buffer{};
    for (;;)
    {
        buffer.assign(bufferSize, L'\0');
        ::SetLastError(ERROR_SUCCESS);
        const DWORD result{::GetModuleFileNameW(module, buffer.data(), bufferSize)};
        if (result == 0)
        {
            return HRESULT_FROM_WIN32(::GetLastError());
        }
        if (result < bufferSize)
        {
            buffer.resize(result);
            path = buffer;
            return S_OK;
        }
        if (bufferSize >= maxBufferSize)
        {
            return HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER);
        }
        const DWORD nextSize{bufferSize * 2U};
        bufferSize = (nextSize > maxBufferSize) ? maxBufferSize : nextSize;
    }
}

inline std::wstring SearchPathW(LPCWSTR lpPath, LPCWSTR lpFileName, LPCWSTR lpExtension)
{
    DWORD bufferSize{::SearchPathW(lpPath, lpFileName, lpExtension, 0, nullptr, nullptr)};
    if (bufferSize == 0)
    {
        return {};
    }
    std::wstring buffer(bufferSize, L'\0');
    DWORD result{::SearchPathW(lpPath, lpFileName, lpExtension, static_cast<DWORD>(buffer.size()), buffer.data(), nullptr)};
    if (result > 0 && result < buffer.size())
    {
        buffer.resize(result);
        return buffer;
    }
    return {};
}

extern "C" IMAGE_DOS_HEADER __ImageBase;
inline HINSTANCE GetModuleInstanceHandle() noexcept
{
    return reinterpret_cast<HINSTANCE>(&__ImageBase);
}

} // namespace wil_compat
