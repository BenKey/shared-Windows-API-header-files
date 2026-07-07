/*
Copyright 2019 - 2026 by Benilda Key

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

#include <oleacc.h>
#include <uiautomation.h>
#include <comdef.h>
#include <comip.h>
#include <comutil.h>

#include "Utility.h"

#if (!defined(__UIA_HeadingLevelIds_MODULE_DEFINED__) && defined(__MINGW64__))

constexpr long HeadingLevel_None{80050};
constexpr long HeadingLevel1{80051};
constexpr long HeadingLevel2{80052};
constexpr long HeadingLevel3{80053};
constexpr long HeadingLevel4{80054};
constexpr long HeadingLevel5{80055};
constexpr long HeadingLevel6{80056};
constexpr long HeadingLevel7{80057};
constexpr long HeadingLevel8{80058};
constexpr long HeadingLevel9{80059};

#endif

class DirectAnnotation
{
public:
    DirectAnnotation() = default;
    explicit DirectAnnotation(HWND window);
    explicit DirectAnnotation(HWND window, DWORD objectId);
    ~DirectAnnotation() = default;

    // +++++++++++++++++++++++++++++++++++
    // Disallow copy and assignment.
    // +++++++++++++++++++++++++++++++++++

    DirectAnnotation(const DirectAnnotation&) = delete;
    DirectAnnotation(DirectAnnotation&&) = delete;
    DirectAnnotation& operator=(const DirectAnnotation&) = delete;
    DirectAnnotation& operator=(DirectAnnotation&&) = delete;

    // +++++++++++++++++++++++++++++++++++
    // Utility.
    // +++++++++++++++++++++++++++++++++++

    void SetWindow(HWND window);
    void SetObjectId(DWORD objectId);
    bool ClearProperty(VARIANT varChild, MSAAPROPID idProp);


    // +++++++++++++++++++++++++++++++++++
    // Microsoft Active Accessibility.
    // +++++++++++++++++++++++++++++++++++

    bool SetAccessibleDescription(VARIANT varChild, LPCWSTR description);
    bool SetAccessibleDescription(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleDescriptionF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto description{Utility::FormatArgs(format, args...)};
        if (description.empty())
        {
            return false;
        }
        return SetAccessibleDescription(varChild, description.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleDescriptionF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto description{Utility::FormatArgs(format, args...)};
        if (description.empty())
        {
            return false;
        }
        return SetAccessibleDescription(varChild, description.c_str());
    }
    bool SetAccessibleHelp(VARIANT varChild, LPCWSTR help);
    bool SetAccessibleHelp(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleHelpF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto help{Utility::FormatArgs(format, args...)};
        if (help.empty())
        {
            return false;
        }
        return SetAccessibleHelp(varChild, help.c_str());
    }

    template<typename... Arguments>
    bool SetAccessibleHelpF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto help{Utility::FormatArgs(format, args...)};
        if (help.empty())
        {
            return false;
        }
        return SetAccessibleHelp(varChild, help.c_str());
    }

    bool SetAccessibleName(VARIANT varChild, LPCWSTR name);
    bool SetAccessibleName(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleNameF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto name{Utility::FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetAccessibleName(varChild, name.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleNameF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto name{Utility::FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetAccessibleName(varChild, name.c_str());
    }
    bool SetAccessibleRole(VARIANT varChild, LONG role);
    bool SetAccessibleState(VARIANT varChild, LONG state, bool overrideMSAA);
    bool SetAccessibleValue(VARIANT varChild, LPCWSTR value);
    bool SetAccessibleValue(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetAccessibleValueF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto value{Utility::FormatArgs(format, args...)};
        if (value.empty())
        {
            return false;
        }
        return SetAccessibleValue(varChild, value.c_str());
    }
    template<typename... Arguments>
    bool SetAccessibleValueF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto value{Utility::FormatArgs(format, args...)};
        if (value.empty())
        {
            return false;
        }
        return SetAccessibleValue(varChild, value);
    }

    // +++++++++++++++++++++++++++++++++++
    // UI Automation.
    // +++++++++++++++++++++++++++++++++++

    bool SetControlTypeProperty(VARIANT varChild, LONG controlType);

    bool SetFrameworkIdProperty(VARIANT varChild, LPCWSTR value);
    bool SetFrameworkIdProperty(VARIANT varChild, UINT nID);

    bool SetFullDescriptionProperty(VARIANT varChild, LPCWSTR description);
    bool SetFullDescriptionProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetFullDescriptionPropertyF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto description{Utility::FormatArgs(format, args...)};
        if (description.empty())
        {
            return false;
        }
        return SetFullDescriptionProperty(varChild, description.c_str());
    }
    template<typename... Arguments>
    bool SetFullDescriptionPropertyF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto description{Utility::FormatArgs(format, args...)};
        if (description.empty())
        {
            return false;
        }
        return SetFullDescriptionProperty(varChild, description.c_str());
    }

    bool SetHeadingLevelProperty(VARIANT varChild, long headingLevel);

    bool SetHelpTextProperty(VARIANT varChild, LPCWSTR helpText);
    bool SetHelpTextProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetHelpTextPropertyF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto helpText{Utility::FormatArgs(format, args...)};
        if (helpText.empty())
        {
            return false;
        }
        return SetHelpTextProperty(varChild, helpText.c_str());
    }
    template<typename... Arguments>
    bool SetHelpTextPropertyF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto helpText{Utility::FormatArgs(format, args...)};
        if (helpText.empty())
        {
            return false;
        }
        return SetHelpTextProperty(varChild, helpText.c_str());
    }

    bool SetIsContentElementProperty(VARIANT varChild, bool value);
    bool SetIsControlElementProperty(VARIANT varChild, bool value);
    bool SetIsDataValidForFormProperty(VARIANT varChild, bool value);
    bool SetIsDialogProperty(VARIANT varChild, bool value);
    bool SetIsPasswordProperty(VARIANT varChild, bool value);
    bool SetIsRequiredForFormProperty(VARIANT varChild, bool value);

    bool SetItemStatusProperty(VARIANT varChild, LPCWSTR itemStatus);
    bool SetItemStatusProperty(VARIANT varChild, UINT nID);

    bool SetItemTypeProperty(VARIANT varChild, LPCWSTR itemType);
    bool SetItemTypeProperty(VARIANT varChild, UINT nID);

    bool SetLevelProperty(VARIANT varChild, long level);

    bool SetLocalizedControlTypeProperty(VARIANT varChild, LPCWSTR name);
    bool SetLocalizedControlTypeProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetLocalizedControlTypePropertyF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto name{FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetLocalizedControlTypeProperty(varChild, name.c_str());
    }
    template<typename... Arguments>
    bool SetLocalizedControlTypePropertyF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto name{FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetLocalizedControlTypeProperty(varChild, name.c_str());
    }

    bool SetNameProperty(VARIANT varChild, LPCWSTR name);
    bool SetNameProperty(VARIANT varChild, UINT nID);
    template<typename... Arguments>
    bool SetNamePropertyF(VARIANT varChild, LPCWSTR format, Arguments&&... args)
    {
        if (format == nullptr || format[0] == 0)
        {
            return false;
        }
        const auto name{Utility::FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetNameProperty(varChild, name.c_str());
    }
    template<typename... Arguments>
    bool SetNamePropertyF(VARIANT varChild, UINT format, Arguments&&... args)
    {
        if (format == 0)
        {
            return false;
        }
        const auto name{Utility::FormatArgs(format, args...)};
        if (name.empty())
        {
            return false;
        }
        return SetNameProperty(varChild, name.c_str());
    }

    bool SetPositionInSetProperty(VARIANT varChild, long positionInSet);
    bool SetSizeOfSetProperty(VARIANT varChild, long sizeOfSet);

private:
    bool CommonPrologue(VARIANT varChild);
    bool InitAccPropServices();
    HRESULT get_accState(VARIANT varChild, VARIANT* pvarState);
    HRESULT ClearAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp);
    HRESULT SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, LPCWSTR value);
    HRESULT SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, VARIANT value);
    HRESULT SetAccessibilityPropertyWorker(DWORD idChild, MSAAPROPID idProp, bool value);

private:
#if defined(_MSC_VER)
    _COM_SMARTPTR_TYPEDEF(IAccessible, __uuidof(IAccessible));
    _COM_SMARTPTR_TYPEDEF(IAccPropServices, __uuidof(IAccPropServices));
#else
    using IAccessiblePtr = _com_ptr_t<_com_IIID<IAccessible, &IID_IAccessible>>;
    using IAccPropServicesPtr = _com_ptr_t<_com_IIID<IAccPropServices, &IID_IAccPropServices>>;
#endif
    HWND m_window{nullptr};
    DWORD m_objectId{static_cast<DWORD>(OBJID_CLIENT)};
    IAccPropServicesPtr m_pAccPropServices{nullptr};
    bool m_initAccPropServicesFailed{false};
};
