// MIT license

#include "httpdiot.h"

#ifdef HTTPDIOT_USE_OPENSSL

#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

#define CRYPTO_FREE_MD(x) if(x) EVP_MD_CTX_free(x)

using namespace httpdiot;

Crypto::Crypto()
{
	m_md_sha1_ctx = EVP_MD_CTX_new();
	m_md_sha224_ctx = EVP_MD_CTX_new();
	m_md_sha384_ctx = EVP_MD_CTX_new();
	m_md_sha512_ctx = EVP_MD_CTX_new();
	m_md_sha512_224_ctx = EVP_MD_CTX_new();
	m_md_sha512_256_ctx = EVP_MD_CTX_new();
	m_md_sha3_224 = EVP_MD_CTX_new();
	m_md_sha3_256 = EVP_MD_CTX_new();
	m_md_sha3_384 = EVP_MD_CTX_new();
	m_md_sha3_512 = EVP_MD_CTX_new();
	m_md_shake_128 = EVP_MD_CTX_new();
	m_md_shake_256 = EVP_MD_CTX_new();
	m_md_md2 = EVP_MD_CTX_new();
	m_md_md4 = EVP_MD_CTX_new();
	m_md_md5 = EVP_MD_CTX_new();
	m_md_md5_sha1 = EVP_MD_CTX_new();
	m_md_blake2b512 = EVP_MD_CTX_new();
	m_md_blake2s256 = EVP_MD_CTX_new();
	m_md_mdc2 = EVP_MD_CTX_new();
	m_md_ripemd160 = EVP_MD_CTX_new();
	m_md_whirlpool = EVP_MD_CTX_new();
	m_md_sm3 = EVP_MD_CTX_new();
}

Crypto::~Crypto()
{
	CRYPTO_FREE_MD(m_md_sha1_ctx);
	CRYPTO_FREE_MD(m_md_sha224_ctx);
	CRYPTO_FREE_MD(m_md_sha384_ctx);
	CRYPTO_FREE_MD(m_md_sha512_ctx);
	CRYPTO_FREE_MD(m_md_sha512_224_ctx);
	CRYPTO_FREE_MD(m_md_sha512_256_ctx);
	CRYPTO_FREE_MD(m_md_sha3_224);
	CRYPTO_FREE_MD(m_md_sha3_256);
	CRYPTO_FREE_MD(m_md_sha3_384);
	CRYPTO_FREE_MD(m_md_sha3_512);
	CRYPTO_FREE_MD(m_md_shake_128);
	CRYPTO_FREE_MD(m_md_shake_256);
	CRYPTO_FREE_MD(m_md_md2);
	CRYPTO_FREE_MD(m_md_md4);
	CRYPTO_FREE_MD(m_md_md5);
	CRYPTO_FREE_MD(m_md_md5_sha1);
	CRYPTO_FREE_MD(m_md_blake2b512);
	CRYPTO_FREE_MD(m_md_blake2s256);
	CRYPTO_FREE_MD(m_md_mdc2);
	CRYPTO_FREE_MD(m_md_ripemd160);
	CRYPTO_FREE_MD(m_md_whirlpool);
	CRYPTO_FREE_MD(m_md_sm3);
}

void Crypto::UpdateDigestSHA1(const void* d, size_t sz){
	EVP_DigestInit_ex(m_md_sha1_ctx, EVP_sha1(), 0);
	EVP_DigestUpdate(m_md_sha1_ctx, d, sz);
}

