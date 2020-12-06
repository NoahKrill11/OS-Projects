// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//floppya.img
//
//
// Signed:Noah Krill Date: 11/15/2020

//filesys.c
//Based on a program by Michael Black, 2007
//Revised 11.3.2020 O'Neil

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void error(const char);
void error2();

int main(int argc, char* argv[])
{
	if(argc<2 || argc>3)
	{
		error2();
	}
	if(argv[2]!=NULL)
	{
		char fileName[strlen(argv[2])+1];
		strcpy(fileName,argv[2]);
	}

	int i, j, size, noSecs, startPos;
	//open the floppy image
	FILE* floppy;
	floppy=fopen("floppya.img","r+");
	if (floppy==0)
	{
		printf("floppya.img not found\n");
		exit(0);
	}
	//load the disk map from sector 256
	char map[512];
	fseek(floppy,512*256,SEEK_SET);
	for(i=0; i<512; i++)
		map[i]=fgetc(floppy);

	//load the directory from sector 257
	char dir[512];
	fseek(floppy,512*257,SEEK_SET);
	for (i=0; i<512; i++)
		dir[i]=fgetc(floppy);

    //print disk map
	printf("Disk usage map:\n");
	printf("      0 1 2 3 4 5 6 7 8 9 A B C D E F\n");
	printf("     --------------------------------\n");
	for (i=0; i<16; i++) {
		switch(i) {
			case 15: printf("0xF_ "); break;
			case 14: printf("0xE_ "); break;
			case 13: printf("0xD_ "); break;
			case 12: printf("0xC_ "); break;
			case 11: printf("0xB_ "); break;
			case 10: printf("0xA_ "); break;
			default: printf("0x%d_ ", i); break;
		}
		for (j=0; j<16; j++) {
			if (map[16*i+j]==-1) printf(" X"); else printf(" .");
		}
		printf("\n");
	}
    // print directory
	printf("\nDisk directory:\n");
	printf("Name    Type Start Length\n");
    for (i=0; i<512; i=i+16) {
		if (dir[i]==0) break;
		for (j=0; j<8; j++) {
			if (dir[i+j]==0) printf(" "); else printf("%c",dir[i+j]);
		}
		if ((dir[i+8]=='t') || (dir[i+8]=='T')) printf("text"); else printf("exec");
		printf(" %5d %6d bytes\n", dir[i+9], 512*dir[i+10]);
	}
	

    char val = *argv[1];
   
	//Start of the menu
	if(val=='L'||val=='l')
	{
    int totalused = 0;
    printf("File name\t     Size\n");
    for (int i = 0; i < 512; i = i+16) {
        if (dir[i] == 0) break;
        for (int j=0; j<8; j++) {
            if (dir[i+j]==0) {}
            else 
                printf("%c",dir[i+j]);
        }
        printf(".%c\t\t", dir[i+8]);
        printf("%6d bytes\n", 512*dir[i+10]);
        totalused += 512*dir[i+10];
    }
    int totalfree = 261632 - totalused;
    printf("\nSpace used\tFree space\n");
    printf("%i\t\t   %i\n", totalused, totalfree);
	}
	
    else if((val=='P'&&argv[2]!=NULL)||(val=='p'&&argv[2]!=NULL))
	{
	char filename[strlen(argv[2])+1];
	strcpy(filename,argv[2]);

	char buffer[12288];
    int found = 0;
    int startsector, filesize;
    char filetype, data=1;
    size_t namelength = strlen(filename);
    for (int i = 0; i < 512; i = i+16) {
        if (dir[i] == 0) break;
        for (int j=0; j<8; j++) {
            if (dir[i+j]==0) {}
            else {
                if (strcmp(filename, &dir[i+j]) == 0) {
                    filetype = dir[i+8];
                    if (filetype == 'x') {
                        printf("File is not a printable type\n");
                        exit(1);
                    }
                    else {
                        found = 1;
                        printf("\nFile: ");
                        for (int k = 0; k < namelength; k++){
                            printf("%c",dir[i+j+k]);
                        }
                        printf("\n\n");
                        startsector = dir[i+9];
                        fseek(floppy,512*startsector,SEEK_SET);
                        while (data != 0) {
                            data=fgetc(floppy);
                            printf("%c", data);
                        }
                        printf("\n");
                    }
                }
            }
        }
    }
    if (found == 0) {
        printf("File not found\n");
        exit(1);
    }
}
	else if((val=='M'&&argv[2]!=NULL)||(val=='m'&&argv[2]!=NULL))
    {
    int maps=0;
    char filename[9];
    strcpy(filename,argv[2]);
    char input[512];
    printf("Please enter file contents: \n");
    scanf("%s", input);
    int found = 0;
    int emptyindex = 0;
    int emptysector = 0;
    size_t namelength = strlen(filename);
    for (int i = 0; i < 512; ++i) 
    {
        if (map[i] == 0) 
        {
            maps = i;
            break;
        }
    }
    for (int i = 0; i < 512; i = i+16) 
    {
    if (dir[i] == 0) 
    {
        if (emptyindex == 0) 
        {
            emptyindex = i;
            emptysector = dir[i+9];
        }
    }
    for (int i = 0; i < 512; ++i) 
    {
        if (map[i] == 0) 
        {
            maps = i;
            break;
        }
    for (int j=0; j<8; j++)
     {
        if (dir[i+j]==0) 
        {}
        else 
        {
            if (strcmp(filename, &dir[i+j]) == 0) 
            {
                printf("File already exists\n");
                exit(1);
            }
        }
    }

    int remainder = 0;
    if (namelength <= 8)
    remainder = 8 - namelength;
    else 
    filename[8] = '0';
    for (int i = 0; i < (namelength+remainder); i++) {
    if (i >= namelength)
        dir[emptyindex+i] = '0';
    else
        dir[emptyindex + i] = filename[i];
    }
    map[emptysector] = -1;
    map[maps] = -1;
    dir[emptyindex + 10] = 1;
    dir[emptyindex + 9] = map[maps + 1];
        fseek(floppy,512*emptyindex,SEEK_SET);
        for (i=0; i<512; i++) 
            fputc(input[i],floppy);


    }
}
}



	else if((val=='D'&&argv[2]!=NULL)||(val=='d'&&argv[2]!=NULL))
	{
	int found = 0;
    int startsector, numsectors;
    char filetype, data;
    for (int i = 0; i < 512; i = i+16) {
        if (dir[i] == 0) {}
        else {
            for (int j=0; j<8; j++) {
                if (dir[i+j]==0) {}
                else {
                    if (strcmp(argv[2], &dir[i+j]) == 0) {
                        found = 1;
                        startsector = dir[i+9];
                        numsectors = dir[i+10];
                        for (int k = 0; k < 8; k++) {
                            dir[i+j+k] = 0;
                        }
                        break;
                    }
                }
            }
        }
    }

    if (found == 0) {
        printf("File not found\n");
        exit(1);
    }
    else {
        for (int i = 0; i < numsectors; i++) {
            map[startsector+i] = 0;
        }
        fseek(floppy,512*256,SEEK_SET);
        for (int i=0; i<512; i++) fputc(map[i],floppy);

        fseek(floppy,512*257,SEEK_SET);
        for (int i=0; i<512; i++) fputc(dir[i],floppy);
    }
    map[startsector]=0;
    }
		
	else
	{
	error(val);
	}

	//write the map and directory back to the floppy image
	fseek(floppy,512*256,SEEK_SET);
	for (i=0; i<512; i++) 
	fputc(map[i],floppy);

	fseek(floppy,512*257,SEEK_SET);
	for (i=0; i<512; i++) 
	fputc(dir[i],floppy);
	fclose(floppy);
	return 0;
}

void error(const char val)
{
	printf("%s\n","");
	printf("The command %c does not exist or theres not enough arguments",val);
	printf("%s\n","");
	printf("%s\n","Please note the following commands as examples");
	printf("%s\n","./filesys D filename to delete a file");
	printf("%s\n","./filesys L to list the files on the disk");
	printf("%s\n","./filesys M filename to create a text file and store it to disk" );
	printf("%s\n","./filesys P filename read the named file and print it to screen" );
	exit(0);
}
void error2()
{
	printf("%s\n","");
	printf("%s\n","You have either too few or too many arguments");
	printf("%s\n","Please note the following commands as examples");
	printf("%s\n","./filesys D filename to delete a file");
	printf("%s\n","./filesys L to list the files on the disk");
	printf("%s\n","./filesys M filename to create a text file and store it to disk" );
	printf("%s\n","./filesys P filename read the named file and print it to screen" );
	exit(0);
}


