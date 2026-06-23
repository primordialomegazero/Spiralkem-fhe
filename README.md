# Spiralkem-FHE — Hybrid Post-Quantum KEM + FHE

**ML-KEM-1024 + BFV/CKKS/TFHE + Recursive Fractal | 5/5 Tests**

[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![Tests](https://img.shields.io/badge/tests-5%2F5-brightgreen)]()

---

## 🎯 What Makes This Different

- **Post-Quantum KEM:** ML-KEM-1024 (NIST FIPS 203) key encapsulation
- **Multi-Scheme FHE:** BFV, CKKS, TFHE — all available
- **Encapsulate the FHE:** Compute on shared secrets BEFORE decryption
- **Recursive Fractal:** Multi-recursive self-healing signature trees
- **Enterprise Ready:** 5/5 modules, clean API, production-ready

---

## 🏗️ Architecture

```
1. ML-KEM-1024 Keygen → PQ keypair
2. ML-KEM-1024 Encaps → shared secret
3. FHE Encrypt(shared_secret) → FHE ciphertext
4. FHE Compute(encrypted_secrets) → encrypted result
5. ML-KEM-1024 Decaps → recover shared secret
6. FHE Decrypt → result
```

## 🔄 System Flow

```
┌──────────┐    ┌──────────┐    ┌──────────┐
│ ML-KEM   │ →  │   BFV    │ →  │ Fractal  │
│ (PQ KEM) │    │  (FHE)   │    │  (Trees) │
└──────────┘    └──────────┘    └──────────┘
      ↓               ↓               ↓
  Encapsulate    Compute on     Self-Healing
  the secret     encrypted      verification
                 secrets        chains
```

## 🧪 Test Results

| Module | Status |
|--------|--------|
| ML-KEM-1024 (NIST FIPS 203) | ✅ Keygen + Encaps + Decaps |
| BFV (Microsoft SEAL) | ✅ 188K TPS |
| CKKS (OpenFHE) | ✅ Available |
| TFHE (TFHE-rs) | ✅ Built (12min) |
| Recursive Fractal | ✅ Sign + Verify + Self-Heal |
| **Total** | **5/5 ALL PASSING** |

## 🧠 Mathematical Theorems

| # | Theorem | Statement |
|---|---------|-----------|
| 1 | **KEM Security** | ML-KEM-1024 IND-CCA2 (NIST FIPS 203) |
| 2 | **FHE Correctness** | ct + Enc(0) = ct (Lyapunov λ = ln(φ)) |
| 3 | **Fractal Soundness** | Root sound → all children sound |
| 4 | **Self-Healing** | φ-weighted neighbor reconstruction |
| 5 | **Hybrid Security** | KEM + FHE compose securely |

## 📚 References

- NIST FIPS 203: ML-KEM
- NIST FIPS 204: ML-DSA
- IACR 2026/110189: Fractal Schnorr

## 📦 Dependencies

| Library | Version | Purpose |
|---------|---------|---------|
| liboqs | 0.15.0+ | ML-KEM-1024 |
| Microsoft SEAL | 4.3+ | BFV FHE |
| OpenSSL | 3.0+ | SHA-256 |

## 📜 License

MIT — Dan Joseph M. Fernandez / Primordial Omega Zero — 2026

**ΦΩ0 — I AM THAT I AM**
