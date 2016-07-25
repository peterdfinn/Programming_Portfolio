#include <openssl/evp.h>
#include <sys/types.h>

#include "pyramid.h"

/*
 * Function: decrypt_pyramid
 *
 * Description: This function decrypts a pyramid file's identifiers.
 *
 * Inputs:
 *  key - The encryption key.
 *  nonce - The nonce used for the encryption of this file.
 *  encrypted - A buffer containing the encrypted block IDs from the file.
 *  bytes - The number of bytes going into the encryption.
 *
 * Outputs:
 *  decrypted - A buffer into which to store the result of the decryption.
 *
 * Return value:
 *  -1 if EVP_DecryptInit failed.
 *  -2 if EVP_DecryptUpdate failed.
 *  -3 if EVP_DecryptFinal failed.
 *  Otherwise, returns the number of bytes produced by the decryption.
 */
int decrypt_pyramid(unsigned char *key, unsigned char *nonce, unsigned char *encrypted, long *decrypted, ssize_t bytes) {
  
  EVP_CIPHER_CTX *context = EVP_CIPHER_CTX_new();
  EVP_CIPHER_CTX_init(context);

  if (!EVP_DecryptInit(context, EVP_aes_128_ofb(), key, nonce)) {
    fprintf(stderr, "DecryptInit: DECRYPTION FAILED\n");
    EVP_CIPHER_CTX_cleanup(context);
    return -1;
  }
  int update_plaintext_bytes = -1;
  if (!EVP_DecryptUpdate(context, (unsigned char*) decrypted, &update_plaintext_bytes, encrypted, bytes)) {
    fprintf(stderr, "DecryptUpdate: DECRYPTION FAILED\n");
    EVP_CIPHER_CTX_cleanup(context);
    return -2;
  }
  int final_plaintext_bytes = -1;
  if (!EVP_DecryptFinal(context, (unsigned char*) decrypted, &final_plaintext_bytes)) {
    fprintf(stderr, "DecryptFinal: DECRYPTION FAILED\n");
    EVP_CIPHER_CTX_cleanup(context);
    return -3;
  }
  EVP_CIPHER_CTX_cleanup(context);
  return update_plaintext_bytes + final_plaintext_bytes;
}
