
#include "header.h"

char *replaceArguments(char *data, char *name, char *body);

void replaceMacro(char **data, char **s, char *name, char *body, int *size)
{
    int i, j, k, f;
    int op = 0, m;

    // Allocate temporary memory
    *s = realloc(*s, *size + 1);

    // Check whether it is a function-like macro
    if(strchr(name, '('))
        op = 1;

    if(op == 0)
    {
        for(i = 0, m = 0, f = 0; data[0][i]; i++, m++)
        {
            if(data[0][i] == '"')
                f++;

            if(f == 0)
            {
                if(i > 0)
                    if((data[0][i-1] >= 'a' && data[0][i-1] <= 'z') ||
                       (data[0][i-1] >= 'A' && data[0][i-1] <= 'Z'))
                        goto l;

                for(j = 0, k = i; name[j]; j++, k++)
                    if(data[0][k] != name[j])
                        break;

                if(name[j] == 0 &&
                   ((data[0][k] > 0 && data[0][k] < 47) ||
                    (data[0][k] > 57 && data[0][k] < 65) ||
                    (data[0][k] > 90 && data[0][k] < 97) ||
                    data[0][k] > 122))
                {
                    int lenBody = strlen(body);
                    int lenName = strlen(name);

                    *size += lenBody - lenName + 1;
                    *s = realloc(*s, *size + 1);

                    for(j = 0; body[j]; j++)
                        s[0][m++] = body[j];

                    m--;
                    i = k - 1;
                    continue;
                }
            }

l:
            s[0][m] = data[0][i];

            if(f == 2)
                f = 0;
        }
    }
    else if(op == 1)
    {
        for(i = 0, m = 0, f = 0; data[0][i]; i++, m++)
        {
            if(data[0][i] == '"')
                f++;

            if(f == 0)
            {
                if(i > 0)
                    if((data[0][i-1] >= 'a' && data[0][i-1] <= 'z') ||
                       (data[0][i-1] >= 'A' && data[0][i-1] <= 'Z'))
                        goto v;

                for(j = 0, k = i; name[j]; j++, k++)
                {
                    if(data[0][k] == name[j])
                    {
                        if(name[j] == '(')
                            break;
                    }
                    else
                        break;
                }

                if(name[j] == '(')
                {
                    char *tBody = replaceArguments(data[0] + k, name, body);

                    int lenName = strlen(name);
                    int lenBody = strlen(body);

                    *size += lenBody - lenName + 1;
                    *s = realloc(*s, *size + 1);

                    for(j = 0; tBody[j]; j++)
                        s[0][m++] = tBody[j];

                    m--;

                    k++;

                    // Skip original macro arguments
                    for(int c = 1; data[0][k]; k++)
                    {
                        if(data[0][k] == '(')
                            c++;

                        if(data[0][k] == ')')
                            c--;

                        if(c == 0)
                            break;
                    }

                    i = k;
                    free(tBody);
                    continue;
                }
            }

v:
            s[0][m] = data[0][i];

            if(f == 2)
                f = 0;
        }
    }

    s[0][m] = '\0';

    // Copy temporary string back into original data
    *data = realloc(*data, *size + 1);

    i = 0;
    while(s[0][i])
    {
        data[0][i] = s[0][i];
        i++;
    }

    data[0][i] = '\0';

    int length = strlen(*data);
    *size = length;

    *data = realloc(*data, length + 1);

    // Clear macro name and body
    for(i = 0; name[i]; i++)
        name[i] = 0;

    for(i = 0; body[i]; i++)
        body[i] = 0;

    free(*s);
    *s = NULL;
}


char *replaceArguments(char *data, char *name, char *body)
{
    int i, j, f = 0;

    arg *head = 0;
    arg *prev;

    int c = 0, k;

    // Store formal parameters in linked list
    for(i = 0, f = 0; name[i]; i++)
    {
        if(name[i] == '(')
        {
            c = 1;
            continue;
        }

        if(c == 0)
            continue;

        if(f == 0)
            if((name[i] >= 'a' && name[i] <= 'z') ||
               (name[i] >= 'A' && name[i] <= 'Z') ||
               name[i] == '_')
                f = 1;

        if(name[i] == ')')
            break;

        if(f == 1)
        {
            int k = 0;

            arg *new = calloc(1, sizeof(arg));

            for(j = i;
                (name[j] >= 'a' && name[j] <= 'z') ||
                (name[j] >= 'A' && name[j] <= 'Z') ||
                (name[j] >= '0' && name[j] <= '9') ||
                name[j] == '_';
                j++, k++)
            {
                new->formal[k] = name[j];
            }

            i = j - 1;
            f = 0;

            if(head == 0)
            {
                head = new;
                prev = new;
            }
            else
            {
                prev->next = new;
                prev = new;
            }
        }
    }

    // Store actual arguments in linked list
    arg *p = head;

    for(i = 0, f = 0, c = 0; data[i]; i++)
    {
        if(f == 0)
            if(data[i] == '(')
            {
                f = 1;
                continue;
            }

        if(data[i] == ')')
            break;

        if(f == 1)
        {
            for(j = i, k = 0, c = 0; data[j]; j++, k++)
            {
                if(data[j] == '(')
                    c++;

                if(data[j] == ')')
                    c--;

                if(data[j] == ',' && c == 0)
                    break;

                if(c < 0)
                    break;

                p->actual[k] = data[j];
            }

            i = j;
            p = p->next;

            if(p == 0)
                break;
        }
    }

    // Replace formal parameters with actual parameters
    char *tBody = NULL;

    p = head;

    int allocation = 1;
    int Index = 0;

    for(i = 0; body[i]; i++)
    {
        p = head;
        f = 0;

        while(p)
        {
            int k = 0;

            for(j = i; p->formal[k] && body[j]; k++, j++)
                if(p->formal[k] != body[j])
                    break;

            if(p->formal[k] == '\0')
            {
                for(j = 0; p->actual[j]; j++)
                {
                    tBody = realloc(tBody, allocation * sizeof(char));
                    tBody[Index++] = p->actual[j];
                    allocation++;
                }

                int len = strlen(p->formal);
                i += len - 1;

                f = 1;
                break;
            }

            p = p->next;
        }

        if(f == 0)
        {
            tBody = realloc(tBody, allocation * sizeof(char));
            tBody[Index++] = body[i];
            allocation++;
        }
    }

    tBody = realloc(tBody, allocation * sizeof(char));
    tBody[Index] = '\0';

    // Free linked list nodes
    p = head;

    while(p)
    {
        head = p->next;
        free(p);
        p = head;
    }

    return tBody;
}

