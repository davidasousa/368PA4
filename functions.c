#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "hbt.h"

static Tnode* CR(Tnode* head) // Case1A
{
    Tnode* A = head;
    Tnode* B = head -> left;
    Tnode* C = B -> right;

    A -> left = C;
    B -> right = A;

    A -> balance = 0;
    B -> balance = 0;

    return B;
}

static Tnode* CCR(Tnode* head) // Case1B
{
    Tnode* A = head;
    Tnode* B = head -> right;
    Tnode* C = B -> left;

    A -> right = C;
    B -> left = A;

    A -> balance = 0;
    B -> balance = 0;

    return B;
}

static Tnode* Case2L(Tnode* head)
{
    Tnode* A = head;
    Tnode* B = head -> left;
    Tnode* C = head -> left -> right;

    int CBal = C -> balance;

    A -> left = CCR(B);
    Tnode* temp = CR(A);

    if(CBal == 1)
    {
        A -> balance = -1; //should be -1
        B -> balance = 0;
    }
    else if(CBal == 0)
    {
        A -> balance = 0;
        B -> balance = 0;
    }
    else // bal == -1
    {
        A -> balance = 0;
        B -> balance = 1;
    }

    return temp;
}

static Tnode* Case2R(Tnode* head)
{
    Tnode* A = head;
    Tnode* B = head -> right;
    Tnode* C = head -> right -> left;

    int CBal = C -> balance;

    A -> right = CR(B);
    Tnode* temp = CCR(A);

    if(CBal== 1)
    {
        A -> balance = 0;
        B -> balance = -1;
    }
    else if(CBal == 0)
    {
        A -> balance = 0;
        B -> balance = 0;
    }
    else // bal == -1
    {
        A -> balance = 1;
        B -> balance = 0;
    }

    return temp;
}

static Tnode* Case3L(Tnode* head)
{
    Tnode* B = head;
    Tnode* A = head -> left;
    Tnode* C = head -> left -> right;

    A -> right = B;
    B -> left = C;

    A -> balance = -1;
    B -> balance = 1;

    return A;
}

static Tnode* Case3R(Tnode* head)
{
    Tnode* B = head;
    Tnode* A = head -> right;
    Tnode* C = head -> right -> left;

    A -> left = B;
    B -> right = C;

    A -> balance = 1;
    B -> balance = -1;

    return A;
}

static Tnode* make_Tnode(int val)
{
    Tnode* new_tnode = malloc(sizeof(*new_tnode));
    *new_tnode = (Tnode){.key = val, .balance = 0, .left = NULL, .right = NULL};
    return new_tnode;     
}

Tnode* insert_to_tree(Tnode* head, int key)
{
    Tnode* curr = head;
    Tnode* pcurr = NULL;
    Tnode* ya = head;
    Tnode* pya = NULL;

    while(curr != NULL)
    {
        if(key > curr -> key)
        {
            pcurr = curr;
            curr = curr -> right;
        }
        else
        {
            pcurr = curr;
            curr = curr -> left;     
        }
        
        if(curr != NULL && curr -> balance != 0) // Updating The YA
        {
            pya = pcurr;
            ya = curr;
        }
    }

    Tnode* new_node = make_Tnode(key);
    if(key > pcurr -> key) // Finding The Side Where The Insertion Happend
        pcurr -> right = new_node;
    else
        pcurr -> left = new_node;

    // update balances from head -> new_node
    curr = ya;
    while(curr != new_node)
    {
        if(key > curr -> key)
        {
            curr -> balance += -1;
            curr = curr -> right;
        }
        else
        {
            curr -> balance += 1;
            curr = curr -> left;
        }
    }

    // Check For Rebalance
    if(ya -> balance < 2 && ya -> balance > -2)
        return head;
    
    Tnode* child;
    if(key > ya -> key)
        child = ya -> right;
    else
        child = ya -> left;

    if(ya -> balance == 2 && child -> balance == 1) // CASE1A
        curr = CR(ya);

    if(ya -> balance == -2 && child -> balance == -1) // CASE1B
        curr = CCR(ya);

    if(ya -> balance == 2 && child -> balance == -1) // CASE2L
        curr = Case2L(ya);

    if(ya -> balance == -2 && child -> balance == 1) //CASE2R
        curr = Case2R(ya);

    // FINISH
    if(pya == NULL)
    {
        return curr;
    }
    else
    {
        if(key > pya -> key)
            pya -> right = curr;
        else
            pya -> left = curr;
    }
    return head;
}

