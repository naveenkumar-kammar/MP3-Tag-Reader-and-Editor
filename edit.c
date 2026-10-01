#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mp3.h"

void edit_mp3_details(char *file_name, char *option, char *new_text)
{
    /* Variables for storing frame size and total ID3 tag size */
    unsigned int size;
    unsigned int tag_size;

    /* Variable for storing frame encoding and 10-byte ID3 header */
    unsigned char frame_encode;
    unsigned char header[10];

    /* Variables for storing target and current frame IDs */
    char frame_id[5];
    char current_frame_id[5];

    /* Variables for storing frame size bytes and frame flags */
    unsigned char size_bytes[4];
    unsigned char flags[2];

    /* File pointers for original MP3 and temporary MP3 files */
    FILE *fptr, *fp;

    /* Convert option to frame ID */
    if (strcmp(option, "-t") == 0)
    {
        strcpy(frame_id, "TIT2");
    }
    else if (strcmp(option, "-A") == 0)
    {
        strcpy(frame_id, "TPE1");
    }
    else if (strcmp(option, "-a") == 0)
    {
        strcpy(frame_id, "TALB");
    }
    else if (strcmp(option, "-y") == 0)
    {
        strcpy(frame_id, "TYER");
    }
    else if (strcmp(option, "-c") == 0)
    {
        strcpy(frame_id, "TCON");
    }
    else if (strcmp(option, "-C") == 0)
    {
        strcpy(frame_id, "COMM");
    }
    else
    {
        printf(RED "Invalid edit option\n" RESET);
        return;
    }

    /* Open original MP3 file in binary read mode */
    fptr = fopen(file_name, "rb");

    if (fptr == NULL)
    {
        printf(RED "Unable to open file\n" RESET);
        return;
    }

    /* Read 10-byte ID3 header from the original MP3 file */
    if (fread(header, 1, 10, fptr) != 10)
    {
        printf(RED "Error reading MP3 header\n" RESET);
        fclose(fptr);
        return;
    }

    /* Read original ID3 tag size from the header */
    tag_size = ((unsigned int)(header[6] & 0x7F) << 21) |
               ((unsigned int)(header[7] & 0x7F) << 14) |
               ((unsigned int)(header[8] & 0x7F) << 7) |
               (unsigned int)(header[9] & 0x7F);

    /* Create temporary file in binary write mode */
    fp = fopen("temp.mp3", "wb");

    if (fp == NULL)
    {
        printf(RED "Unable to create temporary file\n" RESET);
        fclose(fptr);
        return;
    }

    /* Copy the original 10-byte ID3 header to the temporary file */
    if (fwrite(header, 1, 10, fp) != 10)
    {
        printf(RED "Error writing MP3 header\n" RESET);
        fclose(fptr);
        fclose(fp);
        return;
    }

    /* Read all six frames */
    for (int i = 0; i < 6; i++)
    {
        /* Read frame ID from the original MP3 file */
        if (read_frame_id(fptr, current_frame_id) == 0)
        {
            printf(RED "Error reading frame ID\n" RESET);
            fclose(fptr);
            fclose(fp);
            return;
        }

        /* Read 4-byte frame size from the original MP3 file */
        if (fread(size_bytes, 1, 4, fptr) != 4)
        {
            printf(RED "Error reading frame size\n" RESET);
            fclose(fptr);
            fclose(fp);
            return;
        }

        /* Convert big endian size */
        size = ((unsigned int)size_bytes[0] << 24) |
               ((unsigned int)size_bytes[1] << 16) |
               ((unsigned int)size_bytes[2] << 8) |
               (unsigned int)size_bytes[3];

        /* Check selected frame */
        if (strcmp(current_frame_id, frame_id) == 0)
        {
            /* Variable for storing the size of the new frame data */
            unsigned int new_size = strlen(new_text) + 1;

            /* Update total ID3 tag size */
            tag_size = tag_size - size + new_size;

            /* Write selected frame ID to the temporary file */
            fwrite(current_frame_id, 1, 4, fp);

            /* Reuse size_bytes for new frame size */
            size_bytes[0] = (new_size >> 24) & 0xFF;
            size_bytes[1] = (new_size >> 16) & 0xFF;
            size_bytes[2] = (new_size >> 8) & 0xFF;
            size_bytes[3] = new_size & 0xFF;

            /* Write new frame size to the temporary file */
            fwrite(size_bytes, 1, 4, fp);

            /* Read and write flags */
            if (fread(flags, 1, 2, fptr) != 2)
            {
                printf(RED "Error reading frame flags\n" RESET);
                fclose(fptr);
                fclose(fp);
                return;
            }

            /* Write original frame flags to the temporary file */
            fwrite(flags, 1, 2, fp);

            /* Read encoding */
            if (read_frame_encoding(fptr, &frame_encode) == 0)
            {
                printf(RED "Error reading frame encoding\n" RESET);
                fclose(fptr);
                fclose(fp);
                return;
            }

            /* Use UTF-8 encoding */
            frame_encode = 0;

            /* Write encoding byte to the temporary file */
            fwrite(&frame_encode, 1, 1, fp);

            /* Write new text to the selected frame */
            fwrite(new_text, 1, strlen(new_text), fp);

            /* Skip old frame data except encoding byte */
            if (fseek(fptr, size - 1, SEEK_CUR) != 0)
            {
                printf(RED "Error skipping old frame data\n" RESET);
                fclose(fptr);
                fclose(fp);
                return;
            }
        }
        else
        {
            /* Pointer for dynamically allocated unchanged frame data */
            unsigned char *data_buffer;

            /* Write unchanged frame ID and size */
            fwrite(current_frame_id, 1, 4, fp);
            fwrite(size_bytes, 1, 4, fp);

            /* Read and write flags */
            if (fread(flags, 1, 2, fptr) != 2)
            {
                printf(RED "Error reading frame flags\n" RESET);
                fclose(fptr);
                fclose(fp);
                return;
            }

            /* Write unchanged frame flags to the temporary file */
            fwrite(flags, 1, 2, fp);

            /* Allocate memory for complete frame data */
            data_buffer = malloc(size);

            if (data_buffer == NULL)
            {
                printf(RED "Memory allocation failed\n" RESET);
                fclose(fptr);
                fclose(fp);
                return;
            }

            /* Read complete unchanged frame data */
            if (fread(data_buffer, 1, size, fptr) != size)
            {
                printf(RED "Error reading frame data\n" RESET);
                free(data_buffer);
                fclose(fptr);
                fclose(fp);
                return;
            }

            /* Write complete unchanged frame data to the temporary file */
            if (fwrite(data_buffer, 1, size, fp) != size)
            {
                printf(RED "Error writing frame data\n" RESET);
                free(data_buffer);
                fclose(fptr);
                fclose(fp);
                return;
            }

            /* Release memory allocated for the frame data */
            free(data_buffer);
        }
    }

    /* Copy remaining MP3 audio data */
    {
        /* Buffer and variable for copying audio data in blocks */
        unsigned char audio_buffer[1024];
        int bytes;

        /* Read remaining audio data and write it to the temporary file */
        while ((bytes = fread(audio_buffer, 1, sizeof(audio_buffer), fptr)) != 0)
        {
            fwrite(audio_buffer, 1, bytes, fp);
        }
    }

    /* Close files */
    fclose(fptr);
    fclose(fp);

    /* Re-open temporary file to update ID3 header */
    fp = fopen("temp.mp3", "r+b");

    if (fp == NULL)
    {
        printf(RED "Unable to reopen temporary file\n" RESET);
        return;
    }

    /* Convert tag size to ID3 sync-safe format */
    header[6] = (tag_size >> 21) & 0x7F;
    header[7] = (tag_size >> 14) & 0x7F;
    header[8] = (tag_size >> 7) & 0x7F;
    header[9] = tag_size & 0x7F;

    /* Go to beginning */
    fseek(fp, 0, SEEK_SET);

    /* Write updated 10-byte ID3 header */
    fwrite(header, 1, 10, fp);

    fclose(fp);

    /* Remove original file */
    if (remove(file_name) != 0)
    {
        printf(RED "Error removing original file\n" RESET);
        return;
    }

    /* Rename temporary file */
    if (rename("temp.mp3", file_name) != 0)
    {
        printf(RED "Error replacing original file\n" RESET);
        return;
    }

    /* Print update message */
    if (strcmp(option, "-t") == 0)
    {
        printf(GREEN "| 🎶  Updated Title : %s\n" RESET, new_text);
        printf(GRAY "+---------------------------------------------------------------------------+\n" RESET);
        printf(ORANGE "✔️  Title updated successfully\n" RESET);
    }
    else if (strcmp(option, "-A") == 0)
    {
        printf(GREEN "| 🎤  Updated Artist : %s\n" RESET, new_text);
        printf(GRAY "+---------------------------------------------------------------------------+\n" RESET);
        printf(ORANGE "✔️  Artist updated successfully\n" RESET);
    }
    else if (strcmp(option, "-a") == 0)
    {
        printf(GREEN "| 💿  Updated Album : %s\n" RESET, new_text);
        printf(GRAY "+---------------------------------------------------------------------------+\n" RESET);
        printf(ORANGE "✔️  Album updated successfully\n" RESET);
    }
    else if (strcmp(option, "-y") == 0)
    {
        printf(GREEN "| 📅  Updated Year : %s\n" RESET, new_text);
        printf(GRAY "+---------------------------------------------------------------------------+\n" RESET);
        printf(ORANGE "✔️  Year updated successfully\n" RESET);
    }
    else if (strcmp(option, "-c") == 0)
    {
        printf(GREEN "| 🎼  Updated Genre : %s\n" RESET, new_text);
        printf(GRAY "+---------------------------------------------------------------------------+\n" RESET);
        printf(ORANGE "✔️  Genre updated successfully\n" RESET);
    }
    else if (strcmp(option, "-C") == 0)
    {
        printf(GREEN "| 💬  Updated Comment : %s\n" RESET, new_text);
        printf(GRAY "+---------------------------------------------------------------------------+\n" RESET);
        printf(ORANGE "✔️  Comment updated successfully\n" RESET);
    }
}