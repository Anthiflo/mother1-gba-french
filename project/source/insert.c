// you are a hot dog

// i hope you like messy code

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct tableEntry
{
	int  hexVal;
	char str[500];
};

struct romArea
{
	int  address;
	int  size;
};

int        tableLen = 0;
struct 	   tableEntry table[500];

void 		  LoadTable(void);
void          PrepString(char[], char[], int);
unsigned char ConvChar(unsigned char);
void          ConvComplexString(char[], int&);
void          CompileCC(char[], int&, unsigned char[], int&);
int           CharToHex(char);
unsigned int  hstrtoi(char*);
void          StartWritingInRom(int address);
void          WriteInRom(int character);
void          WriteReport();
void          InsertMainStuff(void);
void          InsertSpecialText(void);
void          InsertAltWindowData(void);
void          InsertItemArticles(void);
void          InsertEnemyArticles(void);
void          InsertItemClasses(void);
void          InsertEnemyClasses(void);
void          InsertEnemyLongNames(void);

void          LoadM2Table(void);
void          InsertM2WindowText(void);
void          InsertM2MiscText(void);
void          InsertM2Items(void);
void          InsertM2Enemies(void);
void          InsertM2PSI1(void);
void          InsertM2Locations(void);
void          ConvComplexMenuString(char[], int&);

//void          UpdatePointers(int, int, FILE*, char*);
//void          InsertEnemies(FILE*);
//void          InsertMenuStuff1(FILE*);
//void          InsertStuff(FILE*, char[], int, int, int);
//void          InsertMenuStuff2(FILE*);

int quoteCount = 0;

struct romArea writtenAreas[10000];
int writtenAreasCount = 0;
int currentWrittenArea = 0;

FILE* romStream;

//=================================================================================================

int main(int argc, char *argv[])
{
	romStream = fopen("test.gba", "rb+");
	if (romStream == NULL)
	{
		printf("Can't open test.gba\n");
		fclose(romStream);
		return -1;
	}
	
	printf("\r\n MOTHER 1 STUFF\r\n");
	printf("=====================================\r\n");
	LoadTable();
	InsertMainStuff();
	InsertSpecialText();
	InsertAltWindowData();
	InsertItemArticles();
    InsertEnemyArticles();
    InsertItemClasses();
    InsertEnemyClasses();
    InsertEnemyLongNames();


	printf("\r\n MOTHER 2 STUFF\r\n");
	printf("=====================================\r\n");
	LoadM2Table();
	InsertM2WindowText();
	InsertM2Items();
	InsertM2Enemies();
	InsertM2PSI1();
	InsertM2Locations();
	InsertM2MiscText();

	fclose(romStream);

    printf("\r\nDone!\r\n");
	printf("\r\n%d areas written\r\n",writtenAreasCount);
	
	if (argc > 1) {
		WriteReport();
	}

	return 0;
}

//=================================================================================================

void ConvComplexString(char str[5000], int& newLen)
{
	char          newStr[5000] = "";
	unsigned char newStream[100];
	int           streamLen =  0;
	int           len = strlen(str) - 1; // minus one to take out the newline
	int           counter = 0;
	int           i;

	newLen = 0;

    quoteCount = 0;

    while (counter < len)
    {
		//printf("%c", str[counter]);
		if (str[counter] == '[')
		{
		   CompileCC(str, counter, newStream, streamLen);
		   for (i = 0; i < streamLen; i++)
		   {
			   newStr[newLen] = newStream[i];
			   newLen++;
		   }
		   counter++; // to skip past the ]
		}
		else
		{
		   newStr[newLen] = ConvChar(str[counter]);
		   newLen++;

		   counter++;
		}
	}

    for (i = 0; i < 5000; i++)
       str[i] = '\0';
	for (i = 0; i < newLen; i++)
	   str[i] = newStr[i];
}

