/*
Documentation

Name        : Naveenkumar Kammar
Student id  : 26018_039
Batch id    : 26018D
Start Date  : 24/09/2026
End date    : 04/10/2026

Description:
The following main functions are implemented in the MP3 Tag Reader and Editor project to view and modify MP3 metadata.

main()                  :-> Handles command line arguments and selects the required operation such as help, view or edit.
                         -> Validates the arguments, checks the MP3 file extension and calls the appropriate function.

view_mp3_details()      :-> Reads and displays the metadata details from an MP3 file.
                         -> Displays the song title, artist, album, year, genre and comment stored in the ID3 tags.

edit_mp3_details()      :-> Modifies the selected metadata field of an MP3 file based on the edit option provided.
                         -> Updates the selected ID3 tag while preserving the remaining metadata and audio data.

print_help_menu()       :-> Displays the available commands and operations supported by the project.
                         -> Provides the options for viewing MP3 details and editing the title, artist, album, year, genre and comment.


Sample input:
$ ./a.out -v AudioFile1.mp3

sample output:
+=============================================================================+
|                      🎵 MP3 TAG READER AND EDITOR 🎵                       |
+=============================================================================+

========================= 👁️ VIEW AUDIO FILE DETAILS =========================

Loading...[###########################################################] 100%

+---------------------------------------------------------------------------+
| 🎶  Title   : Sunnysunny
| 🎤  Artist  : Yo Yo Honey Singh - [SongsPk.CC]
| 💿  Album   : Yaariyan
| 📅  Year    : 2013
| 🎼  Genre   : Bollywood Music - [SongsPk.CC]
| 💬  Comment : eng
+---------------------------------------------------------------------------+

================== AUDIO FILE DETAILS DISPLAYED SUCCESSFULLY =================

✨✨✨   THANK YOU!   ✨✨✨

*/





#include <stdio.h>
#include <string.h>
#include "mp3.h"

/* Main function */
int main(int argc, char *argv[])
{
    int j, delay, progress;

    /* Validate command line arguments */
    if (argc < 2)
    {
        /* Display invalid argument message */
        print_invalid_arguments();
    }

    /* Display help menu */
    else if ((((strcmp(argv[1], "-h")) == 0) || (strcmp(argv[1], "-help")) == 0))
    {
        /* Display project title */
        print_project_title();

        printf(MAGENTA "\n================================ 📖 HELP MENU ================================\n" RESET);

        /* Display help menu */
        print_help_menu();

        printf(MAGENTA "\n==============================================================================\n\n" RESET);
        printf(BLUE "\r✨✨✨   THANK YOU!   ✨✨✨\n\n" RESET);
    }

    /* Check view option */
    else if (strcmp(argv[1], "-v") == 0)
    {
        /* Validate view arguments */
        if (argc != 3)
        {
            /* Display invalid argument message */
            print_invalid_arguments();
        }
        else
        {
            /* Check MP3 file extension */
            if (strlen(argv[2]) >= 4 && strcmp(argv[2] + strlen(argv[2]) - 4, ".mp3") == 0)
            {
                /* Display project title */
                print_project_title();

                printf(MAGENTA "\n========================= 👁️  VIEW AUDIO FILE DETAILS =========================\n\n" RESET);

                /* Display loading progress */
                for (progress = 1; progress <= 100; progress++)
                {
                    printf(DARK_PURPLE "\rLoading...[" RESET);
                    for (j = 1; j <= 59; j++)
                    {
                        j <= progress * 59 / 100 ? printf(DARK_PURPLE "#" RESET) : printf(DARK_PURPLE " " RESET);
                    }
                    printf(DARK_PURPLE "] %i%%" RESET, progress);
                    fflush(stdout);
                    for (delay = 0xffff00; delay--;);
                }
                printf("\n");

                printf(GRAY "\n+---------------------------------------------------------------------------+\n" RESET);

                /* Display MP3 details */
                view_mp3_details(argv[2]);

                printf(GRAY "+---------------------------------------------------------------------------+\n" RESET);
                printf(MAGENTA "\n================== AUDIO FILE DETAILS DISPLAYED SUCCESSFULLY =================\n\n" RESET);
                printf(BLUE "\r✨✨✨   THANK YOU!   ✨✨✨\n\n" RESET);
            }
            else
            {
                printf(RED "File is not an MP3 file\n" RESET);
            }
        }
    }

    /* Check edit option */
    else if (strcmp(argv[1], "-e") == 0)
    {
        /* Validate edit arguments */
        if (argc < 5)
        {
            print_invalid_arguments();
        }

        /* Check MP3 file extension */
        else if (strlen(argv[2]) >= 4 && strcmp(argv[2] + strlen(argv[2]) - 4, ".mp3") == 0)
        {
            char new_text[256];
            int i;

            /* Initialize new text */
            new_text[0] = '\0';

            for (i = 4; i < argc; i++)
            {
                /* Combine command line text */
                strcat(new_text, argv[i]);

                /* Add space between words */
                if (i < argc - 1)
                {
                    strcat(new_text, " ");
                }
            }

            /* Display project title */
            print_project_title();

            printf(MAGENTA "\n========================= ✏️  EDIT AUDIO FILE DETAILS =========================\n\n" RESET);

            /* Edit MP3 details */
            edit_mp3_details(argv[2], argv[3], new_text);

            printf(MAGENTA "\n==================== AUDIO FILE DETAILS EDITED SUCCESSFULLY ==================\n\n" RESET);
            printf(BLUE "\r✨✨✨   THANK YOU!   ✨✨✨\n\n" RESET);
        }
        else
        {
            printf(RED "File is not an MP3 file\n" RESET);
        }
    }

    /* Handle invalid option */
    else
    {
        /* Display invalid argument message */
        print_invalid_arguments();
    }

    return 0;
}