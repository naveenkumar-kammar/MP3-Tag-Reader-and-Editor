#ifndef MP3_H
#define MP3_H

#include <stdio.h>

/* Define terminal text colors */
#define RESET "\033[0m"
#define RED "\033[0;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN "\033[0;36m"
#define WHITE "\033[0;37m"
#define DARK_GREEN "\033[0;32m"
#define DARK_BLUE "\033[1;34m"
#define DARK_PURPLE "\033[1;35m"
#define GRAY "\033[0;90m"
#define ORANGE "\033[38;5;208m"

/* Declare project title function */
void print_project_title(void);

/* Declare invalid argument function */
void print_invalid_arguments(void);

/* Declare help menu function */
void print_help_menu(void);

/* Declare MP3 view function */
void view_mp3_details(char *file_name);

/* Declare frame ID function */
int read_frame_id(FILE *fptr, char *frame_id);

/* Declare frame size function */
unsigned int read_frame_size(FILE *fptr);

/* Declare frame encoding function */
int read_frame_encoding(FILE *fptr, unsigned char *frame_encode);

/* Declare frame data function */
int read_frame_data(FILE *fptr, unsigned int frame_size, char *frame_data);

/* Declare frame display function */
void display_frame(char *frame_id, char *frame_data);

/* Declare MP3 edit function */
void edit_mp3_details(char *file_name, char *frame_id, char *new_text);

#endif