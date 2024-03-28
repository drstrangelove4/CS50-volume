#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------------------------------------------------

// Function Signatures
int check_inputs(int total_inputs);
int convert_file(char input_file[], char output_file[], float ratio);

// ----------------------------------------------------------------------------------------------------------------------

// Constants
const int HEADER_SIZE = 44;
const int REQUIRED_INPUTS = 4;

// ----------------------------------------------------------------------------------------------------------------------

// Custom types used for copying data
typedef int16_t BYTE;

// ----------------------------------------------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
    /// Check for valid inputs
    if (check_inputs(argc) == 1)
    {
        return 1;
    }

    // Convert the scaling factor from a string to a double
    float scaling_factor = strtof(argv[argc - 1], NULL);
    if (scaling_factor == 0.0)
    {
        printf("There is nothing to do\n");
        return 1;
    }

    // Convert the file to it's new volume
    int status = convert_file(argv[1], argv[2], scaling_factor);
    if (status != 0)
    {
        printf("Could not open file.\n");
        return 1;
    }

    return 0;
}

// ----------------------------------------------------------------------------------------------------------------------

int check_inputs(int total_inputs)
// Returns 0 if there are 4 inputs, else returns 1.
{
    if (total_inputs < REQUIRED_INPUTS || total_inputs > REQUIRED_INPUTS)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }
    return 0;
}

// ----------------------------------------------------------------------------------------------------------------------

int convert_file(char input_file[], char output_file[], float ratio)
// Responsible for handling files
{
    // Open the file at PATH provided by user
    FILE *src = fopen(input_file, "r");

    // Check if there was an error (like wrong path)
    if (src == NULL)
    {
        return 1;
    }

    // Make a file at the destination PATH
    FILE *dst = fopen(output_file, "w");

    // Use the BYTE type to copy data
    BYTE copy_byte;

    // Copy the header
    uint8_t header[HEADER_SIZE];
    fread(header, HEADER_SIZE, 1, src);
    fwrite(header, HEADER_SIZE, 1, dst);

    // While we are still reading data from source, copy it to destination
    while (fread(&copy_byte, sizeof(copy_byte), 1, src) != 0)
    {
        // Change the volume by the ratio factor and write the data.
        copy_byte *= ratio;
        fwrite(&copy_byte, sizeof(copy_byte), 1, dst);
    }

    // Close files
    fclose(dst);
    fclose(src);

    return 0;
}