void StartWritingInRom(int address) {
	//printf("Write in ROM at 0x8%06X\n", address);
	int i;
	int foundArea = 0;
	for (i = 0; i < writtenAreasCount; i++) {
		if (writtenAreas[i].address + writtenAreas[i].size == address) {
			currentWrittenArea = i;
			foundArea = 1;
			break;
		}
	}
	if (!foundArea) {
		currentWrittenArea = writtenAreasCount;
		writtenAreasCount++;
	}
	writtenAreas[currentWrittenArea].address = address;
	writtenAreas[currentWrittenArea].size = 0;
	fseek(romStream, address, SEEK_SET);
}

void WriteInRom(int character) {
	writtenAreas[currentWrittenArea].size++;
	fputc(character, romStream);
}

void WriteReport() {
	FILE* reportStream;
	reportStream = fopen("insert_report.txt", "w+");
	int i;
	for (i = 0; i < writtenAreasCount; i++) {
		fprintf(reportStream, "0x8%06X:0x%X\n", writtenAreas[i].address, writtenAreas[i].size); 
	}
	fclose(reportStream);
}

//=================================================================================================

void CompileCC(char str[5000], int& strLoc, unsigned char newStream[100], int& streamLen)
{
   char  str2[5000] = "";
   char* ptr[5000];
   int   ptrCount = 0;
   int   totalLength = strlen(str);
   int   i;
   int   j;
   FILE* fin;
   char  hexVal[100] = "";
   char  specialStr[100] = "";
   int   retVal = 0;


   // we're gonna mess with the original string, so make a backup for later
   strcpy(str2, str);

   // first we gotta parse the codes, what a pain
   ptr[ptrCount++] = &str[strLoc + 1];
   while (str[strLoc] != ']' && strLoc < totalLength)
   {
      if (str[strLoc] == ' ')
      {
         ptr[ptrCount++] = &str[strLoc + 1];
         str[strLoc] = 0;
      }

      strLoc++;
   }

   if (str[strLoc] == ']')
      str[strLoc] = 0;


   // Capitalize all the arguments for ease of use
   for (i = 0; i < ptrCount; i++)
   {
      for (j = 0; j < strlen(ptr[i]); j++)
         ptr[i][j] = toupper(ptr[i][j]);
   }

   // now the actual compiling into the data stream
   streamLen = 0;
   if (strcmp(ptr[0], "END") == 0)
       newStream[streamLen++] = 0x00;
   else if (strcmp(ptr[0], "BREAK") == 0)
       newStream[streamLen++] = 0x02;
   else if (strcmp(ptr[0], "PAUSE") == 0)
   {
	   newStream[streamLen++] = 0x03;
       newStream[streamLen++] = 0x02;
   }



   else if ((isalpha(ptr[0][0]) == true) && (strlen(ptr[0]) != 2))
   {
	    i = 0;

		while ((i < tableLen) && (retVal == 0))
		{
			if (strcmp(ptr[0], table[i].str) == 0)
			   retVal = table[i].hexVal;
			else
			   i++;
		}

		newStream[streamLen++] = retVal;
		//newStream[streamLen++] = 0x00;
		if (retVal == 0)
		   printf("Couldn't convert control code: %s\n", ptr[0]);


      //printf("%s\r\n", ptr[0]);

   }



   // going to assume raw codes now, in 2-char hex things, like [FA 1A 2C EE]
   else if (strlen(ptr[0]) == 2)
   {
      for (i = 0; i < ptrCount; i++)
         newStream[streamLen++] = hstrtoi(ptr[i]);
   }

   else
      printf("UNKNOWN CONTROL CODE: %s\n", ptr[0]);

   // restore backup string
   strcpy(str, str2);
}

//=================================================================================================

