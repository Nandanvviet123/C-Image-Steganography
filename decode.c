#include <stdio.h>   // Standard I/O functions
#include "encode.h"   // Header file containing encoding-related definitions 
#include "types.h"     // Custom type definitions (like Status, uint, etc.)
#include "string.h"     // For string operations like strcpy, strcmp, strstr, etc.
#include "common.h"    // Common macros like MAGIC_STRING 
#include "decode.h"    // Header containing declarations for decoding-related functions
#include <stdlib.h>   // Standard library for memory allocation, exit(), etc.


/**********************************************************************************/
// Function to read and validate command-line arguments for decoding
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{

    // Check if 3rd argument (argv[2]) contains a ".bmp" file (the stego image)
    if(strstr(argv[2],".bmp")!= NULL)
    {
        printf("stego.bmp is present\n");
        decInfo->input_image_fname = argv[2]; // Store the input image filename
    }
    else
    {
        printf("beautiful.bmp is not present\n");
        return e_failure;  // Return failure if image file not found
    }
     
    /***************************************************************************************/
    // Check if 4th argument (argv[3]) — the output filename — is provided
    
    if(argv[3]== NULL)
    {
        printf("destination file is not present\n");
        printf("So Created as default.txt as o/p\n");
        strcpy(decInfo->output_fname,"output");    // Default output name if not provided
    }
 
    else if(strchr(argv[3],'.')!= NULL)  // If output file name contains an extension ('.')
    {
        printf("destination file is present\n");

        char temp[100];
        strcpy(temp,argv[3]);  // Copy filename to temp for processing

        char *token=strtok(temp,".");  // Split filename from its extension
        if(token!=NULL)
        {
            strcpy(decInfo->output_fname,token); // Store only the base name 
        }
        
    }
        

    return e_success;  // Return success after validating all arguments
    
} 


/**********************************************************************************/

Status do_decoding(DecodeInfo *decInfo)  // Main decoding workflow controller
{
    
    if(open_files_for_decode(decInfo) != e_success)   // Open the required files
    {
        printf("all files are not--opened  successfully\n");
        return e_failure;
    }
    if(decode_magic_string(decInfo->fptr_input_image,decInfo)!=e_success) // Decode the magic string from image
    {
        printf("Decoding magic string from the stego image is not---successful\n");
        return e_failure;
    }
     if(decode_secret_file_extn(decInfo)!=e_success)  // Decode the secret file extension
     {
        printf("Decoding secret_file_extn from the stego image is not---successful\n");
        return e_failure;
     }
     
     if(decode_secret_file_size(decInfo->size_secret_file,decInfo)!=e_success) //  Decode the size of the secret file
     {
        printf("Decoding secret_file_size from the stego image is not--- successful\n");
        return e_failure;
     }
 
    if(decode_secret_file_data(decInfo)!=e_success)  // Decode the actual secret file data
    {
        printf("Decoding secret_file_data from the stego image is not---successful\n");
        return e_failure;
    }
}


/**********************************************************************/

Status open_files_for_decode(DecodeInfo *decInfo)  // Function to open the image file for reading
{
    decInfo->fptr_input_image = fopen(decInfo->input_image_fname,"rb");  // Open the stego image in binary read mode

    if(decInfo->fptr_input_image != NULL)
    {
        printf("input file is opened successfully\n");
    }
    else
    {
        printf("source file is opening unsuccessful\n");
        return e_failure; // Return failure if file cannot be opened
    }
    return e_success;  // File opened successfully
} 
 
/********************************************************************/
// Function to decode and verify the magic string

Status decode_magic_string(FILE *fptr_input_image, DecodeInfo *decInfo)
{
    fseek(fptr_input_image,54,SEEK_SET);  // Skip BMP header (54 bytes)
    int size=strlen(MAGIC_STRING);  // Get length of MAGIC_STRING
    char magic__string[size+1];  // Buffer to store decoded string
    magic__string[size]= '\0';  // Null-terminate string
 
    if(decode_data_from_image( size,fptr_input_image,magic__string)==e_success)  // Decode bytes equal to the size of MAGIC_STRING
    {
           if(strcmp(magic__string,MAGIC_STRING)==0) // Compare decoded string with expected MAGIC_STRING
            {
                printf("Magic string matched\n");
                return e_success;
            }

            else
            {
                printf("Magic string not matched\n");
                return e_failure;
            }
    }

    else
    {
        return e_failure;
    }
}

