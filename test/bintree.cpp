#include <iostream>
#include <iomanip>

#define swap(a, b) { a^= b; b^=a; a^=b; }

struct node
{
    int val_;
    node* left_;
    node* right_;

    node(int e = 0)
    : val_(e)
    {
        std::cout << "val_ = " << val_ << '\n';
        left_ = NULL;
        right_ = NULL;
    }

    friend bool operator< (node &a, node &b)
    {
        return (a.val_ < b.val_);
    }

    friend bool operator> (node &a, node &b)
    {
        return (a.val_ > b.val_);
    }

    friend std::ostream& operator<< (std::ostream &o, node *nd)
    {
        std::cout << "|";

        if (nd && nd->left_)
            std::cout << nd->left_->val_;

        std::cout << "<-";

        if (nd)
            std::cout << nd->val_;

        std::cout << "->";

        if (nd && nd->right_)
            std::cout << nd->right_->val_;

        std::cout << "|";
        std::cout << '\n';

        return o;
    }
};

class bintree
{
protected:

    node *root_;

public:

    bintree() : root_(NULL)
    {
    }

    bintree(int e)
    {
        root_ = new node(e);
    }

    void crash_all(node* root)
    {
        if (root)
        {
            if (root->left_)
                crash_all(root->left_);

            if (root->right_)
                crash_all(root->right_);

            delete root;
        }
    }

    ~bintree()
    {
        if (root_)
            crash_all(root_);
    }

    void insert(int e, node *root)
    {
        if (!root)
        {
            std::cout << "root == NULL\n";
            root = new node(e);
            std::cout << root->val_ << '\n';
        }
        else
        {
            if (root->val_ < e)
            {
                if (root->right_)
                {
                    insert(e, root->right_);
                }
                else
                {
                    root->right_ = new node(e);
                }
            }
            else if (root->val_ > e)
            {
                if (root->left_)
                {
                    insert(e, root->left_);
                }
                else
                {
                    root->left_ = new node(e);
                }
            }
            else;
        }
    }

    void print_tree(node *root)
    {
        if (root)
        {
            std::cout << root;

            if (root->left_)
            {
                print_tree(root->left_);
            }
            if (root->right_)
            {
                print_tree(root->right_);
            }
        }
    }


    node* find(int e, node* root)
    {
        if (root)
        {
            if (root->val_ > e)
            {
                return find(e, root->left_);
            }
            else if (root->val_ < e)
            {
                return find(e, root->right_);
            }
            else
            {
                return root;
            }
        }
        else
        {
            return NULL;
        }
    }

    node* find(int e)
    {
        return find(e, root_);
    }

    void remove(int e)
    {
        node* nd = find(e, root_);

        if (nd)
        {
            if (nd->right_)
            {
                nd->val_ = nd->right_->val_;
                delete nd->right_;
                nd->right_ = NULL;
                return;
            }

            if (nd->left_)
            {
                nd->val_ = nd->left_->val_;
                delete nd->left_;
                nd->left_ = NULL;
                return;
            }

            std::cout << "LEAF:" << nd << '\n';
            delete nd;
        }
    }

    void insert(int e)
    {
        insert(e, root_);
    }

    void print_tree()
    {
        std::cout << "----------------------------------->\n";
        print_tree(root_);
        std::cout << "----------------------------------||\n";
    }
};


int main(int argc, char* argv[])
{
    bintree b;// = bintree(10);

    b.print_tree();
    b.insert(10);
    b.print_tree();
    b.insert(2);
    b.print_tree();
    b.insert(19);
    b.print_tree();
    b.insert(13);
    b.print_tree();

    std::cout << b.find(19);
    std::cout << b.find(10);
 
    b.insert(11);
    b.print_tree();
    b.insert(4);
    b.print_tree();

    b.insert(25);
    b.print_tree();

    b.remove(19);
    b.print_tree();

    b.remove(2);
    b.print_tree();

    return 0;
}

