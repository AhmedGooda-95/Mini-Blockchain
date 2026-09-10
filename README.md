# 🔗 Mini Blockchain in C++

A simple educational blockchain implementation built from scratch using **C++**.

This project demonstrates the core concepts behind blockchain technology, including **blocks, SHA-256 hashing, proof-of-work mining, chain validation, previous-hash linking, and tamper detection**.

---

## 🚀 Project Overview

This project implements a basic blockchain where each block contains:

- Block index
- Timestamp
- Transaction/data
- Previous block hash
- Current block hash
- Nonce
- Mining difficulty

Each block is cryptographically linked to the previous block using its hash.

If someone modifies the data inside an existing block, its hash changes, causing the blockchain validation process to detect the tampering.

---

## 🧱 Blockchain Structure

The blockchain follows this structure:

```text
Genesis Block
     ↓
Block #1
     ↓
Block #2
     ↓
Block #3