/*******************************************************************************************/
// Function to decode multiple characters from the image

Status decode_data_from_image( int size, FILE *fptr_input_image,char *bucket)
{
    char buff[8];  // Buffer to store 8 bytes for each character
    char decoded_word[size+1];  // Buffer to store decoded characters
    decoded_word[size] = '\0';  // null terminate
    char ch=0;  // Temporary variable to hold one decoded character

    for(int i=0;i<size;i++)
    {
        // reading 8 bytes of data from the source image and storing it in a array
        if(fread(buff,1,8,fptr_input_image)!=8)
        {
            return e_failure;
        }
        
        if(decode_byte_from_lsb(&ch,buff) != e_success)  // Decode 1 byte (character) from LSBs of 8 bytes
        {
            return e_failure;
        }

        decoded_word[i]=ch; // Store decoded character
        
    }
    
        decoded_word[size]= '\0';  // Null terminate
        strcpy(bucket,decoded_word);  // Copy decoded data into destination buffer
        return e_success;
}

/***************************************************************************/
// Function to decode a single byte (character) from 8 LSB bits

Status decode_byte_from_lsb(char *ch,char *buff)
{
      *ch=0;  // Initialize decoded byte to 0
        for(int j=0;j<8;j++)
        {
           
           *ch=(*ch<<1)|(buff[j]&1);  // Left shift by 1 and OR with the LSB of each byte
        
        }
 
        return e_success;
}


/***************************************************************/
// Function to decode 32 bits (4 bytes) into an integer

Status decode_size_from_lsb(uint *size,char *buff)
{
      uint ch=0;  // Variable to store decoded size
        for(int j=0;j<32;j++)
        {
           
           ch=(ch<<1)|(buff[j]&1);  // Extract LSB and combine bits to reconstruct integer
        
        }
        *size=ch;  // Store final decoded value
 
        return e_success;
}


/*********************************************************************/

// Function to decode secret file extension (like ".txt" or ".c")

Status decode_secret_file_extn( DecodeInfo *decInfo)
{
    // reading the 32 bytes of data from the source image to embedd 32 bits into it
    char buff[32];
    uint size=0;
    
    if(fread(buff,1,32,decInfo->fptr_input_image)!=32)
        {
            return e_failure;
        }

    if(decode_size_from_lsb(&size,buff)==e_success)
    {
        printf("Decoding the size of the secret file extn is successful\n");
    }
    // now after this call the size contains the size of the secret file extn....


    char arr[10];
    if(decode_data_from_image(size, decInfo->fptr_input_image,arr)==e_success)
    {
        printf("secret file extn decoded successfully\n");
        
    }
    else
    {
        printf("secret file extn decoding was unsuccessful\n");
        return e_failure;
    }

    // Now the arr contains the file extn...
    strcat(decInfo->output_fname,arr);

    decInfo->fptr_output = fopen(decInfo->output_fname,"w");
    return e_success;
}


/***********************************************************************************/
Status decode_secret_file_size(uint size_secret_file,DecodeInfo *decInfo)  
{
    char buff[32];
    uint size=0;
    // reading the 32 bytes of data from the source image to embedd 32 bits into it
    if(fread(buff,1,32,decInfo->fptr_input_image)!=32)
        {
            return e_failure;
        }

    if(decode_size_from_lsb(&size,buff)==e_success)
    {
        printf("Decoding the size of the secret file is successful\n");
        
    }
    // now after this call the size contains the size of the secret file extn
    decInfo->size_secret_file=size;
    return e_success;

}


/*******************************************************************/


Status decode_secret_file_data(DecodeInfo *decInfo)
{

    char *arr=malloc(decInfo->size_secret_file);
    if(decode_data_from_image(decInfo->size_secret_file, decInfo->fptr_input_image,arr)==e_success)
    {
        printf("Secret file data decoded successfully\n");

    }
    else
    {
        printf("Secret file data decoding unsuccessful\n");

    }
    
    if (decInfo->fptr_output == NULL)
    {
        printf("Output file pointer is NULL!\n");
        free(arr);
        return e_failure;
    }
    fwrite(arr, 1, decInfo->size_secret_file, decInfo->fptr_output);
    return e_success;
    
}