unsigned char ConvChar(unsigned char ch)
{
	unsigned char retVal = 0;
	char          origChar[100] = "";
	int           i = 0;

	while ((i < tableLen) && (retVal == 0))
	{
		sprintf(origChar, "%c", ch);


		if (strcmp(origChar, table[i].str) == 0)
		{
		   retVal = table[i].hexVal;
		   if (ch == '\"')
		   {
			   // implementing smart quotes
			   if (quoteCount % 2 == 0)
			      retVal = 0xAC;

			   quoteCount++;
		   }
	    }
		else
		   i++;
	}

	if (retVal == 0)
		printf("UNABLE TO CONVERT CHARACTER: %c %02X\n", ch, ch);

	return retVal;
}

//=================================================================================================

void LoadTable(void)
{
   FILE* fin;
   char  tempStr[500] = "";
   int i;

   tableLen = 0;

   fin = fopen("eng_table.txt", "r");
   if (fin == NULL)
   {
	   printf("Can't open eng_table.txt!\n");
	   return;
   }

   /*i = fgetc(fin);
   i = fgetc(fin);
   i = fgetc(fin);*/


   fscanf(fin, "%s", tempStr);
   table[tableLen].hexVal = hstrtoi(tempStr);
   fscanf(fin, "%s", table[tableLen].str);
   while (!feof(fin))
   {
	   tableLen++;

   	   fscanf(fin, "%s", tempStr);
   	   table[tableLen].hexVal = hstrtoi(tempStr);
       fscanf(fin, "%s", table[tableLen].str);
       //printf("%s\n", table[tableLen].str);
   }

   table[0x01].str[0] = ' ';
   table[0x01].str[1] = '\0';

   fclose(fin);
}

//=================================================================================================

unsigned int hstrtoi(char* string)
{
   unsigned int retval = 0;

   for (int i = 0; i < strlen(string); i++)
   {
      retval <<= 4;
      retval += CharToHex(string[i]);
   }

   return retval;
}

//=================================================================================================

int CharToHex(char ch)
{
   // Converts a single hex character to an integer.

   int retVal = 0;

   ch = toupper(ch);

   switch (ch)
   {
      case '0':
      {
         retVal = 0;
         break;
      }
      case '1':
      {
         retVal = 1;
         break;
      }
      case '2':
      {
         retVal = 2;
         break;
      }
      case '3':
      {
         retVal = 3;
         break;
      }
      case '4':
      {
         retVal = 4;
         break;
      }
      case '5':
      {
         retVal = 5;
         break;
      }
      case '6':
      {
         retVal = 6;
         break;
      }
      case '7':
      {
         retVal = 7;
         break;
      }
      case '8':
      {
         retVal = 8;
         break;
      }
      case '9':
      {
         retVal = 9;
         break;
      }
      case 'A':
      {
         retVal = 10;
         break;
      }
      case 'B':
      {
         retVal = 11;
         break;
      }
      case 'C':
      {
         retVal = 12;
         break;
      }
      case 'D':
      {
         retVal = 13;
         break;
      }
      case 'E':
      {
         retVal = 14;
         break;
      }
      case 'F':
      {
         retVal = 15;
         break;
      }
   }

   return retVal;
}

//=================================================================================================

void PrepString(char str[5000], char str2[5000], int startPoint)
{
	int j;
	int ctr;

    for (j = 0; j < 5000; j++)
	    str2[j] = '\0';

    ctr = 0;
	for (j = startPoint; j < strlen(str); j++)
	{
	   str2[ctr] = str[j];
	   ctr++;
	}

}

//=================================================================================================

