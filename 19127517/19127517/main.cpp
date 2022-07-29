//
//  main.cpp
//  19127517
//
//  Created by jason on 22/07/2022.
//

#include <iostream>
using namespace std;
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

struct Node
{
    int key;
    Node *left;
    Node *right;
    int height;
    
    ~Node(){
        if (left)
            DeleteTree(left);
        
            
    }
};

Node* createNode(int key)
{
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return(node);
}

void LNR(Node* &pRoot)
{
    if(pRoot == NULL)
        return;
    else
    {
        LNR(pRoot->left);
        cout<<pRoot->key<<' ';
        LNR(pRoot->right);
    }
}

void NLR(Node* &pRoot)
{
    if(pRoot == NULL)
        return;
    else
    {
        cout<<pRoot->key<<' ';
        LNR(pRoot->left);
        LNR(pRoot->right);
    }
}

void LRN(Node* &pRoot)
{
    if(pRoot == NULL)
        return;
    else
    {
        LNR(pRoot->left);
        LNR(pRoot->right);
        cout<<pRoot->key<<' ';
    }
}

int height(Node *N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

Node* rightRotate(Node *&y)
{
    Node *x = y->left;
    Node *T2 = x->right;
 
    x->right = y;
    y->left = T2;
 
    // Update heights
    y->height = max(height(y->left),
                    height(y->right)) + 1;
    x->height = max(height(x->left),
                    height(x->right)) + 1;
 
    return x;
}

Node* leftRotate(Node *&x)
{
    Node *y = x->right;
    Node *T2 = y->left;
 
    // Perform rotation
    y->left = x;
    x->right = T2;
 
    // Update heights
    x->height = max(height(x->left),
                    height(x->right)) + 1;
    y->height = max(height(y->left),
                    height(y->right)) + 1;

    return y;
}

int getBalance(Node *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insertAVL(Node*& node, int key)
{
    /* 1. Perform the normal BST insertion */
    if (node == NULL)
    {
        return createNode(key);
    }
 
    if (key < node->key)
        node->left = insertAVL(node->left, key);
    else if (key > node->key)
        node->right = insertAVL(node->right, key);
    else // Equal keys are not allowed in BST
        return node;
 
    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),
                        height(node->right));
 
    /* 3. Get the balance factor of this ancestor
        node to check whether this node became
        unbalanced */
    int balance = getBalance(node);
 
    // If this node becomes unbalanced, then there are 4 cases
 
    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);
 
    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
 
    // Left Right Case
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
 
    // Right Left Case
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node* minValueNode(Node* node)
{
    Node* current = node;
 
    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;
 
    return current;
}

