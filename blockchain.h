// ============================================================
//  blockchain.h — سلسلة الكتل (Blockchain)
//  تجمع البلوكات مع بعض + التحقق من صحة السلسلة
// ============================================================
#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <vector>
#include <iostream>
#include "block.h"

class Blockchain {
public:
    int difficulty;  // صعوبة التعدين لكل السلسلة

    // ───── المُنشئ: إنشاء بلوك التكوين (Genesis Block) ─────
    Blockchain(int diff = 4) : difficulty(diff) {
        std::cout << "\n  ╔═══════════════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "  ║                    🔗 Mini Blockchain — C++                          ║" << std::endl;
        std::cout << "  ║                    Difficulty: " << difficulty << " zeros                               ║" << std::endl;
        std::cout << "  ╚═══════════════════════════════════════════════════════════════════════╝" << std::endl;

        std::cout << "\n  >> Creating Genesis Block (Block #0)..." << std::endl;
        Block genesis(0, "Genesis Block - The Beginning", "0000000000000000000000000000000000000000000000000000000000000000", difficulty);
        genesis.mineBlock();
        chain.push_back(genesis);
    }

    // ───── إضافة بلوك جديد ─────
    void addBlock(const std::string& data) {
        int newIndex = chain.size();
        std::string prevHash = chain.back().hash;

        std::cout << "\n  >> Adding Block #" << newIndex << ": \"" << data << "\"" << std::endl;

        Block newBlock(newIndex, data, prevHash, difficulty);
        newBlock.mineBlock();
        chain.push_back(newBlock);
    }

    // ───── التحقق من صحة السلسلة كاملة ─────
    bool isChainValid() const {
        std::cout << "\n  ╔═══════════════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "  ║                  🔍 Validating Blockchain...                         ║" << std::endl;
        std::cout << "  ╚═══════════════════════════════════════════════════════════════════════╝" << std::endl;

        for (size_t i = 1; i < chain.size(); i++) {
            const Block& current = chain[i];
            const Block& previous = chain[i - 1];

            // فحص 1: هل الهاش المحسوب يطابق الهاش المخزن؟
            if (current.hash != current.calculateHash()) {
                std::cout << "  [✗] Block #" << i << " — Hash mismatch! (data was tampered)" << std::endl;
                return false;
            }

            // فحص 2: هل الهاش السابق يطابق هاش البلوك الي قبله؟
            if (current.previousHash != previous.hash) {
                std::cout << "  [✗] Block #" << i << " — Previous hash mismatch! (chain is broken)" << std::endl;
                return false;
            }

            // فحص 3: هل الهاش يبدأ بالعدد المطلوب من الأصفار؟
            std::string target(difficulty, '0');
            if (current.hash.substr(0, difficulty) != target) {
                std::cout << "  [✗] Block #" << i << " — Proof of Work is invalid!" << std::endl;
                return false;
            }

            std::cout << "  [✓] Block #" << i << " — Valid ✓" << std::endl;
        }

        std::cout << "\n  ══════════════════════════════════════════════" << std::endl;
        std::cout << "  ✅  Blockchain is VALID! All " << chain.size() << " blocks verified." << std::endl;
        std::cout << "  ══════════════════════════════════════════════" << std::endl;
        return true;
    }

    // ───── طباعة كل البلوكات ─────
    void printChain() const {
        std::cout << "\n  ╔═══════════════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "  ║                  📋 Full Blockchain View                             ║" << std::endl;
        std::cout << "  ╚═══════════════════════════════════════════════════════════════════════╝" << std::endl;

        for (const auto& block : chain) {
            block.print();
            if (block.index < (int)chain.size() - 1) {
                std::cout << "  " << std::string(10, ' ') << "⬇ linked by previousHash" << std::endl;
            }
        }
    }

    // ───── محاولة تزوير بلوك (للعرض التعليمي) ─────
    void tamperBlock(int index, const std::string& newData) {
        if (index <= 0 || index >= (int)chain.size()) {
            std::cout << "  [!] Invalid block index for tampering." << std::endl;
            return;
        }

        std::cout << "\n  ╔═══════════════════════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "  ║              ⚠️  TAMPERING with Block #" << index << "...                          ║" << std::endl;
        std::cout << "  ╚═══════════════════════════════════════════════════════════════════════╝" << std::endl;

        std::cout << "  [!] Old data: \"" << chain[index].data << "\"" << std::endl;
        chain[index].data = newData;
        std::cout << "  [!] New data: \"" << chain[index].data << "\"" << std::endl;
        std::cout << "  [!] Hash is now INVALID — the chain will detect this!" << std::endl;
    }

    // الحصول على عدد البلوكات
    int getChainSize() const {
        return chain.size();
    }

private:
    std::vector<Block> chain;  // السلسلة الفعلية
};

#endif // BLOCKCHAIN_H
