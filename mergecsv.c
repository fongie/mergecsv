/* mergecsv.c
 *
 * Working with .csv files:
 * Puts the data from column INPUT_DATA_COLUMN (change if you wish) from file in arg1 to file in arg2, at column specified in arg3.
 * Result is output to OUTPUT_FILE_NAME (change if you wish).
 * Just type "mergecsv" in command line to get this info on how to use it.
 *
 * This code was created by Max Körlinge to help Ida Eriksson with managing huge data sheets for her masters project.
 */
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> //used only for fileexists function, unnecessery?

#define DELIMITER ','
#define OUTPUT_FILE_NAME "merge_out.csv"
#define INPUT_DATA_COLUMN 4
#define MERGE_FILE_LINES_TOSKIP 2
#define INPUT_FILE_LINES_TOSKIP 1

/* Merges data from inpbuff with the mergebuff line, by insertings the data in int column 
 * returns a char* buffer with the new merged line, ready to be inserted into output file
 */
char *merge_line(char *inpbuff, char *mergebuff, char *outbuff, int column) {
 
    //debug
    //printf("mergebuff is:\n%s\n", mergebuff);
    //printf("Inpbuff is:\n%s\n", inpbuff);

    /* Find spot in the mergefiles line where new data should be put
     */
    int f;
    int i = 0;
    for (f = 1; f < column; f++) {
        while (mergebuff[i] != DELIMITER) {
            outbuff[i] = mergebuff[i];
            i++;
        }
        outbuff[i] = mergebuff[i];
        i++;
    } //i is now set on the index where new data should be written
    //we need to mark where in mergebuff we stopped, so we can keep filling that in later
    int mergebuff_marker = i;

    /* Find spot in inputfile where the data to be input is
     */
    int j = 0;
    for (f = 1; f < INPUT_DATA_COLUMN; f++) {
        while (inpbuff[j] != DELIMITER) {
            j++;
        }
        j++;
    } //j is now set on the index where new data is in inputfiles line

    /* Input data from inpbuff into its place
     */
    while (inpbuff[j] != '\n') {
        outbuff[i++] = inpbuff[j++];
    } //i is now pointing to next empty spot in outbuff, j is no more used.

    while ((mergebuff[mergebuff_marker] != 0x01)) { //0x01 is SOH, "start of header", which apparently
                                                    //the csv file separates lines with - NOT nullbyte
        //printf("Outbuff (i-1) = %c\n", outbuff[i-1]);
        outbuff[i++] = mergebuff[mergebuff_marker++];
    }

    //debug
    //printf("Outbuff after inputting rest of line from mergebuffer:\n");
    //printf("%s\n", outbuff);
    return outbuff;
}


/* Returns 1 if date is same in the buffers, or 0 if it is not
 */
int check_date(char *inpbuff, char *mergebuff) {

    int i;
    while (mergebuff[i] != DELIMITER) {
        //printf("%d", i);
        if (mergebuff[i] != inpbuff[i]) {
                //printf("NO MATCH ON YEAR");
                return 0;
                break;
                }
        i++;
        }
    //printf("Year match\n");
    i++;

    while (mergebuff[i] != DELIMITER) {
        //printf("%d", i);
        if (mergebuff[i] != inpbuff[i]) {
                //printf("NO MATCH ON MONTH");
                return 0;
                break;
                }
        i++;
        }
    //printf("Month match\n");
    i++;

    while (mergebuff[i] != DELIMITER) {
        //printf("%d", i);
        if (mergebuff[i] != inpbuff[i]) {
                //printf("NO MATCH ON DAY");
                return 0;
                break;
                }
        i++;
        }
    //printf("Day match\n");
    i++;

    return 1;
}

int fileExist(const char *filename) {
    return GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES;
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("\n\t\tError: None or too many arguments provided.\n\n \
                Use: '%s file1 file2 column'\n\n \
                where file1 = file we are writing from\n \
                file2 = file we are merging into\n \
                and column = number of the column (note: must be empty in the mergefile)\n \
                ", argv[0]);
        return 1;
    }

    printf("Attempting to write contents of '%s' into  column %s in '%s'...\n", argv[1], argv[3], argv[2]);

    /* Handling arguments (putting them into vars)
     */
    const char *input_file_name = argv[1];
    const char *merge_file_name = argv[2];
    long column;
    column = strtol(argv[3], NULL, 10);

    /* Check if file names exist
     */
    if (!(fileExist(input_file_name))) {
        printf("Cannot find input file: '%s'!\n Exiting.\n", input_file_name);
        return 1;
    } else if (!(fileExist(merge_file_name))) {
        printf("Cannot find mergefile: '%s'!\n Exiting.\n", merge_file_name);
        return 1;
    }
        
    /* Open necessery files to READ and WRITE
     * and create their respective stringbuffers (for lines to be fed into)
     */
    FILE *merge_file;
    merge_file = fopen(merge_file_name, "r");
    FILE *input_file;
    input_file = fopen(input_file_name, "r");
    FILE *output_file;
    output_file = fopen(OUTPUT_FILE_NAME, "w");

    char mergebuff[512]; //mergebuff larger because this file can have really big lines
    char inpbuff[255];
    char outbuff[512];
    
    /* Read lines we want to skip:
     * and input the ones from mergefile into output
     * How many lines we skip in files are in **_FILE_LINES_TOSKIP in top of file
     * Lines from the mergefile are automatically input into the output file
     */
    int m, n;
    for (m = 0; m < MERGE_FILE_LINES_TOSKIP; m++) {
        fgets(mergebuff, 512, (FILE*)merge_file);
        fputs(mergebuff, output_file);
    }
    for (n = 0; n < INPUT_FILE_LINES_TOSKIP; n++) {
        fgets(inpbuff, 255, (FILE*)input_file);
    }

    /* Main loop where each line is merged and put into file "merge_out.csv"
     */
    fgets(mergebuff, 512, (FILE*)merge_file); //needed to init cause "else" statement below should not
    fgets(inpbuff, 255, (FILE*)input_file);   //forward input file pointer
    while (1) {

        if (feof(input_file))
            break;
        if (feof(merge_file))
            break;

        if (check_date(inpbuff, mergebuff)) {
            merge_line(inpbuff, mergebuff, outbuff, column);
            //printf("Outbuff after merge: %s", outbuff);
            //fprintf(output_file, "%s", outbuff);
            fputs(outbuff, output_file);
            fgets(mergebuff, 512, (FILE*)merge_file);
            fgets(inpbuff, 255, (FILE*)input_file);
        } else {                                        //if date in mergebuff doesnt exist in inp, print
            fputs(mergebuff, output_file);              //whole mergebuffer and wait with the inpbuffer
            fgets(mergebuff, 512, (FILE*)merge_file);
        }
    }
    
    fclose(merge_file);
    fclose(input_file);
    fclose(output_file);

    printf("Success! The output is in 'merge_out.csv'");

    return 0;
}
