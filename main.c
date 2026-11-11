#include "header.h"

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("USAGE: ./a.out src_file\n");
		return 0;
	}

	// Creating .i file
	int lenn = strlen(argv[1]);
	char *iFile = calloc(lenn + 1, sizeof(char));
	strcpy(iFile, argv[1]);
	iFile[lenn - 1] = 'i';

	FILE *Main = fopen(iFile, "w");
	if(Main == NULL)
	{
		printf("Output file cannot be created\n");
		free(iFile);
		return 0;
	}

	// Finding the total length of source file
	FILE *fpp = fopen(argv[1], "r");
	if(fpp == NULL)
	{
		printf("File is not present\n");
		free(iFile);
		fclose(Main);
		return 0;
	}

	fseek(fpp, 0, SEEK_END);
	int size = ftell(fpp);
	fclose(fpp);
	free(iFile);

	// Opening source file to read data
	FILE *fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		printf("File is not present\n");
		fclose(Main);
		return 0;
	}

	int ch;

	char *data = calloc(size + 1, sizeof(char));
	int i = 0, longLine = 0, c = 0;

	while((ch = fgetc(fp)) != EOF)
	{
		c++;
		data[i++] = ch;

		// Finding the longest line length
		if(ch == '\n')
		{
			if(c > longLine)
				longLine = c;

			c = 0;
		}
	}

	if(c > longLine)
		longLine = c;

	data[i] = '\0';

	// Removing comments
	removeComments(data);

	// Storing comment-free data in temporary file
	FILE *ft = fopen("t", "w");

	i = 0;
	while(data[i])
		fputc(data[i++], ft);

	fclose(ft);

	// Including header files
	headerInclusion(Main, "t", size);

	FILE *ftt = fopen("t", "r");
	if(ftt == NULL)
	{
		printf("Temporary file cannot be opened\n");
		free(data);
		fclose(fp);
		fclose(Main);
		return 0;
	}

	char *line = calloc(longLine + 1, sizeof(char));
	char *s = calloc(size + 1, sizeof(char));
	char *name = calloc(longLine + 1, sizeof(char));
	char *body = calloc(longLine + 1, sizeof(char));

	int j = 0, k = 0, f, len = 0, flag = 0, m = 0;
	int Index = 0;

	// Replacing macros
	while(fgets(line, longLine + 1, ftt))
	{
		if(strstr(line, "#define"))
		{
			if(strstr(data, line))
			{
				for(i = 0, k = 0, f = 0, j = 0, flag = 0, m = 0;
					line[i]; i++)
				{
					if(f == 0)
					{
						if(line[i] == 32)
							m = 1;

						if(m == 1)
						{
							if((line[i] >= 'a' && line[i] <= 'z') ||
							   (line[i] >= 'A' && line[i] <= 'Z') ||
							   line[i] == '_' ||
							   (line[i] >= '0' && line[i] <= '9'))
								f = 1;
						}
					}

					if(f == 1)
					{
						// Identifying function-like macros
						if(line[i] == '(')
							flag = 1;

						if(flag == 0)
						{
							if(line[i] == 32)
							{
								f = 2;
								continue;
							}
						}

						if(flag == 1)
						{
							if(line[i] == ')')
								flag = 0;
						}

						name[j++] = line[i];
					}

					if(f == 2)
					{
						if(line[i] == '\n')
							continue;

						body[k++] = line[i];
					}
				}

				name[j] = '\0';
				body[k] = '\0';

				// Calling macro replacement function
				replaceMacro(&data, &s, name, body, &size);
			}
		}
	}

	len = strlen(data);
	data = realloc(data, len + 1);

	char st[20] = "main(";

	// Finding main function
	for(i = 0, Index = 0, f = 0; data[i]; i++)
	{
		if(data[i] == '\n')
			Index = i + 1;

		for(j = 0, k = i; st[j]; j++, k++)
		{
			if(st[j] != data[k])
				break;
		}

		if(st[j] == '\0')
		{
			f = 1;
			break;
		}
	}

	if(f == 1)
		i = Index;
	else
		i = 0;

	// Writing final preprocessed code into .i file
	while(data[i])
		fputc(data[i++], Main);

	// Freeing allocated memory
	free(data);
	free(name);
	free(body);
	free(line);
	free(s);

	remove("t");

	fclose(fp);
	fclose(Main);
	fclose(ftt);

	return 0;
}
