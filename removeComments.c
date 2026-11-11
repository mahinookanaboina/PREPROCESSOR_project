#include "header.h"

void removeComments(char *s)   // Removing comments
{
	int i, j, k, f = 0;

	for(i = 0; s[i]; i++)
	{
		if(s[i] == '/')
		{
			// Removing single-line comments
			if(s[i + 1] == '/')
			{
				while(s[i] != '\n' && s[i])
					s[i++] = 32;

				continue;
			}

			// Removing multi-line comments
			if(s[i + 1] == '*')
			{
				f = 1;

				while(s[i])
				{
					if(s[i] == '*' && s[i + 1] == '/')
					{
						s[i] = 32;
						s[i + 1] = 32;
						i++;
						break;
					}

					// Preserve newline characters
					if(s[i] == '\n')
					{
						i++;
						continue;
					}

					s[i++] = 32;
				}
			}
		}
	}
}
