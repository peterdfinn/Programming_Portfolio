#include <openssl/err.h>
#include <openssl/evp.h>

#include "pyramid.h"

/*
 * Function: sign_pyramid
 *
 * Description: This function signs an encrypted pyramid file using SHA384.
 *
 * Inputs:
 *  buf - A copy of the nonce-and-pyramid to be signed.
 *  pyramid_size - The size of the nonce-and-pyramid to be signed.
 *
 * Outputs:
 *  sig - The signature produced by the signing is stored here.
 *  siglen - This is a pointer to the length of the signature stored in sig.
 *
 * Return value:
 *  0 if the signing succeeded.
 *  -2 if the signature produced is of an unexpected length.
 *  -3 if EVP_SignInit failed.
 *  -4 if EVP_SignUpdate failed.
 *  -5 if EVP_DigestFinal failed.
 *
 */
int sign_pyramid(const void *buf, size_t pyramid_size, char *sig, unsigned *siglen) {
  EVP_MD_CTX* ctx = EVP_MD_CTX_create();
  if (EVP_SignInit(ctx, EVP_sha384()) != 1) {
    EVP_MD_CTX_destroy(ctx);
    return -3;
  }
  if (EVP_SignUpdate(ctx, buf, pyramid_size) != 1) {
    EVP_MD_CTX_destroy(ctx);
    return -4; 
  }
  if (EVP_DigestFinal(ctx, sig, siglen) != 1) {
    EVP_MD_CTX_destroy(ctx);
    return -5;
  }
  if (*siglen != SIGLEN) return -2;
  EVP_MD_CTX_destroy(ctx);
  return 0;
}

