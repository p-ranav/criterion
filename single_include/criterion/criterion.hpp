
#pragma once

namespace indicators {
enum class Color { grey, red, green, yellow, blue, magenta, cyan, white, unspecified };
}

#pragma once
namespace indicators {
enum class FontStyle { bold, dark, italic, underline, blink, reverse, concealed, crossed };
}

#pragma once

namespace indicators {
enum class ProgressType { incremental, decremental };
}

//!
//! termcolor
//! ~~~~~~~~~
//!
//! termcolor is a header-only c++ library for printing colored messages
//! to the terminal. Written just for fun with a help of the Force.
//!
//! :copyright: (c) 2013 by Ihor Kalnytskyi
//! :license: BSD, see LICENSE for details
//!

#ifndef TERMCOLOR_HPP_
#define TERMCOLOR_HPP_

// the following snippet of code detects the current OS and
// defines the appropriate macro that is used to wrap some
// platform specific things
#if defined(_WIN32) || defined(_WIN64)
#   define TERMCOLOR_OS_WINDOWS
#elif defined(__APPLE__)
#   define TERMCOLOR_OS_MACOS
#elif defined(__unix__) || defined(__unix)
#   define TERMCOLOR_OS_LINUX
#else
#   error unsupported platform
#endif


// This headers provides the `isatty()`/`fileno()` functions,
// which are used for testing whether a standart stream refers
// to the terminal. As for Windows, we also need WinApi funcs
// for changing colors attributes of the terminal.
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
#   include <unistd.h>
#elif defined(TERMCOLOR_OS_WINDOWS)
#if defined(_MSC_VER)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#   include <io.h>
#   include <windows.h>
#endif
#endif


#include <iostream>
#include <cstdio>


namespace termcolor
{
    // Forward declaration of the `_internal` namespace.
    // All comments are below.
    namespace _internal
    {
        // An index to be used to access a private storage of I/O streams. See
        // colorize / nocolorize I/O manipulators for details.
        static int colorize_index = std::ios_base::xalloc();

        inline FILE* get_standard_stream(const std::ostream& stream);
        inline bool is_colorized(std::ostream& stream);
        inline bool is_atty(const std::ostream& stream);

    #if defined(TERMCOLOR_OS_WINDOWS)
        inline void win_change_attributes(std::ostream& stream, int foreground, int background=-1);
    #endif
    }

    inline
    std::ostream& colorize(std::ostream& stream)
    {
        stream.iword(_internal::colorize_index) = 1L;
        return stream;
    }

    inline
    std::ostream& nocolorize(std::ostream& stream)
    {
        stream.iword(_internal::colorize_index) = 0L;
        return stream;
    }

