// #ifndef DECODE_H
// #define DECODE_H

// #include"types.h"

// typedef struct _DecodeInfo
// {
//     /* Secret File Info */
//     char *secret_fname;
//     FILE *fptr_secret;
//     char extn_secret_file[10];
//     int size_secret_file;
    
//     /* Stego Image Info */
//     char *stego_image_fname;
//     FILE *fptr_stego_image;

// }DecodeInfo;

// /* Decoding function prototype */

// /* Check operation type */
// OperationType check_operation_type(char *argv[]);

// /* Read and validate Encode args from argv */
// Status read_and_validate_encode_args(char *argv[], DecodeInfo *decInfo);

// /* Perform the encoding */
// Status do_encoding( DecodeInfo *decInfo);

// /* Get File pointers for i/p and o/p files */
// Status open_files( DecodeInfo *decInfo);

// /* Store Magic String */
// Status decode_magic_string(const char *magic_string,  DecodeInfo *decInfo);

// /* Decode function, which does the real decoding */
// Status decode_data_to_image(const char *data, int size, FILE *fptr_stego_image);

// /* Decode a byte into LSB of image data array */
// Status decode_byte_to_lsb(char* data, char *image_buffer);

// uint decode_size_to_lsb(FILE*fptr_stego_image);

// /* decode secret file extenstion */
// Status decode_secret_file_extn(const char *file_extn, DecodeInfo *decInfo);

// Status decode_secret_file_data(DecodeInfo *decInfo);






// #endif
#ifndef DECODE_H
#define DECODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * decoding  source Image to secret file 
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* stego input Image info */
    char *input_image_fname;
    FILE *fptr_input_image;
    char input_file_extn[5];
    uint input_file_extn_size;
    
    /* output txt File Info */
    char output_fname[256];
    FILE *fptr_output;
    long size_secret_file;

} DecodeInfo;


/* Determine whether the user wants to encode or decode based on argv */
OperationType check_operation_type(char *argv[]);

/* Read and validate command-line arguments for decoding and populate DecodeInfo */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the full decoding process using the information in DecodeInfo */
Status do_decoding(DecodeInfo *decInfo);

/* Decode the secret file data from the stego image and write to output file */
Status decode_secret_file_data(DecodeInfo *decInfo); 

/* Decode the secret file extension from the stego image and prepare output file */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode a 32-bit size value from the least significant bits of a buffer */
Status decode_size_from_lsb(uint *size, char *buff);

/* Decode a single byte from the least significant bits of a buffer */
Status decode_byte_from_lsb(char *ch, char *buff);

/* Decode 'size' bytes of data from the stego image into the provided buffer */
Status decode_data_from_image(int size, FILE *fptr_input_image, char *bucket);

/* Decode and verify the magic string in the stego image to ensure validity */
Status decode_magic_string(FILE *fptr_input_image, DecodeInfo *decInfo);

/* Open the necessary files for decoding (stego image and output file) */
Status open_files_for_decode(DecodeInfo *decInfo);

/* Decode the size of the secret file from the stego image and store in DecodeInfo */
Status decode_secret_file_size(uint size_secret_file, DecodeInfo *decInfo);


#endif