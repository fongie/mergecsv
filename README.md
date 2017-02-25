## mergecsv
by Max Körlinge

mergecsv is a small c program for merging a column of data in a .csv file into the column of another .csv file.

mergeconstant.c is complementary, if you wish to input a constant into an entire column in a .csv sheet, using the same logic as in mergecsv.

### Usage

0. Change definitions at top of code to suit your data needs (like lines to skip, which column is data in, etc)
1. Compile (only tested with gcc)
2. Command line: mergecsv (will tell you the syntax)
3. Output is in new file merge_out.csv (note, if this file exists, it is overridden when the program runs)

### Restrictions
Note that the columns structure is somewhat specific right now, requiring the .csv files to look like this:

* Each data "line" in the csv files need to start like this: year,month,day,.... Program checks if these are the same, to see if the data for this day exists in input file or if it should be skipped. If you wish to change this, it's in the check_date function.
* Input file data needs to start at column the column after "day" (column 4). The program was created for files where the input file only has one column (column 4) to input, but it should work as well if you for example want to input all columns that follow as well, because the program inputs everything after the year,month,date columns into the output file, at the spot specified by column.

If someone is looking for the program to work differently, you can either change it to your needs, or contact me and I might be able to help out.