void Crypto::UpdateDigestSHA224(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha224_ctx, EVP_sha224(), 0);
	EVP_DigestUpdate(m_md_sha224_ctx, d, sz);
}
void Crypto::UpdateDigestSHA384(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha384_ctx, EVP_sha384(), 0);
	EVP_DigestUpdate(m_md_sha384_ctx, d, sz);
}
void Crypto::UpdateDigestSHA512(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha512_ctx, EVP_sha512(), 0);
	EVP_DigestUpdate(m_md_sha512_ctx, d, sz);
}
void Crypto::UpdateDigestSHA512_224(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha512_224_ctx, EVP_sha512_224(), 0);
	EVP_DigestUpdate(m_md_sha512_224_ctx, d, sz);
}
void Crypto::UpdateDigestSHA512_256(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha512_256_ctx, EVP_sha512_256(), 0);
	EVP_DigestUpdate(m_md_sha512_256_ctx, d, sz);
}
void Crypto::UpdateDigestSHA3_224(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha3_224, EVP_sha3_224(), 0);
	EVP_DigestUpdate(m_md_sha3_224, d, sz);
}
void Crypto::UpdateDigestSHA3_256(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha3_256, EVP_sha3_256(), 0);
	EVP_DigestUpdate(m_md_sha3_256, d, sz);
}
void Crypto::UpdateDigestSHA3_384(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha3_384, EVP_sha3_384(), 0);
	EVP_DigestUpdate(m_md_sha3_384, d, sz);
}
void Crypto::UpdateDigestSHA3_512(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sha3_512, EVP_sha3_512(), 0);
	EVP_DigestUpdate(m_md_sha3_512, d, sz);
}
void Crypto::UpdateDigestSHAKE_128(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_shake_128, EVP_shake128(), 0);
	EVP_DigestUpdate(m_md_shake_128, d, sz);
}
void Crypto::UpdateDigestSHAKE_256(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_shake_256, EVP_shake256(), 0);
	EVP_DigestUpdate(m_md_shake_256, d, sz);
}
void Crypto::UpdateDigestMD2(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_md2, EVP_md2(), 0);
	EVP_DigestUpdate(m_md_md2, d, sz);
}
void Crypto::UpdateDigestMD4(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_md4, EVP_md4(), 0);
	EVP_DigestUpdate(m_md_md4, d, sz);
}
void Crypto::UpdateDigestMD5(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_md5, EVP_md5(), 0);
	EVP_DigestUpdate(m_md_md5, d, sz);
}
void Crypto::UpdateDigestMD5_SHA1(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_md5_sha1, EVP_md5_sha1(), 0);
	EVP_DigestUpdate(m_md_md5_sha1, d, sz);
}
void Crypto::UpdateDigestBLAKE2b512(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_blake2b512, EVP_blake2b512(), 0);
	EVP_DigestUpdate(m_md_blake2b512, d, sz);
}
void Crypto::UpdateDigestBLAKE2s256(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_blake2s256, EVP_blake2s256(), 0);
	EVP_DigestUpdate(m_md_blake2s256, d, sz);
}
void Crypto::UpdateDigestMDC2(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_mdc2, EVP_mdc2(), 0);
	EVP_DigestUpdate(m_md_mdc2, d, sz);
}
void Crypto::UpdateDigestRIPEMD160(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_ripemd160, EVP_ripemd160(), 0);
	EVP_DigestUpdate(m_md_ripemd160, d, sz);
}
void Crypto::UpdateDigestWHIRLPOOL(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_whirlpool, EVP_whirlpool(), 0);
	EVP_DigestUpdate(m_md_whirlpool, d, sz);
}
void Crypto::UpdateDigestSM3(const void* d, size_t sz) {
	EVP_DigestInit_ex(m_md_sm3, EVP_sm3(), 0);
	EVP_DigestUpdate(m_md_sm3, d, sz);
}

void Crypto::FinalDigestSHA1(unsigned char* md, unsigned int* sz){
	EVP_DigestFinal(m_md_sha1_ctx, md, sz);
}

void Crypto::FinalDigestSHA224(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha224_ctx, md, sz);
}
void Crypto::FinalDigestSHA384(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha384_ctx, md, sz);
}
void Crypto::FinalDigestSHA512(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha512_ctx, md, sz);
}
void Crypto::FinalDigestSHA512_224(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha512_224_ctx, md, sz);
}
void Crypto::FinalDigestSHA512_256(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha512_256_ctx, md, sz);
}
void Crypto::FinalDigestSHA3_224(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha3_224, md, sz);
}
void Crypto::FinalDigestSHA3_256(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha3_256, md, sz);
}
void Crypto::FinalDigestSHA3_384(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha3_384, md, sz);
}
void Crypto::FinalDigestSHA3_512(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sha3_512, md, sz);
}
void Crypto::FinalDigestSHAKE_128(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_shake_128, md, sz);
}
void Crypto::FinalDigestSHAKE_256(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_shake_256, md, sz);
}
void Crypto::FinalDigestMD2(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_md2, md, sz);
}
void Crypto::FinalDigestMD4(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_md4, md, sz);
}
void Crypto::FinalDigestMD5(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_md5, md, sz);
}
void Crypto::FinalDigestMD5_SHA1(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_md5_sha1, md, sz);
}
void Crypto::FinalDigestBLAKE2b512(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_blake2b512, md, sz);
}
void Crypto::FinalDigestBLAKE2s256(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_blake2s256, md, sz);
}
void Crypto::FinalDigestMDC2(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_mdc2, md, sz);
}
void Crypto::FinalDigestRIPEMD160(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_ripemd160, md, sz);
}
void Crypto::FinalDigestWHIRLPOOL(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_whirlpool, md, sz);
}
void Crypto::FinalDigestSM3(unsigned char* md, unsigned int* sz) {
	EVP_DigestFinal(m_md_sm3, md, sz);
}

#endif
