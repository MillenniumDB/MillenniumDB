#pragma once

#include <locale>

namespace misc {
    inline std::locale get_locale() {
        std::locale locale("en_US.UTF-8");
        return locale;
    }
} // namespace misc