static Tnode* rebalance(Tnode* parent, Tnode* child)
{            
    if(parent -> balance == -2 && child -> balance == -1)
        return CCR(parent);
    
    if(parent -> balance == 2 && child -> balance == 1)
        return CR(parent);

    if(parent -> balance == -2 && child -> balance == 1)
        return Case2R(parent);

    if(parent -> balance == 2 && child -> balance == -1)
        return Case2L(parent);

    if(parent -> balance == 2 && child -> balance == 0)
        return Case3L(parent);

    if(parent -> balance == -2 && child -> balance == 0)
        return Case3R(parent);

    return parent;
}

static void Case3Del(Tnode* curr, Tnode* prev, int* swap_val, bool* pass)
{
    if(curr -> right != NULL)
        Case3Del(curr -> right, curr, swap_val, pass);

    if(*pass == false)
    {
        *swap_val = curr -> key;
        prev -> right = NULL;
        free(curr);
        *pass = true;
        return;
    }
    else
    {
        curr -> balance += 1;

        if(curr -> balance == 2 || curr -> balance == -2)
        {
            Tnode* parent = curr;
            Tnode* child;
            if(parent -> balance == 2)
                child = parent -> left;
            else
                child = parent -> right;

            // is changed in here from -1 to 0
            curr = rebalance(parent, child);

            if (parent -> key > prev -> key)
                prev -> right = curr;
            else
                prev -> left = curr;
        }

    return;
    
    }
}


int deletion(Tnode** head, Tnode* curr, Tnode* prev, int key)
{
    if(curr == NULL)
        return 0;

    int height_change = 0;
    if(key > curr -> key)
        height_change = deletion(head, curr -> right, curr, key);
    else if(key < curr -> key)
        height_change = deletion(head, curr -> left, curr, key);

    if(curr -> key == key) // Deletion
    {   
        if(curr -> left == NULL && curr -> right == NULL) // Case 1
        {
            if(prev -> left == curr)
                prev -> left = NULL;
            else
                prev -> right = NULL;
            free(curr);

            return 1;
        }
        if(curr -> left == NULL && curr -> right != NULL) // Case 2 Right
        {
            Tnode* temp = curr -> right;
            curr -> right = NULL;
            curr -> key = temp -> key;
            curr -> balance = 0;
            free(temp);

            return 1;
        }
        if(curr -> left != NULL && curr -> right == NULL) // Case 2 Left
        {
            Tnode* temp = curr -> left;
            curr -> left = NULL;
            curr -> key = temp -> key;
            curr -> balance = 0;
            free(temp);

            return 1;
        }
        if(curr -> left != NULL && curr -> right != NULL) // Case 3
        {
            int balance_before = curr -> balance;

            if(curr -> left -> right == NULL)
            {
                Tnode* temp = curr -> left;
                curr -> key = temp -> key;
                curr -> left = temp -> left;
                curr -> balance += -1;
                free(temp);
            }
            else
            {
                int swap_val;
                bool pass = false;
                Case3Del(curr -> left, NULL, &swap_val, &pass);
                curr -> key = swap_val;
            }

            int balance_after = curr -> balance;

            if(balance_before == -1 && balance_after == 0)
                return 1;

            if(balance_before == 1 && balance_after == 0)
                return 1;

            return 0;
        }
        else
            return 0; // Should Be Gone

    }
    else // Update Balance / Check For Rebalance
    {
        int balance_before = curr -> balance;

        if(key > curr -> key)
            curr -> balance += height_change;
        else
            curr -> balance -= height_change;
        
        if(curr -> balance == 2 || curr -> balance == -2)
        {
            Tnode* parent = curr;
            Tnode* child;
            if(parent -> balance == 2)
                child = parent -> left;
            else
                child = parent -> right;

            curr = rebalance(parent, child);

            if(prev == NULL)
                *head = curr;
            else if (parent -> key > prev -> key)
                prev -> right = curr;
            else
                prev -> left = curr;
        }

        int balance_after = curr -> balance;

        if(balance_before == -1 && balance_after == 0)
            return 1;

        if(balance_before == 1 && balance_after == 0)
            return 1;

        return 0;
    }
}

