#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#define BFSZ	1024

extern char encrypt(char inchar, char key);
extern char decrypt(char inchar, char key);
void usage(char **argv);

int set = 0;
int decrypt_prog = -1; //1 if decrypt, 0 if encrypt
char* Bookfile = NULL;
int offset_val = 0;

int main (int argc, char **argv) {
    //parse the flags
    int opt;
    while ((opt = getopt(argc, argv, "deb:o:")) != -1) {
        switch(opt){
            case 'd':
                if (!set){
                    set = 1;
                    decrypt_prog = 1;
                }
                break;

            case 'e':
                if (!set){
                    set = 1;
                    decrypt_prog = 0;
                }
                else{
                    fprintf(stderr, "Cannot query both d and e\n");
                    return EXIT_FAILURE;
                }
                break;

            case 'b':
                Bookfile = optarg;
                //Bookfile = malloc(strlen(optarg)); 
                //strcpy(Bookfile, optarg); //string ?????????
                break;

            case 'o':
                offset_val = (int)strtol(optarg,NULL,0);
                break;

            case '?':
                fprintf(stderr,"incorrect input\n");
                return EXIT_FAILURE;

            default:
                fprintf(stderr,"incorrect input\n");
                return EXIT_FAILURE;
        }

    } //end while loop

    if (!set){
        usage(argv);
        return EXIT_FAILURE;
    }
    if (Bookfile==NULL){
        fprintf(stderr, "Missing input: Bookfile path\n");
        return EXIT_FAILURE;
    }

    char *encryption_file = argv[optind];
    if (encryption_file==NULL){
        fprintf(stderr,"Unable to open file: (null)\n");
        return EXIT_FAILURE;
    }

    FILE *BookPtr = NULL;
    struct stat BookInfo;
    long int BookSize;

    // open the file
    if ((BookPtr = fopen(Bookfile, "r")) == NULL){
        fprintf(stderr,"Unable to open file: (null)\n");
        return EXIT_FAILURE;
    }
    // get the size
    if (stat(Bookfile, &BookInfo)!=0){
        printf("Failed to retrieve the book info\n");
        return EXIT_FAILURE;
    }
    else{
        BookSize = BookInfo.st_size;
    }

    //read to an array
    char *BookBuffer = malloc(BookSize);
    fread(BookBuffer,1,BookSize,BookPtr);
    fclose(BookPtr);

    //encrypt
    FILE *encryFilePtr;
    char *encryLine;
    char encryBuffer[BookSize];
    int key_index;
    
    if (!decrypt_prog){
        
        if ((encryFilePtr = fopen(encryption_file, "w+")) == NULL){
            fprintf(stderr,"Unable to open file: (null)\n");
            return EXIT_FAILURE;
        }

        key_index = offset_val%BookSize; //where to start
        if (stdin==NULL){
            return EXIT_FAILURE;
        }
        while ((encryLine = fgets(encryBuffer,BFSZ, stdin)) != NULL){
            for (int i=0; i<(int)strlen(encryBuffer); i++){
                char inchar = encryLine[i];
                char key = BookBuffer[key_index%BookSize];
                char output = encrypt(inchar,key);
                key_index++;
                fwrite(&output, sizeof(char), 1, encryFilePtr);////
            }  
        }

        fclose(encryFilePtr);
    }

    else if (decrypt_prog){

        if ((encryFilePtr = fopen(encryption_file, "r")) == NULL){
            fprintf(stderr,"Unable to open file: (null)\n");
            return EXIT_FAILURE;
        }

        key_index = offset_val%BookSize;
        
        while(fread(encryBuffer, sizeof(char),1, encryFilePtr)==1){
            char inchar = encryBuffer[0];
            char key = BookBuffer[key_index%BookSize];
            char output = decrypt(inchar,key);
            key_index++;
            printf("%c", output);
        }

        fclose(encryFilePtr);
    }
    free(BookBuffer);
}

void usage(char **argv) {
    fprintf(stderr,"Usage: %s (-d|-e) -b <bookfile> [-o <offset>] <filename>\n", *argv);
    return;
}