void InsertMainStuff(void)
{
	FILE* fin;
	char  str[5000];
	char  str2[5000];
	int   lineNum = 0;
    int   loc = 0xF7EA00;
	int   ptrLoc;
	int   temp;
	int   len;
	int   i;

	fin = fopen("m1_main_text.txt", "r");
	if (fin == NULL)
	{
		printf("Can't open m1_main_text.txt\n");
		return;
	}

	fgets(str, 5000, fin);
	while(strstr(str, "-E: ") == NULL)
	{
	   fgets(str, 5000, fin);
	}
    while(!feof(fin))
    {
  		PrepString(str, str2, 7);

  		if (str2[0] != '\n')
  		{
			//printf("%d", str2[0]);
//                       printf("%X %s\n", loc, str);
			ptrLoc = 0xF27A90 + lineNum * 4;
			StartWritingInRom(ptrLoc);

			temp = loc + 0x8000000;
	        WriteInRom(temp & 0x000000FF);
            WriteInRom((temp & 0x0000FF00) >> 8);
	        WriteInRom((temp & 0x00FF0000) >> 16);
            WriteInRom(temp >> 24);

			ConvComplexString(str2, len);
			str2[len] = 0x00;
			len++;

            StartWritingInRom(loc);
            for (i = 0; i < len; i++)
            {
			   //printf("%02X ", str2[i]);
               WriteInRom(str2[i]);
			}
			//printf("\n");

		    loc += len;
		}


        lineNum++;
	    fgets(str, 5000, fin);
	    while(strstr(str, "-E: ") == NULL && !feof(fin))
	    {
			fgets(str, 5000, fin);
    	}

	}

    printf(" Main text:\tINSERTED\r\n");

	fclose(fin);
}

//=================================================================================================

void InsertSpecialText(void)
{
	FILE* fin;
	char  str[5000];
	char  str2[5000];
	char  line[5000];
	int   loc;
	int   temp;
	int   len;
	int   i;


    fin = fopen("m1_misc_text.txt", "r");
    if (fin == NULL)
    {
		printf("Can't open m1_misc_text.txt");
		return;
	}

    //fscanf(fin, "%x", &loc);
//    fscanf(fin, "%s", str);
    //fscanf(fin, "%s", line);
    fgets(line, 5000, fin);
    while(!feof(fin))
    {
		if (line[0] != '/' && line[0] != '\r' && line[0] != 10)
		{
		   sscanf(line, "%x %[^\t\n]", &loc, str);
           strcat(str, " ");

           //printf("%2d %X - %s\n", line[0], loc, str);
     	   PrepString(str, str2, 0);
		   ConvComplexString(str2, len);

           StartWritingInRom(loc);
           for (i = 0; i < len; i++)
	          WriteInRom(str2[i]);
	    }

	    //fscanf(fin, "%s", line);
	    fgets(line, 5000, fin);

    	//fscanf(fin, "%x", &loc);
//    	fscanf(fin, "%s", str);
	}

    printf(" Misc. text:\t\tINSERTED\r\n");

	fclose(fin);
}

void InsertAltWindowData(void)
{
	FILE* fin;
	char  str[1000];
	int   lineNum;
	int   insertLoc = 0xFED000;
	int   totalSize = 0x1000;
	int   totalFound = 0;

	fin = fopen("m1_small_windows_list.txt", "r");
	if (!fin)
	{
		printf("Can't open m1_small_windows_list.txt, doh\r\n");
		return;
	}

	StartWritingInRom(insertLoc);
	for (int i = 0; i < totalSize; i++)
	   WriteInRom(0);

	fscanf(fin, "%x", &lineNum);
    while(!feof(fin))
    {
		if (lineNum < totalSize)
		{
			StartWritingInRom(insertLoc + lineNum);
			WriteInRom(1);
			totalFound++;
		}

	   fscanf(fin, "%x", &lineNum);
	}

	fclose(fin);

    printf(" Alt. windows:\t\tINSERTED (Total: %d)\r\n", totalFound);
    return;
}

