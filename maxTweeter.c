#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FIELD_LENGTH 30
#define MAX_LINES 20000


//function to return the file extension of a filename
const char* get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

//function to count how many fields there are
//and to return which field name field is
int get_name_field(char* line, int* field_count){
    char *token;
    int index = -1;
    *field_count = 0;
    
    token = strtok(line, ",");

    while(token!= NULL){
        //if the field is the name, record what line it is
        if(strcmp(token, "\"name\"") == 0){
            //make sure name field appears only once
            if(index == -1){
                index = *field_count;
            }
            else{
                return -1;
            }
        }
        //count how many fields there are
        *field_count += 1;
        token = strtok(NULL, ",");

    }
    //if name field never found
    if(index == -1){
        return -1;
    } 
    return index;

}

//Structure to hold all the varialbes
//@name: An array that holds the name of the tweeter
//@tweets: The number of tweets a tweeter
struct tweeter{
    char* name;
    int count;
};



int main(int argc,char *argv[])
{
    //The expected number of arguments passed on the command line is
    //2 because even the executables name string is on argc
    //should be of the format: [./executable] [CSV_FILE]
    //@argc: The input from command line
    if(argc!=2){
        printf("unexpected number of arguments\n");
        return -1;
    }

    //put the file name into variable file, and check if the extension is valid
    char* file = argv[1];
    if(strcmp(get_filename_ext(file), "csv") != 0)
    {
        printf("invalid filename\n");
        return -1;
    }
    //open the csv file
    FILE* stream = fopen(file, "r");
    if(stream == NULL){
        printf("File not found!\n");
        return -1;
    }

    char line[1000];
    int field_count, name_field;
    if(fgets(line, 377, stream) == NULL){
        printf("An error occured or File was empty!\n");
        free(stream);
        return -1;
    }
    name_field = get_name_field(line, &field_count);

    if(name_field == -1){
        free(stream);
        printf("No name field in csv!\n");
        return -1;
    }

    struct tweeter *tweets = (struct tweeter *)malloc(sizeof(struct tweeter) * MAX_LINES);
    int tweeter_count = 0;
    char* token;
    while (fgets(line, 375, stream)){
        int field_number = 0;
        token = strtok(line, ",");

        while(token != NULL){               
            if(field_number == name_field){
                for(int i = 0; i <= tweeter_count; i++){ 

                    if(i == tweeter_count){
			tweets[tweeter_count].name = (char*)malloc(sizeof(token));
			strcpy(tweets[tweeter_count].name, token);
                        //tweets[tweeter_count].name = token;
                        printf("Name: %s\n", tweets[i].name);
                        tweets[tweeter_count].count = 1;
                        tweeter_count++;
                        break;
                    }else{
                        if(strcmp(tweets[i].name, token) == 0){
                            printf("Name: %s\n", tweets[i].name);
                            tweets[i].count++;
                            break;
                        }
                    }
                }
            }
            if(field_number != field_count){
                printf("Too many entries on this line!\n");
                free (tweets);
                free(stream);
                return -1;
            }
            
            field_number++;
            token = strtok(NULL, ",");

        }
    }


    //use bubblesort to sort out the struct
    struct tweeter temp;
    for(int i = 0; i < tweeter_count - 1; i++) {
        for(int k = 0; k < tweeter_count - 1 - i; k++) {
            if(tweets[k].count < tweets[k+1].count) {
                temp = tweets[k];
                tweets[k] = tweets[k+1];
                tweets[k+1] = temp;
            }
        }
    }

    //print out the first 10 lines of the struct
    int prints = 0;
    while(prints < 20){ 
        printf("<%s>: <%i>\n", tweets[prints].name, tweets[prints].count);
        prints++;
    }


 
    free (tweets);
    free(stream);
    return 0;

}
