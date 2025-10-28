#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */


Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{   
    if(strstr(argv[2],".bmp")!=NULL) 
    {
        printf(">beautiful.bpm file is present\n");
        encInfo -> src_image_fname = argv[2];
    }
    else{
         printf("beatiful.bpm is not present\n");
         return e_failure;
    }

    if(strstr(argv[3],".txt")!=NULL) 
    {
        printf(">secret.txt file  is present\n");
        encInfo -> secret_fname = argv[3];
        strcpy(encInfo ->extn_secret_file,strstr(argv[3],".txt"));
    }
    else{
         printf(">secret.txt file  is not present\n");
         return e_failure;
    }

    if(argv[4]==NULL) 
    {
        encInfo -> stego_image_fname = "stego.bmp" ;
        printf("%s",encInfo -> stego_image_fname);
         
        
    }
    else{
        printf(">stego.bpm file is present\n");
        encInfo ->  stego_image_fname = argv[4];
    }
    return e_success ; 
    
}

/**********************************************************************************/
Status do_encoding(EncodeInfo *encInfo )
{   
     if(open_files(encInfo)!=e_success)
     {
        printf("files are not opened\n");
        return e_failure;
    }
    if(check_capacity(encInfo)!=e_success) 
    {
        printf("check capacity is failed\n");
        return e_failure;
    }
    if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)!=e_success) 
    {
        printf("copy bmp header failed\n");
        return e_failure;
    }
    if(encode_magic_string(MAGIC_STRING, encInfo)!=e_success)
    {
        printf("magic string is not encoded\n");
        return e_failure;
    }
    if (encode_secret_file_extn(encInfo->extn_secret_file, encInfo) != e_success) 
    {
        printf("secret file extn is not--encoded\n");
        return e_failure;
    }
  
    if(encode_secret_file_size(encInfo->size_secret_file,encInfo)!=e_success) 
    {
        printf("secret file size is not--encoded\n");
        return e_failure;
    }
    if (encode_secret_file_data(encInfo) != e_success) 
    {
        printf("secret file data is not--encoded\n");
        return e_failure;
    }
    if (copy_remaining_img_data(encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success) 
    {
        printf("copy_remaining data is failed\n");
        return e_failure;
    }
     printf("<<< Encoding completed successfully >>>\n");
      return e_success;
}


/**************************************************************************************/
Status open_files(EncodeInfo *encInfo)
{
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname,"rb");
    if(encInfo->fptr_src_image == NULL)
    {
        printf("source file is not present\n");
        return e_failure;
    }
    else{
        printf("source file is present\n");
    }

    encInfo->fptr_secret = fopen(encInfo->secret_fname,"rb");
    if(encInfo->fptr_secret==NULL)
    {
        printf("secret file is not present\n");
        return e_failure;
    }
     else{
        printf("secret file is present\n");
    }

    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname,"wb");
    printf("stego file is opened\n");

    return e_success;
}


/**************************************************************************************/

uint get_image_size_for_bmp(FILE *fptr_src_image) //size for beautiful.bmp file
{  
   uint height,width;
   //move pointer to 18th position to and fetch width and height from beautiful.bmp
   fseek(fptr_src_image,18,SEEK_SET); 
   fread(&width,sizeof(int),1,fptr_src_image);//fetch 4 bytes sizeof(int) from 18th pos
    // width = ftell(fptr_src_image); //for width pos we want to use ftell
   printf("width--->%d\n",width);
   ///
   //fetch 4 bytes sizeof(int) from 22th pos after width
   fseek(fptr_src_image,22,SEEK_SET);
   fread(&height,sizeof(int),1,fptr_src_image);
    // height = ftell(fptr_src_image);
   printf("height--->%d\n",height);
      // move file pointer to pixel data start (after 54-byte header)
    fseek(fptr_src_image, 54, SEEK_SET);
   return 3*width*height; // multiple with 3*h*w for size of the file--
}

///////////////////////////
 uint get_file_size(FILE *fptr_secret)// size for secret.txt
 {
    fseek(fptr_secret,0,SEEK_END); //moving file poniter to the end of the file
    uint size = ftell(fptr_secret);
     fseek(fptr_secret, 0, SEEK_SET); // reset pointer to start
    return size;
 }
 
