#include <stdio.h>
// Add your system includes here.
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include "ptree.h"

// Defining the constants described in ptree.h
const unsigned int MAX_PATH_LENGTH = 1024;

// If TEST is defined (see the Makefile), will look in the tests 
// directory for PIDs, instead of /proc.
#ifdef TEST
    const char *PROC_ROOT = "tests";
#else
    const char *PROC_ROOT = "/proc";
#endif


/*
 * Creates a PTree rooted at the process pid. The root of the tree is
 * placed in root. The function returns 0 if the tree was created 
 * successfully and 1 if the tree could not be created or if at least
 * one PID was encountered that could not be found or was not an 
 * executing process.
 */
int generate_ptree(struct TreeNode **root, pid_t pid) {
    // Here's a trick to generate the name of a file to open. Note 
    // that it uses the PROC_ROOT variable
    char procfile[MAX_PATH_LENGTH + 1];
    
    if (sprintf(procfile, "%s/%d/exe", PROC_ROOT, pid) < 0) {
        fprintf(stderr, "sprintf failed to produce a filename\n");
        return 1;
    }
    
    *root = malloc(sizeof(struct TreeNode));
    // **root.pid = pid;
    (*root)->pid =  pid;
    (*root)->name = NULL;
    (*root)->child_procs = NULL;
    (*root)->next_sibling = NULL;

    // Check if the file exists 
    struct stat info;
    if (lstat(procfile, &info) == 0){
        // Get path of the cmdline 
        if (sprintf(procfile, "%s/%d/cmdline", PROC_ROOT, pid) < 0) {
            fprintf(stderr, "sprintf failed to produce a filename\n");
            return 1;
        }
        // Open it for reading and store the path in buffer
        FILE *fp = fopen(procfile, "r");
        if (fp == NULL) {
            fprintf(stderr, "fopen failed\n");
            return 1;
        }
        char buffer[MAX_PATH_LENGTH +1];
        int index = fread(buffer, 1, sizeof(buffer), fp );
        if (index != 0 ){
            buffer[index]='\0';
            (*root)->name = malloc(strlen(basename(buffer)) + 1);
            strcpy((*root)->name, basename(buffer));
        } else {
            (*root)->name = NULL;
            
        }
        
        if (fclose(fp) != 0){
            fprintf(stderr, "fclose failed\n");
            return 1;
        }
        

        if (sprintf(procfile, "%s/%d/task/%d/children", PROC_ROOT, pid, pid) < 0) {
            fprintf(stderr, "sprintf failed to produce a filename\n");
	  
            return 1;
        }

        fp = fopen(procfile, "r");
        if (fp == NULL) {
            fprintf(stderr, "fopen failed %d\n", errno);
            return 1;
        }
        if (lstat(procfile, &info) != 0){
	    (*root)->child_procs = NULL;
	    return 0;
             
        } else {
                
            int pID1;
            char a;
            struct TreeNode *currchild = NULL;
            // (*root)->child_procs = malloc(sizeof(struct TreeNode));
        
            while (fscanf(fp, "%d%c", &pID1, &a) != -1){
            

                struct TreeNode *child; 
                generate_ptree(&child, pID1);
                
                if (currchild == NULL){
                    (*root)->child_procs = malloc(sizeof(struct TreeNode));
                    (*root)->child_procs = child;
                    // (*root)->next_sibling = malloc(sizeof(struct TreeNode));
                    currchild = child;
                
                    
                }else {
                    currchild->next_sibling = child;
                    // (*root)->child_procs->next_sibling = child;
                    // currchild->child_procs = child->child_procs;
                    
                    currchild = child;
                    
                }
            }
        }
        


            
        if (fclose(fp) != 0){
            fprintf(stderr, "fclose failed\n");
            perror("fopen failedddd");
            return 1;
        }


    } else {
        fprintf(stderr, "Not a running process\n");
        return 1;  
    }

    // printf("%s\n", procfile);

    // Your implementation goes here.


    return 0;
}




/*
 * Prints the TreeNodes encountered on a preorder traversal of an PTree
 * to a specified maximum depth. If the maximum depth is 0, then the 
 * entire tree is printed.
 */
void print_ptree(struct TreeNode *root, int max_depth) {
 
    static int depth = 0;
    

    if(root == NULL || (depth == max_depth && max_depth != 0)) {
        return;
      
    } else {
        printf("%*s", depth * 2, "");
        printf("%d: %s\n",root->pid, root->name);
        
        while (root->child_procs != NULL)  {
            depth +=1;
            print_ptree(root->child_procs, max_depth);  
            root->child_procs = root->child_procs->child_procs; 
            depth -=1; 
        }
        print_ptree(root->next_sibling, max_depth);    
     }
}




