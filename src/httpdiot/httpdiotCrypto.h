// MIT license
#pragma once
#ifndef __HTTPDIOT_CRPTH_
#define __HTTPDIOT_CRPTH_

#include "openssl/evp.h"

namespace httpdiot
{

	class Crypto
	{
		EVP_MD_CTX* m_md_sha1_ctx = 0;
		EVP_MD_CTX* m_md_sha224_ctx = 0;
		EVP_MD_CTX* m_md_sha384_ctx = 0;
		EVP_MD_CTX* m_md_sha512_ctx = 0;
		EVP_MD_CTX* m_md_sha512_224_ctx = 0;
		EVP_MD_CTX* m_md_sha512_256_ctx = 0;
		EVP_MD_CTX* m_md_sha3_224 = 0;
		EVP_MD_CTX* m_md_sha3_256 = 0;
		EVP_MD_CTX* m_md_sha3_384 = 0;
		EVP_MD_CTX* m_md_sha3_512 = 0;
		EVP_MD_CTX* m_md_shake_128 = 0;
		EVP_MD_CTX* m_md_shake_256 = 0;
		EVP_MD_CTX* m_md_md2 = 0;
		EVP_MD_CTX* m_md_md4 = 0;
		EVP_MD_CTX* m_md_md5 = 0;
		EVP_MD_CTX* m_md_md5_sha1 = 0;
		EVP_MD_CTX* m_md_blake2b512 = 0;
		EVP_MD_CTX* m_md_blake2s256 = 0;
		EVP_MD_CTX* m_md_mdc2 = 0;
		EVP_MD_CTX* m_md_ripemd160 = 0;
		EVP_MD_CTX* m_md_whirlpool = 0;
		EVP_MD_CTX* m_md_sm3 = 0;
	public:
		Crypto();
		~Crypto();

		unsigned char hashString[200];

		void UpdateDigestSHA1(const void* d, size_t sz);
		void UpdateDigestSHA224(const void* d, size_t sz);
		void UpdateDigestSHA384(const void* d, size_t sz);
		void UpdateDigestSHA512(const void* d, size_t sz);
		void UpdateDigestSHA512_224(const void* d, size_t sz);
		void UpdateDigestSHA512_256(const void* d, size_t sz);
		void UpdateDigestSHA3_224(const void* d, size_t sz);
		void UpdateDigestSHA3_256(const void* d, size_t sz);
		void UpdateDigestSHA3_384(const void* d, size_t sz);
		void UpdateDigestSHA3_512(const void* d, size_t sz);
		void UpdateDigestSHAKE_128(const void* d, size_t sz);
		void UpdateDigestSHAKE_256(const void* d, size_t sz);
		void UpdateDigestMD2(const void* d, size_t sz);
		void UpdateDigestMD4(const void* d, size_t sz);
		void UpdateDigestMD5(const void* d, size_t sz);
		void UpdateDigestMD5_SHA1(const void* d, size_t sz);
		void UpdateDigestBLAKE2b512(const void* d, size_t sz);
		void UpdateDigestBLAKE2s256(const void* d, size_t sz);
		void UpdateDigestMDC2(const void* d, size_t sz);
		void UpdateDigestRIPEMD160(const void* d, size_t sz);
		void UpdateDigestWHIRLPOOL(const void* d, size_t sz);
		void UpdateDigestSM3(const void* d, size_t sz);

		void FinalDigestSHA1(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA224(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA384(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA512(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA512_224(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA512_256(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA3_224(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA3_256(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA3_384(unsigned char* md, unsigned int* sz);
		void FinalDigestSHA3_512(unsigned char* md, unsigned int* sz);
		void FinalDigestSHAKE_128(unsigned char* md, unsigned int* sz);
		void FinalDigestSHAKE_256(unsigned char* md, unsigned int* sz);
		void FinalDigestMD2(unsigned char* md, unsigned int* sz);
		void FinalDigestMD4(unsigned char* md, unsigned int* sz);
		void FinalDigestMD5(unsigned char* md, unsigned int* sz);
		void FinalDigestMD5_SHA1(unsigned char* md, unsigned int* sz);
		void FinalDigestBLAKE2b512(unsigned char* md, unsigned int* sz);
		void FinalDigestBLAKE2s256(unsigned char* md, unsigned int* sz);
		void FinalDigestMDC2(unsigned char* md, unsigned int* sz);
		void FinalDigestRIPEMD160(unsigned char* md, unsigned int* sz);
		void FinalDigestWHIRLPOOL(unsigned char* md, unsigned int* sz);
		void FinalDigestSM3(unsigned char* md, unsigned int* sz);
	};

}

#endif