void InsertItemArticles(void)
{
	FILE* fin;
	char  line[1000];
	char* str;
	int   lineNum = 0;
	int   startLoc = 0xFFE000;
	int   i;

	fin = fopen("m1_item_articles.txt", "r");
	if (!fin)
	{
		printf("Can't open m1_item_articles.txt, doh\r\n");
		return;
	}

	fgets(line, 1000, fin);
    while(!feof(fin))
    {
		line[strcspn(line, "\n")] = '\0';
        if (line[0] != '/') {
            str = &line[13];
            
            StartWritingInRom(startLoc + lineNum);
            WriteInRom(hstrtoi(str));

            lineNum++;
        }
        fgets(line, 1000, fin);
	}

    printf(" Item articles:\t\tINSERTED\r\n");

	return;
}

void InsertEnemyArticles(void)
{
	FILE* fin;
	char  line[1000];
	char* str;
	int   lineNum = 0;
	int   startLoc = 0xFFE080;
	int   i;

	fin = fopen("m1_enemy_articles.txt", "r");
	if (!fin)
	{
		printf("Can't open m1_enemy_articles.txt, doh\r\n");
		return;
	}

	fgets(line, 1000, fin);
    while(!feof(fin))
    {
		line[strcspn(line, "\n")] = '\0';
        if (line[0] != '/') {
            str = &line[14];
            
            StartWritingInRom(startLoc + lineNum);
            WriteInRom(hstrtoi(str));

            lineNum++;
        }
	    fgets(line, 1000, fin);
	}

    printf(" Enemy articles:\tINSERTED\r\n");

	return;
}

void InsertItemClasses(void)
{
	FILE* fin;
	char  line[1000];
	char* str;
	int   lineNum = 0;
	int   startLoc = 0xFFE100;
	int   i;

	fin = fopen("m1_item_classes.txt", "r");
	if (!fin)
	{
		printf("Can't open m1_item_classes.txt, doh\r\n");
		return;
	}

	fgets(line, 1000, fin);
    while(!feof(fin))
    {
        line[strcspn(line, "\n")] = '\0';

        if (line[0] != '/') {
            StartWritingInRom(startLoc + lineNum * 0x8);
            for (i = 0; i < 0x8; i++)
               WriteInRom(0);

			if (strlen(line) > 0) {
				StartWritingInRom(startLoc + lineNum * 0x8);
				for (i = 0; i < strlen(line); i++)
				   WriteInRom(ConvChar(line[i]));
		    }

            lineNum++;
        }
	    fgets(line, 1000, fin);
	}

    printf(" Item classes:\t\tINSERTED\r\n");

	return;
}

void InsertEnemyClasses(void)
{
	FILE* fin;
	char  line[1000];
	char* str;
	int   lineNum = 0;
	int   startLoc = 0xFFE400;
	int   i;

	fin = fopen("m1_enemy_classes.txt", "r");
	if (!fin)
	{
		printf("Can't open m1_enemy_classes.txt, doh\r\n");
		return;
	}

	fgets(line, 1000, fin);
    while(!feof(fin))
    {
        line[strcspn(line, "\n")] = '\0';

        if (line[0] != '/') {
            StartWritingInRom(startLoc + lineNum * 0x8);
            for (i = 0; i < 0x8; i++)
               WriteInRom(0);

			if (strlen(line) > 0) {
				StartWritingInRom(startLoc + lineNum * 0x8);
				for (i = 0; i < strlen(line); i++)
				   WriteInRom(ConvChar(line[i]));
		    }

            lineNum++;
        }
	    fgets(line, 1000, fin);
	}

    printf(" Enemy classes:\t\tINSERTED\r\n");

	return;
}

void InsertEnemyLongNames(void)
{
	FILE* fin;
	char  line[1000];
	char* str;
	char  str2[5000];
	int   len;
	int   lineNum = 0;
	int   startLoc = 0xFDF300;
	int   i;

	fin = fopen("m1_enemy_long_names.txt", "r");
	if (!fin)
	{
		printf("Can't open m1_enemy_long_names.txt, doh\r\n");
		return;
	}

	fgets(line, 1000, fin);
    while(!feof(fin))
    {
        if (line[0] != '/') {
            line[strcspn(line, "\n")] = '\0';
            str = &line[14];
            StartWritingInRom(startLoc + lineNum * 0x19);
            for (i = 0; i < 0x19; i++)
               WriteInRom(0);
			if (strlen(str) > 0) {
				PrepString(str,str2,0);
				ConvComplexString(str2,len);
				StartWritingInRom(startLoc + lineNum * 0x19);
				for (i = 0; i < len; i++)
				   WriteInRom(str2[i]);
			}

            lineNum++;
        }
	    fgets(line, 1000, fin);
	}

    printf(" Enemy long names:\tINSERTED\r\n");

	return;
}


