// ============================================================
//  sha256.h — SHA-256 هاش كامل بدون مكتبات خارجية
//  Implementation based on FIPS PUB 180-4
// ============================================================
#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <cstring>

class SHA256 {
public:
    // ───── الدالة الرئيسية: أعطيها نص → ترجعلك الهاش ─────
    static std::string hash(const std::string& input) {
        // 1) تجهيز الرسالة (Padding)
        uint8_t* msg = preprocess(input);
        uint64_t msgLen = preprocessedLength(input);

        // 2) القيم الابتدائية (أول 32 بت من الجذور التربيعية لأول 8 أعداد أولية)
        uint32_t h0 = 0x6a09e667, h1 = 0xbb67ae85,
                 h2 = 0x3c6ef372, h3 = 0xa54ff53a,
                 h4 = 0x510e527f, h5 = 0x9b05688c,
                 h6 = 0x1f83d9ab, h7 = 0x5be0cd19;

        // 3) معالجة كل بلوك (512 بت = 64 بايت)
        for (uint64_t i = 0; i < msgLen; i += 64) {
            uint32_t w[64];

            // تقسيم البلوك إلى 16 كلمة (32 بت لكل كلمة)
            for (int j = 0; j < 16; j++) {
                w[j] = (msg[i + j * 4] << 24) |
                       (msg[i + j * 4 + 1] << 16) |
                       (msg[i + j * 4 + 2] << 8) |
                       (msg[i + j * 4 + 3]);
            }

            // توسيع الـ 16 كلمة إلى 64 كلمة
            for (int j = 16; j < 64; j++) {
                uint32_t s0 = rotr(w[j - 15], 7) ^ rotr(w[j - 15], 18) ^ (w[j - 15] >> 3);
                uint32_t s1 = rotr(w[j - 2], 17) ^ rotr(w[j - 2], 19) ^ (w[j - 2] >> 10);
                w[j] = w[j - 16] + s0 + w[j - 7] + s1;
            }

            // متغيرات العمل
            uint32_t a = h0, b = h1, c = h2, d = h3,
                     e = h4, f = h5, g = h6, h = h7;

            // 64 جولة ضغط
            for (int j = 0; j < 64; j++) {
                uint32_t S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
                uint32_t ch = (e & f) ^ (~e & g);
                uint32_t temp1 = h + S1 + ch + K[j] + w[j];
                uint32_t S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
                uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
                uint32_t temp2 = S0 + maj;

                h = g;  g = f;  f = e;
                e = d + temp1;
                d = c;  c = b;  b = a;
                a = temp1 + temp2;
            }

            h0 += a; h1 += b; h2 += c; h3 += d;
            h4 += e; h5 += f; h6 += g; h7 += h;
        }

        delete[] msg;

        // 4) تجميع النتيجة كنص هيكس
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        ss << std::setw(8) << h0 << std::setw(8) << h1
           << std::setw(8) << h2 << std::setw(8) << h3
           << std::setw(8) << h4 << std::setw(8) << h5
           << std::setw(8) << h6 << std::setw(8) << h7;
        return ss.str();
    }

private:
    // ───── ثوابت K (أول 32 بت من الجذور التكعيبية لأول 64 عدد أولي) ─────
    static constexpr uint32_t K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
        0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
        0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
        0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
        0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
        0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
        0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
        0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
        0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
    };

    // دوران يميني (Right Rotate)
    static uint32_t rotr(uint32_t x, int n) {
        return (x >> n) | (x << (32 - n));
    }

    // حساب طول الرسالة بعد التجهيز
    static uint64_t preprocessedLength(const std::string& input) {
        uint64_t len = input.size();
        uint64_t bitLen = len * 8;
        // +1 للبايت 0x80، ثم padding حتى يصبح الطول ≡ 56 (mod 64)، ثم +8 للطول
        uint64_t padded = len + 1;
        while (padded % 64 != 56) padded++;
        return padded + 8;
    }

    // تجهيز الرسالة: إضافة padding + طول الرسالة الأصلي
    static uint8_t* preprocess(const std::string& input) {
        uint64_t len = input.size();
        uint64_t bitLen = len * 8;
        uint64_t paddedLen = preprocessedLength(input);

        uint8_t* msg = new uint8_t[paddedLen];
        std::memset(msg, 0, paddedLen);
        std::memcpy(msg, input.c_str(), len);

        msg[len] = 0x80; // إضافة بت 1

        // إضافة الطول الأصلي كـ 64-بت big-endian في آخر 8 بايت
        for (int i = 0; i < 8; i++) {
            msg[paddedLen - 1 - i] = (uint8_t)(bitLen >> (i * 8));
        }

        return msg;
    }
};

#endif // SHA256_H
