#define NR 27 // the American alphabet(26 letters) plus blank.
typedef char var_type; // the key is a set of characters

typedef struct trie_node{
	bool NotLeaf;
	trie_node *pChildren[NR];
	var_type word[20];
}node;

trie_node *NewIntern();
trie_node *NewLeaf(char word[]);

int main()
{
	return 0;
}