//=================================================================================================
//=================================================================================================
//=================================================================================================

void LoadM2Table(void)
{
   FILE* fin;
   char  tempStr[500] = "";
   int i;

   tableLen = 0;

   fin = fopen("m2_jpn_table.txt", "r");
   if (fin == NULL)
   {
	   printf("Can't open m2_jpn_table.txt!\n");
	   return;
   }

   i = fgetc(fin);
   i = fgetc(fin);
   i = fgetc(fin);


   fscanf(fin, "%s", tempStr);
   table[tableLen].hexVal = hstrtoi(tempStr);
   fscanf(fin, "%s", table[tableLen].str);
   while (!feof(fin))
   {
	   tableLen++;

   	   fscanf(fin, "%s", tempStr);
   	   table[tableLen].hexVal = hstrtoi(tempStr);
       fscanf(fin, "%s", table[tableLen].str);
   }

   table[0x00].str[1] = '\0';
   table[0x4D].str[0] = ' ';

   fclose(fin);
}

//=================================================================================================

void InsertM2WindowText(void)
{
	FILE* fin;
	char  str[5000];
	char  str2[5000];
	char  line[5000];
	int   loc;
	int   temp;
	int   len;
	int   i;


    fin = fopen("m2_window_text.txt", "r");
    if (fin == NULL)
    {
		printf("Can't open m1_window_text.txt");
		return;
	}

    //fscanf(fin, "%x", &loc);
//    fscanf(fin, "%s", str);
    //fscanf(fin, "%s", line);
    fgets(line, 5000, fin);
    while(!feof(fin))
    {
		if (line[0] != '/' && line[0] != '\r' && line[0] != 10)
		{
		   sscanf(line, "%x %[^\t\n]", &loc, str);
           strcat(str, " ");

           //printf("%2d %X - %s\n", line[0], loc, str);
     	   PrepString(str, str2, 0);
		   ConvComplexMenuString(str2, len);

           StartWritingInRom(loc);
           for (i = 0; i < len; i++)
	          WriteInRom(str2[i]);
	    }

	    //fscanf(fin, "%s", line);
	    fgets(line, 5000, fin);

    	//fscanf(fin, "%x", &loc);
//    	fscanf(fin, "%s", str);
	}

    printf(" Misc. text:\tINSERTED\r\n");

	fclose(fin);
}

//=================================================================================================

void InsertM2Items(void)
{
	FILE* fin;
	char  str[5000];
	char  str2[5000];
	int   lineNum = 0;
    int   loc = 0xB30000;
	int   ptrLoc;
	int   temp;
	int   len;
	int   i;

	fin = fopen("m2_items.txt", "r");
	if (fin == NULL)
	{
		printf("Can't open m2_items.txt\n");
		return;
	}

	fgets(str, 5000, fin);
	while(strstr(str, "-E: ") == NULL)
	{
	   fgets(str, 5000, fin);
	}
    while(!feof(fin))
    {
  		PrepString(str, str2, 7);

  		if (str2[0] != '\n')
  		{
			//printf("%d", str2[0]);
			//printf(str2);
//                       printf("%X %s\n", loc, str);
			ptrLoc = 0xb1af94 + lineNum * 4;
			StartWritingInRom(ptrLoc);

			temp = (loc - 0xb1a694);
	        WriteInRom(temp & 0x000000FF);
            WriteInRom((temp & 0x0000FF00) >> 8);
	        WriteInRom((temp & 0x00FF0000) >> 16);
            WriteInRom(temp >> 24);

			ConvComplexString(str2, len);
            str2[len++] = 0x00;
			str2[len++] = 0xFF;

            StartWritingInRom(loc);
            for (i = 0; i < len; i++)
            {
			   //printf("%02X ", str2[i]);
               WriteInRom(str2[i]);
			}
			//printf("\n");

		    loc += len;
		}


        lineNum++;
	    fgets(str, 5000, fin);
	    while(strstr(str, "-E: ") == NULL && !feof(fin))
	    {
			fgets(str, 5000, fin);
    	}

	}

    printf(" Item names:\tINSERTED\r\n");

	fclose(fin);
}

