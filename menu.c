#include <stdio.h>
#include "mp3.h"

/* Display project title */
void print_project_title(void)
{
    printf(CYAN "\n+=============================================================================+\n" RESET);
    printf(DARK_BLUE "|                      🎵  MP3 TAG READER AND EDITOR 🎵                       |\n" RESET);
    printf(CYAN "+=============================================================================+\n" RESET);
}

/* Display invalid argument message */
void print_invalid_arguments(void)
{
    printf(GRAY "\n+----------------------------------------------------------------------------+\n" RESET);
    printf(RED "| ERROR : INVALID ARGUMENTS IN COMMAND                                       |\n" RESET);
    printf(YELLOW "|                                                                            |\n" RESET);
    printf(YELLOW "| USAGE :                                                                    |\n" RESET);
    printf(YELLOW "| To view Use : './a.out -v filename.mp3'                                    |\n" RESET);
    printf(YELLOW "| To edit Use : './a.out -e filename.mp3 -t/-A/-a/-y/-c/-C changing_text'    |\n" RESET);
    printf(YELLOW "| To get help use : './a.out -h/-help'                                       |\n" RESET);
    printf(GRAY "+----------------------------------------------------------------------------+\n\n" RESET);
}

/* Display help menu */
void print_help_menu(void)
{
    printf(GRAY "\n+--------- 🆘 Main Operations ---------+\n" RESET);
    printf(DARK_GREEN "| 1. -v -> to view mp3 file contents   |\n" RESET);
    printf(DARK_GREEN "| 2. -e -> to edit mp3 file contents   |\n" RESET);
    printf(DARK_GREEN "| 3. -h -> to view help menu           |\n" RESET);
    printf(GRAY "+--------------------------------------+\n" RESET);
    printf(GRAY "\n+--------- 🛠️  Edit Operations ---------+\n" RESET);
    printf(DARK_GREEN "| 1. -t -> to edit song title          |\n" RESET);
    printf(DARK_GREEN "| 2. -A -> to edit artist name         |\n" RESET);
    printf(DARK_GREEN "| 3. -a -> to edit album name          |\n" RESET);
    printf(DARK_GREEN "| 4. -y -> to edit year                |\n" RESET);
    printf(DARK_GREEN "| 5. -c -> to edit genre               |\n" RESET);
    printf(DARK_GREEN "| 6. -C -> to edit comment             |\n" RESET);
    printf(GRAY "+--------------------------------------+\n" RESET);
    printf(GRAY "\n+------------------------ 💻 Command Line Formats ------------------------+\n" RESET);
    printf(DARK_GREEN "| To view Use : './a.out -v filename.mp3'                                 |\n" RESET);
    printf(DARK_GREEN "| To edit Use : './a.out -e filename.mp3 -t/-A/-a/-y/-c/-C changing_text' |\n" RESET);
    printf(DARK_GREEN "| To get help use : './a.out -h/-help'                                    |\n" RESET);
    printf(GRAY "+-------------------------------------------------------------------------+\n" RESET);
}