#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>

#include "ptree.h"


int main(int argc, char **argv) {
    // TODO: Update error checking and add support for the optional -d flag
    // printf("Usage:\n\tptree [-d N] PID\n");

    // NOTE: This only works if no -d option is provided and does not
    // error check the provided argument or generate_ptree. Fix this!
    if (argc != 2 && argc != 4){
        fprintf(stderr, "Invalid number of arguments.\n");
        return 1;
    } 

    int depth;
    int d;
    // char* optarg = NULL;
    opterr = 0;

    while ((d = getopt (argc, argv, "d:")) != -1)
        switch (d){
            case 'd':
                depth = strtol(optarg, NULL, 10);
                break;
            case '?':
                if (optopt == 'd')
                    fprintf (stderr, "Option -%d requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%d'.\n", optopt);
                else
                    fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort ();
            }


    
    int pID = strtol(argv[optind], NULL, 10);
    if (pID == 0){
        fprintf(stderr, "pID provided is not a number.\n");
        return 1;
    }

    struct TreeNode *root;
    // root->pid = 0;
    // root->name = NULL;
    // root->child_procs = NULL;
    // root->next_sibling = NULL;
    if (generate_ptree(&root, pID) == 1){
        fprintf(stderr, "Tree could not be created or PID not found/running.\n");
        return 2;
    }

    print_ptree(root, depth);

    

    // struct TreeNode *currchild = root->child_procs;
    // struct TreeNode *currSibling = root;
    
    // while (currchild->child_procs != NULL){
    //     while (currchild->next_sibling != NULL){

    //     }
    //     free(currchild)
    //     currchild = currchild->child_procs;
    //     currSibling = currchild->next_sibling;
    //     free(currchild);
    //     cu
    // }
    // struct TreeNode *curr = root;
    
    // curr = freeMe;
    
    // while (curr->next_sibling != NULL){
    //     while (curr->child_procs != NULL){
    //         freeMe = curr;
    //         curr = curr->child_procs;
    //         free(freeMe);
    //     }
    //     freeMe = root;
    //     curr = curr->next_sibling;
    //     free(freeMe);
    // }


    

    return 0;
}

// def freeNode(struct TreeNode a){


// }

