#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>	
#include <stdbool.h>
#include <stdio.h>

char *concat(const char *str1, const char *str2)
{
	char *concatinated = malloc(strlen(str1) + strlen(str2) + 1);
	
	strcpy(concatinated, str1);
	strcat(concatinated, str2);
	return concatinated;
}


bool is_equal(const char s1[], const char s2[])
{
	int i = 0;
	bool areEqual;
	
	while(s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
		++i;
	
	if(s1[i] == '\0' && s2[i] == '\0')
		areEqual = true;
	else
		areEqual = false;
		
	return areEqual;
}


void v_option(char *source, char *destination)
{
	printf("%s -> %s\n", source, destination);
}


void i_option(char *mv, char *source, char *destination)
{
	char choice;
	void print_error(char *mv, char *source, char *destination);
	
	struct stat file_type;
	
	char *dest;
	char *dir;
	
	if(access(destination, F_OK) != -1)
	{
		if(stat(destination, &file_type) != -1 && (file_type.st_mode & S_IFDIR))
		{
			dest = concat("/", source);
			dir = concat(destination, dest);
			
			if(rename(source, dir) == -1)
			{
				print_error(mv, source, destination);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			printf("overwrite %s? (y/n) ", destination);
			scanf("%c", &choice);
			if(choice == 'y' || choice == 'Y')
			{
				if(rename(source, destination) == -1)
				{
					print_error(mv, source, destination);
				}
			}
			else if(choice == 'n' || choice == 'N')
			{
				printf("%s not replaced\n", destination);
				exit(EXIT_SUCCESS);
			}
			else
			{
				printf("ERROR: choice not recognized\n");
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
		}
	}	
	else
	{
		if(rename(source, destination) == -1)
		{
			print_error(mv, source, destination);
		}
		exit(EXIT_SUCCESS);
	}	
}


void n_option(char *mv, char *source, char *destination)
{
	void print_error(char *mv, char *source, char *destination);
	
	struct stat file_type;
	
	char *dest;
	char *dir;
	
	if(access(destination, F_OK) != -1)
	{
		exit(EXIT_FAILURE);
	}
	else
	{
		if(stat(destination, &file_type) != -1 && (file_type.st_mode & S_IFDIR))
		{
			dest = concat("/", source);
			dir = concat(destination, dest);
			
			if(rename(source, dir) == -1)
			{
				print_error(mv, source, destination);
			}
			exit(EXIT_SUCCESS);
		}
		else
		{
			if(rename(source, destination) == -1)
			{
				print_error(mv, source, destination);
			}
			exit(EXIT_SUCCESS);
		}
	}
}


void f_option(char *mv, char *source, char *destination)
{
	void print_error(char *mv, char *source, char *destination);
	
	struct stat file_type;
	
	char *dest;
	char *dir;
	
	if(stat(destination, &file_type) != -1 && (file_type.st_mode & S_IFDIR))
	{
		dest = concat("/", source);
		dir = concat(destination, dest);
			
		if(rename(source, dir) == -1)
		{
			print_error(mv, source, destination);
		}
		exit(EXIT_SUCCESS);
	}
	else
	{
		if(rename(source, destination) == -1)
		{
			print_error(mv, source, destination);
		}
		exit(EXIT_SUCCESS);
	}
}


void print_error(char *mv, char *source, char *destination)
{
	fprintf(stderr, "%s: %s: %s: cannot move to %s\n", 
				mv, source, strerror(errno), destination);	
	exit(EXIT_FAILURE);
}


void print_syntax_error(char *mv)
{
	fprintf(stderr, "SYNTAX ERROR:\nUSAGE: %s [-f  -i  -n  -v] [source] [destination]\n", mv);
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{	
	struct stat file_type;
	char *dest;
	char *dir;
	
	errno = 0;
	if(argc == 3)
	{
		if(stat(argv[2], &file_type) != -1 && (file_type.st_mode & S_IFDIR))
		{
			dest = concat("/", argv[1]);
			dir = concat(argv[2], dest);
			
			if(rename(argv[1], dir) == -1)
			{
				print_error(argv[0], argv[1], argv[2]);
			}
		}
		else
		{
			if(rename(argv[1], argv[2]) == -1)
			{
				print_error(argv[0], argv[1], argv[2]);
			}
		}
	}
	else if(argc == 4)
	{
		if(is_equal(argv[1], "-v"))
		{
			v_option(argv[2], argv[3]);
		}
		else if(is_equal(argv[1], "-i"))
		{
			i_option(argv[0], argv[2], argv[3]);
		}
		else if(is_equal(argv[1], "-n"))
		{
			n_option(argv[0], argv[2], argv[3]);
		}
		else if(is_equal(argv[1], "-f"))
		{
			f_option(argv[0], argv[2], argv[3]);
		}
		else
			print_syntax_error(argv[0]);
		
		if(stat(argv[3], &file_type) != -1 && (file_type.st_mode & S_IFDIR))
		{
			dest = concat("/", argv[2]);
			dir = concat(argv[3], dest);
			
			if(rename(argv[2], dir) == -1)
			{
				print_error(argv[0], argv[2], argv[3]);
			}
		}
		else
		{
			if(rename(argv[2], argv[3]) == -1)
			{
				print_error(argv[0], argv[2], argv[3]);
			}
		}
	}
	else
	{
		print_syntax_error(argv[0]);
	}
	
	return 0;
}
