#include "libs.h"

typedef struct {
    char FileName[LengthMax];
    int size;
} HeaderInfo;
typedef struct {
    char FileName[LengthMax];
    char Dir[LengthMax];
    int size;
} FileInfo;

HeaderInfo headerInfo;
char path[LengthMax];
int ArchiveDescriptor;

void Pack (char* directory);
void ArchiveFile (char *FIleName);
void Unpack (char *archive, char *dir);
void AddFileToArchive (char *archive, char* filename);

struct option g_oLongOptions[]={
        {"new", required_argument,0, 'p'},
        {"add", required_argument,0, 'a'},
        {"addfile", required_argument,0, 'f'},
        {"unpack", required_argument,0, 'u'},
        {0,0,0,0}

};

int main(int argc, char *argv[]) {

    int param;
    int optionIndex=0;

    param=getopt_long(argc, argv, "p:a:f:u:", g_oLongOptions, &optionIndex);
    if (param != -1)
    {
        switch (param) {
            case 'p':
                //Pack();
                break;
            case 'a':

                break;
            case 'f':
                //AddFile();
                break;
            case 'u':
                //Unpack();
                break;

            default:
                abort();
        }
    }
    return 0;
}

void Pack(char* directory)
{
    DIR * dp;
    struct dirent *entry;
    struct stat statbuf;
    int Dir;

    if ((dp=opendir(directory))==NULL)
    {
        fprintf(stderr, "can't open dir: %s\n", directory);
        perror("ERROR:");

        exit(false);
    }
    chdir(directory);
    while ((entry=readdir(dp))!=NULL)
    {
        lstat(entry->d_name, &statbuf);
        if (S_ISDIR(statbuf.st_mode))
        {
            if (strcmp(".", entry->d_name)==0 || strcmp("..", entry->d_name)==0)
                continue;
            Dir = 1;
            strcpy(path+strlen(path), "/");
            strcpy(path+strlen(path), entry->d_name);
            Pack(entry->d_name);
        } else{
            ArchiveFile(entry->d_name);
            Dir=1;
        }
    }
    if (!Dir)
    {
        FileInfo fileinfo;
        headerInfo.size+=sizeof(fileinfo);
        *fileinfo.FileName='\0';
        memcpy(fileinfo.Dir, path, strlen(path)+1);
        fileinfo.size=0;
        write(ArchiveDescriptor, &fileinfo, sizeof(fileinfo));

    }
    char* endline;
    if ((endline=strrchr(path, '/'))!= NULL)
        *endline='\0';
    chdir("..");
    closedir(dp);
}

void ArchiveFile(char* FileName){
    FileInfo  fileInfo;
    char buf[bufsize];
    struct stat statbuf;
    int fd;
    if ((fd=open(FileName, O_RDONLY))==0)
    {
        fprintf(stderr, "can;t open file %s\n", FileName);
        exit(1);
    }
    headerInfo.size+=sizeof(fileInfo);
    memcpy(fileInfo.FileName, FileName, strlen(FileName)+1);
    memcpy(fileInfo.Dir, path, strlen(path)+1);
    fstat(fd, &statbuf);
    fileInfo.size=statbuf.st_size;
    write(ArchiveDescriptor, &fileInfo, sizeof(fileInfo));
    int temp;
    while ((temp=read(fd, buf, sizeof(buf)))>0)
    {
        headerInfo.size+=temp;
        write(ArchiveDescriptor, buf, temp);
    }
    close(fd);
}

void Unpack(char* archive, char *dir)
{
    char *endline;
    struct stat statbuf;
    char unpackDir[LengthMax];
    if((endline=strrchr(archive, '/'))!=NULL)
        endline++;
    else
        endline=archive;

    if (dir!=NULL)
    {
        strcpy(unpackDir, dir);
        strcpy(unpackDir+strlen(unpackDir), "/");
    }
}