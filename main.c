#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "hbt.h"
#include "functions.h"

static int find_height(Tnode* head);

static Tnode* make_Tnode(int val)
{
    Tnode* new_tnode = malloc(sizeof(*new_tnode));
    *new_tnode = (Tnode){.key = val, .balance = 0, .left = NULL, .right = NULL};
    return new_tnode;     
}

static void free_tree(Tnode* head)
{
    if(head == NULL)
        return;
    free_tree(head -> left);
    free_tree(head -> right);
    free(head);
    return;
}

static void read_file(FILE* fp, int* pass_ch, int* pass_int, int* val, char* operation)
{
    *pass_int = fread(val, sizeof(*val), 1, fp);
    *pass_ch = fread(operation, sizeof(*operation), 1, fp);
    return;
}

static void write_to_file(Tnode* head, FILE* fp)
{
    if(head == NULL)
        return;

    int val = head -> key;
    fwrite(&val, sizeof(int), 1, fp);

    char links = 0;
    if(head -> right != NULL)
        links += 1;
    if(head -> left != NULL)
        links += 2;

    fputc(links, fp);
    write_to_file(head -> left, fp);
    write_to_file(head -> right, fp);
    return;
}

static Tnode* create_eval_tree(FILE* fp)
{
    int key = 0;
    int links = 0;

    fread(&key, sizeof(int), 1, fp);
    fread(&links, sizeof(char), 1, fp);

    Tnode* head = malloc(sizeof(*head));
    *head = (Tnode) {.left = NULL, .right = NULL, .key = key, .balance = 0};

    if(links == 3)
    {
        head -> left = create_eval_tree(fp);
        head -> right = create_eval_tree(fp);
    }
    else if(links == 2)
        head -> left = create_eval_tree(fp);
    else if(links == 1)
        head -> right = create_eval_tree(fp);
    return head;
}

static void check_bst(Tnode* head, bool* is_bst)
{
    if(head == NULL)
        return;

    if(head -> left != NULL)
    {
        if(head -> key < head -> left -> key)
            *is_bst = false;
    }
    if(head -> right != NULL)
    {
        if(head -> key > head -> right -> key)
            *is_bst = false;
    }
    
    check_bst(head -> left, is_bst);
    check_bst(head -> right, is_bst);
    return;
}

static int find_height(Tnode* head)
{
    if(head == NULL)
        return 1;
    
    int left_height = find_height(head -> left);
    int right_height = find_height(head -> right);

    if(left_height > right_height)
        return 1 + left_height;
    else
        return 1 + right_height; 
}

static void check_bal(Tnode* head, bool* is_bal)
{
    if(head == NULL)
        return;

    int left_bal = find_height(head -> left);
    int right_bal = find_height(head -> right);
    int balance = left_bal - right_bal;

    if(balance > 1 || balance < -1)
        *is_bal = false;

    check_bal(head -> left, is_bal);
    check_bal(head -> right, is_bal);

    return; 
}

int main(int argc, char* argv[])
{
    if(strcmp(argv[1], "-b") == 0)
    {
        FILE* fp = fopen(argv[2], "r");
        if(fp == NULL)
        {
            fprintf(stdout, "%d\n", -1);
            return EXIT_FAILURE;     
        }

        int key;
        char operation;
        int pass_key;
        int pass_op;

        read_file(fp, &pass_op, &pass_key, &key, &operation);

        Tnode* head_node;
        if(pass_key)
            head_node = make_Tnode(key);
        else
        {
            fprintf(stdout, "%d\n", 0);
            return EXIT_FAILURE;
        }

        while(!feof(fp))
        {
            read_file(fp, &pass_op, &pass_key, &key, &operation);
            if(pass_op && pass_key)
            {
                if(operation == 'i')
                    head_node = insert_to_tree(head_node, key);
                else if(operation == 'd')
                    deletion(&head_node, head_node, NULL, key);
            }
        }
        fclose(fp);
        
        fp = fopen(argv[3], "w");
        if(fp == NULL)
            return EXIT_FAILURE;

        write_to_file(head_node, fp);
        fclose(fp);
        free_tree(head_node);
        fprintf(stdout, "%d\n", 1);
        return EXIT_SUCCESS;     
    }
    if(strcmp(argv[1], "-e") == 0)
    {
        int can_file_open = 1;

        FILE* fp = fopen(argv[2], "r");
        if(fp == NULL)
            can_file_open = -1;

        Tnode* head = NULL;
        bool is_bst = true;
        bool is_bal = true;

        if(can_file_open == 1)
        {
            head = create_eval_tree(fp);
            check_bst(head, &is_bst);
            check_bal(head, &is_bal);

            free_tree(head);
            fclose(fp);
        }

        int bst = 0;
        if(is_bst)
            bst = 1;

        int bal = 0;
        if(is_bal)
            bal = 1;

        fprintf(stdout, "%d,%d,%d\n", can_file_open, bst, bal);

        if(can_file_open == 1)
            return EXIT_SUCCESS;
        else
            return EXIT_FAILURE;
    }
}
