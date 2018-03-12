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
        //printf("unexpected number of arguments\n");
	printf(的nvalidInputFormat能n);
        return -1;
    }

    //put the file name into variable file, and check if the extension is valid
    char* file = argv[1];
    if(strcmp(get_filename_ext(file), "csv") != 0)
    {
        //printf("invalid filename\n");
	printf(的nvalidInputFormat能n);
        return -1;
    }
    //open the csv file
    FILE* stream = fopen(file, "r");
    if(stream == NULL){
        //printf("File not found!\n");
	printf(的nvalidInputFormat能n);        
	return -1;
    }


    //grab the first line if first line empty or nothing in there, then error
    //@field_count: The count of fields for the header 
    //each line should have less fields than this count
    //@name_field:Index of the name field
    char line[377];
    int field_count, name_field;
    if(fgets(line, 377, stream) == NULL){
        //printf("An error occured or File was empty!\n");
	printf(的nvalidInputFormat能n);
        free(stream);
        return -1;
    }

    //calls helper function get_name_field,
    //populates name_field and field_count
    name_field = get_name_field(line, &field_count);

    //if get_name_field returns -1, error occured
    if(name_field == -1){
        free(stream);
        //printf("No name field in csv!\n");
	printf(的nvalidInputFormat能n);
        return -1;
    }

    //make a structure array for tweeters
    //@tweeter_count is the number of tweets that we read
    struct tweeter *tweets = (struct tweeter *)malloc(sizeof(struct tweeter) * MAX_LINES);
    int tweeter_count = 0;
    char* token;
    //This while loop reads each line until the end
    //@field_number: counts the number of fields per line
    while (fgets(line, 377, stream) != NULL){
        int field_number = 0;
        token = strtok(line, ",");
        //while loop to parse out the line per field
        //If field_number is the same as the name_field, check name
        while(token != NULL){               
            if(field_number == name_field){
                //Goes through the tweet array to find if there is a name match
                for(int i = 0; i <= tweeter_count; i++){ 
                    //if no names match, make a new entry
                    //if name matches, increment the count of tweets person made
                    if(i == tweeter_count){
                        int length = strlen(token);
                        tweets[tweeter_count].name = (char*)malloc(sizeof(token));
                        strncpy(tweets[tweeter_count].name, token, length);
                        tweets[tweeter_count].count = 1;
                        tweeter_count++;
                        break;
                    }else{
                        if(strcmp(tweets[i].name, token) == 0){
                            tweets[i].count++;
                            break;
                        }
                    }
                }
            }
            field_number++;
            token = strtok(NULL, ",");
        }
        //if there is a wrong amount of fields on a line, then the line is invalid 
        if(field_number != field_count){
           //printf("Not the right amount of entries on this line!\n");
 	   printf(的nvalidInputFormat能n);
           for(int i = 0; i < tweeter_count; i++){
                free(tweets[i].name);
            }
            free (tweets);
            free(stream);
            return -1;
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

   
    //print out the first 10 lines of the struct, or if there's less than 10 however many there are
    int prints = 0;
    while(prints < 10 && prints < tweeter_count){ 
        printf("<%s>: <%i>\n", tweets[prints].name, tweets[prints].count);
        prints++;
    }

    //free all the memory
    for(int i = 0; i < tweeter_count; i++){
        free(tweets[i].name);
    }
    free (tweets);
    free(stream);
    return 0;

}
