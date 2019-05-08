/**//*
 * This program will load a text file and print it to screen
 * 4/2/2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>


// Read all of the lines from file and put into an array
// line in an array pointed to by lines_array_ptr .
void read_lines_from_file(char *file_name, char ***lines_array_ptr)
{
    char *line = NULL;
    size_t lsize;
    int num_lines = 0;

    int start_of_line = 0;
    int len_current_line = 0;
    char c;
    int line_counter = 0;

    // opens a file
    FILE *fp = fopen(file_name, "r"); // opens file in read mode
    if (fp == NULL){
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE); //bad news, quit program
    }

    // count number of lines in file
    while ((c = getc(fp)) != EOF) {
        if (c == '\n') num_lines++;
    }
    char **lines_array = malloc(sizeof(char *) * num_lines);


    fseek(fp, 0, SEEK_SET); //rewinds pointer to top of file

    //printf("reading thru file\n");
    /* Loop through until we are done with the file. */
    /* Get the next line */

    while(getline(&line, &lsize, fp) != -1) {
        lines_array[line_counter] = strdup(line);
        //printf("Line is %s\n", line);
        //
        // /* Show the line details */
        // printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_counter,
        //  line_size, lsize, line);
        // //printf("Conditional: %d, %d\n", line_counter >= start_line, line_counter < start_line + n_lines);
        // if(line_counter >= start_line && line_counter < start_line + n_lines){
        //     (*lines_array_ptr)[line_counter - start_line] = strdup(line); //character pointer
        //     //printf("array %s\n", *lines_array_ptr[line_counter]);
        //     //printf("WROTE LINE\n");
        //     //printf("Line is: %s\n", line);
        // }
        // /* Increment our line counter */
        line_counter++;
    }
    // for format lines
    // while(line_counter < start_line + n_lines) {
    //     (*lines_array_ptr)[line_counter] = strdup("\n");
    //     line_counter++;
    // }
    //printf("Loop completed.\n");
    free(line); //free line buffer
    fclose(fp);

    *lines_array_ptr = lines_array;
    // printf("Testing line from lines_array: %s\n", lines_array[3]);
    // printf("Testing line: %s\n", (*lines_array_ptr)[3]);
}

//this function does this _____
int count_digits(int n)
{
    int c = 0;
    while (n) {
        n = n / 10;
        c++;
    }
    return c;
}

// takes in the file pointer, array of pointers to line contents,
// number of lines and number of columns
// formats them to fill the terminal display window
// adds line numbers with a gutter
/*
void get_formatted_lines(State *state, char ***lines_array_ptr, int n_lines, int n_cols)
{
    int gutter_size;
    char **raw_lines;
    char *line;

    // get info about window size
    // calculate gutter size
    gutter_size = 2 + count_digits(start_line + n_lines);

    // get raw lines
    //read_lines_from_file(fp, &raw_lines, start_line, n_lines);

    int line_length = strlen(raw_lines[i])+1;

    char **formatted_line_ptrs = malloc(sizeof(char*)*n_lines);
    // write raw lines
    for (int i = 0; i < n_lines; i++) {
        line = malloc(n_cols * sizeof(char));
        *formatted_line_ptrs[i] = line;
        // memcopy raw into line (destination, source, number of bytes)
        if(line_length + gutter_size < n_cols){
            memcpy(
                (void *)(&line + gutter_size),
                (void *) raw_lines[i],
                (line_length) * sizeof(char)
            );
        }
    }
    *lines_array_ptr = raw_lines;

    // trim lines
    // lines = malloc(sizeof(char *[n_env_lines]));
    // for (int )

    // draw line numbers
}
*/

void print_lines(char **lines_array_ptr, int n_lines)
{
    for (int i = 0; i < n_lines; i++) {
        // if(i<10){
        //     printf("0%d %s", i, lines_array_ptr[i]);
        // }
        // else{
        //     printf("%d %s", i, lines_array_ptr[i]);
        // }
        printf("%s\n",lines_array_ptr[i]);
    }
}


//our three "modes" for the editor
enum Mode {
    normal = 0,
    insert = 1,
    command = 2,
};


// define the state of our window
typedef struct {
    char **lines; // array of lines from our file
    int top_line; // the line index of first line to be displayed
    int window_height, window_width; // window dimensions
    int cursor_row, cursor_col; //referenced to lines not window
    enum Mode mode;
    // maybe some command history stuff
} State;

/* Make a State.
*/
State *make_state(char *file_name) {

    State *state = malloc(sizeof(State)); //dynamically allocate memory
    read_lines_from_file(file_name, &(state->lines));
    state->top_line = 0;
    // state->window_width = num_cols;
    // state->window_height = num_rows;
    state->mode = normal;
    state->cursor_row = 0;
    state-> cursor_col = 0;

    return state;
}


/*   Prints out the current "state" object
    -formatted lines
    -printed to window
    -prints cursor with color indicator
    -prints Mode
*/
void print_state(State *state, int num_columns, int num_rows){
    //print u"\u001b[31mHelloWorld\u001b[0m";
    printf("\033[1;31m");
    printf("Running function: print_state\n");
    printf("\033[0m");

    // int n_lines
    int gutter_size;
    char **raw_lines;
    char *line;
    char spaces[5] = "   ";
    char zero[3] = "0";
    char numbers[3];
    // calculate gutter size
    gutter_size = 2 + count_digits(state->top_line + num_rows);
    // printf("Gutter is %d\n", gutter_size);

    char **formatted_line_ptrs = malloc(sizeof(char*) * num_rows);
    // write raw lines

    char strC[50];

    for (int i = 0; i < num_rows; i++) {
        line = malloc(num_columns * sizeof(char));
        int line_length = strlen(state->lines[i]+1);
        // printf("Line length is %d\n", line_length);
        char *newline = (state->lines)[i];

        // if less than 10
        char result[50];
        sprintf(result, "%d", i);
        if(i < 10){
            strncpy(strC, zero, 4);
            strcat(strC, result);
            strncpy(result, strC, 5);
        }

        strncpy(line, result, 3);
        strncat(line, spaces, 4);
        if(line_length < num_columns){
            strncat(line, newline, line_length);
        }
        else{
            strncat(line, newline, num_columns-gutter_size-1); //dest, source, size
        }
        formatted_line_ptrs[i] = line;
    }
    print_lines(formatted_line_ptrs, 24);
    // print_lines((state->lines), 24);
}


int main (int argc, char *argv[])
{
    // declarations
    struct winsize w;
    int n_env_lines, n_env_cols;
    char **lines_array;

    // make state (reading from file)
    State *state = make_state(argv[1]);

    // Loop
    while(1) {
        // update(state)
        // update_state(state);

        // get window dimensions
        if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &w)) {
            printf("Unable to access line or column counts\n");
            exit(EXIT_FAILURE);
        }

        n_env_lines = w.ws_row;
        n_env_cols = w.ws_col;
        printf("Number of lines is: %i\n", n_env_lines);

        // print(state, width, height)
        print_state(state, n_env_cols, n_env_lines);

        break;
    }

    // write file
    // write_state_to_file(argv[1]);
    // cleanup
    // free stuff





    return 0;
}
