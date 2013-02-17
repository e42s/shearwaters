#include <string>
#include <map>
#include <fstream>
#include <iostream>
using namespace std;

class TreeNode
{
public:
    string name;
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
    TreeNode(const string& nm) : name(nm), left(NULL), right(NULL){}
};

TreeNode* buildTree(const char* fileName)
{
    TreeNode* root = new TreeNode("root");
    map<string, TreeNode*> classLib;
    ifstream ifs(fileName);
    TreeNode* pLast = root;
    while (ifs.good())
    {
        char buf[1024] = {0};
        if(ifs.getline(buf, sizeof(buf)))
        {
            char * p = buf;
            while(*++p != ',');
            *p = '\0';
            ++p;
            TreeNode* pParent = NULL;
            TreeNode* pChild = NULL;
            if(classLib.count(buf))
                pParent = classLib[buf];
            else
            {
                pParent = new TreeNode(buf);
                classLib[buf] = pParent;
                //whenever a new parent was created, add it as the far right descendant of root
                if(pLast == root)
                    pLast->left = pParent;
                else
                    pLast->right = pParent;
                pParent->parent = pLast;
                pLast = pParent;
            }
            if(classLib.count(p))
            {
                pChild = classLib[p];
                //since I'm going to have the right parent, so remove me from the original position
                TreeNode* p = pChild->parent;
                if(p->right == pChild)
                    p->right = pChild->right;
                else
                    p->left = pChild->right;
                if(pChild->right)
                {
                    pChild->right->parent = p;
                    pChild->right = NULL;
                }
            }
            else
            {
                pChild = new TreeNode(p);
                classLib[p] = pChild;
            }

            if(pParent->left == NULL)
            {
                //if I'm the first child of pParent, then I'm the left.
                pParent->left = pChild;
                pChild->parent = pParent;
            }
            else
            {
                TreeNode* p = pParent->left;
                while(p && p->right) p = p->right;
                p->right = pChild;
                pChild->parent = p;
            }
        }
    } 
    return root;
}
void print(TreeNode* tree,int level)
{
    if(tree == NULL)
    {
        return;
    }
    for(int i=0;i<level;i++)
        cout << "    ";
    cout << tree->name <<endl;
    print(tree->left,level+1);
    print(tree->right,level);
}
static char* helpMsg = "Usage:\n"
    "\t\tclasstree <in_file>\n"
    "\t\tThe file content shall be as follows:\n"
    "\t\tclassA,classB\n"
    "\t\tclassB,classC\n"
    "\t\tclassB,classD\n"
    "\t\tclassA,classE\n"
    "Example:\n"
    "\t\t$ ./classtree2.exe kjs.txt\n"
    "\t\troot\n"
    "\t\t    classA\n"
    "\t\t        classB\n"
    "\t\t            classC\n"
    "\t\t            classD\n"
    "\t\t        classE\n";
int main(int argc,char **argv)
{
    if(argc < 2)
    {
        printf(helpMsg);
        exit(0);
    }
    TreeNode* pTree = buildTree(argv[1]);
    print(pTree,0);
    return 0;
}
