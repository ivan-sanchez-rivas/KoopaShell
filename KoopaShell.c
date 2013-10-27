#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>

static char *myArgv[20]; //arrays donde se van a guardar los argv
static char *searchPath[10];// array que guarda todos los paths

void Llenarargv(char *tmp_argv)
{
	char *foo = tmp_argv;
	int index = 0;
	char ret[100];
	bzero(ret, 100);
	while(*foo != '\0') { // \0 significa que es el final de un array o string
		if(index == 20)
		{
			break;
		}
		if(*foo == ' ') {
			if(myArgv[index] == NULL)
				myArgv[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);//Se le asigna memoria a myArgv por cada dato que hay en ret
			else 
			{
			bzero(myArgv[index], strlen(myArgv[index]));//strlen te da el length del string de myArgv
			}
			strncpy(myArgv[index], ret, strlen(ret));
			strncat(myArgv[index], "\0", 1);	
			bzero(ret, 100); //Coloca "n" veces bytes que valen 0  en el area apuntada del ret;
			index++;
		} else {
			strncat(ret, foo, 1);
		}
		foo++;
	}
	myArgv[index] = (char *)malloc(sizeof(char) * strlen(ret) + 1);
	strncpy(myArgv[index], ret, strlen(ret));
	strncat(myArgv[index], "\0", 1);
}

void ConseguirCamino(char **tmpenvp, char *bin_path)
{
	int count = 0;
	char *tmp;
	int index=0;
	char ret[100];
	while(1)
	{
		tmp = strstr(tmpenvp[count], "PATH");
		if(tmp == NULL)
		{
		count++;
		}
		else
		{
		break;
		}
	}
        strncpy(bin_path, tmp, strlen(tmp));
        
	
	char *tmp2 = bin_path;
	

	while(*tmp2 != '=')
		tmp2++;
	tmp2++;

	while(*tmp2 != '\0') {
		if(*tmp2 == ':') {
			strncat(ret, "/", 1);
			searchPath[index] = (char *) malloc(sizeof(char) * (strlen(ret) + 1));
			strncat(searchPath[index], ret, strlen(ret));
			strncat(searchPath[index], "\0", 1);
			index++;
			bzero(ret, 100);;
		} else {
			strncat(ret, tmp2, 1);
		}
		tmp2++;
	}
}
int UnirCamino(char *cmd)
{
	char ret[100];
	int index;
	int fd;
	bzero(ret, 100);
	for(index=0;searchPath[index]!=NULL;index++) {
		strcpy(ret, searchPath[index]);
		strncat(ret, cmd, strlen(cmd));
		if((fd = open(ret, O_RDONLY)) > 0) { // con Path
			strncpy(cmd, ret, strlen(ret));
			close(fd);
			return 0;
		}
	}
	return 0;
}


int main(int argc, char *argv[], char *envp[])
{
	char c;
	int i, file;
	char *tmp = (char *)malloc(sizeof(char) * 100);
	char *path = (char *)malloc(sizeof(char) * 100);
	char *cmd = (char *)malloc(sizeof(char) * 100);
	ConseguirCamino(envp, path);
	printf("KoopaShell: ");
	while(c != EOF) {
		c = getchar();
		if(c=='\n')
    {
       				 if(tmp[0] == '\0') {
					   printf("KoopaShell: ");
				   } else {
					   Llenarargv(tmp);
					   strncpy(cmd, myArgv[0], strlen(myArgv[0]));
					   strncat(cmd, "\0", 1);
					   if(index(cmd, '/') == NULL)
					   {
						   if(UnirCamino(cmd) == 0)
						   {
							   int i;
							   if(fork() == 0) {
		    				           i = execve(cmd, myArgv, envp);
							   if(i < 0)
							   {
							   printf("%s: %s\n","No existe comando",cmd);
							   exit(1);
						  	   }
							   } else {
							   wait(NULL);
								}
							   chdir("KoopaShell");
						   }
					   }
					   else
					   {
						   if((file = open(cmd, O_RDONLY)) > 0)
						   {
							   close(file);
							   int i;
							   if(fork() == 0) {
	    				           i = execve(cmd, argv, envp);
							   if(i < 0)
							   {
							   printf("%s: %s\n","No existe comando",cmd);
							   exit(1);
						  	   }
							   } else {
							   wait(NULL);
								};
						   }
						   else
						   {
							   printf("%s: %s\n","Camino Incorrecto y/o comando no encontrado",cmd);
						   }
					   }
					   int index;
					   for(index=0;myArgv[index]!=NULL;index++) {
						bzero(myArgv[index], strlen(myArgv[index])+1);
						myArgv[index] = NULL;
						free(myArgv[index]);
					   }
					   printf("KoopaShell: ");
					bzero(cmd, 100);
				   }
				bzero(tmp,100);
		}
		else
        {
            strncat(tmp, &c, 1);
        }
	}
	return 0;
}


