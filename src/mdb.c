// Copyright 2016-2017 Benjamin 'Benno' Falkner. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MDB_PAGE_SIZE 4096

#define MDB_VER_JET3 0
#define MDB_VER_JET4 1
#define MDB_VER_ACCDB2007 0x02
#define MDB_VER_ACCDB2010 0x0103


static char  JET3_XOR[] = { 0x86,0xfb,0xec,0x37,0x5d,0x44,0x9c,0xfa,0xc6,
                            0x5e,0x28,0xe6,0x13,0xb6,0x8a,0x60,0x54,0x94};

static short JET4_XOR[] = { 0x6aba,0x37ec,0xd561,0xfa9c,0xcffa,
                            0xe628,0x272f,0x608a,0x0568,0x367b,
			    0xe3c9,0xb1df,0x654b,0x4313,0x3ef3,
			    0x33b1,0xf008,0x5b79,0x24ae,0x2a7c};

size_t readMDBPage(char *filen, size_t pageSize, char *buffer)
{
    size_t r;
    FILE *f;

    f = fopen(filen,"rb");
    if( f==NULL ) {
        fprintf(stderr,"ERROR: could not open %s\n",filen);
        exit(1);
    }
    r = fread(buffer,1,pageSize,f);
    if( r != pageSize ) {
        fprintf(stderr,"ERROR: could not read page from %s (%li/%li)\n",filen,r,pageSize);
        exit(1);
    }
    fclose(f);
    return r;
}

int scanMDBPage(char *buffer){
    int i;
    int version;
    char pwd[40];
    short *pwd4 = (short*)pwd;
    short magic;

    //page id
    if(buffer[0]!=0){
        fprintf(stderr,"ERROR no vaild db\n");
	return 1;
    }

    //Version
    version = *((int*)&buffer[0x14]);
    switch(version){
        case MDB_VER_JET3:
		printf("DB Version: JET 3\n");
		break;
        case MDB_VER_JET4:
		printf("DB Version: JET 4\n");
		break;
        case MDB_VER_ACCDB2007:
		printf("DB Version: AccessDB 2007\n");
		break;
        case MDB_VER_ACCDB2010:
		printf("DB Version: AccessDB 2010\n");
		break;
        default:
		fprintf(stderr,"ERROR unkown version: %x\n",version);
                return 1;
    }
   

    //Password extract
    if( version==0 ) { // JET 3
        memcpy(pwd,buffer+0x42,20);
	for(i=0;i<18;i++){
	    pwd[i] ^= JET3_XOR[i];
	}
	printf("Password: %20s\n",pwd);

    } else if ( version==1 ) { // JET 4
        memcpy(pwd,buffer+0x42,40);
	magic = *((short*)&buffer[0x66]);
	magic ^= JET4_XOR[18];

	for(i=0;i<18;i++){
	    pwd4[i] ^= JET4_XOR[i];
	    if(pwd4[i]>255){
	        pwd4[i] ^= magic;
	    }
	    pwd[i] = pwd4[i];
	}

	printf("Password: %20s\n",pwd);
    }
    return 0; 
}


int main (int argc, char **argv)
{
    char page[MDB_PAGE_SIZE];
    
    //Check Arguments
    printf("MDB Access Tool\n");
    if (argc < 2){
        printf("Missing: file\n");
	return 1;
    }

    //Read data
    printf("Reading: %s\n", argv[1]);
    readMDBPage(argv[1],MDB_PAGE_SIZE,page);
    scanMDBPage(page);

    return 0;
}
