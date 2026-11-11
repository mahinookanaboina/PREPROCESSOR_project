#include "header.h"

void headerInclusion(FILE *Main, char *file, int size)  // Header file inclusion
{
	char *s = malloc(size + 1);
	char path[30] = "/usr/include/";  // Predefined header file path
	char h[30];

	int i = 0, j = 0, len, k = 0;
	char ch, f = 0;

	FILE *temp = fopen(file, "r");

	if(temp == NULL)
	{
		printf("In Header: file is not present\n");
		free(s);
		return;
	}

	len = strlen(path);

	while(fgets(s, size + 1, temp))
	{
		// Stop processing when main function is encountered
		if(strstr(s, "main"))
			break;

		// If the line does not contain include, go to normal processing
		if(!strstr(s, "#include"))
			goto l;

		// Extract header filename from #include statement
		for(i = 0, j = 0, k = len, f = 0; s[i]; i++)
		{
			if(f == 1 && s[i] == '>')
				break;

			if(f == 1)
				path[k++] = s[i];

			if(f == 2 && s[i] == '"')
				break;

			if(f == 2)
				h[j++] = s[i];

			if(s[i] == '<')
				f = 1;

			if(s[i] == '"')
				f = 2;
		}

		// Predefined header file inclusion
		if(f == 1)
		{
			path[k] = '\0';

			FILE *fp = fopen(path, "r");

			if(fp == NULL)
			{
				printf("%s header file is not present\n", path);
				continue;
			}

			// Copy predefined header contents into output file
			while((ch = fgetc(fp)) != EOF)
				fputc(ch, Main);

			fclose(fp);
			continue;
		}

		// User-defined header file inclusion
		if(f == 2)
		{
			h[j] = '\0';

			UserDefinedHeader(Main, h, size);
			continue;
		}

l:
		// Skip macro definitions in this function
		if(strstr(s, "#define"))
			continue;

		if(strstr(s, "main()"))
			break;

		// Copy normal source code into output file
		for(i = 0; s[i]; i++)
			fputc(s[i], Main);
	}

	fclose(temp);
	free(s);
}


void UserDefinedHeader(FILE *Main, char *file, int size)
{
	char *s = malloc(size + 1);
	char path[30] = "/usr/include/";
	char h[30];

	int i = 0, j = 0, len, k = 0;
	char ch, f = 0;

	FILE *temp = fopen(file, "r");

	if(temp == NULL)
	{
		printf("In Header: file is not present\n");
		free(s);
		return;
	}

	len = strlen(path);

	while(fgets(s, size + 1, temp))
	{
		// If no include statement is present, copy the line
		if(!strstr(s, "#include"))
			goto l;

		// Extract included header filename
		for(i = 0, j = 0, k = len, f = 0; s[i]; i++)
		{
			if(f == 1 && s[i] == '>')
				break;

			if(f == 1)
				path[k++] = s[i];

			if(f == 2 && s[i] == '"')
				break;

			if(f == 2)
				h[j++] = s[i];

			if(s[i] == '<')
				f = 1;

			if(s[i] == '"')
				f = 2;
		}

		// Include predefined header file
		if(f == 1)
		{
			path[k] = '\0';

			FILE *fp = fopen(path, "r");

			if(fp == NULL)
			{
				printf("%s header file is not present\n", path);
				continue;
			}

			// Copy predefined header contents
			while((ch = fgetc(fp)) != EOF)
				fputc(ch, Main);

			fclose(fp);
			continue;
		}

l:
		// Copy normal contents of user-defined header
		for(i = 0; s[i]; i++)
			fputc(s[i], Main);
	}

	fclose(temp);
	free(s);
}