//=================================================================================================

void InsertM2Enemies(void)
{
	FILE* fin;
	char  str[5000];
	char  str2[5000];
	int   lineNum = 0;
    int   loc = 0xB31000;
	int   ptrLoc;
	int   temp;
	int   len;
	int   i;

	fin = fopen("m2_enemies.txt", "r");
	if (fin == NULL)
	{
		printf("Can't open m2_enemies.txt\n");
		return;
	}

	fgets(str, 5000, fin);
	while(strstr(str, "-E: ") == NULL)
	{
	   fgets(str, 5000, fin);
	}
    while(!feof(fin))
    {
  		PrepString(str, str2, 7);

  		if (str2[0] != '\n')
  		{
			//printf("%d", str2[0]);
//			printf(str2);
//                       printf("%X %s\n", loc, str);
			ptrLoc = 0xb1a2f0 + lineNum * 4;
			StartWritingInRom(ptrLoc);

			temp = (loc - 0xb19ad0);
	        WriteInRom(temp & 0x000000FF);
            WriteInRom((temp & 0x0000FF00) >> 8);
	        WriteInRom((temp & 0x00FF0000) >> 16);
            WriteInRom(temp >> 24);

			ConvComplexString(str2, len);
            str2[len++] = 0x00;
			str2[len++] = 0xFF;

            StartWritingInRom(loc);
            for (i = 0; i < len; i++)
            {
			   //printf("%02X ", str2[i]);
               WriteInRom(str2[i]);
			}
			//printf("\n");

		    loc += len;
		}


        lineNum++;
	    fgets(str, 5000, fin);
	    while(strstr(str, "-E: ") == NULL && !feof(fin))
	    {
			fgets(str, 5000, fin);
    	}

	}

    printf(" Enemy names:\tINSERTED\r\n");

	fclose(fin);
}


//=================================================================================================

void InsertM2MiscText(void)
{
	FILE* fin;
	char  str[5000];
	char  str2[5000];
	char  line[5000];
	int   loc;
	int   temp;
	int   len;
	int   i;


    fin = fopen("m2_misc_text.txt", "r");
    if (fin == NULL)
    {
		printf("Can't open m2_misc_text.txt");
		return;
	}

    fgets(line, 5000, fin);
    while(!feof(fin))
    {
		if (line[0] != '/' && line[0] != '\r' && line[0] != 10)
		{
		   sscanf(line, "%x %[^\t\n]", &loc, str);
           strcat(str, " ");

           //printf("%2d %X - %s\n", line[0], loc, str);
     	   PrepString(str, str2, 0);
		   ConvComplexString(str2, len);

           StartWritingInRom(loc);
           for (i = 0; i < len; i++)
	          WriteInRom(str2[i]);
	    }

	    //fscanf(fin, "%s", line);
	    fgets(line, 5000, fin);

    	//fscanf(fin, "%x", &loc);
//    	fscanf(fin, "%s", str);
	}

    printf(" Misc. text:\tINSERTED\r\n");

	fclose(fin);
}



