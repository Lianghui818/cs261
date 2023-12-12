/*
 * This file is where you should implement your binary search tree.  It already
 * contains skeletons of the functions you need to implement (along with
 * documentation for each function).  Feel free to implement any additional
 * functions you might need.  Also, don't forget to include your name and
 * @oregonstate.edu email address below.
 *
 * Name: Lianghui Wang
 * Email: wangl9@oregonstate.edu
 */

#include <stdlib.h>

#include "bst.h"

/*
 * This structure represents a single node in a BST.  In addition to containing
 * pointers to its two child nodes (i.e. `left` and `right`), it contains two
 * fields representing the data stored at this node.  The `key` field is an
 * integer value that should be used as an identifier for the data in this
 * node.  Nodes in the BST should be ordered based on this `key` field.  The
 * `value` field stores data associated with the key.
 *
 * You should not modify this structure.
 */
struct bst_node {
  int key;
  void* value;
  struct bst_node* left;
  struct bst_node* right;
};


/*
 * This structure represents an entire BST.  It specifically contains a
 * reference to the root node of the tree.
 *
 * You should not modify this structure.
 */
struct bst {
  struct bst_node* root;
};

void free_node(struct bst_node* node);
int size(struct bst_node* node);
void* bst_parent(struct bst* bst, int key);
void* bst_successor(struct bst_node* bst_node);
int height(struct bst_node* node);
int path_sum(struct bst_node* node, int sum, int objective);
int range_sum(struct bst_node* node, int sum, int low, int high);


/*
 * This function should allocate and initialize a new, empty, BST and return
 * a pointer to it.
 */
struct bst* bst_create() {
  struct bst* new_bst = malloc(sizeof(struct bst));
  if (new_bst) {
    new_bst->root = NULL;
  }
  return new_bst;
}

/*
 * This function should free the memory associated with a BST.  While this
 * function should up all memory used in the BST itself, it should not free
 * any memory allocated to the pointer values stored in the BST.  This is the
 * responsibility of the caller.
 *
 * Params:
 *   bst - the BST to be destroyed.  May not be NULL.
 */

void free_node(struct bst_node* node) {
  if (node != NULL) {
    free_node(node->left);
    free_node(node->right);
    free(node);
    node = NULL;
  }
}

void bst_free(struct bst* bst) {
  if(bst == NULL){
    return;
  }
  free_node(bst->root);
  free(bst);
  return;
}

/*
 * This function should return the total number of elements stored in a given
 * BST.
 *
 * Params:
 *   bst - the BST whose elements are to be counted.  May not be NULL.
 */

int size(struct bst_node* node) {
  return node == NULL ? 0 : size(node->left) + size(node->right) + 1;
}

int bst_size(struct bst* bst) {
  return size(bst->root);
}

/*
 * This function should insert a new key/value pair into the BST.  The key
 * should be used to order the key/value pair with respect to the other data
 * stored in the BST.  The value should be stored along with the key, once the
 * right location in the tree is found.
 *
 * Params:
 *   bst - the BST into which a new key/value pair is to be inserted.  May not
 *     be NULL.
 *   key - an integer value that should be used to order the key/value pair
 *     being inserted with respect to the other data in the BST.
 *   value - the value being inserted into the BST.  This should be stored in
 *     the BST alongside the key.  Note that this parameter has type void*,
 *     which means that a pointer of any type can be passed.
 */
void bst_insert(struct bst* bst, int key, void* value) {
  if(bst == NULL){
    return;
  }

  struct bst_node* new_node = malloc(sizeof(struct bst_node));
  new_node->key = key;
  new_node->value = value;
  new_node->left = NULL;
  new_node->right = NULL;
  if(bst->root == NULL){
    bst->root = new_node;
    return;
  }

  struct bst_node* parent = NULL;
  struct bst_node* tmp = bst->root;
  while(tmp != NULL){
    parent = tmp;
    if(key < tmp->key){
      tmp = tmp->left;
      if(tmp == NULL){
        parent->left = new_node;
      }
    }
    else{
      tmp = tmp->right;
      if(tmp == NULL){
        parent->right = new_node;
      }
    }
  }
  return;
}

/*
 * This function should remove a key/value pair with a specified key from a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should remove the first one it encounters (i.e. the one closest to
 * the root of the tree).
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair to be removed from the BST.
 */
void bst_remove(struct bst* bst, int key) {
  if(bst == NULL){
    return;
  }

  struct bst_node* parent = bst_parent(bst, key);
  struct bst_node* tmp = NULL;
  if(parent == NULL){
    tmp = bst->root;
  }
  else{
    if(key < parent->key){
      tmp = parent->left;
    }
    else{
      tmp = parent->right;
    }
  }
  if(tmp->left == NULL && tmp->right == NULL){
    if(parent == NULL){
      bst->root = NULL;
    }
    else{
      if(tmp->key < parent->key){
        parent->left == NULL;
      }
      else{
        parent->right == NULL;
      }
    }
  }
  else if(tmp->left == NULL || tmp->right == NULL){
    if(parent == NULL){
      if(tmp->left != NULL){
        bst->root = tmp->left;
      }
      else{
        bst->root = tmp->right;
      }
    }
    else{
      if(tmp->key < parent->key){
        if(tmp->left == NULL){
          parent->left = tmp->right;
        }
        else{
          parent->left = tmp->left;
        }
      }
      else{
        if(tmp->left == NULL){
          parent->right = tmp->right;
        }
        else{
          parent->right = tmp->left;
        }
      }
    }
  }
  else{
    struct bst_node* suc = bst_successor(tmp->right);
    struct bst_node* suc_parent = bst_parent(bst, suc->key);
    suc->left = tmp->left;
    if (suc != tmp->right) {
      suc_parent->left = suc->right;
      suc->right = tmp->right;
    }
    if (parent == NULL) {
      bst->root = suc;
    } else {
      if (suc->key < parent->key) {
        parent->left = suc;
      } else {
        parent->right = suc;
      }
    }
  }
  free(tmp);
  return;
}

