#include <stdio.h>
#include <string.h>
#include "mp3.h"

/* Display MP3 details */
void view_mp3_details(char *file_name)
{
    /* Variables for storing frame information */
    int i;
    char frame_id[5];
    unsigned int size;
    unsigned char frame_encode;
    char frame_data[100];

    /* File pointer for MP3 file */
    FILE *fptr;

    /* Open MP3 file */
    fptr = fopen(file_name, "rb");

    /* Validate file opening */
    if (fptr == NULL)
    {
        printf(RED "Unable to open file\n" RESET);
        return;
    }

    /* Skip ID3 header */
    fseek(fptr, 10, SEEK_SET);

    /* Read all MP3 frames */
    for (i = 0; i < 6; i++)
    {
        /* Read 4-byte frame ID */
        if (read_frame_id(fptr, frame_id) == 0)
        {
            printf(RED "Failed to read frame ID\n" RESET);
            fclose(fptr);
            return;
        }

        /* Read frame size bytes */
        size = read_frame_size(fptr);
        if (size == 0)
        {
            printf(RED "Failed to read frame size\n" RESET);
            fclose(fptr);
            return;
        }

        /* Skip frame flags */
        if (fseek(fptr, 2, SEEK_CUR) != 0)
        {
            printf(RED "Failed to skip frame flags\n" RESET);
            fclose(fptr);
            return;
        }

        /* Read frame encoding */
        if (read_frame_encoding(fptr, &frame_encode) == 0)
        {
            printf(RED "Failed to read frame encoding\n" RESET);
            fclose(fptr);
            return;
        }

        /* Read frame data */
        if (read_frame_data(fptr, size, frame_data) == 0)
        {
            printf(RED "Failed to read frame data\n" RESET);
            fclose(fptr);
            return;
        }

        /* Display frame details */
        display_frame(frame_id, frame_data);
    }

    /* Close MP3 file */
    fclose(fptr);
}

/* Read frame ID */
int read_frame_id(FILE *fptr, char *frame_id)
{
    /* Read frame data */
    if (fread(frame_id, 4, 1, fptr) != 1)
    {
        return 0;
    }

    /* Add string terminator */
    frame_id[4] = '\0';

    return 1;
}

/* Read frame size */
unsigned int read_frame_size(FILE *fptr)
{
    /* Variables for storing 4-byte frame size and converted size */
    unsigned char frame_size[4];
    unsigned int size;

    /* Read frame size bytes */
    if (fread(frame_size, 1, 4, fptr) != 4)
    {
        return 0;
    }

    /* Convert frame size */
    size = ((unsigned int)frame_size[0] << 24) | ((unsigned int)frame_size[1] << 16) |
           ((unsigned int)frame_size[2] << 8) | (unsigned int)frame_size[3];

    return size;
}

/* Read frame encoding */
int read_frame_encoding(FILE *fptr, unsigned char *frame_encode)
{
    /* Read encoding byte */
    if (fread(frame_encode, 1, 1, fptr) != 1)
    {
        return 0;
    }

    return 1;
}

/* Read frame data */
int read_frame_data(FILE *fptr, unsigned int frame_size, char *frame_data)
{
    /* Variable for storing actual frame data size excluding encoding byte */
    unsigned int data_size;

    /* Calculate data size */
    data_size = frame_size - 1;

    /* Limit data size */
    if (data_size > 99)
    {
        data_size = 99;
    }

    /* Read frame data */
    if (fread(frame_data, 1, data_size, fptr) != data_size)
    {
        return 0;
    }

    /* Add string terminator */
    frame_data[data_size] = '\0';

    return 1;
}

/* Display frame details */
void display_frame(char *frame_id, char *frame_data)
{
    /* Display title */
    if (strcmp(frame_id, "TIT2") == 0)
    {
        printf(GREEN "| 🎶  Title   : %s\n" RESET, frame_data);
    }

    /* Display artist */
    else if (strcmp(frame_id, "TPE1") == 0)
    {
        printf(GREEN "| 🎤  Artist  : %s\n" RESET, frame_data);
    }

    /* Display album */
    else if (strcmp(frame_id, "TALB") == 0)
    {
        printf(GREEN "| 💿  Album   : %s\n" RESET, frame_data);
    }

    /* Display year */
    else if (strcmp(frame_id, "TYER") == 0)
    {
        printf(GREEN "| 📅  Year    : %s\n" RESET, frame_data);
    }

    /* Display genre */
    else if (strcmp(frame_id, "TCON") == 0)
    {
        printf(GREEN "| 🎼  Genre   : %s\n" RESET, frame_data);
    }

    /* Display comment */
    else if (strcmp(frame_id, "COMM") == 0)
    {
        printf(GREEN "| 💬  Comment : %s\n" RESET, frame_data);
    }
}