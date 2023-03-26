#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "hbt.h"

static void print_tree(Tnode* head)
{
    if(head == NULL)
        return;

    print_tree(head -> left);
    printf("%d Bal: %d\n", head -> key, head -> balance);
    print_tree(head -> right);

    return;
}

static Tnode* CR(Tnode* ya)
{
    Tnode* A = ya;
    Tnode* B = ya -> left;
    Tnode* T2 = B -> right;

    B -> right = A;
    A -> left = T2;

    return B;
}

static Tnode* CCR(Tnode* ya)
{
    Tnode* A = ya;
    Tnode* B = ya -> right;
    Tnode* T2 = ya -> right -> left;

    B -> left = A;
    A -> right = T2;

    return B;
}

static Tnode* insert_to_tree(Tnode* new_node, Tnode* head)
{
    Tnode* pya = NULL;
    Tnode* ya = head;
    Tnode* curr = head;
    Tnode* pcurr = NULL;
    Tnode* q = NULL;
    bool top_level = false;

    while(curr != NULL)
    {
        if(new_node -> key > curr -> key)
            q = curr -> right;
        else
            q = curr -> left;

        // Update Youngest Ancestor
        
        if(q != NULL && q -> balance != 0)
        {
            pya = curr;
            ya = q;
        }
        
        pcurr = curr;
        curr = q;
    }

    if(ya == head)
        top_level = true;

    // Insert
    if(new_node -> key > pcurr -> key)
        pcurr -> right = new_node;
    else
        pcurr -> left = new_node;
    
    // Update Balance
    curr = ya;
    while(curr != new_node)
    {
        if(curr -> key > new_node -> key)
        {
            curr -> balance += 1;
            curr = curr -> left;
        }
        else
        {
            curr -> balance -= 1;
            curr = curr -> right;
        }
    }

    // Check If Rebalancing Is Nessecary

    if(ya -> balance > -2 && ya -> balance < 2)
        return head;
    
    // Find The Child Where The New Insertion Happend
    Tnode* child = NULL;
    if(new_node -> key > ya -> key)
        child = ya -> right;
    else
        child = ya -> left;
    // Rebalancing

    // Same Direction Rebalance
    if(ya -> balance == 2 && child -> balance == 1)
    {
        curr = CR(ya);
        ya -> balance = 0;
        child -> balance = 0;
        printf("CR\n");
    }
    if(ya -> balance == -2 && child -> balance == -1)
    {
        printf("CCR at %d, Curr = %d\n", ya -> key, curr -> key);
        curr = CCR(ya);
        ya -> balance = 0;
        child -> balance = 0;
    }
    // Opposite Direction Rebalance

    if(pya == NULL)
        head = curr;
    else
        if(new_node -> key < pya -> key)
            pya -> left = curr;
        else
            pya -> right = curr;
    

    if(top_level)
        return curr;
    else
        return head;
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

static Tnode* make_Tnode(FILE* fp, int* pass_ch)
{
    int num;
    char ch = 0;

    fread(&num, sizeof(num), 1, fp);
    *pass_ch = fread(&ch, sizeof(ch), 1, fp);

    if(*pass_ch)
    {
        Tnode* new_tnode = malloc(sizeof(*new_tnode));
        *new_tnode = (Tnode){.key = num, .balance = 0, .left = NULL, .right = NULL};
        return new_tnode;     
    }
    return NULL;
}

int main(int argc, char* argv[])
{
    FILE* fp = fopen(argv[1], "r");
    int pass_ch;

    Tnode* head_node = make_Tnode(fp, &pass_ch);

    //while(!feof(fp))
    for(int i = 5; i > 0; i--)
    {
        Tnode* new_tnode = make_Tnode(fp, &pass_ch);
        if(pass_ch)
            head_node = insert_to_tree(new_tnode, head_node);
        print_tree(head_node);
        printf("\n");
    }
    fclose(fp);

    free_tree(head_node);

    return EXIT_SUCCESS;
}
