// ============================================================
//  block.h — هيكل البلوك الواحد في السلسلة
//  كل بلوك يحتوي على: فهرس، وقت، بيانات، هاش سابق، هاش حالي
// ============================================================
#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <ctime>
#include <sstream>
#include <iostream>
#include "sha256.h"

class Block {
public:
    int         index;          // رقم البلوك في السلسلة
    std::string timestamp;      // وقت إنشاء البلوك
    std::string data;           // البيانات (مثلاً: معاملة مالية)
    std::string previousHash;   // هاش البلوك السابق (الرابط بين البلوكات)
    std::string hash;           // هاش هذا البلوك
    int         nonce;          // الرقم العشوائي المستخدم في التعدين
    int         difficulty;     // مستوى الصعوبة (عدد الأصفار المطلوبة)

    // ───── المُنشئ (Constructor) ─────
    Block(int idx, const std::string& blockData, const std::string& prevHash, int diff = 4)
        : index(idx), data(blockData), previousHash(prevHash),
          nonce(0), difficulty(diff)
    {
        timestamp = getCurrentTime();
        hash = calculateHash();
    }

    // ───── حساب الهاش ─────
    // يجمع كل بيانات البلوك في نص واحد ثم يحسب SHA-256
    std::string calculateHash() const {
        std::stringstream ss;
        ss << index << timestamp << data << previousHash << nonce;
        return SHA256::hash(ss.str());
    }

    // ───── التعدين (Mining / Proof of Work) ─────
    // يبحث عن هاش يبدأ بعدد معين من الأصفار
    // كلما زادت الصعوبة، زاد الوقت المطلوب
    void mineBlock() {
        std::string target(difficulty, '0');  // مثلاً difficulty=4 → "0000"

        std::cout << "\n  [⛏] جاري تعدين البلوك #" << index << " ..." << std::endl;

        while (hash.substr(0, difficulty) != target) {
            nonce++;                    // جرب رقم جديد
            hash = calculateHash();    // احسب الهاش مرة ثانية
        }

        std::cout << "  [✓] تم التعدين! Nonce = " << nonce << std::endl;
        std::cout << "  [#] Hash = " << hash << std::endl;
    }

    // ───── طباعة معلومات البلوك ─────
    void print() const {
        std::cout << "\n  ┌─────────────────────────────────────────────────────────────────────┐" << std::endl;
        std::cout << "  │ Block #" << index << std::string(61 - std::to_string(index).size(), ' ') << "│" << std::endl;
        std::cout << "  ├─────────────────────────────────────────────────────────────────────┤" << std::endl;
        std::cout << "  │ Timestamp  : " << timestamp << std::string(55 - timestamp.size(), ' ') << "│" << std::endl;
        std::cout << "  │ Data       : " << data << std::string(55 - std::min(data.size(), (size_t)55), ' ') << "│" << std::endl;
        std::cout << "  │ Nonce      : " << nonce << std::string(55 - std::to_string(nonce).size(), ' ') << "│" << std::endl;
        std::cout << "  │ Difficulty : " << difficulty << std::string(55 - std::to_string(difficulty).size(), ' ') << "│" << std::endl;
        std::cout << "  │ Prev Hash  : " << previousHash.substr(0, 55) << std::string(55 - std::min(previousHash.size(), (size_t)55), ' ') << "│" << std::endl;
        std::cout << "  │ Hash       : " << hash.substr(0, 55) << std::string(55 - std::min(hash.size(), (size_t)55), ' ') << "│" << std::endl;
        std::cout << "  └─────────────────────────────────────────────────────────────────────┘" << std::endl;
    }

private:
    // الحصول على الوقت الحالي كنص
    std::string getCurrentTime() const {
        std::time_t now = std::time(nullptr);
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return std::string(buf);
    }
};

#endif // BLOCK_H
