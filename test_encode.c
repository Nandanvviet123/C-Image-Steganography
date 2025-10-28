/******************************************************************************
 * Project: LSB Image Steganography
 * Author: Nandan U
 * Date: June 2024
 * Batch: 25021A
 *
 * Description:
 *   This program hides a secret file inside a BMP image (encoding)
 *   or extracts the secret file from a stego BMP image (decoding),
 *   based on command-line options.
 *
 * Usage:
 *   Encoding: ./stego -e <input.bmp> <secret.txt> <output.bmp>
 *   Decoding: ./stego -d <stego.bmp> <output.txt>
 *
 * Example:
 *   ./stego -e beautiful.bmp secret.txt stego.bmp
 *   ./stego -d stego.bmp recovered.txt
 ******************************************************************************/

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include<string.h>
#include"common.h"

char MAGIC_STRING[MAGIC_STRING_SIZE];//declare char array for magic string--

OperationType check_operation_type(char *argv[]) // check opertaion--encoding--decoding--
{
    {
        if(strcmp(argv[1],"-e")==0)
        {
            return e_encode ;
        }
        else if(strcmp(argv[1],"-d")==0)
        {
            return e_decode ; 
        }
        else
        {
           return e_unsupported ;
        }
    }
  }





int main(int argc ,char*argv[]) // passing agruments (argc-->argument count)--(argv-->argument variable--)
{
    if(argc>=3)
    {
    if(check_operation_type(argv) == e_encode)//operation for encoding--
    {
        printf("----------you have choosed encoding----------\n");
        printf("enter the magic string\n");
        scanf(" %[^\n]",MAGIC_STRING);
            EncodeInfo encInfo; // creating variable encInfo---
                if(read_and_validate_encode_args(argv,&encInfo)==e_success) // check files are present are not--
                {
                printf("Read and validate is sucess\n");
                if(do_encoding(&encInfo)==e_success)
                {
                    printf("encoding sucessfull\n");
                }
                else
                {
                    printf("encoding unsuccessfull\n");
                }
                }
                else
                {
                printf("read and validaiate unsucessfull\n");
                return e_failure;
                }
    }


    ////////////////////////
        else  if(check_operation_type(argv) == e_decode)//operation for decoding---
            {
                printf("you have choosed decoding\n");
                  printf("enter the magic string\n");
                 scanf(" %[^\n]",MAGIC_STRING);
                DecodeInfo decInfo;
                if(read_and_validate_decode_args(argv,&decInfo)==e_success)
                {
                printf("read and validate successfull\n");
                    if(do_decoding(&decInfo)==e_success) 
                    {
                        printf("Decoding Success!!\n");
                    }
                    else
                    {
                        printf("Decoding Failed!!\n");
                    }
                }
                
                else
                {
                    printf("validation failed--\n");
                    return e_failure;
                }
            }

            else
            {
                printf("it is unsupported\n");
                return  e_failure;
            }
            return e_success;
       }
    else{
        printf("_INVALID_\n>encoding :-\n>decoding :-\n");
        return e_failure;
    }

}
///////////////////////  

