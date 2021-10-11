#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

int gsi_is_print_dir_recursive(char* s_dirname, int i_current_level,
												int i_max_level)
{
	int i_level = 0;
	DIR *p_dir = NULL;
	struct dirent *p_dirent = NULL;
	char s_new_dirname[PATH_MAX];

	// Check input validation
	if ((NULL == s_dirname) || ((0 >= i_max_level) && (-1 != i_max_level)))
	{
		return -1;
	}

	if ((-1 == i_max_level) || (i_current_level < i_max_level))
	{
		//Open the directory and return a directory stream pointer to the first directory item
		p_dir = opendir(s_dirname);
		if (NULL == p_dir)
		{
			return -1;
		}

		//Read each directory item in a loop, when it returns NULL, the reading is complete
		while (NULL != (p_dirent = readdir(p_dir)))
		{
			// Skip "." & ".." directories
			if ((strcmp(p_dirent->d_name, ".")  == 0) ||
				(strcmp(p_dirent->d_name, "..") == 0))
			{
				continue;
			}

			for (i_level = 0; i_level < i_current_level; ++i_level)
			{
				printf("│");
				printf("     ");
			}
			printf("├── ");
			printf("%s\n", p_dirent->d_name);


			//If the directory item is still a directory, enter the directory
			if (DT_DIR == p_dirent->d_type)
			{
				// Get Current directory length
				int i_dir_len = strlen(s_dirname) + 1;

				// Reset buffer before use
				memset(s_new_dirname, 0, sizeof(s_new_dirname));

				// Create new path to go deep inside the tree structure
				sprintf(s_new_dirname,"%s/%s" ,s_dirname, p_dirent->d_name);

				// Call recursion on the directory entry found
				gsi_is_print_dir_recursive(s_new_dirname, i_current_level + 1, i_max_level);
			}
		}

		closedir(p_dir);
	}

	return 0;
	
}



int main(int argc , char *argv[])
{
    char direntName[256];
    memset(direntName, 0, sizeof(direntName));
 
    if(argc == 1)
    {
        direntName[0] = '.';
    }
    else if(argc == 2)
    {
        strcat(direntName, argv[1]);
    }
    else
    {
        printf("argument error");
    }
 
     //Output directory structure
    printf("%s\n", direntName);
    
    gsi_is_print_dir_recursive(direntName, 0, -1);
    return 0;


}


