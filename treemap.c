#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * newTree = (TreeMap *)malloc(sizeof(TreeMap));
    if (newTree == NULL) return NULL;
    newTree->root = NULL;
    newTree->current = NULL;
    newTree->lower_than = lower_than;
    return newTree;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL) return;

    TreeNode * newNode = createTreeNode(key, value);
    if (newNode == NULL) return;

    if (tree->root == NULL) {
        tree->root = newNode;
        return;
    }

    TreeNode * aux = tree->root;
    TreeNode * parent = NULL;

    while (aux != NULL) {
        parent = aux;
        if (is_equal(tree, aux->pair->key, key))return;      
        if (tree->lower_than(newNode->pair->key, aux->pair->key)) aux = aux->left;
        else aux = aux->right;
    }

    newNode->parent = parent;
    if (tree->lower_than(newNode->pair->key, parent->pair->key)) parent->left = newNode;
    else parent->right = newNode;

    tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL) return NULL;
    while(x->left != NULL) {
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node){
    if (node == NULL) return;

    // Nodo sin hijos
    if (node->left == NULL && node->right == NULL) {
        if (node->parent == NULL) {
            tree->root = NULL;
        } else {
            if (node->parent->left == node)
                node->parent->left = NULL;
            else
                node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
        return;
    }

    // Nodo con un solo hijo
    if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        child->parent = node->parent;

        if (node->parent == NULL) {
            tree->root = child;
        } else {
            if (node->parent->left == node)
                node->parent->left = child;
            else
                node->parent->right = child;
        }

        free(node->pair);
        free(node);
        return;
    }

    // Nodo con dos hijos
    TreeNode* successor = minimum(node->right);
    Pair* tempPair = node->pair;
    node->pair = successor->pair;
    successor->pair = tempPair;
    removeNode(tree, successor);
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if(tree == NULL || tree->root == NULL) return NULL;
    TreeNode* aux = tree->root;
    while(aux != NULL){
        if(is_equal(tree, aux->pair->key, key)){
            tree->current = aux;
            return aux->pair;
        }
        else if(tree->lower_than(key, aux->pair->key)) aux = aux->left;
        else aux = aux->right;
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;

    TreeNode* node = tree->root;
    TreeNode* successor = NULL;

    while (node != NULL) {
        if (tree->lower_than(key, node->pair->key)) {
            successor = node;
            node = node->left;  
        } else {
            node = node->right;  
        }
    }

    if (successor == NULL) return NULL;  
    return successor->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    if(tree == NULL || tree->root == NULL) return NULL;
    return minimum(tree->root)->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;

    TreeNode* node = tree->current;

    if (node->right != NULL) {
        node = minimum(node->right);
    } else {
        while (node->parent != NULL && node == node->parent->right) {
            node = node->parent;
        }
        node = node->parent;
    }

    tree->current = node;
    if (node == NULL) return NULL;
    return node->pair;
}

