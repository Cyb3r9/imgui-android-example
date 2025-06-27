/* ---------------------------- 🌸 FLORA OBFUSCATOR v3.1 💞 -------------------
 * Fixed version that properly handles CURL and string operations
 * https://t.me/@niggapie
 */

#pragma once
#include <algorithm>
#include <string>
#include <type_traits>

// 🌸 Compile-time magic
#if __cplusplus >= 202002L
    #define BUNGA_CONSTEVAL consteval
#else
    #define BUNGA_CONSTEVAL constexpr
#endif

// 🍀 Generate unique key for each flower
#ifndef BUNGA_DEFAULT_NEKTAR
    #define BUNGA_DEFAULT_NEKTAR taman::sari_madu(__LINE__)
#endif

namespace taman {
    using akar = size_t;
    using nektar = uint64_t;

    template <typename T>
    using serbuk = std::remove_cv_t<std::remove_reference_t<T>>;

    // 🌼 Generate sweet nectar from line number
    BUNGA_CONSTEVAL nektar sari_madu(nektar biji) {
        biji ^= biji >> 33;
        biji *= 0xff51afd7ed558ccd;
        biji ^= biji >> 33;
        biji *= 0xc4ceb9fe1a85ec53;
        biji ^= biji >> 33;
        return biji | 0x0101010101010101ull;
    }

    // 🌷 Apply pollen transformation
    template <typename SERBUK>
    constexpr void semprot_serbuk(SERBUK* data, akar size, nektar madu) {
        for (akar i = 0; i < size; i++) {
            data[i] ^= SERBUK((madu >> ((i % 8) * 8)) & 0xFF);
        }
    }

    // 🌱 Flower bud (compile-time obfuscation)
    template <akar KELOPAK, nektar MADU, typename SERBUK = char>
    class kuncup {
    public:
        BUNGA_CONSTEVAL kuncup(const SERBUK* data) {
            for (akar i = 0; i < KELOPAK; i++) {
                m_serbuk[i] = data[i];
            }
            semprot_serbuk(m_serbuk, KELOPAK, MADU);
        }

        constexpr const SERBUK* serbuk() const noexcept { return m_serbuk; }
        constexpr akar jumlah_kelopak() const noexcept { return KELOPAK; }

    private:
        SERBUK m_serbuk[KELOPAK]{};
    };

    // 🌸 Blooming flower (runtime handling)
    template <akar KELOPAK, nektar MADU, typename SERBUK = char>
    class bunga {
    public:
        bunga(const kuncup<KELOPAK, MADU, SERBUK>& kuncup) {
            for (akar i = 0; i < KELOPAK; i++) {
                m_serbuk[i] = kuncup.serbuk()[i];
            }
        }

        ~bunga() { bersihkan(); }

        // 💐 Get decrypted string (const version)
        std::string ambil_string() const {
            SERBUK temp[KELOPAK];
            std::copy(m_serbuk, m_serbuk + KELOPAK, temp);
            semprot_serbuk(temp, KELOPAK, MADU);
            return std::string(temp, KELOPAK - 1); // Exclude null terminator
        }

        // 💐 Get decrypted C-string (const version)
        const char* ambil_cstring() const {
            thread_local static SERBUK temp[KELOPAK];
            std::copy(m_serbuk, m_serbuk + KELOPAK, temp);
            semprot_serbuk(temp, KELOPAK, MADU);
            return temp;
        }

        // 🌹 Get string length
        constexpr akar panjang() const noexcept { return KELOPAK - 1; }

    private:
        SERBUK m_serbuk[KELOPAK];
        bool m_tertutup{true};

        void bersihkan() {
            volatile SERBUK* p = m_serbuk;
            for (akar i = 0; i < KELOPAK; i++) {
                p[i] = 0;
            }
        }
    };

    // 🌻 Create flower bud from string literal
    template <akar KELOPAK, nektar MADU = BUNGA_DEFAULT_NEKTAR, typename SERBUK = char>
    BUNGA_CONSTEVAL auto tanam_kuncup(const SERBUK(&data)[KELOPAK]) {
        return kuncup<KELOPAK, MADU, SERBUK>(data);
    }
}

// 💮 Main macro for flower creation
#define sakura(data) \
    []() -> auto& { \
        using serbuk_t = taman::serbuk<decltype(*data)>; \
        constexpr auto kelopak = sizeof(data)/sizeof(data[0]); \
        constexpr auto kuncup = taman::tanam_kuncup<kelopak, BUNGA_DEFAULT_NEKTAR, serbuk_t>(data); \
        thread_local auto bunga = taman::bunga<kelopak, BUNGA_DEFAULT_NEKTAR, serbuk_t>(kuncup); \
        return bunga; \
    }()



/*         
MIT License

Copyright (c) Meta Platforms, Inc. and affiliates.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