    inline
    std::ostream& reset(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[00m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1, -1);
        #endif
        }
        return stream;
    }

    inline
    std::ostream& bold(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[1m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    inline
    std::ostream& dark(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[2m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    inline
    std::ostream& italic(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[3m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    inline
    std::ostream& underline(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[4m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    inline
    std::ostream& blink(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[5m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    inline
    std::ostream& reverse(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[7m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    inline
    std::ostream& concealed(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[8m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    inline
    std::ostream& crossed(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[9m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    template <uint8_t code> inline
    std::ostream& color(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            char command[12];
            std::snprintf(command, sizeof(command), "\033[38;5;%dm", code);
            stream << command;
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    template <uint8_t code> inline
    std::ostream& on_color(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            char command[12];
            std::snprintf(command, sizeof(command), "\033[48;5;%dm", code);
            stream << command;
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    template <uint8_t r, uint8_t g, uint8_t b> inline
    std::ostream& color(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            char command[20];
            std::snprintf(command, sizeof(command), "\033[38;2;%d;%d;%dm", r, g, b);
            stream << command;
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    template <uint8_t r, uint8_t g, uint8_t b> inline
    std::ostream& on_color(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            char command[20];
            std::snprintf(command, sizeof(command), "\033[48;2;%d;%d;%dm", r, g, b);
            stream << command;
        #elif defined(TERMCOLOR_OS_WINDOWS)
        #endif
        }
        return stream;
    }

    inline
    std::ostream& grey(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[30m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream,
                0   // grey (black)
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& red(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[31m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream,
                FOREGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& green(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[32m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream,
                FOREGROUND_GREEN
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& yellow(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[33m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream,
                FOREGROUND_GREEN | FOREGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& blue(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[34m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream,
                FOREGROUND_BLUE
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& magenta(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[35m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream,
                FOREGROUND_BLUE | FOREGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& cyan(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[36m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream,
                FOREGROUND_BLUE | FOREGROUND_GREEN
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& white(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[37m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream,
                FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED
            );
        #endif
        }
        return stream;
    }



    inline
    std::ostream& on_grey(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[40m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                0   // grey (black)
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& on_red(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[41m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                BACKGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& on_green(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[42m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                BACKGROUND_GREEN
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& on_yellow(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[43m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                BACKGROUND_GREEN | BACKGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& on_blue(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[44m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                BACKGROUND_BLUE
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& on_magenta(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[45m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                BACKGROUND_BLUE | BACKGROUND_RED
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& on_cyan(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[46m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                BACKGROUND_GREEN | BACKGROUND_BLUE
            );
        #endif
        }
        return stream;
    }

    inline
    std::ostream& on_white(std::ostream& stream)
    {
        if (_internal::is_colorized(stream))
        {
        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            stream << "\033[47m";
        #elif defined(TERMCOLOR_OS_WINDOWS)
            _internal::win_change_attributes(stream, -1,
                BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED
            );
        #endif
        }

        return stream;
    }



    //! Since C++ hasn't a way to hide something in the header from
    //! the outer access, I have to introduce this namespace which
    //! is used for internal purpose and should't be access from
    //! the user code.
    namespace _internal
    {
        //! Since C++ hasn't a true way to extract stream handler
        //! from the a given `std::ostream` object, I have to write
        //! this kind of hack.
        inline
        FILE* get_standard_stream(const std::ostream& stream)
        {
            if (&stream == &std::cout)
                return stdout;
            else if ((&stream == &std::cerr) || (&stream == &std::clog))
                return stderr;

            return nullptr;
        }

        // Say whether a given stream should be colorized or not. It's always
        // true for ATTY streams and may be true for streams marked with
        // colorize flag.
        inline
        bool is_colorized(std::ostream& stream)
        {
            return is_atty(stream) || static_cast<bool>(stream.iword(colorize_index));
        }

        //! Test whether a given `std::ostream` object refers to
        //! a terminal.
        inline
        bool is_atty(const std::ostream& stream)
        {
            FILE* std_stream = get_standard_stream(stream);

            // Unfortunately, fileno() ends with segmentation fault
            // if invalid file descriptor is passed. So we need to
            // handle this case gracefully and assume it's not a tty
            // if standard stream is not detected, and 0 is returned.
            if (!std_stream)
                return false;

        #if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
            return ::isatty(fileno(std_stream));
        #elif defined(TERMCOLOR_OS_WINDOWS)
            return ::_isatty(_fileno(std_stream));
        #endif
        }

    #if defined(TERMCOLOR_OS_WINDOWS)
        //! Change Windows Terminal colors attribute. If some
        //! parameter is `-1` then attribute won't changed.
        inline void win_change_attributes(std::ostream& stream, int foreground, int background)
        {
            // yeah, i know.. it's ugly, it's windows.
            static WORD defaultAttributes = 0;

            // Windows doesn't have ANSI escape sequences and so we use special
            // API to change Terminal output color. That means we can't
            // manipulate colors by means of "std::stringstream" and hence
            // should do nothing in this case.
            if (!_internal::is_atty(stream))
                return;

            // get terminal handle
            HANDLE hTerminal = INVALID_HANDLE_VALUE;
            if (&stream == &std::cout)
                hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
            else if (&stream == &std::cerr)
                hTerminal = GetStdHandle(STD_ERROR_HANDLE);

            // save default terminal attributes if it unsaved
            if (!defaultAttributes)
            {
                CONSOLE_SCREEN_BUFFER_INFO info;
                if (!GetConsoleScreenBufferInfo(hTerminal, &info))
                    return;
                defaultAttributes = info.wAttributes;
            }

            // restore all default settings
            if (foreground == -1 && background == -1)
            {
                SetConsoleTextAttribute(hTerminal, defaultAttributes);
                return;
            }

            // get current settings
            CONSOLE_SCREEN_BUFFER_INFO info;
            if (!GetConsoleScreenBufferInfo(hTerminal, &info))
                return;

            if (foreground != -1)
            {
                info.wAttributes &= ~(info.wAttributes & 0x0F);
                info.wAttributes |= static_cast<WORD>(foreground);
            }

            if (background != -1)
            {
                info.wAttributes &= ~(info.wAttributes & 0xF0);
                info.wAttributes |= static_cast<WORD>(background);
            }

            SetConsoleTextAttribute(hTerminal, info.wAttributes);
        }
    #endif // TERMCOLOR_OS_WINDOWS

    } // namespace _internal

} // namespace termcolor


#undef TERMCOLOR_OS_WINDOWS
#undef TERMCOLOR_OS_MACOS
#undef TERMCOLOR_OS_LINUX

#endif // TERMCOLOR_HPP_

#pragma once
#include <utility>


#if defined(_WIN32)
#include <windows.h>

namespace indicators {

static inline std::pair<size_t, size_t> terminal_size() {
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int cols, rows;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
  return {static_cast<size_t>(rows), static_cast<size_t>(cols)};
}

size_t terminal_width() { return terminal_size().second; }

} // namespace indicators

#else

#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO

namespace indicators {

static inline std::pair<size_t, size_t> terminal_size() {
  struct winsize size;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
  return {static_cast<size_t>(size.ws_row), static_cast<size_t>(size.ws_col)};
}

static inline size_t terminal_width() { return terminal_size().second; }

} // namespace indicators

#endif


/*
Activity Indicators for Modern C++
https://github.com/p-ranav/indicators

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Dawid Pilarski <dawid.pilarski@panicsoftware.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

#include <cstddef>
// #include <indicators/color.hpp>
// #include <indicators/font_style.hpp>
// #include <indicators/progress_type.hpp>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace indicators {

namespace details {

template <bool condition> struct if_else;

template <> struct if_else<true> { using type = std::true_type; };

template <> struct if_else<false> { using type = std::false_type; };

template <bool condition, typename True, typename False> struct if_else_type;

template <typename True, typename False> struct if_else_type<true, True, False> {
  using type = True;
};

template <typename True, typename False> struct if_else_type<false, True, False> {
  using type = False;
};

template <typename... Ops> struct conjuction;

template <> struct conjuction<> : std::true_type {};

template <typename Op, typename... TailOps>
struct conjuction<Op, TailOps...>
    : if_else_type<!Op::value, std::false_type, conjuction<TailOps...>>::type {};

template <typename... Ops> struct disjunction;

template <> struct disjunction<> : std::false_type {};

template <typename Op, typename... TailOps>
struct disjunction<Op, TailOps...>
    : if_else_type<Op::value, std::true_type, disjunction<TailOps...>>::type {};

enum class ProgressBarOption {
  bar_width = 0,
  prefix_text,
  postfix_text,
  start,
  end,
  fill,
  lead,
  remainder,
  max_postfix_text_len,
  completed,
  show_percentage,
  show_elapsed_time,
  show_remaining_time,
  saved_start_time,
  foreground_color,
  spinner_show,
  spinner_states,
  font_styles,
  hide_bar_when_complete,
  min_progress,
  max_progress,
  progress_type,
  stream
};

template <typename T, ProgressBarOption Id> struct Setting {
  template <typename... Args,
            typename = typename std::enable_if<std::is_constructible<T, Args...>::value>::type>
  explicit Setting(Args &&... args) : value(std::forward<Args>(args)...) {}
  Setting(const Setting &) = default;
  Setting(Setting &&) = default;

  static constexpr auto id = Id;
  using type = T;

  T value{};
};

template <typename T> struct is_setting : std::false_type {};

template <ProgressBarOption Id, typename T> struct is_setting<Setting<T, Id>> : std::true_type {};

template <typename... Args>
struct are_settings : if_else<conjuction<is_setting<Args>...>::value>::type {};

template <> struct are_settings<> : std::true_type {};

template <typename Setting, typename Tuple> struct is_setting_from_tuple;

template <typename Setting> struct is_setting_from_tuple<Setting, std::tuple<>> : std::true_type {};

template <typename Setting, typename... TupleTypes>
struct is_setting_from_tuple<Setting, std::tuple<TupleTypes...>>
    : if_else<disjunction<std::is_same<Setting, TupleTypes>...>::value>::type {};

template <typename Tuple, typename... Settings>
struct are_settings_from_tuple
    : if_else<conjuction<is_setting_from_tuple<Settings, Tuple>...>::value>::type {};

template <ProgressBarOption Id> struct always_true { static constexpr auto value = true; };

template <ProgressBarOption Id, typename Default> Default &&get_impl(Default &&def) {
  return std::forward<Default>(def);
}

template <ProgressBarOption Id, typename Default, typename T, typename... Args>
auto get_impl(Default && /*def*/, T &&first, Args &&... /*tail*/) ->
    typename std::enable_if<(std::decay<T>::type::id == Id),
                            decltype(std::forward<T>(first))>::type {
  return std::forward<T>(first);
}

template <ProgressBarOption Id, typename Default, typename T, typename... Args>
auto get_impl(Default &&def, T && /*first*/, Args &&... tail) ->
    typename std::enable_if<(std::decay<T>::type::id != Id),
                            decltype(get_impl<Id>(std::forward<Default>(def),
                                                  std::forward<Args>(tail)...))>::type {
  return get_impl<Id>(std::forward<Default>(def), std::forward<Args>(tail)...);
}

template <ProgressBarOption Id, typename Default, typename... Args,
          typename = typename std::enable_if<are_settings<Args...>::value, void>::type>
auto get(Default &&def, Args &&... args)
    -> decltype(details::get_impl<Id>(std::forward<Default>(def), std::forward<Args>(args)...)) {
  return details::get_impl<Id>(std::forward<Default>(def), std::forward<Args>(args)...);
}

template <ProgressBarOption Id> using StringSetting = Setting<std::string, Id>;

template <ProgressBarOption Id> using IntegerSetting = Setting<std::size_t, Id>;

template <ProgressBarOption Id> using BooleanSetting = Setting<bool, Id>;

template <ProgressBarOption Id, typename Tuple, std::size_t counter = 0> struct option_idx;

template <ProgressBarOption Id, typename T, typename... Settings, std::size_t counter>
struct option_idx<Id, std::tuple<T, Settings...>, counter>
    : if_else_type<(Id == T::id), std::integral_constant<std::size_t, counter>,
                   option_idx<Id, std::tuple<Settings...>, counter + 1>>::type {};

template <ProgressBarOption Id, std::size_t counter> struct option_idx<Id, std::tuple<>, counter> {
  static_assert(always_true<(ProgressBarOption)Id>::value, "No such option was found");
};

template <ProgressBarOption Id, typename Settings>
auto get_value(Settings &&settings)
    -> decltype((std::get<option_idx<Id, typename std::decay<Settings>::type>::value>(
        std::declval<Settings &&>()))) {
  return std::get<option_idx<Id, typename std::decay<Settings>::type>::value>(
      std::forward<Settings>(settings));
}

} // namespace details

namespace option {
using BarWidth = details::IntegerSetting<details::ProgressBarOption::bar_width>;
using PrefixText = details::StringSetting<details::ProgressBarOption::prefix_text>;
using PostfixText = details::StringSetting<details::ProgressBarOption::postfix_text>;
using Start = details::StringSetting<details::ProgressBarOption::start>;
using End = details::StringSetting<details::ProgressBarOption::end>;
using Fill = details::StringSetting<details::ProgressBarOption::fill>;
using Lead = details::StringSetting<details::ProgressBarOption::lead>;
using Remainder = details::StringSetting<details::ProgressBarOption::remainder>;
using MaxPostfixTextLen = details::IntegerSetting<details::ProgressBarOption::max_postfix_text_len>;
using Completed = details::BooleanSetting<details::ProgressBarOption::completed>;
using ShowPercentage = details::BooleanSetting<details::ProgressBarOption::show_percentage>;
using ShowElapsedTime = details::BooleanSetting<details::ProgressBarOption::show_elapsed_time>;
using ShowRemainingTime = details::BooleanSetting<details::ProgressBarOption::show_remaining_time>;
using SavedStartTime = details::BooleanSetting<details::ProgressBarOption::saved_start_time>;
using ForegroundColor = details::Setting<Color, details::ProgressBarOption::foreground_color>;
using ShowSpinner = details::BooleanSetting<details::ProgressBarOption::spinner_show>;
using SpinnerStates =
    details::Setting<std::vector<std::string>, details::ProgressBarOption::spinner_states>;
using HideBarWhenComplete =
    details::BooleanSetting<details::ProgressBarOption::hide_bar_when_complete>;
using FontStyles =
    details::Setting<std::vector<FontStyle>, details::ProgressBarOption::font_styles>;
using MinProgress = details::IntegerSetting<details::ProgressBarOption::min_progress>;
using MaxProgress = details::IntegerSetting<details::ProgressBarOption::max_progress>;
using ProgressType = details::Setting<ProgressType, details::ProgressBarOption::progress_type>;
using Stream = details::Setting<std::ostream &, details::ProgressBarOption::stream>;
} // namespace option
} // namespace indicators

#pragma once

#if defined(_MSC_VER)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <cstdio>
#endif

namespace indicators {

#if defined(_MSC_VER)

static inline void show_console_cursor(bool const show) {
  HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

  CONSOLE_CURSOR_INFO cursorInfo;

  GetConsoleCursorInfo(out, &cursorInfo);
  cursorInfo.bVisible = show; // set the cursor visibility
  SetConsoleCursorInfo(out, &cursorInfo);
}

#else

static inline void show_console_cursor(bool const show) {
  std::fputs(show ? "\033[?25h" : "\033[?25l", stdout);
}

#endif

} // namespace indicators
#pragma once

#if defined(_MSC_VER)
#if !defined(NOMINMAX)
#define NOMINMAX
#endif
#include <io.h>
#include <windows.h>
#else
#include <iostream>
#endif

namespace indicators {

#ifdef _MSC_VER

static inline void move(int x, int y) {
  auto hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!hStdout)
    return;

  CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  GetConsoleScreenBufferInfo(hStdout, &csbiInfo);

  COORD cursor;

  cursor.X = csbiInfo.dwCursorPosition.X + x;
  cursor.Y = csbiInfo.dwCursorPosition.Y + y;
  SetConsoleCursorPosition(hStdout, cursor);
}

static inline void move_up(int lines) { move(0, -lines); }
static inline void move_down(int lines) { move(0, -lines); }
static inline void move_right(int cols) { move(cols, 0); }
static inline void move_left(int cols) { move(-cols, 0); }

#else

static inline void move_up(int lines) { std::cout << "\033[" << lines << "A"; }
static inline void move_down(int lines) { std::cout << "\033[" << lines << "B"; }
static inline void move_right(int cols) { std::cout << "\033[" << cols << "C"; }
static inline void move_left(int cols) { std::cout << "\033[" << cols << "D"; }

#endif

} // namespace indicators
#pragma once

// #include <indicators/display_width.hpp>

#include <clocale>
#if __has_include(<codecvt>)
#include <codecvt>
#define INDICATORS_HAVE_CODECVT 1
#endif
#include <cstdlib>
#include <locale>
#include <string>
#include <wchar.h>

namespace unicode {

#if INDICATORS_HAVE_CODECVT
namespace details {

/*
 * This is an implementation of wcwidth() and wcswidth() (defined in
 * IEEE Std 1002.1-2001) for Unicode.
 *
 * http://www.opengroup.org/onlinepubs/007904975/functions/wcwidth.html
 * http://www.opengroup.org/onlinepubs/007904975/functions/wcswidth.html
 *
 * In fixed-width output devices, Latin characters all occupy a single
 * "cell" position of equal width, whereas ideographic CJK characters
 * occupy two such cells. Interoperability between terminal-line
 * applications and (teletype-style) character terminals using the
 * UTF-8 encoding requires agreement on which character should advance
 * the cursor by how many cell positions. No established formal
 * standards exist at present on which Unicode character shall occupy
 * how many cell positions on character terminals. These routines are
 * a first attempt of defining such behavior based on simple rules
 * applied to data provided by the Unicode Consortium.
 *
 * For some graphical characters, the Unicode standard explicitly
 * defines a character-cell width via the definition of the East Asian
 * FullWidth (F), Wide (W), Half-width (H), and Narrow (Na) classes.
 * In all these cases, there is no ambiguity about which width a
 * terminal shall use. For characters in the East Asian Ambiguous (A)
 * class, the width choice depends purely on a preference of backward
 * compatibility with either historic CJK or Western practice.
 * Choosing single-width for these characters is easy to justify as
 * the appropriate long-term solution, as the CJK practice of
 * displaying these characters as double-width comes from historic
 * implementation simplicity (8-bit encoded characters were displayed
 * single-width and 16-bit ones double-width, even for Greek,
 * Cyrillic, etc.) and not any typographic considerations.
 *
 * Much less clear is the choice of width for the Not East Asian
 * (Neutral) class. Existing practice does not dictate a width for any
 * of these characters. It would nevertheless make sense
 * typographically to allocate two character cells to characters such
 * as for instance EM SPACE or VOLUME INTEGRAL, which cannot be
 * represented adequately with a single-width glyph. The following
 * routines at present merely assign a single-cell width to all
 * neutral characters, in the interest of simplicity. This is not
 * entirely satisfactory and should be reconsidered before
 * establishing a formal standard in this area. At the moment, the
 * decision which Not East Asian (Neutral) characters should be
 * represented by double-width glyphs cannot yet be answered by
 * applying a simple rule from the Unicode database content. Setting
 * up a proper standard for the behavior of UTF-8 character terminals
 * will require a careful analysis not only of each Unicode character,
 * but also of each presentation form, something the author of these
 * routines has avoided to do so far.
 *
 * http://www.unicode.org/unicode/reports/tr11/
 *
 * Markus Kuhn -- 2007-05-26 (Unicode 5.0)
 *
 * Permission to use, copy, modify, and distribute this software
 * for any purpose and without fee is hereby granted. The author
 * disclaims all warranties with regard to this software.
 *
 * Latest version: http://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c
 */

struct interval {
  int first;
  int last;
};

/* auxiliary function for binary search in interval table */
static inline int bisearch(wchar_t ucs, const struct interval *table, int max) {
  int min = 0;
  int mid;

  if (ucs < table[0].first || ucs > table[max].last)
    return 0;
  while (max >= min) {
    mid = (min + max) / 2;
    if (ucs > table[mid].last)
      min = mid + 1;
    else if (ucs < table[mid].first)
      max = mid - 1;
    else
      return 1;
  }

  return 0;
}

/* The following two functions define the column width of an ISO 10646
 * character as follows:
 *
 *    - The null character (U+0000) has a column width of 0.
 *
 *    - Other C0/C1 control characters and DEL will lead to a return
 *      value of -1.
 *
 *    - Non-spacing and enclosing combining characters (general
 *      category code Mn or Me in the Unicode database) have a
 *      column width of 0.
 *
 *    - SOFT HYPHEN (U+00AD) has a column width of 1.
 *
 *    - Other format characters (general category code Cf in the Unicode
 *      database) and ZERO WIDTH SPACE (U+200B) have a column width of 0.
 *
 *    - Hangul Jamo medial vowels and final consonants (U+1160-U+11FF)
 *      have a column width of 0.
 *
 *    - Spacing characters in the East Asian Wide (W) or East Asian
 *      Full-width (F) category as defined in Unicode Technical
 *      Report #11 have a column width of 2.
 *
 *    - All remaining characters (including all printable
 *      ISO 8859-1 and WGL4 characters, Unicode control characters,
 *      etc.) have a column width of 1.
 *
 * This implementation assumes that wchar_t characters are encoded
 * in ISO 10646.
 */

static inline int mk_wcwidth(wchar_t ucs) {
  /* sorted list of non-overlapping intervals of non-spacing characters */
  /* generated by "uniset +cat=Me +cat=Mn +cat=Cf -00AD +1160-11FF +200B c" */
  static const struct interval combining[] = {
      {0x0300, 0x036F},   {0x0483, 0x0486},   {0x0488, 0x0489},   {0x0591, 0x05BD},
      {0x05BF, 0x05BF},   {0x05C1, 0x05C2},   {0x05C4, 0x05C5},   {0x05C7, 0x05C7},
      {0x0600, 0x0603},   {0x0610, 0x0615},   {0x064B, 0x065E},   {0x0670, 0x0670},
      {0x06D6, 0x06E4},   {0x06E7, 0x06E8},   {0x06EA, 0x06ED},   {0x070F, 0x070F},
      {0x0711, 0x0711},   {0x0730, 0x074A},   {0x07A6, 0x07B0},   {0x07EB, 0x07F3},
      {0x0901, 0x0902},   {0x093C, 0x093C},   {0x0941, 0x0948},   {0x094D, 0x094D},
      {0x0951, 0x0954},   {0x0962, 0x0963},   {0x0981, 0x0981},   {0x09BC, 0x09BC},
      {0x09C1, 0x09C4},   {0x09CD, 0x09CD},   {0x09E2, 0x09E3},   {0x0A01, 0x0A02},
      {0x0A3C, 0x0A3C},   {0x0A41, 0x0A42},   {0x0A47, 0x0A48},   {0x0A4B, 0x0A4D},
      {0x0A70, 0x0A71},   {0x0A81, 0x0A82},   {0x0ABC, 0x0ABC},   {0x0AC1, 0x0AC5},
      {0x0AC7, 0x0AC8},   {0x0ACD, 0x0ACD},   {0x0AE2, 0x0AE3},   {0x0B01, 0x0B01},
      {0x0B3C, 0x0B3C},   {0x0B3F, 0x0B3F},   {0x0B41, 0x0B43},   {0x0B4D, 0x0B4D},
      {0x0B56, 0x0B56},   {0x0B82, 0x0B82},   {0x0BC0, 0x0BC0},   {0x0BCD, 0x0BCD},
      {0x0C3E, 0x0C40},   {0x0C46, 0x0C48},   {0x0C4A, 0x0C4D},   {0x0C55, 0x0C56},
      {0x0CBC, 0x0CBC},   {0x0CBF, 0x0CBF},   {0x0CC6, 0x0CC6},   {0x0CCC, 0x0CCD},
      {0x0CE2, 0x0CE3},   {0x0D41, 0x0D43},   {0x0D4D, 0x0D4D},   {0x0DCA, 0x0DCA},
      {0x0DD2, 0x0DD4},   {0x0DD6, 0x0DD6},   {0x0E31, 0x0E31},   {0x0E34, 0x0E3A},
      {0x0E47, 0x0E4E},   {0x0EB1, 0x0EB1},   {0x0EB4, 0x0EB9},   {0x0EBB, 0x0EBC},
      {0x0EC8, 0x0ECD},   {0x0F18, 0x0F19},   {0x0F35, 0x0F35},   {0x0F37, 0x0F37},
      {0x0F39, 0x0F39},   {0x0F71, 0x0F7E},   {0x0F80, 0x0F84},   {0x0F86, 0x0F87},
      {0x0F90, 0x0F97},   {0x0F99, 0x0FBC},   {0x0FC6, 0x0FC6},   {0x102D, 0x1030},
      {0x1032, 0x1032},   {0x1036, 0x1037},   {0x1039, 0x1039},   {0x1058, 0x1059},
      {0x1160, 0x11FF},   {0x135F, 0x135F},   {0x1712, 0x1714},   {0x1732, 0x1734},
      {0x1752, 0x1753},   {0x1772, 0x1773},   {0x17B4, 0x17B5},   {0x17B7, 0x17BD},
      {0x17C6, 0x17C6},   {0x17C9, 0x17D3},   {0x17DD, 0x17DD},   {0x180B, 0x180D},
      {0x18A9, 0x18A9},   {0x1920, 0x1922},   {0x1927, 0x1928},   {0x1932, 0x1932},
      {0x1939, 0x193B},   {0x1A17, 0x1A18},   {0x1B00, 0x1B03},   {0x1B34, 0x1B34},
      {0x1B36, 0x1B3A},   {0x1B3C, 0x1B3C},   {0x1B42, 0x1B42},   {0x1B6B, 0x1B73},
      {0x1DC0, 0x1DCA},   {0x1DFE, 0x1DFF},   {0x200B, 0x200F},   {0x202A, 0x202E},
      {0x2060, 0x2063},   {0x206A, 0x206F},   {0x20D0, 0x20EF},   {0x302A, 0x302F},
      {0x3099, 0x309A},   {0xA806, 0xA806},   {0xA80B, 0xA80B},   {0xA825, 0xA826},
      {0xFB1E, 0xFB1E},   {0xFE00, 0xFE0F},   {0xFE20, 0xFE23},   {0xFEFF, 0xFEFF},
      {0xFFF9, 0xFFFB},   {0x10A01, 0x10A03}, {0x10A05, 0x10A06}, {0x10A0C, 0x10A0F},
      {0x10A38, 0x10A3A}, {0x10A3F, 0x10A3F}, {0x1D167, 0x1D169}, {0x1D173, 0x1D182},
      {0x1D185, 0x1D18B}, {0x1D1AA, 0x1D1AD}, {0x1D242, 0x1D244}, {0xE0001, 0xE0001},
      {0xE0020, 0xE007F}, {0xE0100, 0xE01EF}};

  /* test for 8-bit control characters */
  if (ucs == 0)
    return 0;
  if (ucs < 32 || (ucs >= 0x7f && ucs < 0xa0))
    return -1;

  /* binary search in table of non-spacing characters */
  if (bisearch(ucs, combining, sizeof(combining) / sizeof(struct interval) - 1))
    return 0;

  /* if we arrive here, ucs is not a combining or C0/C1 control character */

  return 1 + (ucs >= 0x1100 &&
              (ucs <= 0x115f || /* Hangul Jamo init. consonants */
               ucs == 0x2329 || ucs == 0x232a ||
               (ucs >= 0x2e80 && ucs <= 0xa4cf && ucs != 0x303f) || /* CJK ... Yi */
               (ucs >= 0xac00 && ucs <= 0xd7a3) ||                  /* Hangul Syllables */
               (ucs >= 0xf900 && ucs <= 0xfaff) || /* CJK Compatibility Ideographs */
               (ucs >= 0xfe10 && ucs <= 0xfe19) || /* Vertical forms */
               (ucs >= 0xfe30 && ucs <= 0xfe6f) || /* CJK Compatibility Forms */
               (ucs >= 0xff00 && ucs <= 0xff60) || /* Fullwidth Forms */
               (ucs >= 0xffe0 && ucs <= 0xffe6) || (ucs >= 0x20000 && ucs <= 0x2fffd) ||
               (ucs >= 0x30000 && ucs <= 0x3fffd)));
}

static inline int mk_wcswidth(const wchar_t *pwcs, size_t n) {
  int w, width = 0;

  for (; *pwcs && n-- > 0; pwcs++)
    if ((w = mk_wcwidth(*pwcs)) < 0)
      return -1;
    else
      width += w;

  return width;
}

/*
 * The following functions are the same as mk_wcwidth() and
 * mk_wcswidth(), except that spacing characters in the East Asian
 * Ambiguous (A) category as defined in Unicode Technical Report #11
 * have a column width of 2. This variant might be useful for users of
 * CJK legacy encodings who want to migrate to UCS without changing
 * the traditional terminal character-width behaviour. It is not
 * otherwise recommended for general use.
 */
static inline int mk_wcwidth_cjk(wchar_t ucs) {
  /* sorted list of non-overlapping intervals of East Asian Ambiguous
   * characters, generated by "uniset +WIDTH-A -cat=Me -cat=Mn -cat=Cf c" */
  static const struct interval ambiguous[] = {
      {0x00A1, 0x00A1},    {0x00A4, 0x00A4}, {0x00A7, 0x00A8}, {0x00AA, 0x00AA}, {0x00AE, 0x00AE},
      {0x00B0, 0x00B4},    {0x00B6, 0x00BA}, {0x00BC, 0x00BF}, {0x00C6, 0x00C6}, {0x00D0, 0x00D0},
      {0x00D7, 0x00D8},    {0x00DE, 0x00E1}, {0x00E6, 0x00E6}, {0x00E8, 0x00EA}, {0x00EC, 0x00ED},
      {0x00F0, 0x00F0},    {0x00F2, 0x00F3}, {0x00F7, 0x00FA}, {0x00FC, 0x00FC}, {0x00FE, 0x00FE},
      {0x0101, 0x0101},    {0x0111, 0x0111}, {0x0113, 0x0113}, {0x011B, 0x011B}, {0x0126, 0x0127},
      {0x012B, 0x012B},    {0x0131, 0x0133}, {0x0138, 0x0138}, {0x013F, 0x0142}, {0x0144, 0x0144},
      {0x0148, 0x014B},    {0x014D, 0x014D}, {0x0152, 0x0153}, {0x0166, 0x0167}, {0x016B, 0x016B},
      {0x01CE, 0x01CE},    {0x01D0, 0x01D0}, {0x01D2, 0x01D2}, {0x01D4, 0x01D4}, {0x01D6, 0x01D6},
      {0x01D8, 0x01D8},    {0x01DA, 0x01DA}, {0x01DC, 0x01DC}, {0x0251, 0x0251}, {0x0261, 0x0261},
      {0x02C4, 0x02C4},    {0x02C7, 0x02C7}, {0x02C9, 0x02CB}, {0x02CD, 0x02CD}, {0x02D0, 0x02D0},
      {0x02D8, 0x02DB},    {0x02DD, 0x02DD}, {0x02DF, 0x02DF}, {0x0391, 0x03A1}, {0x03A3, 0x03A9},
      {0x03B1, 0x03C1},    {0x03C3, 0x03C9}, {0x0401, 0x0401}, {0x0410, 0x044F}, {0x0451, 0x0451},
      {0x2010, 0x2010},    {0x2013, 0x2016}, {0x2018, 0x2019}, {0x201C, 0x201D}, {0x2020, 0x2022},
      {0x2024, 0x2027},    {0x2030, 0x2030}, {0x2032, 0x2033}, {0x2035, 0x2035}, {0x203B, 0x203B},
      {0x203E, 0x203E},    {0x2074, 0x2074}, {0x207F, 0x207F}, {0x2081, 0x2084}, {0x20AC, 0x20AC},
      {0x2103, 0x2103},    {0x2105, 0x2105}, {0x2109, 0x2109}, {0x2113, 0x2113}, {0x2116, 0x2116},
      {0x2121, 0x2122},    {0x2126, 0x2126}, {0x212B, 0x212B}, {0x2153, 0x2154}, {0x215B, 0x215E},
      {0x2160, 0x216B},    {0x2170, 0x2179}, {0x2190, 0x2199}, {0x21B8, 0x21B9}, {0x21D2, 0x21D2},
      {0x21D4, 0x21D4},    {0x21E7, 0x21E7}, {0x2200, 0x2200}, {0x2202, 0x2203}, {0x2207, 0x2208},
      {0x220B, 0x220B},    {0x220F, 0x220F}, {0x2211, 0x2211}, {0x2215, 0x2215}, {0x221A, 0x221A},
      {0x221D, 0x2220},    {0x2223, 0x2223}, {0x2225, 0x2225}, {0x2227, 0x222C}, {0x222E, 0x222E},
      {0x2234, 0x2237},    {0x223C, 0x223D}, {0x2248, 0x2248}, {0x224C, 0x224C}, {0x2252, 0x2252},
      {0x2260, 0x2261},    {0x2264, 0x2267}, {0x226A, 0x226B}, {0x226E, 0x226F}, {0x2282, 0x2283},
      {0x2286, 0x2287},    {0x2295, 0x2295}, {0x2299, 0x2299}, {0x22A5, 0x22A5}, {0x22BF, 0x22BF},
      {0x2312, 0x2312},    {0x2460, 0x24E9}, {0x24EB, 0x254B}, {0x2550, 0x2573}, {0x2580, 0x258F},
      {0x2592, 0x2595},    {0x25A0, 0x25A1}, {0x25A3, 0x25A9}, {0x25B2, 0x25B3}, {0x25B6, 0x25B7},
      {0x25BC, 0x25BD},    {0x25C0, 0x25C1}, {0x25C6, 0x25C8}, {0x25CB, 0x25CB}, {0x25CE, 0x25D1},
      {0x25E2, 0x25E5},    {0x25EF, 0x25EF}, {0x2605, 0x2606}, {0x2609, 0x2609}, {0x260E, 0x260F},
      {0x2614, 0x2615},    {0x261C, 0x261C}, {0x261E, 0x261E}, {0x2640, 0x2640}, {0x2642, 0x2642},
      {0x2660, 0x2661},    {0x2663, 0x2665}, {0x2667, 0x266A}, {0x266C, 0x266D}, {0x266F, 0x266F},
      {0x273D, 0x273D},    {0x2776, 0x277F}, {0xE000, 0xF8FF}, {0xFFFD, 0xFFFD}, {0xF0000, 0xFFFFD},
      {0x100000, 0x10FFFD}};

  /* binary search in table of non-spacing characters */
  if (bisearch(ucs, ambiguous, sizeof(ambiguous) / sizeof(struct interval) - 1))
    return 2;

  return mk_wcwidth(ucs);
}

static inline int mk_wcswidth_cjk(const wchar_t *pwcs, size_t n) {
  int w, width = 0;

  for (; *pwcs && n-- > 0; pwcs++)
    if ((w = mk_wcwidth_cjk(*pwcs)) < 0)
      return -1;
    else
      width += w;

  return width;
}

// convert UTF-8 string to wstring
static inline std::wstring utf8_decode(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.from_bytes(str);
}

// convert wstring to UTF-8 string
static inline std::string utf8_encode(const std::wstring &str) {
  std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
  return myconv.to_bytes(str);
}

} // namespace details

static inline int display_width(const std::string &input) {
  using namespace unicode::details;
  return mk_wcswidth(utf8_decode(input).c_str(), input.size());
}

static inline int display_width(const std::wstring &input) {
  return details::mk_wcswidth(input.c_str(), input.size());
}

#else

static inline int display_width(const std::string &input) {
  return input.length();
}

static inline int display_width(const std::wstring &input) {
  return input.length();
}

#endif

} // namespace unicode

// #include <indicators/setting.hpp>
// #include <indicators/termcolor.hpp>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ostream>
#include <string>
#include <vector>

#include <cassert>
#include <cmath>

namespace indicators {
namespace details {

inline void set_stream_color(std::ostream &os, Color color) {
  switch (color) {
  case Color::grey:
    os << termcolor::grey;
    break;
  case Color::red:
    os << termcolor::red;
    break;
  case Color::green:
    os << termcolor::green;
    break;
  case Color::yellow:
    os << termcolor::yellow;
    break;
  case Color::blue:
    os << termcolor::blue;
    break;
  case Color::magenta:
    os << termcolor::magenta;
    break;
  case Color::cyan:
    os << termcolor::cyan;
    break;
  case Color::white:
    os << termcolor::white;
    break;
  default:
    assert(false);
  }
}

inline void set_font_style(std::ostream &os, FontStyle style) {
  switch (style) {
  case FontStyle::bold:
    os << termcolor::bold;
    break;
  case FontStyle::dark:
    os << termcolor::dark;
    break;
  case FontStyle::italic:
    os << termcolor::italic;
    break;
  case FontStyle::underline:
    os << termcolor::underline;
    break;
  case FontStyle::blink:
    os << termcolor::blink;
    break;
  case FontStyle::reverse:
    os << termcolor::reverse;
    break;
  case FontStyle::concealed:
    os << termcolor::concealed;
    break;
  case FontStyle::crossed:
    os << termcolor::crossed;
    break;
  default:
    break;
  }
}

inline std::ostream &write_duration(std::ostream &os, std::chrono::nanoseconds ns) {
  using namespace std;
  using namespace std::chrono;
  using days = duration<int, ratio<86400>>;
  char fill = os.fill();
  os.fill('0');
  auto d = duration_cast<days>(ns);
  ns -= d;
  auto h = duration_cast<hours>(ns);
  ns -= h;
  auto m = duration_cast<minutes>(ns);
  ns -= m;
  auto s = duration_cast<seconds>(ns);
  if (d.count() > 0)
    os << setw(2) << d.count() << "d:";
  if (h.count() > 0)
    os << setw(2) << h.count() << "h:";
  os << setw(2) << m.count() << "m:" << setw(2) << s.count() << 's';
  os.fill(fill);
  return os;
}

class BlockProgressScaleWriter {
public:
  BlockProgressScaleWriter(std::ostream &os, size_t bar_width) : os(os), bar_width(bar_width) {}

  std::ostream &write(float progress) {
    std::string fill_text{"█"};
    std::vector<std::string> lead_characters{" ", "▏", "▎", "▍", "▌", "▋", "▊", "▉"};
    auto value = std::min(1.0f, std::max(0.0f, progress / 100.0f));
    auto whole_width = std::floor(value * bar_width);
    auto remainder_width = fmod((value * bar_width), 1.0f);
    auto part_width = std::floor(remainder_width * lead_characters.size());
    std::string lead_text = lead_characters[size_t(part_width)];
    if ((bar_width - whole_width - 1) < 0)
      lead_text = "";
    for (size_t i = 0; i < whole_width; ++i)
      os << fill_text;
    os << lead_text;
    for (size_t i = 0; i < (bar_width - whole_width - 1); ++i)
      os << " ";
    return os;
  }

private:
  std::ostream &os;
  size_t bar_width = 0;
};

class ProgressScaleWriter {
public:
  ProgressScaleWriter(std::ostream &os, size_t bar_width, const std::string &fill,
                      const std::string &lead, const std::string &remainder)
      : os(os), bar_width(bar_width), fill(fill), lead(lead), remainder(remainder) {}

  std::ostream &write(float progress) {
    auto pos = static_cast<size_t>(progress * bar_width / 100.0);
    for (size_t i = 0, current_display_width = 0; i < bar_width;) {
      std::string next;

      if (i < pos) {
        next = fill;
        current_display_width = unicode::display_width(fill);
      } else if (i == pos) {
        next = lead;
        current_display_width = unicode::display_width(lead);
      } else {
        next = remainder;
        current_display_width = unicode::display_width(remainder);
      }

      i += current_display_width;

      if (i > bar_width) {
        // `next` is larger than the allowed bar width
        // fill with empty space instead
        os << std::string((bar_width - (i - current_display_width)), ' ');
        break;
      }

      os << next;
    }
    return os;
  }

private:
  std::ostream &os;
  size_t bar_width = 0;
  std::string fill;
  std::string lead;
  std::string remainder;
};

class IndeterminateProgressScaleWriter {
public:
  IndeterminateProgressScaleWriter(std::ostream &os, size_t bar_width, const std::string &fill,
                                   const std::string &lead)
      : os(os), bar_width(bar_width), fill(fill), lead(lead) {}

  std::ostream &write(size_t progress) {
    for (size_t i = 0; i < bar_width;) {
      std::string next;
      size_t current_display_width = 0;

      if (i < progress) {
        next = fill;
        current_display_width = unicode::display_width(fill);
      } else if (i == progress) {
        next = lead;
        current_display_width = unicode::display_width(lead);
      } else {
        next = fill;
        current_display_width = unicode::display_width(fill);
      }

      i += current_display_width;

      if (i > bar_width) {
        // `next` is larger than the allowed bar width
        // fill with empty space instead
        os << std::string((bar_width - (i - current_display_width)), ' ');
        break;
      }

      os << next;
    }
    return os;
  }

private:
  std::ostream &os;
  size_t bar_width = 0;
  std::string fill;
  std::string lead;
};

} // namespace details
} // namespace indicators
#pragma once

// #include <indicators/details/stream_helper.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
// #include <indicators/color.hpp>
// #include <indicators/setting.hpp>
// #include <indicators/terminal_size.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

namespace indicators {

class ProgressBar {
  using Settings =
      std::tuple<option::BarWidth, option::PrefixText, option::PostfixText,
                 option::Start, option::End, option::Fill, option::Lead,
                 option::Remainder, option::MaxPostfixTextLen,
                 option::Completed, option::ShowPercentage,
                 option::ShowElapsedTime, option::ShowRemainingTime,
                 option::SavedStartTime, option::ForegroundColor,
                 option::FontStyles, option::MinProgress, option::MaxProgress,
                 option::ProgressType, option::Stream>;

public:
  template <typename... Args,
            typename std::enable_if<
                details::are_settings_from_tuple<
                    Settings, typename std::decay<Args>::type...>::value,
                void *>::type = nullptr>
  explicit ProgressBar(Args &&... args)
      : settings_(
            details::get<details::ProgressBarOption::bar_width>(
                option::BarWidth{100}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::prefix_text>(
                option::PrefixText{}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::postfix_text>(
                option::PostfixText{}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::start>(
                option::Start{"["}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::end>(
                option::End{"]"}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::fill>(
                option::Fill{"="}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::lead>(
                option::Lead{">"}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::remainder>(
                option::Remainder{" "}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::max_postfix_text_len>(
                option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::completed>(
                option::Completed{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::show_percentage>(
                option::ShowPercentage{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::show_elapsed_time>(
                option::ShowElapsedTime{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::show_remaining_time>(
                option::ShowRemainingTime{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::saved_start_time>(
                option::SavedStartTime{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::foreground_color>(
                option::ForegroundColor{Color::unspecified},
                std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::font_styles>(
                option::FontStyles{std::vector<FontStyle>{}},
                std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::min_progress>(
                option::MinProgress{0}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::max_progress>(
                option::MaxProgress{100}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::progress_type>(
                option::ProgressType{ProgressType::incremental},
                std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::stream>(
                option::Stream{std::cout}, std::forward<Args>(args)...)) {

    // if progress is incremental, start from min_progress
    // else start from max_progress
    const auto type = get_value<details::ProgressBarOption::progress_type>();
    if (type == ProgressType::incremental)
      progress_ = get_value<details::ProgressBarOption::min_progress>();
    else
      progress_ = get_value<details::ProgressBarOption::max_progress>();
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(
        !std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                             std::declval<Settings>()))>::type>::value,
        "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(
        !std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                             std::declval<Settings>()))>::type>::value,
        "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

  void
  set_option(const details::Setting<
             std::string, details::ProgressBarOption::postfix_text> &setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = setting.value;
    if (setting.value.length() >
        get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() =
          setting.value.length();
    }
  }

  void set_option(
      details::Setting<std::string, details::ProgressBarOption::postfix_text>
          &&setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() =
        std::move(setting).value;
    auto &new_value = get_value<details::ProgressBarOption::postfix_text>();
    if (new_value.length() >
        get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() =
          new_value.length();
    }
  }

  void set_progress(size_t new_progress) {
    {
      std::lock_guard<std::mutex> lock(mutex_);
      progress_ = new_progress;
    }

    save_start_time();
    print_progress();
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      const auto type = get_value<details::ProgressBarOption::progress_type>();
      if (type == ProgressType::incremental)
        progress_ += 1;
      else
        progress_ -= 1;
    }
    save_start_time();
    print_progress();
  }

  size_t current() {
    std::lock_guard<std::mutex> lock{mutex_};
    return std::min(
        progress_,
        size_t(get_value<details::ProgressBarOption::max_progress>()));
  }

  bool is_completed() const {
    return get_value<details::ProgressBarOption::completed>();
  }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  template <details::ProgressBarOption id>
  auto get_value()
      -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const -> decltype(
      (details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  size_t progress_{0};
  Settings settings_;
  std::chrono::nanoseconds elapsed_;
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
  std::mutex mutex_;

  template <typename Indicator, size_t count> friend class MultiProgress;
  template <typename Indicator> friend class DynamicProgress;
  std::atomic<bool> multi_progress_mode_{false};

  void save_start_time() {
    auto &show_elapsed_time =
        get_value<details::ProgressBarOption::show_elapsed_time>();
    auto &saved_start_time =
        get_value<details::ProgressBarOption::saved_start_time>();
    auto &show_remaining_time =
        get_value<details::ProgressBarOption::show_remaining_time>();
    if ((show_elapsed_time || show_remaining_time) && !saved_start_time) {
      start_time_point_ = std::chrono::high_resolution_clock::now();
      saved_start_time = true;
    }
  }

  std::pair<std::string, size_t> get_prefix_text() {
    std::stringstream os;
    os << get_value<details::ProgressBarOption::prefix_text>();
    const auto result = os.str();
    const auto result_size = unicode::display_width(result);
    return {result, result_size};
  }

  std::pair<std::string, size_t> get_postfix_text() {
    std::stringstream os;
    const auto max_progress =
        get_value<details::ProgressBarOption::max_progress>();

    if (get_value<details::ProgressBarOption::show_percentage>()) {
      os << " "
         << std::min(static_cast<size_t>(static_cast<float>(progress_) /
                                         max_progress * 100),
                     size_t(100))
         << "%";
    }

    auto &saved_start_time =
        get_value<details::ProgressBarOption::saved_start_time>();

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      os << " [";
      if (saved_start_time)
        details::write_duration(os, elapsed_);
      else
        os << "00:00s";
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "<";
      else
        os << " [";

      if (saved_start_time) {
        auto eta = std::chrono::nanoseconds(
            progress_ > 0 ? static_cast<long long>(elapsed_.count() *
                                                   max_progress / progress_)
                          : 0);
        auto remaining = eta > elapsed_ ? (eta - elapsed_) : (elapsed_ - eta);
        details::write_duration(os, remaining);
      } else {
        os << "00:00s";
      }

      os << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "]";
    }

    os << " " << get_value<details::ProgressBarOption::postfix_text>();

    const auto result = os.str();
    const auto result_size = unicode::display_width(result);
    return {result, result_size};
  }

public:
  void print_progress(bool from_multi_progress = false) {
    std::lock_guard<std::mutex> lock{mutex_};

    auto &os = get_value<details::ProgressBarOption::stream>();

    const auto type = get_value<details::ProgressBarOption::progress_type>();
    const auto min_progress =
        get_value<details::ProgressBarOption::min_progress>();
    const auto max_progress =
        get_value<details::ProgressBarOption::max_progress>();
    if (multi_progress_mode_ && !from_multi_progress) {
      if ((type == ProgressType::incremental && progress_ >= max_progress) ||
          (type == ProgressType::decremental && progress_ <= min_progress)) {
        get_value<details::ProgressBarOption::completed>() = true;
      }
      return;
    }
    auto now = std::chrono::high_resolution_clock::now();
    if (!get_value<details::ProgressBarOption::completed>())
      elapsed_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
          now - start_time_point_);

    if (get_value<details::ProgressBarOption::foreground_color>() !=
        Color::unspecified)
      details::set_stream_color(
          os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);

    const auto prefix_pair = get_prefix_text();
    const auto prefix_text = prefix_pair.first;
    const auto prefix_length = prefix_pair.second;
    os << prefix_text;

    os << get_value<details::ProgressBarOption::start>();

    details::ProgressScaleWriter writer{
        os, get_value<details::ProgressBarOption::bar_width>(),
        get_value<details::ProgressBarOption::fill>(),
        get_value<details::ProgressBarOption::lead>(),
        get_value<details::ProgressBarOption::remainder>()};
    writer.write(double(progress_) / double(max_progress) * 100.0f);

    os << get_value<details::ProgressBarOption::end>();

    const auto postfix_pair = get_postfix_text();
    const auto postfix_text = postfix_pair.first;
    const auto postfix_length = postfix_pair.second;
    os << postfix_text;

    // Get length of prefix text and postfix text
    const auto start_length = get_value<details::ProgressBarOption::start>().size();
    const auto bar_width = get_value<details::ProgressBarOption::bar_width>();
    const auto end_length = get_value<details::ProgressBarOption::end>().size();
    const auto terminal_width = terminal_size().second;
    // prefix + bar_width + postfix should be <= terminal_width
    const int remaining = terminal_width - (prefix_length + start_length + bar_width + end_length + postfix_length);
    if (remaining > 0) {
      os << std::string(remaining, ' ') << "\r";
    } else if (remaining < 0) {
      // Do nothing. Maybe in the future truncate postfix with ...
    }
    os.flush();

    if ((type == ProgressType::incremental && progress_ >= max_progress) ||
        (type == ProgressType::decremental && progress_ <= min_progress)) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators

#pragma once

// #include <indicators/color.hpp>
// #include <indicators/details/stream_helper.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
// #include <indicators/setting.hpp>
// #include <indicators/terminal_size.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <utility>

namespace indicators {

class BlockProgressBar {
  using Settings = std::tuple<option::ForegroundColor, option::BarWidth, option::Start, option::End,
                              option::PrefixText, option::PostfixText, option::ShowPercentage,
                              option::ShowElapsedTime, option::ShowRemainingTime, option::Completed,
                              option::SavedStartTime, option::MaxPostfixTextLen, option::FontStyles,
                              option::MaxProgress, option::Stream>;

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit BlockProgressBar(Args &&... args)
      : settings_(details::get<details::ProgressBarOption::foreground_color>(
                      option::ForegroundColor{Color::unspecified}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::bar_width>(option::BarWidth{100},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::start>(option::Start{"["},
                                                                  std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::end>(option::End{"]"},
                                                                std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::prefix_text>(
                      option::PrefixText{""}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::postfix_text>(
                      option::PostfixText{""}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_percentage>(
                      option::ShowPercentage{true}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_elapsed_time>(
                      option::ShowElapsedTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::show_remaining_time>(
                      option::ShowRemainingTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::completed>(option::Completed{false},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::saved_start_time>(
                      option::SavedStartTime{false}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_postfix_text_len>(
                      option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::font_styles>(
                      option::FontStyles{std::vector<FontStyle>{}}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_progress>(
                      option::MaxProgress{100}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::stream>(option::Stream{std::cout},
                                                                   std::forward<Args>(args)...)) {}

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

  void set_option(
      const details::Setting<std::string, details::ProgressBarOption::postfix_text> &setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = setting.value;
    if (setting.value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = setting.value.length();
    }
  }

  void
  set_option(details::Setting<std::string, details::ProgressBarOption::postfix_text> &&setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = std::move(setting).value;
    auto &new_value = get_value<details::ProgressBarOption::postfix_text>();
    if (new_value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = new_value.length();
    }
  }

  void set_progress(float value) {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ = value;
    }
    save_start_time();
    print_progress();
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ += 1;
    }
    save_start_time();
    print_progress();
  }

  size_t current() {
    std::lock_guard<std::mutex> lock{mutex_};
    return std::min(static_cast<size_t>(progress_),
                    size_t(get_value<details::ProgressBarOption::max_progress>()));
  }

  bool is_completed() const { return get_value<details::ProgressBarOption::completed>(); }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  template <details::ProgressBarOption id>
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  Settings settings_;
  float progress_{0.0};
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
  std::mutex mutex_;

  template <typename Indicator, size_t count> friend class MultiProgress;
  template <typename Indicator> friend class DynamicProgress;
  std::atomic<bool> multi_progress_mode_{false};

  void save_start_time() {
    auto &show_elapsed_time = get_value<details::ProgressBarOption::show_elapsed_time>();
    auto &saved_start_time = get_value<details::ProgressBarOption::saved_start_time>();
    auto &show_remaining_time = get_value<details::ProgressBarOption::show_remaining_time>();
    if ((show_elapsed_time || show_remaining_time) && !saved_start_time) {
      start_time_point_ = std::chrono::high_resolution_clock::now();
      saved_start_time = true;
    }
  }

  std::pair<std::string, size_t> get_prefix_text() {
    std::stringstream os;
    os << get_value<details::ProgressBarOption::prefix_text>();
    const auto result = os.str();
    const auto result_size = unicode::display_width(result);
    return {result, result_size};
  }

  std::pair<std::string, size_t> get_postfix_text() {
    std::stringstream os;
    const auto max_progress = get_value<details::ProgressBarOption::max_progress>();
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time_point_);

    if (get_value<details::ProgressBarOption::show_percentage>()) {
      os << " " << std::min(static_cast<size_t>(progress_ / max_progress * 100.0), size_t(100))
         << "%";
    }

    auto &saved_start_time = get_value<details::ProgressBarOption::saved_start_time>();

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      os << " [";
      if (saved_start_time)
        details::write_duration(os, elapsed);
      else
        os << "00:00s";
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "<";
      else
        os << " [";

      if (saved_start_time) {
        auto eta = std::chrono::nanoseconds(
            progress_ > 0 ? static_cast<long long>(elapsed.count() * max_progress / progress_) : 0);
        auto remaining = eta > elapsed ? (eta - elapsed) : (elapsed - eta);
        details::write_duration(os, remaining);
      } else {
        os << "00:00s";
      }

      os << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "]";
    }

    os << " " << get_value<details::ProgressBarOption::postfix_text>();

    const auto result = os.str();
    const auto result_size = unicode::display_width(result);
    return {result, result_size};
  }

public:
  void print_progress(bool from_multi_progress = false) {
    std::lock_guard<std::mutex> lock{mutex_};

    auto &os = get_value<details::ProgressBarOption::stream>();

    const auto max_progress = get_value<details::ProgressBarOption::max_progress>();
    if (multi_progress_mode_ && !from_multi_progress) {
      if (progress_ > max_progress) {
        get_value<details::ProgressBarOption::completed>() = true;
      }
      return;
    }

    if (get_value<details::ProgressBarOption::foreground_color>() != Color::unspecified)
      details::set_stream_color(os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);

    const auto prefix_pair = get_prefix_text();
    const auto prefix_text = prefix_pair.first;
    const auto prefix_length = prefix_pair.second;
    os << prefix_text;

    os << get_value<details::ProgressBarOption::start>();

    details::BlockProgressScaleWriter writer{os,
                                             get_value<details::ProgressBarOption::bar_width>()};
    writer.write(progress_ / max_progress * 100);

    os << get_value<details::ProgressBarOption::end>();

    const auto postfix_pair = get_postfix_text();
    const auto postfix_text = postfix_pair.first;
    const auto postfix_length = postfix_pair.second;
    os << postfix_text;

    // Get length of prefix text and postfix text
    const auto start_length = get_value<details::ProgressBarOption::start>().size();
    const auto bar_width = get_value<details::ProgressBarOption::bar_width>();
    const auto end_length = get_value<details::ProgressBarOption::end>().size();
    const auto terminal_width = terminal_size().second;
    // prefix + bar_width + postfix should be <= terminal_width
    const int remaining = terminal_width - (prefix_length + start_length + bar_width + end_length + postfix_length);
    if (remaining > 0) {
      os << std::string(remaining, ' ') << "\r";
    } else if (remaining < 0) {
      // Do nothing. Maybe in the future truncate postfix with ...
    }
    os.flush();

    if (progress_ > max_progress) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators

#pragma once

// #include <indicators/details/stream_helper.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
// #include <indicators/color.hpp>
// #include <indicators/setting.hpp>
// #include <indicators/terminal_size.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <sstream>
#include <utility>

namespace indicators {

class IndeterminateProgressBar {
  using Settings =
      std::tuple<option::BarWidth, option::PrefixText, option::PostfixText, option::Start,
                 option::End, option::Fill, option::Lead, option::MaxPostfixTextLen,
                 option::Completed, option::ForegroundColor, option::FontStyles, option::Stream>;

  enum class Direction { forward, backward };

  Direction direction_{Direction::forward};

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit IndeterminateProgressBar(Args &&... args)
      : settings_(details::get<details::ProgressBarOption::bar_width>(option::BarWidth{100},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::prefix_text>(
                      option::PrefixText{}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::postfix_text>(
                      option::PostfixText{}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::start>(option::Start{"["},
                                                                  std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::end>(option::End{"]"},
                                                                std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::fill>(option::Fill{"."},
                                                                 std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::lead>(option::Lead{"<==>"},
                                                                 std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::max_postfix_text_len>(
                      option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::completed>(option::Completed{false},
                                                                      std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::foreground_color>(
                      option::ForegroundColor{Color::unspecified}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::font_styles>(
                      option::FontStyles{std::vector<FontStyle>{}}, std::forward<Args>(args)...),
                  details::get<details::ProgressBarOption::stream>(option::Stream{std::cout},
                                                                   std::forward<Args>(args)...)) {
    // starts with [<==>...........]
    // progress_ = 0

    // ends with   [...........<==>]
    //             ^^^^^^^^^^^^^^^^^ bar_width
    //             ^^^^^^^^^^^^ (bar_width - len(lead))
    // progress_ = bar_width - len(lead)
    progress_ = 0;
    max_progress_ = get_value<details::ProgressBarOption::bar_width>() -
                    get_value<details::ProgressBarOption::lead>().size() +
                    get_value<details::ProgressBarOption::start>().size() +
                    get_value<details::ProgressBarOption::end>().size();
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

  void set_option(
      const details::Setting<std::string, details::ProgressBarOption::postfix_text> &setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = setting.value;
    if (setting.value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = setting.value.length();
    }
  }

  void
  set_option(details::Setting<std::string, details::ProgressBarOption::postfix_text> &&setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = std::move(setting).value;
    auto &new_value = get_value<details::ProgressBarOption::postfix_text>();
    if (new_value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = new_value.length();
    }
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      if (get_value<details::ProgressBarOption::completed>())
        return;

      progress_ += (direction_ == Direction::forward) ? 1 : -1;
      if (direction_ == Direction::forward && progress_ == max_progress_) {
        // time to go back
        direction_ = Direction::backward;
      } else if (direction_ == Direction::backward && progress_ == 0) {
        direction_ = Direction::forward;
      }
    }
    print_progress();
  }

  bool is_completed() { return get_value<details::ProgressBarOption::completed>(); }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  template <details::ProgressBarOption id>
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  size_t progress_{0};
  size_t max_progress_;
  Settings settings_;
  std::chrono::nanoseconds elapsed_;
  std::mutex mutex_;

  template <typename Indicator, size_t count> friend class MultiProgress;
  template <typename Indicator> friend class DynamicProgress;
  std::atomic<bool> multi_progress_mode_{false};

  std::pair<std::string, size_t> get_prefix_text() {
    std::stringstream os;
    os << get_value<details::ProgressBarOption::prefix_text>();
    const auto result = os.str();
    const auto result_size = unicode::display_width(result);
    return {result, result_size};
  }

  std::pair<std::string, size_t> get_postfix_text() {
    std::stringstream os;
    os << " " << get_value<details::ProgressBarOption::postfix_text>();

    const auto result = os.str();
    const auto result_size = unicode::display_width(result);
    return {result, result_size};
  }

public:
  void print_progress(bool from_multi_progress = false) {
    std::lock_guard<std::mutex> lock{mutex_};

    auto &os = get_value<details::ProgressBarOption::stream>();

    if (multi_progress_mode_ && !from_multi_progress) {
      return;
    }
    if (get_value<details::ProgressBarOption::foreground_color>() != Color::unspecified)
      details::set_stream_color(os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);

    const auto prefix_pair = get_prefix_text();
    const auto prefix_text = prefix_pair.first;
    const auto prefix_length = prefix_pair.second;
    os << prefix_text;

    os << get_value<details::ProgressBarOption::start>();

    details::IndeterminateProgressScaleWriter writer{
        os, get_value<details::ProgressBarOption::bar_width>(),
        get_value<details::ProgressBarOption::fill>(),
        get_value<details::ProgressBarOption::lead>()};
    writer.write(progress_);

    os << get_value<details::ProgressBarOption::end>();

    const auto postfix_pair = get_postfix_text();
    const auto postfix_text = postfix_pair.first;
    const auto postfix_length = postfix_pair.second;
    os << postfix_text;

    // Get length of prefix text and postfix text
    const auto start_length = get_value<details::ProgressBarOption::start>().size();
    const auto bar_width = get_value<details::ProgressBarOption::bar_width>();
    const auto end_length = get_value<details::ProgressBarOption::end>().size();
    const auto terminal_width = terminal_size().second;
    // prefix + bar_width + postfix should be <= terminal_width
    const int remaining = terminal_width - (prefix_length + start_length + bar_width + end_length + postfix_length);
    if (remaining > 0) {
      os << std::string(remaining, ' ') << "\r";
    } else if (remaining < 0) {
      // Do nothing. Maybe in the future truncate postfix with ...
    }
    os.flush();

    if (get_value<details::ProgressBarOption::completed>() &&
        !from_multi_progress) // Don't std::endl if calling from MultiProgress
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators

#pragma once
#include <atomic>
#include <functional>
#include <iostream>
#include <mutex>
#include <vector>

// #include <indicators/color.hpp>
// #include <indicators/cursor_movement.hpp>
// #include <indicators/details/stream_helper.hpp>

namespace indicators {

template <typename Indicator, size_t count> class MultiProgress {
public:
  template <typename... Indicators,
            typename = typename std::enable_if<(sizeof...(Indicators) == count)>::type>
  explicit MultiProgress(Indicators &... bars) {
    bars_ = {bars...};
    for (auto &bar : bars_) {
      bar.get().multi_progress_mode_ = true;
    }
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), void>::type set_progress(size_t value) {
    if (!bars_[index].get().is_completed())
      bars_[index].get().set_progress(value);
    print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), void>::type set_progress(float value) {
    if (!bars_[index].get().is_completed())
      bars_[index].get().set_progress(value);
    print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), void>::type tick() {
    if (!bars_[index].get().is_completed())
      bars_[index].get().tick();
    print_progress();
  }

  template <size_t index>
  typename std::enable_if<(index >= 0 && index < count), bool>::type is_completed() const {
    return bars_[index].get().is_completed();
  }

private:
  std::atomic<bool> started_{false};
  std::mutex mutex_;
  std::vector<std::reference_wrapper<Indicator>> bars_;

  bool _all_completed() {
    bool result{true};
    for (size_t i = 0; i < count; ++i)
      result &= bars_[i].get().is_completed();
    return result;
  }

public:
  void print_progress() {
    std::lock_guard<std::mutex> lock{mutex_};
    if (started_)
      move_up(count);
    for (auto &bar : bars_) {
      bar.get().print_progress(true);
      std::cout << "\n";
    }
    std::cout << termcolor::reset;
    if (!started_)
      started_ = true;
  }
};

} // namespace indicators

#pragma once
#include <atomic>
#include <functional>
// #include <indicators/color.hpp>
// #include <indicators/setting.hpp>
// #include <indicators/details/stream_helper.hpp>
#include <iostream>
#include <mutex>
#include <vector>

namespace indicators {

template <typename Indicator> class DynamicProgress {
  using Settings = std::tuple<option::HideBarWhenComplete>;

public:
  template <typename... Indicators> explicit DynamicProgress(Indicators &... bars) {
    bars_ = {bars...};
    for (auto &bar : bars_) {
      bar.get().multi_progress_mode_ = true;
      ++total_count_;
      ++incomplete_count_;
    }
  }

  Indicator &operator[](size_t index) {
    print_progress();
    std::lock_guard<std::mutex> lock{mutex_};
    return bars_[index].get();
  }

  size_t push_back(Indicator &bar) {
    std::lock_guard<std::mutex> lock{mutex_};
    bar.multi_progress_mode_ = true;
    bars_.push_back(bar);
    return bars_.size() - 1;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

private:
  Settings settings_;
  std::atomic<bool> started_{false};
  std::mutex mutex_;
  std::vector<std::reference_wrapper<Indicator>> bars_;
  std::atomic<size_t> total_count_{0};
  std::atomic<size_t> incomplete_count_{0};

  template <details::ProgressBarOption id>
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

public:
  void print_progress() {
    std::lock_guard<std::mutex> lock{mutex_};
    auto &hide_bar_when_complete = get_value<details::ProgressBarOption::hide_bar_when_complete>();
    if (hide_bar_when_complete) {
      // Hide completed bars
      if (started_) {
        for (size_t i = 0; i < incomplete_count_; ++i)
          std::cout << "\033[A\r\033[K" << std::flush;
      }
      incomplete_count_ = 0;
      for (auto &bar : bars_) {
        if (!bar.get().is_completed()) {
          bar.get().print_progress(true);
          std::cout << "\n";
          ++incomplete_count_;
        }
      }
      if (!started_)
        started_ = true;
    } else {
      // Don't hide any bars
      if (started_) {
        for (size_t i = 0; i < total_count_; ++i)
          std::cout << "\x1b[A";
      }
      for (auto &bar : bars_) {
        bar.get().print_progress(true);
        std::cout << "\n";
      }
      if (!started_)
        started_ = true;
    }
    total_count_ = bars_.size();
    std::cout << termcolor::reset;
  }
};

} // namespace indicators

#pragma once

// #include <indicators/details/stream_helper.hpp>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
// #include <indicators/color.hpp>
// #include <indicators/setting.hpp>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

namespace indicators {

class ProgressSpinner {
  using Settings =
      std::tuple<option::ForegroundColor, option::PrefixText, option::PostfixText,
                 option::ShowPercentage, option::ShowElapsedTime, option::ShowRemainingTime,
                 option::ShowSpinner, option::SavedStartTime, option::Completed,
                 option::MaxPostfixTextLen, option::SpinnerStates, option::FontStyles,
                 option::MaxProgress, option::Stream>;

public:
  template <typename... Args,
            typename std::enable_if<details::are_settings_from_tuple<
                                        Settings, typename std::decay<Args>::type...>::value,
                                    void *>::type = nullptr>
  explicit ProgressSpinner(Args &&... args)
      : settings_(
            details::get<details::ProgressBarOption::foreground_color>(
                option::ForegroundColor{Color::unspecified}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::prefix_text>(option::PrefixText{},
                                                                  std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::postfix_text>(option::PostfixText{},
                                                                   std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::show_percentage>(option::ShowPercentage{true},
                                                                      std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::show_elapsed_time>(
                option::ShowElapsedTime{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::show_remaining_time>(
                option::ShowRemainingTime{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::spinner_show>(option::ShowSpinner{true},
                                                                   std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::saved_start_time>(
                option::SavedStartTime{false}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::completed>(option::Completed{false},
                                                                std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::max_postfix_text_len>(
                option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::spinner_states>(
                option::SpinnerStates{
                    std::vector<std::string>{"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"}},
                std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::font_styles>(
                option::FontStyles{std::vector<FontStyle>{}}, std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::max_progress>(option::MaxProgress{100},
                                                                   std::forward<Args>(args)...),
            details::get<details::ProgressBarOption::stream>(option::Stream{std::cout},
                                                             std::forward<Args>(args)...)) {}

  template <typename T, details::ProgressBarOption id>
  void set_option(details::Setting<T, id> &&setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = std::move(setting).value;
  }

  template <typename T, details::ProgressBarOption id>
  void set_option(const details::Setting<T, id> &setting) {
    static_assert(!std::is_same<T, typename std::decay<decltype(details::get_value<id>(
                                       std::declval<Settings>()))>::type>::value,
                  "Setting has wrong type!");
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<id>() = setting.value;
  }

  void set_option(
      const details::Setting<std::string, details::ProgressBarOption::postfix_text> &setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = setting.value;
    if (setting.value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = setting.value.length();
    }
  }

  void
  set_option(details::Setting<std::string, details::ProgressBarOption::postfix_text> &&setting) {
    std::lock_guard<std::mutex> lock(mutex_);
    get_value<details::ProgressBarOption::postfix_text>() = std::move(setting).value;
    auto &new_value = get_value<details::ProgressBarOption::postfix_text>();
    if (new_value.length() > get_value<details::ProgressBarOption::max_postfix_text_len>()) {
      get_value<details::ProgressBarOption::max_postfix_text_len>() = new_value.length();
    }
  }

  void set_progress(size_t value) {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ = value;
    }
    save_start_time();
    print_progress();
  }

  void tick() {
    {
      std::lock_guard<std::mutex> lock{mutex_};
      progress_ += 1;
    }
    save_start_time();
    print_progress();
  }

  size_t current() {
    std::lock_guard<std::mutex> lock{mutex_};
    return std::min(progress_, size_t(get_value<details::ProgressBarOption::max_progress>()));
  }

  bool is_completed() const { return get_value<details::ProgressBarOption::completed>(); }

  void mark_as_completed() {
    get_value<details::ProgressBarOption::completed>() = true;
    print_progress();
  }

private:
  Settings settings_;
  size_t progress_{0};
  size_t index_{0};
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
  std::mutex mutex_;

  template <details::ProgressBarOption id>
  auto get_value() -> decltype((details::get_value<id>(std::declval<Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  template <details::ProgressBarOption id>
  auto get_value() const
      -> decltype((details::get_value<id>(std::declval<const Settings &>()).value)) {
    return details::get_value<id>(settings_).value;
  }

  void save_start_time() {
    auto &show_elapsed_time = get_value<details::ProgressBarOption::show_elapsed_time>();
    auto &show_remaining_time = get_value<details::ProgressBarOption::show_remaining_time>();
    auto &saved_start_time = get_value<details::ProgressBarOption::saved_start_time>();
    if ((show_elapsed_time || show_remaining_time) && !saved_start_time) {
      start_time_point_ = std::chrono::high_resolution_clock::now();
      saved_start_time = true;
    }
  }

public:
  void print_progress() {
    std::lock_guard<std::mutex> lock{mutex_};

    auto &os = get_value<details::ProgressBarOption::stream>();

    const auto max_progress = get_value<details::ProgressBarOption::max_progress>();
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time_point_);

    if (get_value<details::ProgressBarOption::foreground_color>() != Color::unspecified)
      details::set_stream_color(os, get_value<details::ProgressBarOption::foreground_color>());

    for (auto &style : get_value<details::ProgressBarOption::font_styles>())
      details::set_font_style(os, style);

    os << get_value<details::ProgressBarOption::prefix_text>();
    if (get_value<details::ProgressBarOption::spinner_show>())
      os << get_value<details::ProgressBarOption::spinner_states>()
              [index_ % get_value<details::ProgressBarOption::spinner_states>().size()];
    if (get_value<details::ProgressBarOption::show_percentage>()) {
      os << " " << 
        std::size_t(progress_ / double(max_progress) * 100)
        << "%";
    }

    if (get_value<details::ProgressBarOption::show_elapsed_time>()) {
      os << " [";
      details::write_duration(os, elapsed);
    }

    if (get_value<details::ProgressBarOption::show_remaining_time>()) {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "<";
      else
        os << " [";
      auto eta = std::chrono::nanoseconds(
          progress_ > 0 ? static_cast<long long>(elapsed.count() * max_progress / progress_) : 0);
      auto remaining = eta > elapsed ? (eta - elapsed) : (elapsed - eta);
      details::write_duration(os, remaining);
      os << "]";
    } else {
      if (get_value<details::ProgressBarOption::show_elapsed_time>())
        os << "]";
    }

    if (get_value<details::ProgressBarOption::max_postfix_text_len>() == 0)
      get_value<details::ProgressBarOption::max_postfix_text_len>() = 10;
    os << " " << get_value<details::ProgressBarOption::postfix_text>()
       << std::string(get_value<details::ProgressBarOption::max_postfix_text_len>(), ' ') << "\r";
    os.flush();
    index_ += 1;
    if (progress_ > max_progress) {
      get_value<details::ProgressBarOption::completed>() = true;
    }
    if (get_value<details::ProgressBarOption::completed>())
      os << termcolor::reset << std::endl;
  }
};

} // namespace indicators

/* Copyright 2017 https://github.com/mandreyel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MIO_MMAP_HEADER
#define MIO_MMAP_HEADER

// #include "mio/page.hpp"
/* Copyright 2017 https://github.com/mandreyel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MIO_PAGE_HEADER
#define MIO_PAGE_HEADER

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace mio {

/**
 * This is used by `basic_mmap` to determine whether to create a read-only or
 * a read-write memory mapping.
 */
enum class access_mode { read, write };

/**
 * Determines the operating system's page allocation granularity.
 *
 * On the first call to this function, it invokes the operating system specific syscall
 * to determine the page size, caches the value, and returns it. Any subsequent call to
 * this function serves the cached value, so no further syscalls are made.
 */
inline size_t page_size() {
  static const size_t page_size = [] {
#ifdef _WIN32
    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    return SystemInfo.dwAllocationGranularity;
#else
    return sysconf(_SC_PAGE_SIZE);
#endif
  }();
  return page_size;
}

/**
 * Alligns `offset` to the operating's system page size such that it subtracts the
 * difference until the nearest page boundary before `offset`, or does nothing if
 * `offset` is already page aligned.
 */
inline size_t make_offset_page_aligned(size_t offset) noexcept {
  const size_t page_size_ = page_size();
  // Use integer division to round down to the nearest page alignment.
  return offset / page_size_ * page_size_;
}

} // namespace mio

#endif // MIO_PAGE_HEADER

#include <cstdint>
#include <iterator>
#include <string>
#include <system_error>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#include <windows.h>
#else // ifdef _WIN32
#define INVALID_HANDLE_VALUE -1
#endif // ifdef _WIN32

namespace mio {

// This value may be provided as the `length` parameter to the constructor or
// `map`, in which case a memory mapping of the entire file is created.
enum { map_entire_file = 0 };

#ifdef _WIN32
using file_handle_type = HANDLE;
#else
using file_handle_type = int;
#endif

// This value represents an invalid file handle type. This can be used to
// determine whether `basic_mmap::file_handle` is valid, for example.
const static file_handle_type invalid_handle = INVALID_HANDLE_VALUE;

template <access_mode AccessMode, typename ByteT> struct basic_mmap {
  using value_type = ByteT;
  using size_type = size_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = value_type *;
  using const_pointer = const value_type *;
  using difference_type = std::ptrdiff_t;
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  using iterator_category = std::random_access_iterator_tag;
  using handle_type = file_handle_type;

  static_assert(sizeof(ByteT) == sizeof(char), "ByteT must be the same size as char.");

private:
  // Points to the first requested byte, and not to the actual start of the mapping.
  pointer data_ = nullptr;

  // Length--in bytes--requested by user (which may not be the length of the
  // full mapping) and the length of the full mapping.
  size_type length_ = 0;
  size_type mapped_length_ = 0;

  // Letting user map a file using both an existing file handle and a path
  // introcudes some complexity (see `is_handle_internal_`).
  // On POSIX, we only need a file handle to create a mapping, while on
  // Windows systems the file handle is necessary to retrieve a file mapping
  // handle, but any subsequent operations on the mapped region must be done
  // through the latter.
  handle_type file_handle_ = INVALID_HANDLE_VALUE;
#ifdef _WIN32
  handle_type file_mapping_handle_ = INVALID_HANDLE_VALUE;
#endif

  // Letting user map a file using both an existing file handle and a path
  // introcudes some complexity in that we must not close the file handle if
  // user provided it, but we must close it if we obtained it using the
  // provided path. For this reason, this flag is used to determine when to
  // close `file_handle_`.
  bool is_handle_internal_;

public:
  /**
   * The default constructed mmap object is in a non-mapped state, that is,
   * any operation that attempts to access nonexistent underlying data will
   * result in undefined behaviour/segmentation faults.
   */
  basic_mmap() = default;

#ifdef __cpp_exceptions
  /**
   * The same as invoking the `map` function, except any error that may occur
   * while establishing the mapping is wrapped in a `std::system_error` and is
   * thrown.
   */
  template <typename String>
  basic_mmap(const String &path, const size_type offset = 0,
             const size_type length = map_entire_file) {
    std::error_code error;
    map(path, offset, length, error);
    if (error) {
      throw std::system_error(error);
    }
  }

  /**
   * The same as invoking the `map` function, except any error that may occur
   * while establishing the mapping is wrapped in a `std::system_error` and is
   * thrown.
   */
  basic_mmap(const handle_type handle, const size_type offset = 0,
             const size_type length = map_entire_file) {
    std::error_code error;
    map(handle, offset, length, error);
    if (error) {
      throw std::system_error(error);
    }
  }
#endif // __cpp_exceptions

  /**
   * `basic_mmap` has single-ownership semantics, so transferring ownership
   * may only be accomplished by moving the object.
   */
  basic_mmap(const basic_mmap &) = delete;
  basic_mmap(basic_mmap &&);
  basic_mmap &operator=(const basic_mmap &) = delete;
  basic_mmap &operator=(basic_mmap &&);

  /**
   * If this is a read-write mapping, the destructor invokes sync. Regardless
   * of the access mode, unmap is invoked as a final step.
   */
  ~basic_mmap();

  /**
   * On UNIX systems 'file_handle' and 'mapping_handle' are the same. On Windows,
   * however, a mapped region of a file gets its own handle, which is returned by
   * 'mapping_handle'.
   */
  handle_type file_handle() const noexcept { return file_handle_; }
  handle_type mapping_handle() const noexcept;

  /** Returns whether a valid memory mapping has been created. */
  bool is_open() const noexcept { return file_handle_ != invalid_handle; }

  /**
   * Returns true if no mapping was established, that is, conceptually the
   * same as though the length that was mapped was 0. This function is
   * provided so that this class has Container semantics.
   */
  bool empty() const noexcept { return length() == 0; }

  /** Returns true if a mapping was established. */
  bool is_mapped() const noexcept;

  /**
   * `size` and `length` both return the logical length, i.e. the number of bytes
   * user requested to be mapped, while `mapped_length` returns the actual number of
   * bytes that were mapped which is a multiple of the underlying operating system's
   * page allocation granularity.
   */
  size_type size() const noexcept { return length(); }
  size_type length() const noexcept { return length_; }
  size_type mapped_length() const noexcept { return mapped_length_; }

  /** Returns the offset relative to the start of the mapping. */
  size_type mapping_offset() const noexcept { return mapped_length_ - length_; }

  /**
   * Returns a pointer to the first requested byte, or `nullptr` if no memory mapping
   * exists.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  pointer data() noexcept {
    return data_;
  }
  const_pointer data() const noexcept { return data_; }

  /**
   * Returns an iterator to the first requested byte, if a valid memory mapping
   * exists, otherwise this function call is undefined behaviour.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  iterator begin() noexcept {
    return data();
  }
  const_iterator begin() const noexcept { return data(); }
  const_iterator cbegin() const noexcept { return data(); }

  /**
   * Returns an iterator one past the last requested byte, if a valid memory mapping
   * exists, otherwise this function call is undefined behaviour.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  iterator end() noexcept {
    return data() + length();
  }
  const_iterator end() const noexcept { return data() + length(); }
  const_iterator cend() const noexcept { return data() + length(); }

  /**
   * Returns a reverse iterator to the last memory mapped byte, if a valid
   * memory mapping exists, otherwise this function call is undefined
   * behaviour.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  reverse_iterator rbegin() noexcept {
    return reverse_iterator(end());
  }
  const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
  const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(end()); }

  /**
   * Returns a reverse iterator past the first mapped byte, if a valid memory
   * mapping exists, otherwise this function call is undefined behaviour.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  reverse_iterator rend() noexcept {
    return reverse_iterator(begin());
  }
  const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
  const_reverse_iterator crend() const noexcept { return const_reverse_iterator(begin()); }

  /**
   * Returns a reference to the `i`th byte from the first requested byte (as returned
   * by `data`). If this is invoked when no valid memory mapping has been created
   * prior to this call, undefined behaviour ensues.
   */
  reference operator[](const size_type i) noexcept { return data_[i]; }
  const_reference operator[](const size_type i) const noexcept { return data_[i]; }

  /**
   * Establishes a memory mapping with AccessMode. If the mapping is unsuccesful, the
   * reason is reported via `error` and the object remains in a state as if this
   * function hadn't been called.
   *
   * `path`, which must be a path to an existing file, is used to retrieve a file
   * handle (which is closed when the object destructs or `unmap` is called), which is
   * then used to memory map the requested region. Upon failure, `error` is set to
   * indicate the reason and the object remains in an unmapped state.
   *
   * `offset` is the number of bytes, relative to the start of the file, where the
   * mapping should begin. When specifying it, there is no need to worry about
   * providing a value that is aligned with the operating system's page allocation
   * granularity. This is adjusted by the implementation such that the first requested
   * byte (as returned by `data` or `begin`), so long as `offset` is valid, will be at
   * `offset` from the start of the file.
   *
   * `length` is the number of bytes to map. It may be `map_entire_file`, in which
   * case a mapping of the entire file is created.
   */
  template <typename String>
  void map(const String &path, const size_type offset, const size_type length,
           std::error_code &error);

  /**
   * Establishes a memory mapping with AccessMode. If the mapping is unsuccesful, the
   * reason is reported via `error` and the object remains in a state as if this
   * function hadn't been called.
   *
   * `path`, which must be a path to an existing file, is used to retrieve a file
   * handle (which is closed when the object destructs or `unmap` is called), which is
   * then used to memory map the requested region. Upon failure, `error` is set to
   * indicate the reason and the object remains in an unmapped state.
   *
   * The entire file is mapped.
   */
  template <typename String> void map(const String &path, std::error_code &error) {
    map(path, 0, map_entire_file, error);
  }

  /**
   * Establishes a memory mapping with AccessMode. If the mapping is
   * unsuccesful, the reason is reported via `error` and the object remains in
   * a state as if this function hadn't been called.
   *
   * `handle`, which must be a valid file handle, which is used to memory map the
   * requested region. Upon failure, `error` is set to indicate the reason and the
   * object remains in an unmapped state.
   *
   * `offset` is the number of bytes, relative to the start of the file, where the
   * mapping should begin. When specifying it, there is no need to worry about
   * providing a value that is aligned with the operating system's page allocation
   * granularity. This is adjusted by the implementation such that the first requested
   * byte (as returned by `data` or `begin`), so long as `offset` is valid, will be at
   * `offset` from the start of the file.
   *
   * `length` is the number of bytes to map. It may be `map_entire_file`, in which
   * case a mapping of the entire file is created.
   */
  void map(const handle_type handle, const size_type offset, const size_type length,
           std::error_code &error);

  /**
   * Establishes a memory mapping with AccessMode. If the mapping is
   * unsuccesful, the reason is reported via `error` and the object remains in
   * a state as if this function hadn't been called.
   *
   * `handle`, which must be a valid file handle, which is used to memory map the
   * requested region. Upon failure, `error` is set to indicate the reason and the
   * object remains in an unmapped state.
   *
   * The entire file is mapped.
   */
  void map(const handle_type handle, std::error_code &error) {
    map(handle, 0, map_entire_file, error);
  }

  /**
   * If a valid memory mapping has been created prior to this call, this call
   * instructs the kernel to unmap the memory region and disassociate this object
   * from the file.
   *
   * The file handle associated with the file that is mapped is only closed if the
   * mapping was created using a file path. If, on the other hand, an existing
   * file handle was used to create the mapping, the file handle is not closed.
   */
  void unmap();

  void swap(basic_mmap &other);

  /** Flushes the memory mapped page to disk. Errors are reported via `error`. */
  template <access_mode A = AccessMode>
  typename std::enable_if<A == access_mode::write, void>::type sync(std::error_code &error);

  /**
   * All operators compare the address of the first byte and size of the two mapped
   * regions.
   */

private:
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  pointer get_mapping_start() noexcept {
    return !data() ? nullptr : data() - mapping_offset();
  }

  const_pointer get_mapping_start() const noexcept {
    return !data() ? nullptr : data() - mapping_offset();
  }

  /**
   * The destructor syncs changes to disk if `AccessMode` is `write`, but not
   * if it's `read`, but since the destructor cannot be templated, we need to
   * do SFINAE in a dedicated function, where one syncs and the other is a noop.
   */
  template <access_mode A = AccessMode>
  typename std::enable_if<A == access_mode::write, void>::type conditional_sync();
  template <access_mode A = AccessMode>
  typename std::enable_if<A == access_mode::read, void>::type conditional_sync();
};

template <access_mode AccessMode, typename ByteT>
bool operator==(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b);

template <access_mode AccessMode, typename ByteT>
bool operator!=(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b);

template <access_mode AccessMode, typename ByteT>
bool operator<(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b);

template <access_mode AccessMode, typename ByteT>
bool operator<=(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b);

template <access_mode AccessMode, typename ByteT>
bool operator>(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b);

template <access_mode AccessMode, typename ByteT>
bool operator>=(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b);

/**
 * This is the basis for all read-only mmap objects and should be preferred over
 * directly using `basic_mmap`.
 */
template <typename ByteT> using basic_mmap_source = basic_mmap<access_mode::read, ByteT>;

/**
 * This is the basis for all read-write mmap objects and should be preferred over
 * directly using `basic_mmap`.
 */
template <typename ByteT> using basic_mmap_sink = basic_mmap<access_mode::write, ByteT>;

/**
 * These aliases cover the most common use cases, both representing a raw byte stream
 * (either with a char or an unsigned char/uint8_t).
 */
using mmap_source = basic_mmap_source<char>;
using ummap_source = basic_mmap_source<unsigned char>;

using mmap_sink = basic_mmap_sink<char>;
using ummap_sink = basic_mmap_sink<unsigned char>;

/**
 * Convenience factory method that constructs a mapping for any `basic_mmap` or
 * `basic_mmap` type.
 */
template <typename MMap, typename MappingToken>
MMap make_mmap(const MappingToken &token, int64_t offset, int64_t length, std::error_code &error) {
  MMap mmap;
  mmap.map(token, offset, length, error);
  return mmap;
}

/**
 * Convenience factory method.
 *
 * MappingToken may be a String (`std::string`, `std::string_view`, `const char*`,
 * `std::filesystem::path`, `std::vector<char>`, or similar), or a
 * `mmap_source::handle_type`.
 */
template <typename MappingToken>
mmap_source make_mmap_source(const MappingToken &token, mmap_source::size_type offset,
                             mmap_source::size_type length, std::error_code &error) {
  return make_mmap<mmap_source>(token, offset, length, error);
}

template <typename MappingToken>
mmap_source make_mmap_source(const MappingToken &token, std::error_code &error) {
  return make_mmap_source(token, 0, map_entire_file, error);
}

/**
 * Convenience factory method.
 *
 * MappingToken may be a String (`std::string`, `std::string_view`, `const char*`,
 * `std::filesystem::path`, `std::vector<char>`, or similar), or a
 * `mmap_sink::handle_type`.
 */
template <typename MappingToken>
mmap_sink make_mmap_sink(const MappingToken &token, mmap_sink::size_type offset,
                         mmap_sink::size_type length, std::error_code &error) {
  return make_mmap<mmap_sink>(token, offset, length, error);
}

template <typename MappingToken>
mmap_sink make_mmap_sink(const MappingToken &token, std::error_code &error) {
  return make_mmap_sink(token, 0, map_entire_file, error);
}

} // namespace mio

// #include "detail/mmap.ipp"
/* Copyright 2017 https://github.com/mandreyel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MIO_BASIC_MMAP_IMPL
#define MIO_BASIC_MMAP_IMPL

// #include "mio/mmap.hpp"

// #include "mio/page.hpp"

// #include "mio/detail/string_util.hpp"
/* Copyright 2017 https://github.com/mandreyel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MIO_STRING_UTIL_HEADER
#define MIO_STRING_UTIL_HEADER

#include <type_traits>

namespace mio {
namespace detail {

template <typename S, typename C = typename std::decay<S>::type,
          typename = decltype(std::declval<C>().data()),
          typename = typename std::enable_if<std::is_same<typename C::value_type, char>::value
#ifdef _WIN32
                                             || std::is_same<typename C::value_type, wchar_t>::value
#endif
                                             >::type>
struct char_type_helper {
  using type = typename C::value_type;
};

template <class T> struct char_type { using type = typename char_type_helper<T>::type; };

// TODO: can we avoid this brute force approach?
template <> struct char_type<char *> { using type = char; };

template <> struct char_type<const char *> { using type = char; };

template <size_t N> struct char_type<char[N]> { using type = char; };

template <size_t N> struct char_type<const char[N]> { using type = char; };

#ifdef _WIN32
template <> struct char_type<wchar_t *> { using type = wchar_t; };

template <> struct char_type<const wchar_t *> { using type = wchar_t; };

template <size_t N> struct char_type<wchar_t[N]> { using type = wchar_t; };

template <size_t N> struct char_type<const wchar_t[N]> { using type = wchar_t; };
#endif // _WIN32

template <typename CharT, typename S> struct is_c_str_helper {
  static constexpr bool value =
      std::is_same<CharT *,
                   // TODO: I'm so sorry for this... Can this be made cleaner?
                   typename std::add_pointer<typename std::remove_cv<typename std::remove_pointer<
                       typename std::decay<S>::type>::type>::type>::type>::value;
};

template <typename S> struct is_c_str {
  static constexpr bool value = is_c_str_helper<char, S>::value;
};

#ifdef _WIN32
template <typename S> struct is_c_wstr {
  static constexpr bool value = is_c_str_helper<wchar_t, S>::value;
};
#endif // _WIN32

template <typename S> struct is_c_str_or_c_wstr {
  static constexpr bool value = is_c_str<S>::value
#ifdef _WIN32
                                || is_c_wstr<S>::value
#endif
      ;
};

template <typename String, typename = decltype(std::declval<String>().data()),
          typename = typename std::enable_if<!is_c_str_or_c_wstr<String>::value>::type>
const typename char_type<String>::type *c_str(const String &path) {
  return path.data();
}

template <typename String, typename = decltype(std::declval<String>().empty()),
          typename = typename std::enable_if<!is_c_str_or_c_wstr<String>::value>::type>
bool empty(const String &path) {
  return path.empty();
}

template <typename String,
          typename = typename std::enable_if<is_c_str_or_c_wstr<String>::value>::type>
const typename char_type<String>::type *c_str(String path) {
  return path;
}

template <typename String,
          typename = typename std::enable_if<is_c_str_or_c_wstr<String>::value>::type>
bool empty(String path) {
  return !path || (*path == 0);
}

} // namespace detail
} // namespace mio

#endif // MIO_STRING_UTIL_HEADER

#include <algorithm>

#ifndef _WIN32
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace mio {
namespace detail {

#ifdef _WIN32
namespace win {

/** Returns the 4 upper bytes of an 8-byte integer. */
inline DWORD int64_high(int64_t n) noexcept { return n >> 32; }

/** Returns the 4 lower bytes of an 8-byte integer. */
inline DWORD int64_low(int64_t n) noexcept { return n & 0xffffffff; }

template <typename String, typename = typename std::enable_if<
                               std::is_same<typename char_type<String>::type, char>::value>::type>
file_handle_type open_file_helper(const String &path, const access_mode mode) {
  return ::CreateFileA(
      c_str(path), mode == access_mode::read ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
}

template <typename String>
typename std::enable_if<std::is_same<typename char_type<String>::type, wchar_t>::value,
                        file_handle_type>::type
open_file_helper(const String &path, const access_mode mode) {
  return ::CreateFileW(
      c_str(path), mode == access_mode::read ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
}

} // namespace win
#endif // _WIN32

/**
 * Returns the last platform specific system error (errno on POSIX and
 * GetLastError on Win) as a `std::error_code`.
 */
inline std::error_code last_error() noexcept {
  std::error_code error;
#ifdef _WIN32
  error.assign(GetLastError(), std::system_category());
#else
  error.assign(errno, std::system_category());
#endif
  return error;
}

template <typename String>
file_handle_type open_file(const String &path, const access_mode mode, std::error_code &error) {
  error.clear();
  if (detail::empty(path)) {
    error = std::make_error_code(std::errc::invalid_argument);
    return invalid_handle;
  }
#ifdef _WIN32
  const auto handle = win::open_file_helper(path, mode);
#else // POSIX
  const auto handle = ::open(c_str(path), mode == access_mode::read ? O_RDONLY : O_RDWR);
#endif
  if (handle == invalid_handle) {
    error = detail::last_error();
  }
  return handle;
}

inline size_t query_file_size(file_handle_type handle, std::error_code &error) {
  error.clear();
#ifdef _WIN32
  LARGE_INTEGER file_size;
  if (::GetFileSizeEx(handle, &file_size) == 0) {
    error = detail::last_error();
    return 0;
  }
  return static_cast<int64_t>(file_size.QuadPart);
#else // POSIX
  struct stat sbuf;
  if (::fstat(handle, &sbuf) == -1) {
    error = detail::last_error();
    return 0;
  }
  return sbuf.st_size;
#endif
}

struct mmap_context {
  char *data;
  int64_t length;
  int64_t mapped_length;
#ifdef _WIN32
  file_handle_type file_mapping_handle;
#endif
};

inline mmap_context memory_map(const file_handle_type file_handle, const int64_t offset,
                               const int64_t length, const access_mode mode,
                               std::error_code &error) {
  const int64_t aligned_offset = make_offset_page_aligned(offset);
  const int64_t length_to_map = offset - aligned_offset + length;
#ifdef _WIN32
  const int64_t max_file_size = offset + length;
  const auto file_mapping_handle = ::CreateFileMapping(
      file_handle, 0, mode == access_mode::read ? PAGE_READONLY : PAGE_READWRITE,
      win::int64_high(max_file_size), win::int64_low(max_file_size), 0);
  if (file_mapping_handle == invalid_handle) {
    error = detail::last_error();
    return {};
  }
  char *mapping_start = static_cast<char *>(::MapViewOfFile(
      file_mapping_handle, mode == access_mode::read ? FILE_MAP_READ : FILE_MAP_WRITE,
      win::int64_high(aligned_offset), win::int64_low(aligned_offset), length_to_map));
  if (mapping_start == nullptr) {
    // Close file handle if mapping it failed.
    ::CloseHandle(file_mapping_handle);
    error = detail::last_error();
    return {};
  }
#else // POSIX
  char *mapping_start =
      static_cast<char *>(::mmap(0, // Don't give hint as to where to map.
                                 length_to_map, mode == access_mode::read ? PROT_READ : PROT_WRITE,
                                 MAP_SHARED, file_handle, aligned_offset));
  if (mapping_start == MAP_FAILED) {
    error = detail::last_error();
    return {};
  }
#endif
  mmap_context ctx;
  ctx.data = mapping_start + offset - aligned_offset;
  ctx.length = length;
  ctx.mapped_length = length_to_map;
#ifdef _WIN32
  ctx.file_mapping_handle = file_mapping_handle;
#endif
  return ctx;
}

} // namespace detail

// -- basic_mmap --

template <access_mode AccessMode, typename ByteT> basic_mmap<AccessMode, ByteT>::~basic_mmap() {
  conditional_sync();
  unmap();
}

template <access_mode AccessMode, typename ByteT>
basic_mmap<AccessMode, ByteT>::basic_mmap(basic_mmap &&other)
    : data_(std::move(other.data_)), length_(std::move(other.length_)),
      mapped_length_(std::move(other.mapped_length_)), file_handle_(std::move(other.file_handle_))
#ifdef _WIN32
      ,
      file_mapping_handle_(std::move(other.file_mapping_handle_))
#endif
      ,
      is_handle_internal_(std::move(other.is_handle_internal_)) {
  other.data_ = nullptr;
  other.length_ = other.mapped_length_ = 0;
  other.file_handle_ = invalid_handle;
#ifdef _WIN32
  other.file_mapping_handle_ = invalid_handle;
#endif
}

template <access_mode AccessMode, typename ByteT>
basic_mmap<AccessMode, ByteT> &basic_mmap<AccessMode, ByteT>::operator=(basic_mmap &&other) {
  if (this != &other) {
    // First the existing mapping needs to be removed.
    unmap();
    data_ = std::move(other.data_);
    length_ = std::move(other.length_);
    mapped_length_ = std::move(other.mapped_length_);
    file_handle_ = std::move(other.file_handle_);
#ifdef _WIN32
    file_mapping_handle_ = std::move(other.file_mapping_handle_);
#endif
    is_handle_internal_ = std::move(other.is_handle_internal_);

    // The moved from basic_mmap's fields need to be reset, because
    // otherwise other's destructor will unmap the same mapping that was
    // just moved into this.
    other.data_ = nullptr;
    other.length_ = other.mapped_length_ = 0;
    other.file_handle_ = invalid_handle;
#ifdef _WIN32
    other.file_mapping_handle_ = invalid_handle;
#endif
    other.is_handle_internal_ = false;
  }
  return *this;
}

template <access_mode AccessMode, typename ByteT>
typename basic_mmap<AccessMode, ByteT>::handle_type
basic_mmap<AccessMode, ByteT>::mapping_handle() const noexcept {
#ifdef _WIN32
  return file_mapping_handle_;
#else
  return file_handle_;
#endif
}

template <access_mode AccessMode, typename ByteT>
template <typename String>
void basic_mmap<AccessMode, ByteT>::map(const String &path, const size_type offset,
                                        const size_type length, std::error_code &error) {
  error.clear();
  if (detail::empty(path)) {
    error = std::make_error_code(std::errc::invalid_argument);
    return;
  }
  const auto handle = detail::open_file(path, AccessMode, error);
  if (error) {
    return;
  }

  map(handle, offset, length, error);
  // This MUST be after the call to map, as that sets this to true.
  if (!error) {
    is_handle_internal_ = true;
  }
}

template <access_mode AccessMode, typename ByteT>
void basic_mmap<AccessMode, ByteT>::map(const handle_type handle, const size_type offset,
                                        const size_type length, std::error_code &error) {
  error.clear();
  if (handle == invalid_handle) {
    error = std::make_error_code(std::errc::bad_file_descriptor);
    return;
  }

  const auto file_size = detail::query_file_size(handle, error);
  if (error) {
    return;
  }

  if (offset + length > file_size) {
    error = std::make_error_code(std::errc::invalid_argument);
    return;
  }

  const auto ctx = detail::memory_map(
      handle, offset, length == map_entire_file ? (file_size - offset) : length, AccessMode, error);
  if (!error) {
    // We must unmap the previous mapping that may have existed prior to this call.
    // Note that this must only be invoked after a new mapping has been created in
    // order to provide the strong guarantee that, should the new mapping fail, the
    // `map` function leaves this instance in a state as though the function had
    // never been invoked.
    unmap();
    file_handle_ = handle;
    is_handle_internal_ = false;
    data_ = reinterpret_cast<pointer>(ctx.data);
    length_ = ctx.length;
    mapped_length_ = ctx.mapped_length;
#ifdef _WIN32
    file_mapping_handle_ = ctx.file_mapping_handle;
#endif
  }
}

template <access_mode AccessMode, typename ByteT>
template <access_mode A>
typename std::enable_if<A == access_mode::write, void>::type
basic_mmap<AccessMode, ByteT>::sync(std::error_code &error) {
  error.clear();
  if (!is_open()) {
    error = std::make_error_code(std::errc::bad_file_descriptor);
    return;
  }

  if (data()) {
#ifdef _WIN32
    if (::FlushViewOfFile(get_mapping_start(), mapped_length_) == 0 ||
        ::FlushFileBuffers(file_handle_) == 0)
#else // POSIX
    if (::msync(get_mapping_start(), mapped_length_, MS_SYNC) != 0)
#endif
    {
      error = detail::last_error();
      return;
    }
  }
#ifdef _WIN32
  if (::FlushFileBuffers(file_handle_) == 0) {
    error = detail::last_error();
  }
#endif
}

template <access_mode AccessMode, typename ByteT> void basic_mmap<AccessMode, ByteT>::unmap() {
  if (!is_open()) {
    return;
  }
  // TODO do we care about errors here?
#ifdef _WIN32
  if (is_mapped()) {
    ::UnmapViewOfFile(get_mapping_start());
    ::CloseHandle(file_mapping_handle_);
  }
#else // POSIX
  if (data_) {
    ::munmap(const_cast<pointer>(get_mapping_start()), mapped_length_);
  }
#endif

  // If `file_handle_` was obtained by our opening it (when map is called with
  // a path, rather than an existing file handle), we need to close it,
  // otherwise it must not be closed as it may still be used outside this
  // instance.
  if (is_handle_internal_) {
#ifdef _WIN32
    ::CloseHandle(file_handle_);
#else // POSIX
    ::close(file_handle_);
#endif
  }

  // Reset fields to their default values.
  data_ = nullptr;
  length_ = mapped_length_ = 0;
  file_handle_ = invalid_handle;
#ifdef _WIN32
  file_mapping_handle_ = invalid_handle;
#endif
}

template <access_mode AccessMode, typename ByteT>
bool basic_mmap<AccessMode, ByteT>::is_mapped() const noexcept {
#ifdef _WIN32
  return file_mapping_handle_ != invalid_handle;
#else // POSIX
  return is_open();
#endif
}

template <access_mode AccessMode, typename ByteT>
void basic_mmap<AccessMode, ByteT>::swap(basic_mmap &other) {
  if (this != &other) {
    using std::swap;
    swap(data_, other.data_);
    swap(file_handle_, other.file_handle_);
#ifdef _WIN32
    swap(file_mapping_handle_, other.file_mapping_handle_);
#endif
    swap(length_, other.length_);
    swap(mapped_length_, other.mapped_length_);
    swap(is_handle_internal_, other.is_handle_internal_);
  }
}

template <access_mode AccessMode, typename ByteT>
template <access_mode A>
typename std::enable_if<A == access_mode::write, void>::type
basic_mmap<AccessMode, ByteT>::conditional_sync() {
  // This is invoked from the destructor, so not much we can do about
  // failures here.
  std::error_code ec;
  sync(ec);
}

template <access_mode AccessMode, typename ByteT>
template <access_mode A>
typename std::enable_if<A == access_mode::read, void>::type
basic_mmap<AccessMode, ByteT>::conditional_sync() {
  // noop
}

template <access_mode AccessMode, typename ByteT>
bool operator==(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b) {
  return a.data() == b.data() && a.size() == b.size();
}

template <access_mode AccessMode, typename ByteT>
bool operator!=(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b) {
  return !(a == b);
}

template <access_mode AccessMode, typename ByteT>
bool operator<(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b) {
  if (a.data() == b.data()) {
    return a.size() < b.size();
  }
  return a.data() < b.data();
}

template <access_mode AccessMode, typename ByteT>
bool operator<=(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b) {
  return !(a > b);
}

template <access_mode AccessMode, typename ByteT>
bool operator>(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b) {
  if (a.data() == b.data()) {
    return a.size() > b.size();
  }
  return a.data() > b.data();
}

template <access_mode AccessMode, typename ByteT>
bool operator>=(const basic_mmap<AccessMode, ByteT> &a, const basic_mmap<AccessMode, ByteT> &b) {
  return !(a < b);
}

} // namespace mio

#endif // MIO_BASIC_MMAP_IMPL

#endif // MIO_MMAP_HEADER
/* Copyright 2017 https://github.com/mandreyel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MIO_PAGE_HEADER
#define MIO_PAGE_HEADER

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace mio {

/**
 * This is used by `basic_mmap` to determine whether to create a read-only or
 * a read-write memory mapping.
 */
enum class access_mode { read, write };

/**
 * Determines the operating system's page allocation granularity.
 *
 * On the first call to this function, it invokes the operating system specific syscall
 * to determine the page size, caches the value, and returns it. Any subsequent call to
 * this function serves the cached value, so no further syscalls are made.
 */
inline size_t page_size() {
  static const size_t page_size = [] {
#ifdef _WIN32
    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    return SystemInfo.dwAllocationGranularity;
#else
    return sysconf(_SC_PAGE_SIZE);
#endif
  }();
  return page_size;
}

/**
 * Alligns `offset` to the operating's system page size such that it subtracts the
 * difference until the nearest page boundary before `offset`, or does nothing if
 * `offset` is already page aligned.
 */
inline size_t make_offset_page_aligned(size_t offset) noexcept {
  const size_t page_size_ = page_size();
  // Use integer division to round down to the nearest page alignment.
  return offset / page_size_ * page_size_;
}

} // namespace mio

#endif // MIO_PAGE_HEADER
/* Copyright 2017 https://github.com/mandreyel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef MIO_SHARED_MMAP_HEADER
#define MIO_SHARED_MMAP_HEADER

// #include "mio/mmap.hpp"

#include <memory>       // std::shared_ptr
#include <system_error> // std::error_code

namespace mio {

/**
 * Exposes (nearly) the same interface as `basic_mmap`, but endowes it with
 * `std::shared_ptr` semantics.
 *
 * This is not the default behaviour of `basic_mmap` to avoid allocating on the heap if
 * shared semantics are not required.
 */
template <access_mode AccessMode, typename ByteT> class basic_shared_mmap {
  using impl_type = basic_mmap<AccessMode, ByteT>;
  std::shared_ptr<impl_type> pimpl_;

public:
  using value_type = typename impl_type::value_type;
  using size_type = typename impl_type::size_type;
  using reference = typename impl_type::reference;
  using const_reference = typename impl_type::const_reference;
  using pointer = typename impl_type::pointer;
  using const_pointer = typename impl_type::const_pointer;
  using difference_type = typename impl_type::difference_type;
  using iterator = typename impl_type::iterator;
  using const_iterator = typename impl_type::const_iterator;
  using reverse_iterator = typename impl_type::reverse_iterator;
  using const_reverse_iterator = typename impl_type::const_reverse_iterator;
  using iterator_category = typename impl_type::iterator_category;
  using handle_type = typename impl_type::handle_type;
  using mmap_type = impl_type;

  basic_shared_mmap() = default;
  basic_shared_mmap(const basic_shared_mmap &) = default;
  basic_shared_mmap &operator=(const basic_shared_mmap &) = default;
  basic_shared_mmap(basic_shared_mmap &&) = default;
  basic_shared_mmap &operator=(basic_shared_mmap &&) = default;

  /** Takes ownership of an existing mmap object. */
  basic_shared_mmap(mmap_type &&mmap) : pimpl_(std::make_shared<mmap_type>(std::move(mmap))) {}

  /** Takes ownership of an existing mmap object. */
  basic_shared_mmap &operator=(mmap_type &&mmap) {
    pimpl_ = std::make_shared<mmap_type>(std::move(mmap));
    return *this;
  }

  /** Initializes this object with an already established shared mmap. */
  basic_shared_mmap(std::shared_ptr<mmap_type> mmap) : pimpl_(std::move(mmap)) {}

  /** Initializes this object with an already established shared mmap. */
  basic_shared_mmap &operator=(std::shared_ptr<mmap_type> mmap) {
    pimpl_ = std::move(mmap);
    return *this;
  }

#ifdef __cpp_exceptions
  /**
   * The same as invoking the `map` function, except any error that may occur
   * while establishing the mapping is wrapped in a `std::system_error` and is
   * thrown.
   */
  template <typename String>
  basic_shared_mmap(const String &path, const size_type offset = 0,
                    const size_type length = map_entire_file) {
    std::error_code error;
    map(path, offset, length, error);
    if (error) {
      throw std::system_error(error);
    }
  }

  /**
   * The same as invoking the `map` function, except any error that may occur
   * while establishing the mapping is wrapped in a `std::system_error` and is
   * thrown.
   */
  basic_shared_mmap(const handle_type handle, const size_type offset = 0,
                    const size_type length = map_entire_file) {
    std::error_code error;
    map(handle, offset, length, error);
    if (error) {
      throw std::system_error(error);
    }
  }
#endif // __cpp_exceptions

  /**
   * If this is a read-write mapping and the last reference to the mapping,
   * the destructor invokes sync. Regardless of the access mode, unmap is
   * invoked as a final step.
   */
  ~basic_shared_mmap() = default;

  /** Returns the underlying `std::shared_ptr` instance that holds the mmap. */
  std::shared_ptr<mmap_type> get_shared_ptr() { return pimpl_; }

  /**
   * On UNIX systems 'file_handle' and 'mapping_handle' are the same. On Windows,
   * however, a mapped region of a file gets its own handle, which is returned by
   * 'mapping_handle'.
   */
  handle_type file_handle() const noexcept {
    return pimpl_ ? pimpl_->file_handle() : invalid_handle;
  }

  handle_type mapping_handle() const noexcept {
    return pimpl_ ? pimpl_->mapping_handle() : invalid_handle;
  }

  /** Returns whether a valid memory mapping has been created. */
  bool is_open() const noexcept { return pimpl_ && pimpl_->is_open(); }

  /**
   * Returns true if no mapping was established, that is, conceptually the
   * same as though the length that was mapped was 0. This function is
   * provided so that this class has Container semantics.
   */
  bool empty() const noexcept { return !pimpl_ || pimpl_->empty(); }

  /**
   * `size` and `length` both return the logical length, i.e. the number of bytes
   * user requested to be mapped, while `mapped_length` returns the actual number of
   * bytes that were mapped which is a multiple of the underlying operating system's
   * page allocation granularity.
   */
  size_type size() const noexcept { return pimpl_ ? pimpl_->length() : 0; }
  size_type length() const noexcept { return pimpl_ ? pimpl_->length() : 0; }
  size_type mapped_length() const noexcept { return pimpl_ ? pimpl_->mapped_length() : 0; }

  /**
   * Returns a pointer to the first requested byte, or `nullptr` if no memory mapping
   * exists.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  pointer data() noexcept {
    return pimpl_->data();
  }
  const_pointer data() const noexcept { return pimpl_ ? pimpl_->data() : nullptr; }

  /**
   * Returns an iterator to the first requested byte, if a valid memory mapping
   * exists, otherwise this function call is undefined behaviour.
   */
  iterator begin() noexcept { return pimpl_->begin(); }
  const_iterator begin() const noexcept { return pimpl_->begin(); }
  const_iterator cbegin() const noexcept { return pimpl_->cbegin(); }

  /**
   * Returns an iterator one past the last requested byte, if a valid memory mapping
   * exists, otherwise this function call is undefined behaviour.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  iterator end() noexcept {
    return pimpl_->end();
  }
  const_iterator end() const noexcept { return pimpl_->end(); }
  const_iterator cend() const noexcept { return pimpl_->cend(); }

  /**
   * Returns a reverse iterator to the last memory mapped byte, if a valid
   * memory mapping exists, otherwise this function call is undefined
   * behaviour.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  reverse_iterator rbegin() noexcept {
    return pimpl_->rbegin();
  }
  const_reverse_iterator rbegin() const noexcept { return pimpl_->rbegin(); }
  const_reverse_iterator crbegin() const noexcept { return pimpl_->crbegin(); }

  /**
   * Returns a reverse iterator past the first mapped byte, if a valid memory
   * mapping exists, otherwise this function call is undefined behaviour.
   */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  reverse_iterator rend() noexcept {
    return pimpl_->rend();
  }
  const_reverse_iterator rend() const noexcept { return pimpl_->rend(); }
  const_reverse_iterator crend() const noexcept { return pimpl_->crend(); }

  /**
   * Returns a reference to the `i`th byte from the first requested byte (as returned
   * by `data`). If this is invoked when no valid memory mapping has been created
   * prior to this call, undefined behaviour ensues.
   */
  reference operator[](const size_type i) noexcept { return (*pimpl_)[i]; }
  const_reference operator[](const size_type i) const noexcept { return (*pimpl_)[i]; }

  /**
   * Establishes a memory mapping with AccessMode. If the mapping is unsuccesful, the
   * reason is reported via `error` and the object remains in a state as if this
   * function hadn't been called.
   *
   * `path`, which must be a path to an existing file, is used to retrieve a file
   * handle (which is closed when the object destructs or `unmap` is called), which is
   * then used to memory map the requested region. Upon failure, `error` is set to
   * indicate the reason and the object remains in an unmapped state.
   *
   * `offset` is the number of bytes, relative to the start of the file, where the
   * mapping should begin. When specifying it, there is no need to worry about
   * providing a value that is aligned with the operating system's page allocation
   * granularity. This is adjusted by the implementation such that the first requested
   * byte (as returned by `data` or `begin`), so long as `offset` is valid, will be at
   * `offset` from the start of the file.
   *
   * `length` is the number of bytes to map. It may be `map_entire_file`, in which
   * case a mapping of the entire file is created.
   */
  template <typename String>
  void map(const String &path, const size_type offset, const size_type length,
           std::error_code &error) {
    map_impl(path, offset, length, error);
  }

  /**
   * Establishes a memory mapping with AccessMode. If the mapping is unsuccesful, the
   * reason is reported via `error` and the object remains in a state as if this
   * function hadn't been called.
   *
   * `path`, which must be a path to an existing file, is used to retrieve a file
   * handle (which is closed when the object destructs or `unmap` is called), which is
   * then used to memory map the requested region. Upon failure, `error` is set to
   * indicate the reason and the object remains in an unmapped state.
   *
   * The entire file is mapped.
   */
  template <typename String> void map(const String &path, std::error_code &error) {
    map_impl(path, 0, map_entire_file, error);
  }

  /**
   * Establishes a memory mapping with AccessMode. If the mapping is unsuccesful, the
   * reason is reported via `error` and the object remains in a state as if this
   * function hadn't been called.
   *
   * `handle`, which must be a valid file handle, which is used to memory map the
   * requested region. Upon failure, `error` is set to indicate the reason and the
   * object remains in an unmapped state.
   *
   * `offset` is the number of bytes, relative to the start of the file, where the
   * mapping should begin. When specifying it, there is no need to worry about
   * providing a value that is aligned with the operating system's page allocation
   * granularity. This is adjusted by the implementation such that the first requested
   * byte (as returned by `data` or `begin`), so long as `offset` is valid, will be at
   * `offset` from the start of the file.
   *
   * `length` is the number of bytes to map. It may be `map_entire_file`, in which
   * case a mapping of the entire file is created.
   */
  void map(const handle_type handle, const size_type offset, const size_type length,
           std::error_code &error) {
    map_impl(handle, offset, length, error);
  }

  /**
   * Establishes a memory mapping with AccessMode. If the mapping is unsuccesful, the
   * reason is reported via `error` and the object remains in a state as if this
   * function hadn't been called.
   *
   * `handle`, which must be a valid file handle, which is used to memory map the
   * requested region. Upon failure, `error` is set to indicate the reason and the
   * object remains in an unmapped state.
   *
   * The entire file is mapped.
   */
  void map(const handle_type handle, std::error_code &error) {
    map_impl(handle, 0, map_entire_file, error);
  }

  /**
   * If a valid memory mapping has been created prior to this call, this call
   * instructs the kernel to unmap the memory region and disassociate this object
   * from the file.
   *
   * The file handle associated with the file that is mapped is only closed if the
   * mapping was created using a file path. If, on the other hand, an existing
   * file handle was used to create the mapping, the file handle is not closed.
   */
  void unmap() {
    if (pimpl_)
      pimpl_->unmap();
  }

  void swap(basic_shared_mmap &other) { pimpl_.swap(other.pimpl_); }

  /** Flushes the memory mapped page to disk. Errors are reported via `error`. */
  template <access_mode A = AccessMode,
            typename = typename std::enable_if<A == access_mode::write>::type>
  void sync(std::error_code &error) {
    if (pimpl_)
      pimpl_->sync(error);
  }

  /** All operators compare the underlying `basic_mmap`'s addresses. */

  friend bool operator==(const basic_shared_mmap &a, const basic_shared_mmap &b) {
    return a.pimpl_ == b.pimpl_;
  }

  friend bool operator!=(const basic_shared_mmap &a, const basic_shared_mmap &b) {
    return !(a == b);
  }

  friend bool operator<(const basic_shared_mmap &a, const basic_shared_mmap &b) {
    return a.pimpl_ < b.pimpl_;
  }

  friend bool operator<=(const basic_shared_mmap &a, const basic_shared_mmap &b) {
    return a.pimpl_ <= b.pimpl_;
  }

  friend bool operator>(const basic_shared_mmap &a, const basic_shared_mmap &b) {
    return a.pimpl_ > b.pimpl_;
  }

  friend bool operator>=(const basic_shared_mmap &a, const basic_shared_mmap &b) {
    return a.pimpl_ >= b.pimpl_;
  }

private:
  template <typename MappingToken>
  void map_impl(const MappingToken &token, const size_type offset, const size_type length,
                std::error_code &error) {
    if (!pimpl_) {
      mmap_type mmap = make_mmap<mmap_type>(token, offset, length, error);
      if (error) {
        return;
      }
      pimpl_ = std::make_shared<mmap_type>(std::move(mmap));
    } else {
      pimpl_->map(token, offset, length, error);
    }
  }
};

/**
 * This is the basis for all read-only mmap objects and should be preferred over
 * directly using basic_shared_mmap.
 */
template <typename ByteT>
using basic_shared_mmap_source = basic_shared_mmap<access_mode::read, ByteT>;

/**
 * This is the basis for all read-write mmap objects and should be preferred over
 * directly using basic_shared_mmap.
 */
template <typename ByteT>
using basic_shared_mmap_sink = basic_shared_mmap<access_mode::write, ByteT>;

/**
 * These aliases cover the most common use cases, both representing a raw byte stream
 * (either with a char or an unsigned char/uint8_t).
 */
using shared_mmap_source = basic_shared_mmap_source<char>;
using shared_ummap_source = basic_shared_mmap_source<unsigned char>;

using shared_mmap_sink = basic_shared_mmap_sink<char>;
using shared_ummap_sink = basic_shared_mmap_sink<unsigned char>;

} // namespace mio

#endif // MIO_SHARED_MMAP_HEADER

#pragma once
#include <utility>

namespace csv2 {

namespace trim_policy {
struct no_trimming {
public:
  static std::pair<size_t, size_t> trim(const char *buffer, size_t start, size_t end) {
    (void)(buffer); // to silence unused parameter warning
    return {start, end};
  }
};

template <char... character_list> struct trim_characters {
private:
  constexpr static bool is_trim_char(char) { return false; }

  template <class... Tail> constexpr static bool is_trim_char(char c, char head, Tail... tail) {
    return c == head || is_trim_char(c, tail...);
  }

public:
  static std::pair<size_t, size_t> trim(const char *buffer, size_t start, size_t end) {
    size_t new_start = start, new_end = end;
    while (new_start != new_end && is_trim_char(buffer[new_start], character_list...))
      ++new_start;
    while (new_start != new_end && is_trim_char(buffer[new_end - 1], character_list...))
      --new_end;
    return {new_start, new_end};
  }
};

using trim_whitespace = trim_characters<' ', '\t'>;
} // namespace trim_policy

template <char character> struct delimiter {
  constexpr static char value = character;
};

template <char character> struct quote_character {
  constexpr static char value = character;
};

template <bool flag> struct first_row_is_header {
  constexpr static bool value = flag;
};

}
#pragma once
#include <cstring>
// #include <csv2/mio.hpp>
// #include <csv2/parameters.hpp>
#include <istream>
#include <string>

namespace csv2 {

template <class delimiter = delimiter<','>, class quote_character = quote_character<'"'>,
          class first_row_is_header = first_row_is_header<true>,
          class trim_policy = trim_policy::trim_whitespace>
class Reader {
  mio::mmap_source mmap_;          // mmap source
  const char *buffer_{nullptr};    // pointer to memory-mapped data
  size_t buffer_size_{0};          // mapped length of buffer
  size_t header_start_{0};         // start index of header (cache)
  size_t header_end_{0};           // end index of header (cache)

public:
  // Use this if you'd like to mmap the CSV file
  template <typename StringType> bool mmap(StringType &&filename) {
    mmap_ = mio::mmap_source(filename);
    if (!mmap_.is_open() || !mmap_.is_mapped())
      return false;
    buffer_ = mmap_.data();
    buffer_size_ = mmap_.mapped_length();
    return true;
  }

  // Use this if you have the CSV contents
  // in an std::string already
  template <typename StringType> bool parse(StringType &&contents) {
    buffer_ = std::forward<StringType>(contents).c_str();
    buffer_size_ = contents.size();
    return buffer_size_ > 0;
  }

  class RowIterator;
  class Row;
  class CellIterator;

  class Cell {
    const char *buffer_{nullptr}; // Pointer to memory-mapped buffer
    size_t start_{0};             // Start index of cell content
    size_t end_{0};               // End index of cell content
    bool escaped_{false};         // Does the cell have escaped content?
    friend class Row;
    friend class CellIterator;

  public:
    // Returns the raw_value of the cell without handling escaped
    // content, e.g., cell containing """foo""" will be returned
    // as is
    template <typename Container> void read_raw_value(Container &result) const {
      if (start_ >= end_)
        return;
      result.reserve(end_ - start_);
      for (size_t i = start_; i < end_; ++i)
        result.push_back(buffer_[i]);
    }

    // If cell is escaped, convert and return correct cell contents,
    // e.g., """foo""" => ""foo""
    template <typename Container> void read_value(Container &result) const {
      if (start_ >= end_)
        return;
      result.reserve(end_ - start_);
      const auto new_start_end = trim_policy::trim(buffer_, start_, end_);
      for (size_t i = new_start_end.first; i < new_start_end.second; ++i)
        result.push_back(buffer_[i]);
      for (size_t i = 1; i < result.size(); ++i) {
        if (result[i] == quote_character::value && result[i - 1] == quote_character::value) {
          result.erase(i - 1, 1);
        }
      }
    }
  };

  class Row {
    const char *buffer_{nullptr}; // Pointer to memory-mapped buffer
    size_t start_{0};             // Start index of row content
    size_t end_{0};               // End index of row content
    friend class RowIterator;
    friend class Reader;

  public:
    // Returns the raw_value of the row
    template <typename Container> void read_raw_value(Container &result) const {
      if (start_ >= end_)
        return;
      result.reserve(end_ - start_);
      for (size_t i = start_; i < end_; ++i)
        result.push_back(buffer_[i]);
    }

    class CellIterator {
      friend class Row;
      const char *buffer_;
      size_t buffer_size_;
      size_t start_;
      size_t current_;
      size_t end_;

    public:
      CellIterator(const char *buffer, size_t buffer_size, size_t start, size_t end)
          : buffer_(buffer), buffer_size_(buffer_size), start_(start), current_(start_), end_(end) {
      }

      CellIterator &operator++() {
        current_ += 1;
        return *this;
      }

      Cell operator*() {
        bool escaped{false};
        class Cell cell;
        cell.buffer_ = buffer_;
        cell.start_ = current_;
        cell.end_ = end_;

        size_t last_quote_location = 0;
        bool quote_opened = false;
        for (auto i = current_; i < end_; i++) {
          current_ = i;
          if (buffer_[i] == delimiter::value && !quote_opened) {
            // actual delimiter
            // end of cell
            cell.end_ = current_;
            cell.escaped_ = escaped;
            return cell;
          } else {
            if (buffer_[i] == quote_character::value) {
              if (!quote_opened) {
                // first quote for this cell
                quote_opened = true;
                last_quote_location = i;
              } else {
                escaped = (last_quote_location == i - 1);
                last_quote_location += (i - last_quote_location) * size_t(!escaped);
                quote_opened = escaped || (buffer_[i + 1] != delimiter::value);
              }
            }
          }
        }
        cell.end_ = current_ + 1;
        return cell;
      }

      bool operator!=(const CellIterator &rhs) { return current_ != rhs.current_; }
    };

    CellIterator begin() const { return CellIterator(buffer_, end_ - start_, start_, end_); }
    CellIterator end() const { return CellIterator(buffer_, end_ - start_, end_, end_); }
  };

  class RowIterator {
    friend class Reader;
    const char *buffer_;
    size_t buffer_size_;
    size_t start_;
    size_t end_;

  public:
    RowIterator(const char *buffer, size_t buffer_size, size_t start)
        : buffer_(buffer), buffer_size_(buffer_size), start_(start), end_(start_) {}

    RowIterator &operator++() {
      start_ = end_ + 1;
      end_ = start_;
      return *this;
    }

    Row operator*() {
      Row result;
      result.buffer_ = buffer_;
      result.start_ = start_;
      result.end_ = end_;

      if (const char *ptr =
              static_cast<const char *>(memchr(&buffer_[start_], '\n', (buffer_size_ - start_)))) {
        end_ = start_ + (ptr - &buffer_[start_]);
        result.end_ = end_;
        start_ = end_ + 1;
      } else {
        // last row
        end_ = buffer_size_;
        result.end_ = end_;
      }
      return result;
    }

    bool operator!=(const RowIterator &rhs) { return start_ != rhs.start_; }
  };

  RowIterator begin() const {
    if (buffer_size_ == 0)
      return end();
    if (first_row_is_header::value) {
      const auto header_indices = header_indices_();
      return RowIterator(buffer_, buffer_size_, header_indices.second  > 0 ? header_indices.second + 1 : 0);
    } else {
      return RowIterator(buffer_, buffer_size_, 0);
    }
  }

  RowIterator end() const { return RowIterator(buffer_, buffer_size_, buffer_size_ + 1); }

private:
  std::pair<size_t, size_t> header_indices_() const {
    size_t start = 0, end = 0;

    if (const char *ptr =
            static_cast<const char *>(memchr(&buffer_[start], '\n', (buffer_size_ - start)))) {
      end = start + (ptr - &buffer_[start]);
    }
    return {start, end};
  }

public:

  Row header() const {
    size_t start = 0, end = 0;
    Row result;
    result.buffer_ = buffer_;
    result.start_ = start;
    result.end_ = end;

    if (const char *ptr =
            static_cast<const char *>(memchr(&buffer_[start], '\n', (buffer_size_ - start)))) {
      end = start + (ptr - &buffer_[start]);
      result.end_ = end;
    }
    return result;
  }

  size_t rows() const {
    size_t result{0};
    if (!buffer_ || buffer_size_ == 0)
      return result;
    for (const char *p = buffer_;
         (p = static_cast<const char *>(memchr(p, '\n', (buffer_ + buffer_size_) - p))); ++p)
      ++result;
    return result;
  }

  size_t cols() const {
    size_t result{0};
    for (const auto cell : header())
      result += 1;
    return result;
  }
};
} // namespace csv2
#pragma once
#include <cstring>
// #include <csv2/parameters.hpp>
#include <fstream>
#include <string>
#include <utility>
#include <iostream>

namespace csv2 {

template <class delimiter = delimiter<','>>
class Writer {
    std::ofstream& stream_;    // output stream for the writer
public:
    template <typename Stream>
    Writer(Stream&& stream) : stream_(std::forward<Stream>(stream)) {}

    ~Writer() {
        stream_.close();
    }

    template <typename Container>
    void write_row(Container&& row) {
        const auto& strings = std::forward<Container>(row);
        const auto delimiter_string = std::string(1, delimiter::value);
        std::copy(strings.begin(), strings.end() - 1,
            std::ostream_iterator<std::string>(stream_, delimiter_string.c_str()));
        stream_ << strings.back() << "\n";
    }

    template <typename Container>
    void write_rows(Container&& rows) {
        const auto& container_of_rows = std::forward<Container>(rows);
        for (const auto& row : container_of_rows) {
            write_row(row);
        }
    }
};

}#pragma once
// #include <criterion/details/csv2.hpp>
#include <iomanip>
#include <sstream>
#include <string>
#include <map>
// #include <criterion/details/benchmark_result.hpp>
#include <string>

namespace criterion {

struct benchmark_result {
  std::string name;
  std::size_t num_warmup_runs;
  std::size_t num_runs;
  std::size_t num_iterations;
  long double best_estimate_mean;
  long double best_estimate_rsd;
  long double overall_best_execution_time;
  long double overall_worst_execution_time;
};

}

namespace criterion {

class csv_writer {

  static std::string duration_to_string(long double duration) {
    std::stringstream os;
    os << std::fixed << std::showpoint << std::setprecision(3) << duration;
    return os.str();
  }

  static std::vector<std::string> to_csv_row(const benchmark_result& result) {
    return std::vector<std::string> {
      "\"" + result.name + "\"",
      std::to_string(result.num_warmup_runs),
      std::to_string(result.num_runs),
      std::to_string(result.num_iterations),
      // nanoseconds
      duration_to_string(result.best_estimate_mean),
      duration_to_string(result.best_estimate_rsd),
      duration_to_string(result.overall_best_execution_time),
      duration_to_string(result.overall_worst_execution_time),
    };
  }

public:
  static bool write_results(const std::string& filename, const std::map<std::string, benchmark_result>& results) {
    bool result{false};

    std::ofstream stream(filename);

    using namespace csv2;
    Writer<delimiter<','>> writer(stream);
    if (stream.is_open()) {
      result = true;

      writer.write_row(std::vector<std::string>{
        "\"Name\"",
        "\"Warmup Runs\"",
        "\"Benchmark Runs\"",
        "\"Iterations per Run\"",
        "\"Best Estimate Mean (ns)\"",
        "\"Best Estimate RSD (%)\"",
        "\"Overall Best Execution Time (ns)\"",
        "\"Overall Worst Execution Time (ns)\""
      });

      for (const auto& kvpair: results) {
        writer.write_row(to_csv_row(kvpair.second));
      }
    }

    stream.close();
    return result;
  }
};

}#pragma once
#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <string_view>

namespace criterion {

struct benchmark_config {
  static inline std::tuple<> empty_tuple{};
  std::string name;
  using Fn = std::function<void(std::chrono::steady_clock::time_point&, // start time stamp
                                std::optional<std::chrono::steady_clock::time_point>&, // teardown time stamp
                                void * parameters)>; // benchmark parameters
  Fn fn;
  std::string parameterized_instance_name = ""; 
  void * parameters = (void *)(&empty_tuple);

  enum class benchmark_reporting_type {
    console
  };
  benchmark_reporting_type reporting_type = benchmark_reporting_type::console;
};

}#pragma once
#include <string>

namespace criterion {

struct benchmark_result {
  std::string name;
  std::size_t num_warmup_runs;
  std::size_t num_runs;
  std::size_t num_iterations;
  long double best_estimate_mean;
  long double best_estimate_rsd;
  long double overall_best_execution_time;
  long double overall_worst_execution_time;
};

}#pragma once
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <csignal>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <optional>
#include <string>
#include <sstream>
#include <thread>
#include <utility>

// #include <criterion/details/benchmark_config.hpp>
// #include <criterion/details/benchmark_result.hpp>
// #include <criterion/details/indicators.hpp>

namespace criterion {

class benchmark {
  benchmark_config config_;
  using Fn = benchmark_config::Fn;

  std::size_t warmup_runs_{3};
  std::size_t num_iterations_{0};
  std::size_t max_num_runs_{0};

  long double estimate_minimum_measurement_cost() {
    using namespace std::chrono;
    std::vector<long double> durations;

    for (std::size_t i = 0; i < 10; i++) {
      const auto start = steady_clock::now();
      // do nothing
      const auto end = steady_clock::now();
      const auto execution_time = static_cast<long double>(duration_cast<std::chrono::nanoseconds>(end - start).count());
      durations.push_back(execution_time);
    }
    return *std::min_element(durations.begin(), durations.end());
  }

  long double estimate_execution_time() {
    using namespace std::chrono;

    long double result;
    bool first_run{true};
    for (std::size_t i = 0; i < warmup_runs_; i++) {
      std::chrono::steady_clock::time_point start_timestamp;
      std::optional<std::chrono::steady_clock::time_point> teardown_timestamp;
      const auto start = steady_clock::now();
      config_.fn(start_timestamp, teardown_timestamp, config_.parameters);
      const auto end = steady_clock::now();
      const auto execution_time = static_cast<long double>(duration_cast<std::chrono::nanoseconds>(end - start).count());
      if (first_run) {
        result = execution_time;
        first_run = false;
      }
      else {
        result = std::min(execution_time, result);
      }
    }
    return result;
  }

  void update_iterations() {
    auto early_estimate_execution_time = estimate_execution_time();

    if (early_estimate_execution_time < 1)
      early_estimate_execution_time = 1;

    num_iterations_ = 10; // fixed
    const auto min_runs = 10;
    const auto min_benchmark_time = early_estimate_execution_time * min_runs * num_iterations_;
    const auto ten_seconds = 1e+10;
    const auto two_seconds = 2e+9;

    auto max_benchmark_time = ten_seconds;
    if (early_estimate_execution_time < 1000) { // nanoseconds
      max_benchmark_time = two_seconds;
    }

    const auto benchmark_time = std::max(double(min_benchmark_time), max_benchmark_time);
    const auto total_iterations = size_t(benchmark_time) / early_estimate_execution_time;

    max_num_runs_ = std::max(size_t(total_iterations / num_iterations_), size_t(min_runs));

    max_num_runs_ = std::min(max_num_runs_, size_t(1E7)); // no more than 1E7 runs, don't need it
  }

  std::string duration_to_string(const long double& ns) {
    std::stringstream os;
    if (ns < 1E3) {
      os << std::setprecision(3) << ns << "ns";
    }
    else if (ns < 1E6) {
      os << std::setprecision(3) << (ns / 1E3) << "us";
    }
    else if (ns < 1E9) {
      os << std::setprecision(3) << (ns / 1E6) << "ms";
    }
    else {
      os << std::setprecision(3) << (ns / 1E9) << "s";
    }
    return os.str();
  }

public:

  benchmark(const benchmark_config& config): 
    config_(config) {}

  static inline std::map<std::string, benchmark_result> results;

  void run() {

    using namespace std::chrono;

    // run empty function to estimate minimum delay in scheduling and executing user function
    const auto estimated_minimum_measurement_cost = estimate_minimum_measurement_cost();

    const std::string prefix = config_.name + config_.parameterized_instance_name + " ";
    // std::cout << termcolor::bold << termcolor::yellow << prefix << termcolor::reset << "\n";

    using namespace indicators;

    show_console_cursor(false);

    // Get an early estimate for execution time
    // Update number of iterations to run for this benchmark based on estimate
    update_iterations();

    ProgressBar spinner{
      option::PrefixText{prefix},
      option::BarWidth{10},
      option::Lead{"■"},
      option::Fill{"■"},
      option::Remainder{"-"},
      option::ForegroundColor{Color::white},
      option::FontStyles{std::vector<FontStyle>{FontStyle::bold}},
      // option::ShowSpinner{false},
      option::ShowElapsedTime{true},
      option::ShowRemainingTime{true}
    };

    spinner.set_option(option::MaxProgress{max_num_runs_});

    long double lowest_rsd = 100;
    long double best_estimate_mean = 0;
    bool first_run{true};

    long double overall_best_execution_time = 0;
    long double overall_worst_execution_time = 0;

    std::size_t num_runs = 0;
    spinner.set_progress(num_runs);

    std::array<long double, 10> durations;

    while(true) {
      // Benchmark runs
      for (std::size_t i = 0; i < num_iterations_; i++) {
        std::optional<std::chrono::steady_clock::time_point> teardown_timestamp;
        auto start = steady_clock::now();
        config_.fn(start, teardown_timestamp, config_.parameters);
        auto end = steady_clock::now();
        if (teardown_timestamp)
          end = teardown_timestamp.value();
        const auto execution_time = duration_cast<std::chrono::nanoseconds>(end - start).count();
        durations[i] = std::abs(execution_time - estimated_minimum_measurement_cost);
      }
      auto size = num_iterations_;
      const long double mean = std::accumulate(durations.begin(), durations.end(), 0.0) / size;

      long double E = 0;
      for (std::size_t i = 0; i < size; i++) {
        E += std::pow(durations[i] - mean, 2);
      }
      const long double variance = E / size;
      const long double standard_deviation = std::sqrt(variance);
      const long double relative_standard_deviation = standard_deviation * 100 / mean;

      if (first_run) {
        lowest_rsd = relative_standard_deviation;
        best_estimate_mean = mean;
        overall_best_execution_time = *std::min_element(durations.begin(), durations.end());
        overall_worst_execution_time = *std::max_element(durations.begin(), durations.end());
        first_run = false;
      }
      else {
        // Save record of lowest RSD
        const auto current_lowest_rsd = lowest_rsd;
        lowest_rsd = std::min(relative_standard_deviation, lowest_rsd);
        if (lowest_rsd < current_lowest_rsd) {
          // There's a new LOWEST relative standard deviation

          if (mean < best_estimate_mean) {
            best_estimate_mean = mean; // new mean is lower
          } 
          else {
            lowest_rsd = current_lowest_rsd; // go back to old estimate
          }
        } else {
          lowest_rsd = current_lowest_rsd; // go back to old estimate
        }

        // Save best and worst duration
        overall_best_execution_time = std::min(overall_best_execution_time, *std::min_element(durations.begin(), durations.end()));
        overall_worst_execution_time = std::max(overall_worst_execution_time, *std::min_element(durations.begin(), durations.end()));
      }

      spinner.set_progress(num_runs);

      // Show iteration as postfix text
      // std::stringstream os;
      // os << "[" << num_runs * num_iterations_ << "/" << max_num_runs_ * num_iterations_ << "]";
      // spinner.set_option(option::PostfixText{os.str()});

      if (num_runs >= max_num_runs_) {
        break;
      }

      num_runs += 1;
    }

    std::cout 
      << termcolor::bold 
      << termcolor::green 
      << std::setprecision(3)
      << "    "
      << duration_to_string(best_estimate_mean) 
      << " ± " << lowest_rsd << "%"
      << " {Best: "
      << duration_to_string(overall_best_execution_time) << ", Worst: "
      << duration_to_string(overall_worst_execution_time)
      << "}\n\n"
      << termcolor::reset;

    results.insert(std::make_pair(
      prefix,
      benchmark_result {
        .name = config_.name + config_.parameterized_instance_name,
        .num_warmup_runs = warmup_runs_,
        .num_runs = max_num_runs_,
        .num_iterations = num_iterations_,
        .best_estimate_mean = best_estimate_mean,
        .best_estimate_rsd = lowest_rsd,
        .overall_best_execution_time = overall_best_execution_time,
        .overall_worst_execution_time = overall_worst_execution_time
      }
    ));

    indicators::show_console_cursor(true);
  }
};

}#pragma once
// #include <criterion/details/benchmark.hpp>
// #include <criterion/details/benchmark_config.hpp>
// #include <criterion/details/csv_writer.hpp>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <string>

namespace criterion {

struct benchmark_registration_helper_struct {
  static std::vector<benchmark_config> &
  registered_benchmarks() {
    static std::vector<benchmark_config> v;
    return v;
  }

  static void register_benchmark(const benchmark_config& config) {
    registered_benchmarks().push_back(config);
  }

  static void execute_registered_benchmarks() {
    for (const auto& config : registered_benchmarks()) {
      benchmark{config}.run();
    }
  }
};

}

#define SETUP_BENCHMARK(...)                                                   \
  __VA_ARGS__                                                                  \
  __benchmark_start_timestamp =                                                \
      std::chrono::steady_clock::now(); // updated benchmark start timestamp

#define TEARDOWN_BENCHMARK(...)                                                \
  __benchmark_teardown_timestamp = std::chrono::steady_clock::now();           \
  __VA_ARGS__

namespace criterion {

struct benchmark_template_registration_helper_struct {
  static std::unordered_multimap<std::string, benchmark_config> & registered_benchmark_templates() {
    static std::unordered_multimap<std::string, benchmark_config> m;
    return m;
  }

  static void register_benchmark_template(const benchmark_config& config) {
    registered_benchmark_templates().insert({config.name, config});
  }

  template <class ArgTuple>
  static void execute_registered_benchmark_template(const std::string& template_name, const std::string& instance_name, ArgTuple& arg_tuple) {
    for (auto& [k, v] : registered_benchmark_templates()) {
      if (k == template_name) {
        benchmark_registration_helper_struct::register_benchmark(
          benchmark_config{ 
            .name = template_name, 
            .fn = v.fn,
            .parameterized_instance_name = instance_name,
            .parameters = (void *)(&arg_tuple)
          }
        );
      }
    }
  }
};

}

#define CONCAT_IMPL(a, b) a##b
#define CONCAT(a, b) CONCAT_IMPL(a, b)

#define BENCHMARK_WITHOUT_PARAMETERS(Name) \
  typedef std::tuple<> CONCAT(Name, BenchmarkParameters); \
  namespace detail {                                                           \
  /* forward declare the benchmark function that we define later */            \
  template <class T = CONCAT(Name, BenchmarkParameters)> \
  struct CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__)) {                    \
    static void CONCAT(Name, CONCAT(_registered_fun_, __LINE__))(                            \
        std::chrono::steady_clock::time_point &,                               \
        std::optional<std::chrono::steady_clock::time_point> &, \
        void *);       \
  };                                                                           \
                                                                               \
  namespace /* ensure internal linkage for struct */                           \
  {                                                                            \
  /* helper struct for static registration in ctor */                          \
  struct CONCAT(Name, CONCAT(_register_struct_, __LINE__)) {                                 \
    CONCAT(Name, CONCAT(_register_struct_, __LINE__))() { /* called once before main */      \
      criterion::benchmark_template_registration_helper_struct::register_benchmark_template(criterion::benchmark_config{                            \
          .name = #Name,                                                        \
          .fn = CONCAT(Name, CONCAT(__benchmark_function_wrapper__,                         \
                       __LINE__))<CONCAT(Name, BenchmarkParameters)>::CONCAT(Name, CONCAT(_registered_fun_, __LINE__))});        \
    }                                                                          \
  } CONCAT(Name, CONCAT(_register_struct_instance_, __LINE__));                              \
  }                                                                            \
  \
  namespace /* ensure internal linkage for struct */                           \
  {                                                                            \
  static CONCAT(Name, BenchmarkParameters) CONCAT(CONCAT(Name, _benchmark_template_parameters), __LINE__) = {}; \
  /* helper struct for static registration in ctor */                          \
  struct CONCAT(Name, CONCAT(_instantiation_struct_, __LINE__)) {                                 \
    CONCAT(Name, CONCAT(_instantiation_struct_, __LINE__))() { /* called once before main */      \
      criterion::benchmark_template_registration_helper_struct::execute_registered_benchmark_template<CONCAT(Name, BenchmarkParameters)>(#Name, "", CONCAT(CONCAT(Name, _benchmark_template_parameters), __LINE__)); \
    }                                                                          \
  } CONCAT(Name, CONCAT(_instantiation_struct_instance_, __LINE__));                              \
  } \
  }                                                                            \
                                                                               \
  /* now actually defined to allow BENCHMARK("name") { ... } syntax */         \
  template <class T> \
  void detail::CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__))<T>::CONCAT(Name, CONCAT(       \
      _registered_fun_, __LINE__))(                                             \
      [[maybe_unused]] std::chrono::steady_clock::time_point &                 \
          __benchmark_start_timestamp,                                         \
      [[maybe_unused]] std::optional<std::chrono::steady_clock::time_point> &  \
          __benchmark_teardown_timestamp, \
      [[maybe_unused]] void * __benchmark_parameters)


#define BENCHMARK_WITH_PARAMETERS(Name, ...) \
  typedef std::tuple<__VA_ARGS__> CONCAT(Name, BenchmarkParameters); \
  namespace detail {                                                           \
  /* forward declare the benchmark function that we define later */            \
  template <class T = CONCAT(Name, BenchmarkParameters)> \
  struct CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__)) {                    \
    static void CONCAT(Name, CONCAT(_registered_fun_, __LINE__))(                            \
        std::chrono::steady_clock::time_point &,                               \
        std::optional<std::chrono::steady_clock::time_point> &, \
        void *);       \
  };                                                                           \
                                                                               \
  namespace /* ensure internal linkage for struct */                           \
  {                                                                            \
  /* helper struct for static registration in ctor */                          \
  struct CONCAT(Name, CONCAT(_register_struct_, __LINE__)) {                                 \
    CONCAT(Name, CONCAT(_register_struct_, __LINE__))() { /* called once before main */      \
      criterion::benchmark_template_registration_helper_struct::register_benchmark_template(criterion::benchmark_config{                            \
          .name = #Name,                                                        \
          .fn = CONCAT(Name, CONCAT(__benchmark_function_wrapper__,                         \
                       __LINE__))<CONCAT(Name, BenchmarkParameters)>::CONCAT(Name, CONCAT(_registered_fun_, __LINE__))});        \
    }                                                                          \
  } CONCAT(Name, CONCAT(_register_struct_instance_, __LINE__));                              \
  }                                                                            \
  }                                                                            \
                                                                               \
  /* now actually defined to allow BENCHMARK("name") { ... } syntax */         \
  template <class T> \
  void detail::CONCAT(Name, CONCAT(__benchmark_function_wrapper__, __LINE__))<T>::CONCAT(Name, CONCAT(       \
      _registered_fun_, __LINE__))(                                             \
      [[maybe_unused]] std::chrono::steady_clock::time_point &                 \
          __benchmark_start_timestamp,                                         \
      [[maybe_unused]] std::optional<std::chrono::steady_clock::time_point> &  \
          __benchmark_teardown_timestamp, \
      [[maybe_unused]] void * __benchmark_parameters)

#define GET_ARGUMENT_TUPLE \
  *((T *)__benchmark_parameters)

#define GET_ARGUMENT(index) \
  std::get<index>(*((T *)__benchmark_parameters));

#define BENCHMARK_1(Name) \
  BENCHMARK_WITHOUT_PARAMETERS(Name)

#define BENCHMARK_2(Name, A) BENCHMARK_WITH_PARAMETERS(Name, A)
#define BENCHMARK_3(Name, A, B) BENCHMARK_WITH_PARAMETERS(Name, A, B)
#define BENCHMARK_4(Name, A, B, C) BENCHMARK_WITH_PARAMETERS(Name, A, B, C)
#define BENCHMARK_5(Name, A, B, C, D) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D)
#define BENCHMARK_6(Name, A, B, C, D, E) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F)
#define BENCHMARK_7(Name, A, B, C, D, E, F) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F)
#define BENCHMARK_8(Name, A, B, C, D, E, F, G) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G)
#define BENCHMARK_9(Name, A, B, C, D, E, F, G, H) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H)
#define BENCHMARK_10(Name, A, B, C, D, E, F, G, H, I) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I)
#define BENCHMARK_11(Name, A, B, C, D, E, F, G, H, I, J) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J)
#define BENCHMARK_12(Name, A, B, C, D, E, F, G, H, I, J, K) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K)
#define BENCHMARK_13(Name, A, B, C, D, E, F, G, H, I, J, K, L) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L)
#define BENCHMARK_14(Name, A, B, C, D, E, F, G, H, I, J, K, L, M) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M)
#define BENCHMARK_15(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N)
#define BENCHMARK_16(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O)
#define BENCHMARK_17(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P)
#define BENCHMARK_18(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q)
#define BENCHMARK_19(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R)
#define BENCHMARK_20(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S)
#define BENCHMARK_21(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T)
#define BENCHMARK_22(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U)
#define BENCHMARK_23(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V)
#define BENCHMARK_24(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W)
#define BENCHMARK_25(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X)
#define BENCHMARK_26(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y)
#define BENCHMARK_27(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z) BENCHMARK_WITH_PARAMETERS(Name, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z)

// The interim macro that simply strips the excess and ends up with the required macro
#define BENCHMARK_X(Name,x,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,FUNC, ...) FUNC

// BENCHMARK macro supports no more than 26 parameters
// The macro that the programmer uses 
#define BENCHMARK(...) \
  BENCHMARK_X(,##__VA_ARGS__,\
    BENCHMARK_27(__VA_ARGS__),\
    BENCHMARK_26(__VA_ARGS__),\
    BENCHMARK_25(__VA_ARGS__),\
    BENCHMARK_24(__VA_ARGS__),\
    BENCHMARK_23(__VA_ARGS__),\
    BENCHMARK_22(__VA_ARGS__),\
    BENCHMARK_21(__VA_ARGS__),\
    BENCHMARK_20(__VA_ARGS__),\
    BENCHMARK_19(__VA_ARGS__),\
    BENCHMARK_18(__VA_ARGS__),\
    BENCHMARK_17(__VA_ARGS__),\
    BENCHMARK_16(__VA_ARGS__),\
    BENCHMARK_15(__VA_ARGS__),\
    BENCHMARK_14(__VA_ARGS__),\
    BENCHMARK_13(__VA_ARGS__),\
    BENCHMARK_12(__VA_ARGS__),\
    BENCHMARK_11(__VA_ARGS__),\
    BENCHMARK_10(__VA_ARGS__),\
    BENCHMARK_9(__VA_ARGS__),\
    BENCHMARK_8(__VA_ARGS__),\
    BENCHMARK_7(__VA_ARGS__),\
    BENCHMARK_6(__VA_ARGS__),\
    BENCHMARK_5(__VA_ARGS__),\
    BENCHMARK_4(__VA_ARGS__),\
    BENCHMARK_3(__VA_ARGS__),\
    BENCHMARK_2(__VA_ARGS__),\
    BENCHMARK_1(__VA_ARGS__) \
    )

#define REGISTER_BENCHMARK(TemplateName, InstanceName, ...) \
  \
  namespace /* ensure internal linkage for struct */                           \
  {                                                                            \
  static CONCAT(TemplateName, BenchmarkParameters) CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__) = {__VA_ARGS__}; \
  /* helper struct for static registration in ctor */                          \
  struct CONCAT(TemplateName, CONCAT(_instantiation_struct_, __LINE__)) {                                 \
    CONCAT(TemplateName, CONCAT(_instantiation_struct_, __LINE__))() { /* called once before main */      \
      criterion::benchmark_template_registration_helper_struct::execute_registered_benchmark_template<CONCAT(TemplateName, BenchmarkParameters)>(#TemplateName, InstanceName, CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__)); \
    }                                                                          \
  } CONCAT(TemplateName, CONCAT(_instantiation_struct_instance_, __LINE__));                              \
  }

#define GET_FIRST(first, ...) first
#define GET_REST(first, ...) __VA_ARGS__

#define REGISTER_BENCHMARK_N(TemplateName, Index, PackedArgument) \
  \
  namespace /* ensure internal linkage for struct */                           \
  {                                                                            \
  static CONCAT(TemplateName, BenchmarkParameters) CONCAT(CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__), Index) = {GET_REST(PackedArgument)}; \
  /* helper struct for static registration in ctor */                          \
  struct CONCAT(TemplateName, CONCAT(CONCAT(_instantiation_struct_, __LINE__), Index)) {                                 \
    CONCAT(TemplateName, CONCAT(CONCAT(_instantiation_struct_, __LINE__), Index)()) { /* called once before main */      \
      criterion::benchmark_template_registration_helper_struct::execute_registered_benchmark_template<CONCAT(TemplateName, BenchmarkParameters)>(#TemplateName, GET_FIRST(PackedArgument), CONCAT(CONCAT(CONCAT(TemplateName, _benchmark_template_parameters), __LINE__), Index)); \
    }                                                                          \
  } CONCAT(TemplateName, CONCAT(CONCAT(_instantiation_struct_instance_, __LINE__), Index));                              \
  }


#define _Args(...) __VA_ARGS__
#define STRIP_PARENS(X) X
#define PASS_PARAMETERS(X) STRIP_PARENS( _Args X )

#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

#define FOR_EACH_1(what, first, x, ...) what(1, first, x)
#define FOR_EACH_2(what, first, x, ...)\
  what(2, first, x);\
  FOR_EACH_1(what, first, __VA_ARGS__);
#define FOR_EACH_3(what, first, x, ...)\
  what(3, first, x);\
  FOR_EACH_2(what, first, __VA_ARGS__);
#define FOR_EACH_4(what, first, x, ...)\
  what(4, first, x);\
  FOR_EACH_3(what, first, __VA_ARGS__);
#define FOR_EACH_5(what, first, x, ...)\
  what(5, first, x);\
 FOR_EACH_4(what, first, __VA_ARGS__);
#define FOR_EACH_6(what, first, x, ...)\
  what(6, first, x);\
  FOR_EACH_5(what, first, __VA_ARGS__);
#define FOR_EACH_7(what, first, x, ...)\
  what(7, first, x);\
  FOR_EACH_6(what, first, __VA_ARGS__);
#define FOR_EACH_8(what, first, x, ...)\
  what(8, first, x);\
  FOR_EACH_7(what, first, __VA_ARGS__);

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__) 
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N 
#define FOR_EACH_RSEQ_N() 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, first, x, ...) CONCATENATE(FOR_EACH_, N)(what, first, x, __VA_ARGS__)
#define FOR_EACH(what, first, x, ...) FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, first, x, __VA_ARGS__)

#define REGISTER_BENCHMARK_FOR_EACH_HELPER(Index, TemplateName, ...) \
  REGISTER_BENCHMARK_N(TemplateName, Index, PASS_PARAMETERS(__VA_ARGS__))

#define REGISTER_BENCHMARK_FOR_EACH(TemplateName, ...) \
  FOR_EACH(REGISTER_BENCHMARK_FOR_EACH_HELPER, TemplateName, __VA_ARGS__)#pragma once
// #include <criterion/details/indicators.hpp>
// #include <criterion/details/macros.hpp>

static inline void signal_handler(int signal) {
  indicators::show_console_cursor(true);
  std::cout << termcolor::reset;
  exit(signal);
}

#define CRITERION_BENCHMARK_MAIN                                                         \
  int main() {                                                                 \
    std::signal(SIGTERM, signal_handler);                                      \
    std::signal(SIGSEGV, signal_handler);                                      \
    std::signal(SIGINT, signal_handler);                                       \
    std::signal(SIGILL, signal_handler);                                       \
    std::signal(SIGABRT, signal_handler);                                      \
    std::signal(SIGFPE, signal_handler);                                       \
    criterion::benchmark_registration_helper_struct::execute_registered_benchmarks();                                           \
    criterion::csv_writer::write_results("results.csv", criterion::benchmark::results); \
  }