/**********************************************************************************/
Status check_capacity(EncodeInfo *encInfo)
{
     encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
     encInfo->size_secret_file = get_file_size(encInfo->fptr_secret);

     if(encInfo->image_capacity >  encInfo->size_secret_file*8)//check here is image_size is greater than secret_file_size than do op--
     {
        printf("image have space to hold the data\n");
        return e_success;
     }
     else{
        printf("image not have space to hold the data\n");
        return e_failure;
     }
}

/******************************************************************************************/
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
 {
    char header[54];
    fseek(fptr_src_image,0,SEEK_SET);
    if(fread(header,sizeof(char),54,fptr_src_image)!=54)
    {
        printf("cannot read bmp header\n");
        return e_failure;
    }

    
    if(fwrite(header,sizeof(char),54,fptr_dest_image)!=54) 
    {
        printf("cannot write bmp header\n");
        return e_failure;
    }
    return e_success;
 }

 /**************************************************************************************/
 Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo) 
 {
    int len = strlen(magic_string);
    if(encode_data_to_image(magic_string, len, encInfo ->fptr_src_image, encInfo->fptr_stego_image)==e_success) 
    {
        printf("magic string is encoded sucessfully into image\n");
        return e_success;
    }
    else{
        printf("magic string is not-- encoded sucessfully into image\n");
        return e_failure;
    }
 }

/*******************************************************************************************************************/


Status encode_data_to_image(const char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buff[8];

    for(int i=0;i<size;i++)
    {
       
        if(fread(buff,1,8,fptr_src_image)!=8)
        {
            return e_failure;
        }
    
        if(encode_byte_to_lsb(data[i],buff) != e_success)
        {
            return e_failure;
        }

        
        if(fwrite(buff,1,8,fptr_stego_image)!=8)
        {
            return e_failure;
        }
    }

    return e_success;
}



Status encode_byte_to_lsb(char data, char *image_buffer)
{
    for (int i = 0; i < 8; i++)
    {
        
        int bit = (data >> (7 - i)) & 1;

        
        image_buffer[i] = (image_buffer[i] & 0xFE) | bit; //0XfE == (~1)
    }

    return e_success;
}


/**************************************************************************************/
Status encode_size_to_lsb(long data,char *image_buffer)
{
     for (int i = 0; i < 32; i++)
      {
        
        int bit = (data >> (31 - i)) & 1;
        
        image_buffer[i] = (image_buffer[i] & 0xFE) | bit; //0XfE == (~1)
       }

    return e_success;

}

/********************************************************************************************************/

Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
    long size=strlen(file_extn);

    char buff[32];
    
    if(fread(buff,1,32,encInfo->fptr_src_image)!=32)
        {
            return e_failure;
        }

    if(encode_size_to_lsb(size,buff)==e_success)
    {
        printf("Encoding the size of the secret file is successful\n");
    }

    
    if(fwrite(buff,1,32,encInfo->fptr_stego_image)!=32)
        {
            return e_failure;
        }
    
    

    if(encode_data_to_image( file_extn , size , encInfo->fptr_src_image, encInfo->fptr_stego_image)==e_success)
    {
        printf("secret file extn encoded successfully\n");
    }
    else
    {
        printf("secret file extn encoding was unsuccessful\n");
    }
    return e_success;
}

/**************************************************************************************/


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    char arr[32];
    if (fread(arr, 1, 32, encInfo->fptr_src_image) != 32)
        return e_failure;

    encode_size_to_lsb(file_size, arr);

    if (fwrite(arr, 1, 32, encInfo->fptr_stego_image) != 32)
        return e_failure;

    printf("File size encoded successfully\n");
    return e_success;
}

/**************************************************************************************/

Status encode_secret_file_data(EncodeInfo *encInfo)
{
    
    fseek(encInfo->fptr_secret, 0, SEEK_END);
    long size = ftell(encInfo->fptr_secret);
    rewind(encInfo->fptr_secret);

    char ch;
    for (long i = 0; i < size; i++)
    {
        if (fread(&ch, 1, 1, encInfo->fptr_secret) != 1)
        {
            return e_failure;
        }

        if (encode_data_to_image(&ch, 1, encInfo->fptr_src_image, encInfo->fptr_stego_image) != e_success)
        {
            return e_failure;
        }
    }
    return e_success;
}


Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
    while(fread(&ch,1,1,fptr_src)==1)
       fwrite(&ch,1,1,fptr_dest);

    printf("remaining image data successfully copied\n");
    return e_success;
}


