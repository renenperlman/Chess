struct listNode{
	void *data;
	struct listNode *next;
	size_t size;
};

struct linkedList{
	struct listNode *first;
};

struct pos{
	char x;
	int y;
};

struct piece{
	char type;
	int color; // 0 - white, 1 - black
	char moved; // 0 - hasn't moved yet, 1 - moved
	struct pos position;
	(void) genMove(struct piece*, struct linkedList*);
};

struct move{
	struct piece *p;
	struct pos dest;
};

