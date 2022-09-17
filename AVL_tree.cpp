#include <iostream>
#include <vector>
using namespace std;

inline int maxx(int x,int y){
    if (x>y) return x; else return y;
}

class node{
public:
    int data;
    int height;
    node* left=NULL;
    node* right=NULL;
};

inline int acqheight(node* &ptr){//得到高度
    if (ptr==NULL) {return 0;} else {return ptr->height;}
}
node* LeftRotation(node* root){//左旋
    node* tnode=root->right;//新的根
    root->right=root->right->left;//旋转不发生在小于等于两个节点
    tnode->left=root;

    tnode->left->height=maxx(acqheight(tnode->left->left), acqheight(tnode->left->right))+1;
    tnode->height=maxx(acqheight(tnode->left), acqheight(tnode->right))+1;
    return tnode;
}
node* RightRotation(node* root){//右旋
    node* tnode=root->left;//新的根
    root->left=root->left->right;//旋转不发生在小于等于两个节点
    tnode->right=root;

    tnode->right->height=maxx(acqheight(tnode->right->left), acqheight(tnode->right->right))+1;
    tnode->height=maxx(acqheight(tnode->left), acqheight(tnode->right))+1;
    return tnode;
}
node* balance(node* root){//保证avl树的平衡，高度及时更新
    int leftheight= acqheight(root->left);
    int rightheight= acqheight(root->right);
    if (leftheight>rightheight+1){
        //排除左右节点为空，高度已在旋转中调整
        if (acqheight(root->left->right)>acqheight(root->left->left)){
            root->left=LeftRotation(root->left);//所谓LR型
        }
        return RightRotation(root);
    }
    if (leftheight+1<rightheight){
        if (acqheight(root->right->left)> acqheight(root->right->right)){
            root->right=RightRotation(root->right);//所谓RL型
        }
        return LeftRotation(root);
    }
    return root;
}
void ins(node* &root,const int &n){//插入
    if (root==NULL){
        root=new node;
        root->data=n;
        root->height=1;
        return;
    }//入树
    if (root->data==n){return;}//avl不重复
    if (root->data>n){
        ins(root->left,n);
    } else {
        ins(root->right,n);
    }
    root->height=maxx(acqheight(root->left), acqheight(root->right))+1;//更新root高度
    root=balance(root);//balance中高度保持正确
    return;
}
node* getmax(node* &root){//取出该分支最大值
    if (root->right!=NULL){
        node* tnode=getmax(root->right);
        root->height= maxx(acqheight(root->left), acqheight(root->right))+1;
        root=balance(root);
        return tnode;
    }//递归返回root最右侧节点
    node* tnode=root;
    root=root->left;//右边肯定没有
    tnode->left=NULL;
    return tnode;
}
void del(node* &root,const int &n){//删除某个值
    if (root==NULL){return;}//排除删除失败
    if (root->data==n){
        if (root->left==NULL&&root->right==NULL){delete root;root=NULL;return;}//无子节点
        if (root->right==NULL){//无右子节点
            node* tnode=root;
            root=root->left;
            delete tnode;
        }
        if (root->left==NULL){//无左子节点
            node* tnode=root;
            root=root->right;
            delete tnode;
        }
        if (root->right&&root->left){
            node* tnode=root;
            root=getmax(root->left);//找到左支最大值取代root
            root->left=tnode->left;
            root->right=tnode->right;
            delete tnode;
        }
        root->height=maxx(acqheight(root->left), acqheight(root->right))+1;
        root= balance(root);
        return;
    }
    if (root->data>n){del(root->left,n);}
    if (root->data<n){del(root->right,n);}
    root->height=maxx(acqheight(root->left), acqheight(root->right))+1;
    root=balance(root);
}
void nodeout(node* root){//前根序列输出
    if (root==NULL){return;}
    cout<<(root->data)<<endl;
    nodeout(root->left);
    nodeout(root->right);
}

class tree{
public:
    node* root=NULL;
    void destructor(node* &root){
        if (root==NULL){return;}
        destructor(root->left);
        destructor(root->right);
        delete root;
        root=NULL;
    }
    void treeout(){
        nodeout(root);
    }
    ~tree(){ destructor(root);}
};

tree tree1;
int main(){
    char order[5];
    int n;
    while (cin>>order){
        switch (order[0]) {
            case 'i':{//ins n为输入命令
                cin>>n;
                ins(tree1.root,n);
                break;
            }
            case 'd':{//del n删除
                cin>>n;
                del(tree1.root,n);
                break;
            }
            case 'o':{//out输出
                tree1.treeout();
                break;
            }
            case 'e':{//exit 退出
                return 0;
            }
        }
    }
    return 0;
}