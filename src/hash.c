/************************************************
 * Hashtable with string key
 ************************************************/

#include "utils.h"

static rb_node *hashTable[HASH_TABLE_SIZE];

static unsigned hash(char *key)
{
	unsigned h;
	h = 0;

	for (; *key != '\0'; ++key) {
		h <<= 1;
		h += *key;
		h %= HASH_TABLE_SIZE;
	}

	return h;
}

static direction what_child_am_i(rb_node *node)
{
	if (node->parent == 0)
		return Root;
	else if (node->parent->children[Left] == node)
		return Left;
	else
		return Right;
}

static void rotate(direction dir, rb_node **bucket, rb_node *axis)
{
	rb_node *rotated; /* the node to be rotated about the axis */
	rotated = axis->children[1 - dir]; /* must be non-null */
	/* rotation affects four nodes                             */
	/* the axis, the rotated node,                             */
	/* the child of the rotated node in the rotated direction, */
	/* and the parent of the axis                              */

	/* connect the axis with the child of the rotated node */
	axis->children[1 - dir] = rotated->children[dir];
	if (rotated->children[dir] != 0)
		rotated->children[dir]->parent = axis;

	/* connect the parent of the axis with the rotated node */
	rotated->parent = axis->parent;
	switch (what_child_am_i(axis)) {
	case Root:
		*bucket = rotated;
		break;
	case Left:
		axis->parent->children[Left] = rotated;
		break;
	case Right:
		axis->parent->children[Right] = rotated;
		break;
	}

	/* connect the axis node with the rotated node */
	rotated->children[dir] = axis;
	axis->parent = rotated;
	return;
}

/* finds some value matching the key */
static rb_node *_find(rb_node *root, char *key)
{
	rb_node *found;
	int cmp;
	found = 0;

	while (root != 0) {
		if ((cmp = strcmp(key, root->key)) < 0)
			root = root->children[Left];
		else if (cmp > 0)
			root = root->children[Right];
		else {
			found = root;
			break;
		}
	}

	return found;
}

/* called by a non-root node */
static void balance(rb_node **bucket, rb_node *node)
{
	rb_node *this_side;
	rb_node *other_side;
	direction dir;

	this_side = node->parent; /* invariant */

	/* invariant : this_side is not root */
	while (this_side != 0 && this_side->color == Red) {
		dir = what_child_am_i(this_side);
		other_side = this_side->parent->children[1 - dir];

		if (other_side != 0 && other_side->color == Red) {
			this_side->color = Black;
			other_side->color = Black;
			this_side->parent->color = Red;

			node = this_side->parent;
			this_side = node->parent;
		} else {
			if (what_child_am_i(node) != dir) {
				node = this_side;
				rotate(dir, bucket, node);
        this_side = node->parent;
			}
			this_side->color = Black;
			this_side->parent->color = Red;
			rotate(1 - dir, bucket, this_side->parent);
		}
	}

	(*bucket)->color = Black;
	return;
}

static rb_node *_insert(rb_node **bucket, char *key, void *value)
{
	rb_node *node, *root, *prev;
	direction dir;

	node = malloc(sizeof(rb_node));
	if (!node)
		failwith(strerror(errno));
	node->value = value;
	node->key = key;
	node->children[Left] = node->children[Right] = 0;
	node->color = Red;

	root = *bucket;
	prev = 0;

	while (root != 0) {
		prev = root;
		if (strcmp(key, root->key) < 0)
			dir = Left;
		else
			dir = Right; /* multiple nodes with same key might be entered */

		root = root->children[dir];
	}

	node->parent = prev;
	if (prev == 0) {
		*bucket = node;
		node->color = Black;
		return node;
	} else
		prev->children[dir] = node;

	balance(bucket, node);
	return node;
}

void *delete (rb_node *node)
{
	return 0;
}

rb_node *find (char *key)
{
	return _find(*(hashTable + hash(key)), key);
}

rb_node *insert(char *key, void *value)
{
	rb_node **bucket;
	rb_node *ret;
	char *temp_name;
	unsigned h;

	h = hash(key);
	bucket = hashTable + h;

	temp_name = strdup(key);
	return _insert(bucket, temp_name, value);
}

/********************* for debugging purposes *********************/

static void print_rbtree(rb_node *root, int depth)
{
	int i;

	if (!root)
		return;

	if (root->color == Red && root->parent->color == Red) {
		printf("rbtree violated\n");
		exit(1);
	}

	for (i = 0; i < depth; ++i)
		printf("\t");
	printf("%s[%d]\n", root->key, root->color);

	print_rbtree(root->children[Left], depth + 1);
	print_rbtree(root->children[Right], depth + 1);
	return;
}

void print_table(void)
{
	int i;
	for (i = 0; i < HASH_TABLE_SIZE; ++i) {
		printf("Bucket number %d\n", i);
		print_rbtree(hashTable[i], 0);
	}
	return;
}