void ConvComplexMenuString(char str[5000], int& newLen)
{
	char          newStr[5000] = "";
	unsigned char newStream[100];
	int           streamLen =  0;
	int           len = strlen(str) - 1; // minus one to take out the newline
	int           counter = 0;
	int           i;

	newLen = 0;

    quoteCount = 0;

    while (counter < len)
    {
		//printf("%c", str[counter]);
		if (str[counter] == '[')
		{
		   CompileCC(str, counter, newStream, streamLen);
		   for (i = 0; i < streamLen; i++)
		   {
			   newStr[newLen] = newStream[i];
			   newLen++;
		   }
		   counter++; // to skip past the ]
		}
		else
		{
		   newStr[newLen++] = 0x82;
		   newStr[newLen++] = str[counter] + 0x1F;

		   counter++;
		}
	}

    for (i = 0; i < 5000; i++)
       str[i] = '\0';
	for (i = 0; i < newLen; i++)
	   str[i] = newStr[i];
}

//=================================================================================================

void InsertM2PSI1(void)
{
	FILE* fin;
	char  str[5000];
	char  str2[5000];
	int   lineNum = 0;
    int   loc = 0xb1b916;
	int   ptrLoc;
	int   temp;
	int   len;
	int   i;

	fin = fopen("m2_psi.txt", "r");
	if (fin == NULL)
	{
		printf("Can't open m2_psi.txt\n");
		return;
	}

	fgets(str, 5000, fin);
	while(strstr(str, "-E: ") == NULL)
	{
	   fgets(str, 5000, fin);
	}
    while(!feof(fin))
    {
  		PrepString(str, str2, 7);

  		if (str2[0] != '\n')
  		{
			ptrLoc = 0xb1b916 + lineNum * 0xD;
			//printf("%02X - %6X - %s", lineNum, ptrLoc, str2);

			ConvComplexString(str2, len);
            str2[len++] = 0x00;
			str2[len++] = 0xFF;

			StartWritingInRom(ptrLoc);
            for (i = 0; i < len; i++)
            {
			   //printf("%02X ", str2[i]);
               WriteInRom(str2[i]);
			}
			//printf("\n");

		    loc += len;
		}


        lineNum++;
	    fgets(str, 5000, fin);
	    while(strstr(str, "-E: ") == NULL && !feof(fin))
	    {
			fgets(str, 5000, fin);
    	}

	}

    printf(" PSI, etc. #1:\tINSERTED\r\n");

	fclose(fin);
}

//=================================================================================================

void InsertM2Locations(void)
{
	FILE* fin;
	char  str[5000];
	char  str2[5000];
	int   lineNum = 0;
    int   loc = 0xb2ad24;
	int   ptrLoc;
	int   temp;
	int   len;
	int   i;

	fin = fopen("m2_locations.txt", "r");
	if (fin == NULL)
	{
		printf("Can't open m2_locations.txt\n");
		return;
	}

	fgets(str, 5000, fin);
	while(strstr(str, "-E: ") == NULL)
	{
	   fgets(str, 5000, fin);
	}
    while(!feof(fin))
    {
  		PrepString(str, str2, 7);

  		if (str2[0] != '\n')
  		{
			ptrLoc = 0xb2ad24 + lineNum * 0x14;
			//printf("%02X - %6X - %s", lineNum, ptrLoc, str2);

			ConvComplexString(str2, len);
            str2[len++] = 0x00;
			str2[len++] = 0xFF;

			StartWritingInRom(ptrLoc);
            for (i = 0; i < len; i++)
            {
			   //printf("%02X ", str2[i]);
               WriteInRom(str2[i]);
			}
			//printf("\n");

		    loc += len;
		}


        lineNum++;
	    fgets(str, 5000, fin);
	    while(strstr(str, "-E: ") == NULL && !feof(fin))
	    {
			fgets(str, 5000, fin);
    	}

	}

    printf(" Loc. names:\tINSERTED\r\n");

	fclose(fin);
}
