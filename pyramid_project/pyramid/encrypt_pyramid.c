#include <stdlib.h>
#include <strings.h>

#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

#include "pyramid.h"

/*
 * Function: encrypt_pyramid
 *
 * Description: This function encrypts a buffer containing a pyramid using a
 *  randomly generated IV and an input key.
 *
 * Inputs:
 *  from - A buffer containing a copy of the pyramid block identifiers to be
 *   encrypted.
 *  pyra - A pointer to the pyramid struct.  Is assumed to contain the
 *   appropriate encryption key.
 *  pyramid_size - The number of bytes in from.
 *  seed - A seed to feed to the random number generator for the IV.
 *
 * Outputs:
 *  to - This buffer will contain the encrypted pyramid.  Is assumed to be at
 *   least as big as from.
 *  nonce - The randomly generated IV will be stored here.
 *
 * Return value:
 *  -1 if EVP_EncryptInit failed.
 *  -2 if EVP_EncryptUpdate failed.
 *  -3 if EVP_EncryptFinal failed.
 *  Otherwise, the number of bytes produced by the encryption is returned.
 */
int encrypt_pyramid(long *from, long *to, pyramid* pyra, int pyramid_size, unsigned char *nonce, long seed) {
  const void *seed_buf = &seed;
  RAND_seed(seed_buf, sizeof(long));
  BIGNUM *random = BN_new();
  BN_rand(random, 128, -1, 0);
  if (BN_num_bytes(random) < KEY_LENGTH) {
    int i, randint;
    for (i = 0; i < KEY_LENGTH / sizeof(int); ++i) {
      randint = rand();
      bcopy(&randint, nonce, sizeof(int));
    }
  }
  else
    bcopy(random, nonce, KEY_LENGTH);

  EVP_CIPHER_CTX *context = EVP_CIPHER_CTX_new();
  EVP_CIPHER_CTX_init(context);

  if (!EVP_EncryptInit(context, EVP_aes_128_ofb(), pyra->key, nonce)) {
    fprintf(stderr, "EncryptInit: ENCRYPTION FAILED\n");
    EVP_CIPHER_CTX_cleanup(context);
    return -1;
  }
  int update_encrypted_bytes = -1;
  if (!EVP_EncryptUpdate(context, (unsigned char*) to, &update_encrypted_bytes, (unsigned char*) from, pyramid_size)) {
    fprintf(stderr, "EncryptUpdate: ENCRYPTION FAILED\n");
    EVP_CIPHER_CTX_cleanup(context);
    return -2;
  }
  int final_encrypted_bytes = -1;
  if (!EVP_EncryptFinal(context, (unsigned char*) to, &final_encrypted_bytes)) {
    fprintf(stderr, "EncryptFinal: ENCRYPTION FAILED\n");
    EVP_CIPHER_CTX_cleanup(context);
    return -3;
  }
  EVP_CIPHER_CTX_cleanup(context);
  return update_encrypted_bytes + final_encrypted_bytes;
}

