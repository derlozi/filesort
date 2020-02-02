#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>


int getFileType(char* name);

int main (void)
{
    DIR* d;
    struct dirent* file;
    char* newPath = (char*)calloc(512, sizeof(char));

    if(chdir("/home/derlozi") == -1)
    {
        perror("Shit happened");
    }
    d = opendir("./Downloads");

    if(d == NULL)
    {
        perror("Shit happened");
        return -1;
    }

    while ((file = readdir(d)) != NULL)
    {
       if(file->d_type == 8)//8 corresponds to filetype DT_REG, which are normal files (see dirent.h documentation)
       {
            int type = getFileType(file->d_name);
            char oldpath[512]  = "./Downloads/";
            strcat(oldpath, file->d_name);
            switch (type)
            {
            case 0: ;//Document file
                strcpy(newPath, "./Downloads/Documents/");
                strcat(newPath, file->d_name);
                
                break;
            
            case 1: ;//program or archive
                strcpy(newPath, "./Downloads/Compressed/");
                strcat(newPath, file->d_name);
                rename(oldpath, newPath);
                break;


            case 2: ;//Media
                strcpy(newPath, "./Downloads/Media/");
                strcat(newPath, file->d_name);
                rename(oldpath, newPath);
                break;
            
            default:
                break;
            }
            

            
       }
        
        
    } 
    closedir(d);   
}

int getFileType(char* name)
{

    /*
    converts the name of a file to an integer that designates the file type

    will return -1 if no file type could be recognized

    will not work for all files, but that's impossible anyways, and it's good enough to keep some kind of order
    */

    int fileType = -1;

    char* fileEnd;

    char* docFileTypes[] = {".pdf", ".docx", ".doc", ".xls", ".xlsx", NULL}; //list of document file types
    char* progFileTypes[] = {".tar", ".gz", ".deb", ".bz2", NULL}; //list of program file types, mostly compressed installers
    char* mediaFileTypes[] = {".mp3", ".mp4", ".wav", ".jpg", ".jpeg", ".png", NULL};//list of different media file types
    char** fileTypes[] = {docFileTypes, progFileTypes, mediaFileTypes, NULL}; //contains the different types of files, the indices will be the value of the returned file type integer

    /*
    All arrays are Null terminated to let the for loop stop when the end of the array is reached, this makes them adjustable without changing any length variables
    */

    for(int i = strlen(name); i > 0; i--)//gets the part of the filename that should contain the type, it reads the string backwards and "returns" the part after the last '.'
    {
        if(name[i] == '.')
        {
            fileEnd = name+i;
            break;
        }

    }

        for(int i = 0; fileTypes[i] != NULL; i++)//Compares the file ending to the list of file types
        {
            for(int j = 0;fileTypes[i][j] != NULL; j++)
            {
                
                if(strcmp(fileEnd, fileTypes[i][j]) == 0)
                {
                    fileType = i;
                }
            }
        }
    

    return fileType;
}