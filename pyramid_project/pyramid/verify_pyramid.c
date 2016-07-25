#include <openssl/evp.h>
#include <openssl/err.h>

#include "pyramid.h"

/*
 * Function: verify_pyramid
 *
 * Description: This function verifies the signature against the remainder of
 *  the pyramid file.
 *
 * Inputs:
 *  signature - A buffer containing the signature of the pyramid file.
 *  payload - A buffer containing the remainder of the pyramid file: the
 *   payload for the verification.
 *  payload_size - The length, in bytes, of the payload.
 *
 * Outputs:
 *  signature - After a call to verify_pyramid that returns either 0 or 1, the
 *   message digest produced by the verification will be stored here.
 *
 * Return value:
 *  -3 if EVP_VerifyInit failed.
 *  -2 if EVP_VerifyUpdate failed.
 *  -1 if EVP_DigestFinal_ex returned a value not specified by the OpenSSL
 *   documentation.
 *  0 if the verification failed: the block does not match its signature.
 *  1 if the verification succeeded: the block has not been corrupted.
 */
int verify_pyramid(unsigned char *signature, unsigned char *payload, ssize_t payload_size) {
  EVP_MD_CTX* ctx = EVP_MD_CTX_create();
  if (!EVP_VerifyInit(ctx, EVP_sha384())) {
    EVP_MD_CTX_destroy(ctx);
    return -3;
  }
  if (!EVP_VerifyUpdate(ctx, payload, payload_size)) {
    EVP_MD_CTX_destroy(ctx);
    return -2;
  }
  int return_value = EVP_DigestFinal_ex(ctx, signature, NULL);
  if ((return_value == 1) || (return_value == 0)) {
    EVP_MD_CTX_destroy(ctx);
    return return_value;
  }
  EVP_MD_CTX_destroy(ctx);
  return -1;
}
