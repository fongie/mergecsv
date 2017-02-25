/* mergeconstant.c inputs a specified constant into a column in the spreadsheet
 * used in Ida Erikssons master project
 * Code derived from mergecsv.c
 * both by Max Körlinge
 */
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define DELIMITER ','
#define OUTPUT_FILE_NAME "merge_out.csv"

/* Merges data from inpbuff with the mergebuff line, by insertings the data in int column 
 * returns a char* buffer with the new merged line, ready to be inserted into output file
 */
char *merge_line(char *constant, char *mergebuff, char *outbuff, int column) {
 
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

    /* Input constant into its place
     */
    int j = 0;
    while (constant[j] != '\0') {
        outbuff[i++] = constant[j++];
    }
    //i is now pointing to next empty spot in outbuff, j is no more used.

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

int fileExist(const char *filename) {
    return GetFileAttributes(filename) != INVALID_FILE_ATTRIBUTES;
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("\n\t\tError: None or too many arguments provided.\n\n \
                Use: '%s file constant column'\n\n \
                where file is file we are writing to\n \
                constant is the constant we are putting in\n \
                and column = number of the column (note: must be empty in the file)\n \
                ", argv[0]);
        return 1;
    }

    /* Handling arguments (putting them into vars)
     */
    const char *input_file_name = argv[1];
    char *constant = argv[2];
    long column;
    column = strtol(argv[3], NULL, 10);

    printf("Attempting to write the constant %s at column %d in '%s'...\n", constant, column, argv[1]);

    /* Check if file names exist
     */
    if (!(fileExist(input_file_name))) {
        printf("Cannot find file: '%s'!\n Exiting.\n", input_file_name);
        return 1;
    }
        
    /* Open necessery files to READ and WRITE
     * and create their respective stringbuffers (for lines to be fed into)
     */
    FILE *input_file;
    input_file = fopen(input_file_name, "r");
    FILE *output_file;
    output_file = fopen(OUTPUT_FILE_NAME, "w");

    char mergebuff[512]; //mergebuff larger because this file can have really big lines
    char outbuff[512];
    
    /* Read lines we want to skip:
     * and input the ones from mergefile into output
     * mergefile - first two lines
     * This assumes that the data in mergefile starts on LINE 3
     */
    fgets(mergebuff, 512, (FILE*)input_file);
    fputs(mergebuff, output_file);
    fgets(mergebuff, 512, (FILE*)input_file);
    fputs(mergebuff, output_file);

    /* Main loop where each line is merged and put into file "merge_out.csv"
     */
    fgets(mergebuff, 512, (FILE*)input_file); //needed to init cause "else" statement below should not
    while (1) {

        if (feof(input_file))
            break;

        merge_line(constant, mergebuff, outbuff, column);
        //printf("Outbuff after merge: %s", outbuff);
        //fprintf(output_file, "%s", outbuff);
        fputs(outbuff, output_file);
        fgets(mergebuff, 512, (FILE*)input_file);
    }
    
    fclose(input_file);
    fclose(output_file);

    printf("Success! The output is in 'merge_out.csv'");

    return 0;
}