void* bst_parent(struct bst* bst, int key) {
  if(bst == NULL){
    return NULL;
  }

  struct bst_node* parent = NULL;
  struct bst_node* node = bst->root;
  while(node != NULL){
    if(key == node->key){
      return parent;
    } 
    else if(key < node->key){
      parent = node;
      node = node->left;
    } 
    else{
      parent = node;
      node = node->right;
    }
  }
  return NULL;
}

void* bst_successor(struct bst_node* bst_node) {
  if(bst_node == NULL){
    return NULL;
  }
  struct bst_node* node = bst_node;
  while(node->left != NULL){
      node = node->left;
  }
  if(node == NULL){
    return bst_node;
  }
  return node;
}

/*
 * This function should return the value associated with a specified key in a
 * given BST.  If multiple values with the same key exist in the tree, this
 * function should return the first one it encounters (i.e. the one closest to
 * the root of the tree).  If the BST does not contain the specified key, this
 * function should return NULL.
 *
 * Params:
 *   bst - the BST from which a key/value pair is to be removed.  May not
 *     be NULL.
 *   key - the key of the key/value pair whose value is to be returned.
 *
 * Return:
 *   Should return the value associated with the key `key` in `bst` or NULL,
 *   if the key `key` was not found in `bst`.
 */
void* bst_get(struct bst* bst, int key) {
  if(bst == NULL){
    return NULL;
  }

  struct bst_node* node = bst->root;
  while(node != NULL){
    if(key == node->key){
      return node;
    }
    else if(key < node->key){
      node = node->left;
    }
    else{
      node = node->right;
    }
  }
  return NULL;
}

/*****************************************************************************
 **
 ** BST puzzle functions
 **
 *****************************************************************************/

/*
 * This function should return the height of a given BST, which is the maximum
 * depth of any node in the tree (i.e. the number of edges in the path from
 * the root to that node).  Note that the height of an empty tree is -1 by
 * convention.
 *
 * Params:
 *   bst - the BST whose height is to be computed
 *
 * Return:
 *   Should return the height of bst.
 */
 int bst_height(struct bst* bst) {
  return height(bst->root);
 }

int height(struct bst_node* node) {
  if(node == NULL){
    return -1;
  }
  else{
    int height_left = height(node->left);
    int height_right = height(node->right);
    return height_left > height_right ? height_left + 1 : height_right + 1;
  }
}

/*
 * This function should determine whether a specified value is a valid path
 * sum within a given BST.  In other words, this function should check whether
 * the given BST contains any path from the root to a leaf in which the keys
 * sum to the specified value.
 *
 * Params:
 *   bst - the BST whose paths sums to search
 *   sum - the value to search for among the path sums of `bst`
 *
 * Return:
 *   Should return 1 if `bst` contains any path from the root to a leaf in
 *   which the keys add up to `sum`.  Should return 0 otherwise.
 */
int bst_path_sum(struct bst* bst, int sum) {
  return path_sum(bst->root, 0, sum);
}

int path_sum(struct bst_node* node, int sum, int comparison) {
  int result = 0;
  if(node == NULL){
    return 0;
  } 
  else{
    sum += node->key;
    if(node->left == NULL && node->right == NULL && sum == comparison){
      return 1;
    }
    if(node->left != NULL){
      result =  path_sum(node->left, sum, comparison);
    }
    if(result == 1){
      return result;
    }
    if(node->right != NULL){
      result = path_sum(node->right, sum, comparison);
    }
    if(result == 1){
      return result;
    }
  }
  return 0;
}

/*
 * This function should compute a range sum in a given BST.  Specifically, it
 * should compute the sum of all keys in the BST between a given lower bound
 * and a given upper bound.  For full credit, you should not process any subtree
 * whose keys cannot be included in the range sum.
 *
 * Params:
 *   bst - the BST within which to compute a range sum
 *   lower - the lower bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *   upper - the upper bound of the range over which to compute a sum; this
 *     should be considered an *inclusive* bound; in other words a key that's
 *     equal to this bound should be included in the sum
 *
 * Return:
 *   Should return the sum of all keys in `bst` between `lower` and `upper`.
 */
int bst_range_sum(struct bst* bst, int lower, int upper) {
  return range_sum(bst->root, 0, lower, upper);
}

int range_sum(struct bst_node* node, int sum, int lower, int upper) {
  if(node == NULL){
    return sum;
  }
  else{
    if(lower <= node->key && node->key <= upper){
      sum += node->key;
    }
    if(node->left != NULL){
      sum = range_sum(node->left, sum, lower, upper);
    }
    if(node->right != NULL){
      sum = range_sum(node->right, sum, lower, upper);
    }
  }
  return sum;
}