Node* deleteAVL(Node* &root, int key)
{
     
    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
        return root;
 
    // If the key to be deleted is smaller than the root's key, then it lies in left subtree
    if ( key < root->key )
        root->left = deleteAVL(root->left, key);
 
    // If the key to be deleted is greater than the root's key, then it lies in right subtree
    else if( key > root->key )
        root->right = deleteAVL(root->right, key);
 
    // if key is same as root's key, then
    // This is the node to be deleted
    else
    {
        // node with only one child or no child
        if( (root->left == NULL) ||
            (root->right == NULL) )
        {
            Node *temp = root->left ?
                         root->left :
                         root->right;
 
            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
            *root = *temp; // Copy the contents of the non-empty child
            free(temp);
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            Node* temp = minValueNode(root->right);
 
            // Copy the inorder successor's
            // data to this node
            root->key = temp->key;
 
            // Delete the inorder successor
            root->right = deleteAVL(root->right,
                                     temp->key);
        }
    }
 
    // If the tree had only one node
    // then return
    if (root == NULL)
    return root;
 
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(height(root->left),
                           height(root->right));
 
    // STEP 3: GET THE BALANCE FACTOR OF
    // THIS NODE (to check whether this
    // node became unbalanced)
    int balance = getBalance(root);
 
    // If this node becomes unbalanced,
    // then there are 4 cases
 
    // Left Left Case
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
 
    // Left Right Case
    if (balance > 1 && getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
 
    // Right Right Case
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
 
    // Right Left Case
    if (balance < -1 && getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
 
    return root;
}

void DeleteTree(Node* &root)
{
    if (root)
    {
        DeleteTree(root->left);
        DeleteTree(root->right);
        delete root;
    }
}

int Height(Node* pRoot)
{
    if(pRoot==NULL)
        return 0;
    int h1=Height(pRoot->left);
    int h2=Height(pRoot->right);
    return max(h1,h2)+1;
}

Node* CreateTree(int a[], int n){
    Node * root = createNode(a[0]);
    for(int i = 1; i < n; i++){
        insertAVL(root, a[i]);
    }
    return root;
}

bool searchNode(Node* pRoot,int data)
{
    if(pRoot == NULL)
    {
        return false;
    }
    if(pRoot->key == data)
    {
        return true;
    }
    else
    {
        if(pRoot->key > data)
            return searchNode(pRoot->left,data);
        else
            return searchNode(pRoot->right,data);
    }
    return false;
}

bool AVL(Node *root) {
   int leftHeight, rightHeight;
    
   if(root == NULL)
      return 1;
    
   leftHeight = Height(root->left);
   rightHeight = Height(root->right);
    
   if(
      abs(leftHeight-rightHeight) <= 1
      && AVL(root->left)
      && AVL(root->right)
      )
       return 1;
   return 0;
}

void readFile(string filename, vector<vector<int>> &tmp)
{
    string line;
    ifstream in(filename);
    if (!in.is_open())
    {
        cout << "file is not open" << endl;
    }
    else
    {
        int num;
        in >> num;
        in.get();
//        int count;
        for (int i=0; i<num; i++) {
            getline(in, line, '\n');
            string token;
            istringstream iss(line);
            vector<int> row;
//            count=0;
            while (getline(iss, token, ' '))
            {
                row.push_back(stoi(token));
//                count++;
            }
            tmp.push_back(row);
        }
        
//        for (int i=0; i<num; i++) {
//            for (int j=0; j<count; j++) {
//                cout<< tmp[i][j] <<' ';
//            }cout<<endl;
//        }
    }

    in.close();
}

Node* constructTreeUtil(vector<int> pre, int* preIndex, int key,
                        int min, int max, int size)
{
    // Base case
    if (*preIndex >= size)
        return NULL;
 
    Node* root = NULL;
 
    // If current element of pre[] is in range, then
    // only it is part of current subtree
    if (key > min && key < max) {
        // Allocate memory for root of this
        // subtree and increment *preIndex
        root = createNode(key);
        *preIndex = *preIndex + 1;
 
        if (*preIndex < size) {
            // Construct the subtree under root
            // All nodes which are in range
            // {min .. key} will go in left
            // subtree, and first such node
            // will be root of left subtree.
            root->left = constructTreeUtil(pre, preIndex,
                                           pre[*preIndex],
                                           min, key, size);
        }
        if (*preIndex < size) {
            // All nodes which are in range
            // {key..max} will go in right
            // subtree, and first such node
            // will be root of right subtree.
            root->right = constructTreeUtil(pre, preIndex,
                                            pre[*preIndex],
                                            key, max, size);
        }
    }
 
    return root;
}
 
// The main function to construct BST
// from given preorder traversal.
// This function mainly uses constructTreeUtil()
Node* constructTree(vector<int> pre, int size)
{
    int preIndex = 0;
    return constructTreeUtil(pre, &preIndex, pre[0],
                             INT_MIN, INT_MAX, size);
}

int levelNode(Node* root,Node*p){
    if(root==nullptr)
        return -1;
    int count=0;
    bool flag=0;
    while(root){
        if(root->key == p->key){
            flag=1;
            break;
        }
        ++count;
        root= (root->key <p->key)? root->right : root->left;
    }
    return (flag) ? count : -1;
}

void leafTraversal(Node* &tree, Node* &p, vector<int> &levelOfLeavesVector, bool &flag) {
    if(p == NULL)
        return;
    else
    {
        leafTraversal(tree, p->left, levelOfLeavesVector, flag);
        if (p->left == NULL && p->right == NULL) // leaf
        {
            int level = levelNode(tree, p);
            levelOfLeavesVector.push_back(level);
            if (levelOfLeavesVector[0] != level) // every leaves' level must be same with first leaf's one
            {
                flag = false;
                return;
            }
            else
                flag = true;
        }
        leafTraversal(tree, p->right, levelOfLeavesVector, flag);
    }
}

bool checkUtil(struct Node *root,
            int level, int *leafLevel)
{
    // Base case
    if (root == NULL) return true;

    // If a leaf node is encountered
    if (root->left == NULL &&
        root->right == NULL)
    {
        // When a leaf node is found
        // first time
        if (*leafLevel == 0)
        {
            *leafLevel = level; // Set first found leaf's level
            return true;
        }

        // If this is not first leaf node, compare
        // its level with first leaf's level
        return (level == *leafLevel);
    }

    // If this node is not leaf, recursively
    // check left and right subtrees
    return checkUtil(root->left, level + 1, leafLevel) &&
            checkUtil(root->right, level + 1, leafLevel);
}

/* The main function to check
if all leafs are at same level.
It mainly uses checkUtil() */
bool checkLeavesSameDepth(struct Node *root)
{
    int level = 0, leafLevel = 0;
    return checkUtil(root, level, &leafLevel);
}

Node *leastCommonAncestor(Node* root, int n1, int n2)
{
    if (root == NULL)
        return NULL;
 
    // If both n1 and n2 are smaller
    // than root, then LCA lies in left
    if (root->key > n1 && root->key > n2)
        return leastCommonAncestor(root->left, n1, n2);
 
    // If both n1 and n2 are greater than
    // root, then LCA lies in right
    if (root->key < n1 && root->key < n2)
        return leastCommonAncestor(root->right, n1, n2);
 
    return root;
}

int main() {
//7.1
    Node* tree;
    int a[] = {15, 2, 20, 1, 3, 16, 31, 68, 40, 75};
    int n =  sizeof(a)/sizeof(a[0]);
    //CREATE TREE && INSERT NODE && ROTATE
    tree = CreateTree(a, n);
    //TRAVERSAL
    cout<< "LNR\n";
    LNR(tree);
    cout<< "\nNLR\n";
    NLR(tree);
    cout<< "\nLRN\n";
    LRN(tree);
    //DELETE
    deleteAVL(tree, 20);
    //HEIGHT
    cout<< "\nHeight:\n" << Height(tree)<<" vs " << tree->height<<endl;
    //Identify whether a given value is exist in tree
    cout<< "Checking...\n";
    bool check = searchNode(tree, 31);
    (check)
        ?cout<< "\tExists\n"
        :cout<< "\tDoes not exist\n";
    
    DeleteTree(tree);
//7.2
    vector<vector<int>> tmp;
    readFile("/Users/jason/Downloads/19127517/19127517/input.txt", tmp);

    int size0 = sizeof(tmp[0]) / sizeof(tmp[0][0]);
    int size1 = sizeof(tmp[1]) / sizeof(tmp[1][0]);
    int size2 = sizeof(tmp[2]) / sizeof(tmp[2][0]);
    
    Node* root0 = constructTree(tmp[0], size0);
    Node* root1 = constructTree(tmp[1], size1);
    Node* root2 = constructTree(tmp[2], size2);

    fstream out("/Users/jason/Downloads/19127517/19127517/output.txt");
    bool checkAVLroot = AVL(root0);
    (checkAVLroot)
        ?out<< "yes\n"
        :out<< "no\n";
    checkAVLroot = AVL(root1);
    (checkAVLroot)
        ?out<< "yes\n"
        :out<< "no\n";
    checkAVLroot = AVL(root2);
    (checkAVLroot)
        ?out<< "yes\n"
        :out<< "no\n";
    out.close();

    DeleteTree(root0);
    DeleteTree(root1);
    DeleteTree(root2);

//7.3
    struct Node *root3 = createNode(12); root3->left = createNode(5); root3->right = createNode(13);
    vector<int> leavesLevel;
//    (checkLeavesSameDepth(tree))
    bool flag;
    leafTraversal(root3, root3, leavesLevel, flag);
    (flag)
        ?cout << "[root3] All leaves of a AVL tree are the same depth\n"
        :cout << "[root3] All leaves of a AVL tree are NOT the same depth\n";
    
    DeleteTree(root3);

    struct Node *root = createNode(12); root->left = createNode(5); root->right = createNode(13); root->left->left = createNode(1);
    vector<int> leavesLevel2;
//    (checkLeavesSameDepth(root))
    bool flag2;
    leafTraversal(root, root, leavesLevel2, flag2);
    (flag2)
        ?cout << "[root] All leaves of a AVL tree are the same depth\n"
        :cout << "[root] All leaves of a AVL tree are NOT the same depth\n";
    
    DeleteTree(root);

//7.4
    Node* tree4;
    int a4[] = {20, 8, 22, 4, 12, 10, 14};
    int n4 =  sizeof(a4)/sizeof(a4[0]);
    tree4 = CreateTree(a4, n4);
    
    int firstNum = 10, secondNum = 22;
    NLR(tree4);
    cout<<"\nLeast common ancestor " << firstNum << " and " << secondNum << " is " << leastCommonAncestor(tree4, firstNum, secondNum)->key << endl;
    
    DeleteTree(tree4);
    return 0;
}
