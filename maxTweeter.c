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
    token = strtok(line, ",");
    while(token!= NULL)
        token = strtok(line, ",");
        //if the field is the name, record what line it is
        if(strcmp(token, "name") == 0){
            //make sure name field appears only once
            if(index == -1){
                index = *field_count; 
            }
            else{
                return -1;
            }
        //count how many fields there are
        *field_count += *field_count + 1;
    }
    //if name field never found
    if(index == -1){
        return -1;
    }
    return 0;

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
    char* line;
    int field_count, name_field;
    fgets(line, 377, stream);
    name_field = get_name_field(line, &field_count);

    if(name_field == -1){
        printf("No name field in csv!\n");
        return -1;
    }

    struct tweeter *tweets = (struct tweeter *)malloc(sizeof(struct tweeter) * MAX_LINES);
    int tweet_count = 0; 
    char* token;
    while (fgets(line, 377, stream)){
        int field_number = 0;
        token = strtok(line, ",");
        while(token != NULL){
            if(field_number == name_field){
                for(int i = 0; i <= tweet_count; i++){                
                    if(i == tweet_count){
                        tweets[tweet_count].name = token;
                        tweets[tweet_count].count = 1;
                    }else{
                        if(strcmp(tweets[i].name, token) == 0){
                            tweets[i].count++;
                        }
                    }
                }
            }

        if(field_number == field_count){
            printf("Too many entries on this line!\n");
            return -1;
        }
        field_number++;
        token = strtok(line, ",");
        }
    }
    
    return 0;

}
