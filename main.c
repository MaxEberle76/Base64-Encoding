#include <stdio.h>
#define max_size 136
#define ERR -1
//The max_size was defined to be the same size as our biggest array
//Also so that the array sizes can be easily changed for different files

int main(void) {

    //Defining our files
    FILE *to_be_read;
    FILE *read_me_second;
    FILE *to_be_written;

    //Defining our arrays
    int encode[100] = {0};
    char key[64];
    char write_back[max_size];
    int translate[max_size] = {0};

    //Defining and initializing our counters
    int i = 0;
    int n = 0;
    int k = 0;
    int j;
    int c;
    int x;

    //We start with the Key.dat file. First, we open it in read mode...

    to_be_read = fopen("E:\\CLion\\Base64-Encoding\\Key.dat","r");

    if (to_be_read == NULL) {
        printf("The Key.dat file cannot be found\n");
        return ERR;
    }
    else {
        printf("Key.dat file opened successfully\n");
    }
    //...and make sure it was read properly. Then we copy its contents to our "key" array.
    do {
        key[i] = fgetc(to_be_read);
        i++;
    } while(feof(to_be_read) == 0);

    //Don't forget to close the file after!
    fclose(to_be_read);


    //Time to read the Raw.dat file, then copy it into our "encode" array

    read_me_second = fopen("E:\\CLion\\Base64-Encoding\\Raw.dat","r");

    if (read_me_second == NULL) {
        printf("The Raw.dat file cannot be found\n");
        return ERR;
    }
    else {
        printf("Raw.dat file opened successfully\n");
    }
    //Here we are basically copying each character individually until the end of the file is detected
    do {
        encode[n] = fgetc(read_me_second);
        n++;
    } while(feof(read_me_second) == 0);

    //Again, we have to close the file after
    fclose(read_me_second);

    //Now for the fun part, the base64 encoding
    //We start this by setting up our masks. We can accomplish the encoding with just 3 masks
    int mask1 = 0xFC; //0b 1111 1100
    int mask2 = 0xF0; //0b 1111 0000
    int mask3 = 0xC0; //0b 1100 0000

    //Here we begin the encoding loop
    for(c = 0, j = 0; c < max_size; c++, j++) {

        //Since every 3 input values represents 4 output values, we have to account for this
        if((k % 4) == 0 && k != 0) {
            j--;
        }

        //The main part of the loop is split into 4 sections since the process repeats every 4 values
        //We begin by accounting for the first 6 bits of the first input value
        if ((c % 4) == 0) {

            translate[c] = (encode[j] & mask1) >> 2;
        }
            //Then the next 6 bits from the first and second input value
        else if((c % 4) == 1) {

            translate[c] = ((encode[j - 1] & ~mask1) << 4) | ((encode[j] & mask2) >> 4);
        }
            //Then the next 6 bits from the second and third input value
        else if((c % 4) == 2) {

            translate[c] = ((encode[j - 1] & ~mask2) << 2) | ((encode[j] & mask3) >> 6);
        }
            //And finally the last 6 bits from the third input value
        else if((c % 4) == 3) {

            translate[c] = (encode[j - 1] & ~mask3);
        }
        //We increment the counter to keep us in the right pattern, and repeat
        k++;

    }
    //Here, our final value is 4 bits short, so we need to clear the remaining bits and pad it
    translate[max_size - 3] = (translate[max_size - 3] & 0x30); //0x30 = 0b 0011 0000
    translate[max_size - 2] = translate[max_size - 1] = '=';

    //Now we get to open our Encoded.dat file in write mode

    to_be_written = fopen("E:\\CLion\\Base64-Encoding\\Encoded.dat","w");

    if (to_be_written == NULL) {
        printf("The Encoded.dat file cannot be found\n");
        return ERR;
    }
    else {
        printf("Encoded.dat file opened successfully\n");
    }

    //Here we are looping all of the values from our translated file and using the key to encode them into our
    //file that will be written
    for(i = 0; i < (max_size - 2); i++) {

        x = translate[i];
        write_back[i] = key[x];
    }
    //Making sure the padding characters are included
    write_back[max_size - 2] = translate[max_size - 2];
    write_back[max_size - 1] = translate[max_size - 1];

    fprintf(to_be_written, "%s", write_back);

    fclose(to_be_written);

    //And we are done!
    return 0;
}
