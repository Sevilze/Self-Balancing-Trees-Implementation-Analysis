#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <sys/time.h>

typedef struct AVLNode{
    int key;
    struct AVLNode *left, *right;
    int height;
} AVLNode;

// function prototyping avl
int getHeight(AVLNode *node);
int max(int a, int b);
int getBalance(AVLNode *node);
AVLNode *minAVLNode(AVLNode *root);
AVLNode *createAVLNode(int key);
AVLNode *avlRotateLL(AVLNode *node);
AVLNode *avlRotateRR(AVLNode *node);
AVLNode *avlRotateLR(AVLNode *node);
AVLNode *avlRotateRL(AVLNode *node);
AVLNode *avlInsert(AVLNode *node, int key);
AVLNode *avlSearch(AVLNode *root, int key);
AVLNode *avlUpdate(AVLNode *root, int key, int newKey);
AVLNode *avlDelete(AVLNode *root, int key);
void avlPreorder(AVLNode *root) {
    if (root == NULL) return;

    avlPreorder(root->left);
    printf("%d ",root->key);
    avlPreorder(root->right);
}

typedef struct RBNode{
    int key;
    char color;
    struct RBNode *left, *right, *parent;
} RBNode;

// function prototyping red black tree
RBNode *createRBNode(int key);
RBNode *rbHelper(RBNode *root, RBNode *temp);
RBNode *rbSuccessor(RBNode *node);
RBNode *rbSibling(RBNode *node);
void rbRotateLL(RBNode **root, RBNode *buffer);
void rbRotateRR(RBNode **root, RBNode *buffer);
void rbFixRR(RBNode **root, RBNode *buffer);
void rbTransplant(RBNode **root, RBNode *subt, RBNode *newSubt);
void rbFixDB(RBNode **root, RBNode* node);
RBNode *rbSuccessor(RBNode *node);
RBNode *rbReplace(RBNode *node);
void swapValues(RBNode *nodeA, RBNode *nodeB);
void rbDelHelper(RBNode **root, RBNode *node);
RBNode *rbInsert(RBNode **root, int key);
RBNode *rbSearch(RBNode *root, int key);
void rbUpdate(RBNode **root, int key, int newKey);
void rbDelete(RBNode **root, int key);

void rbPreorder(RBNode *root){
    if (root == NULL) return;
	
	printf("%d(%c) ", root->key, root->color);
    rbPreorder(root->left);
    rbPreorder(root->right);
}

const int deg = 3; // minimum degree atau minimum children -> 2t = 6 (b-tree order 6)
typedef struct BTNode{
    int *keys;
    int degree;
    struct BTNode **children;
    int stat;
    bool leaf;
} BTNode;

// function prototyping b-tree
BTNode* createBTNode(int degree, bool leaf);
void btTraverse(BTNode *root);
void splitChild(BTNode *parent, int t, int childIndex, BTNode *child);
void insertNonFull(BTNode *node,int t, int key);
void removeLeaf(BTNode *node, int idx);
void removeNonLeaf(BTNode *node, int t, int idx);
int getPred(BTNode *node, int idx);
int getSucc(BTNode *node, int idx);
void borrowPrev(BTNode *node, int idx);
void borrowNext(BTNode *node, int idx);
void merge(BTNode *node, int t, int idx);
void fill(BTNode *node, int t, int idx);
void btDelHelper(BTNode *root, int t, int key);
void btInsert(BTNode **root, int t, int key);
BTNode* btSearch(BTNode *root, int key);
void btUpdate(BTNode **root, int t, int key, int newKey);
void btDelete(BTNode **root, int t, int key);
void btPreorder(BTNode *root){
	int i;
    if(root == NULL) return;

    for(i = 0; i < root->stat; i++){
        printf("%d ", root->keys[i]);
    }

    if(!root->leaf){
        for(i = 0; i <= root->stat; i++){
            btPreorder(root->children[i]);
        }
    }
}


#define MIN_ID 10000
#define MAX_ID 99999
const int range = MAX_ID - MIN_ID + 1;

// function untuk mengacak array dari data yang digenerate
void shuffleArray(int *array, int size){
	int i,j;
    srand(time(NULL));
    for(i = size - 1; i > 0; i--){
        j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// function untuk menampilkan data yang digenerate
void printUserIDs(int *array, int numIDs){
	int i;
    int elementsPerLine = 15;
    for(i = 0; i < numIDs; i++) {
        printf("%6d ", array[i]);
        if((i + 1) % elementsPerLine == 0) printf("\n");
    }
    if(numIDs % elementsPerLine != 0) printf("\n");
}

// function untuk generate data (user id)
void generateRandomUserIDs(int *array, int numIDs){
	int i;
    int range = MAX_ID - MIN_ID + 1;
    if(numIDs > range){
        printf("Number of user IDs exceeds the range of available unique IDs.\n");
        return;
    }

    if(array == NULL){
        printf("Memory allocation failed.\n");
        return;
    }
    for(i = 0; i < range; i++){
        array[i] = MIN_ID + i;
    }

    shuffleArray(array, range);
    printUserIDs(array, numIDs);
}

// function untuk melakukan operasi insert pada self balancing trees dan analisis waktu
void insertTrees(int *userIDs, int numIDs, AVLNode **avlRoot, RBNode **rbRoot, BTNode **btRoot, int deg){
	int i;
    struct timespec startAVL, endAVL, startRB, endRB, startBT, endBT;
	
	/* perhitungan waktu menggunakan metode monotonic clock dimana perhitungan waktu
	   tidak terdampak oleh perubahan yang terjadi pada sistem. */
    clock_gettime(CLOCK_MONOTONIC, &startAVL);
    for(i = 0; i < numIDs; i++) {
        *avlRoot = avlInsert(*avlRoot, userIDs[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &endAVL);
    double timeAVL = (endAVL.tv_sec - startAVL.tv_sec) + (endAVL.tv_nsec - startAVL.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &startRB);
    for(i = 0; i < numIDs; i++) {
        rbInsert(rbRoot, userIDs[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &endRB);
    double timeRB = (endRB.tv_sec - startRB.tv_sec) + (endRB.tv_nsec - startRB.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &startBT);
    for(i = 0; i < numIDs; i++) {
        btInsert(btRoot, deg, userIDs[i]);
    }
    clock_gettime(CLOCK_MONOTONIC, &endBT);
    double timeBT = (endBT.tv_sec - startBT.tv_sec) + (endBT.tv_nsec - startBT.tv_nsec) / 1e9;
	
	printf("\nInsertion Analysis:\n\n");
    printf("AVL tree insertion: %.8f seconds\n", timeAVL);
    printf("Red-Black tree insertion: %.8f seconds\n", timeRB);
    printf("B-tree insertion: %.8f seconds\n", timeBT);
}

// function untuk generate random index
int getRandom(int min, int max) {
    return rand() % (max - min);
}

// function untuk generate random key
int getKey(int min, int max) {
    return (rand() % (max - min)) + min;
}

// function untuk membuat array menjadi compact setelah melakukan deletion dan dapat melakukan deletion lagi
void compactArray(int *userIDs, int *numIDs){
	int readIndex = 0;
    int writeIndex = 0;
    for (readIndex = 0; readIndex < *numIDs; readIndex++){
        if(userIDs[readIndex] != -1) userIDs[writeIndex++] = userIDs[readIndex];
    }
}

// function untuk melakukan searching pada self balancing trees dan analisis waktu
void searchTrees(int *array, int numIDs, AVLNode *avlRoot, RBNode *rbRoot, BTNode *btRoot){
	int i;
	if (numIDs == 0){
        printf("Trees are empty. No keys to search.\n");
        return;
    }
	
    int numSearches;
    printf("Enter the number of keys to search for: ");
    scanf("%d", &numSearches);
    
    if(numSearches > numIDs){
        printf("Number of searches exceeds the number of keys.\n");
        return;
    }

    struct timespec start, end;
	
    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i = 0; i < numSearches; i++){
        int key = array[getRandom(0, numIDs - 1)];
        avlSearch(avlRoot, key);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double avlTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i = 0; i < numSearches; i++){
        int key = array[getRandom(0, numIDs - 1)];
        rbSearch(rbRoot, key);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double rbTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &start);
    for(i = 0; i < numSearches; i++){
        int key = array[getRandom(0, numIDs - 1)];
        btSearch(btRoot, key);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double btTime = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nSearch Analysis:\n\n");
    printf("AVL Tree search time: %.8f seconds\n", avlTime);
    printf("RB Tree search time: %.8f seconds\n", rbTime);
    printf("BT Tree search time: %.8f seconds\n", btTime);
}

// function untuk melakukan update data pada self balancing trees dan analisis waktu
void updateTrees(int *userIDs, int numIDs, AVLNode **avlRoot, RBNode *rbRoot, BTNode **btRoot, int deg){
	int i;
    if(numIDs == 0){
        printf("Trees are empty. No keys to update.\n");
        return;
    }

    int numUpdates;
    printf("Enter the number of keys to update: ");
    scanf("%d", &numUpdates);

    if(numUpdates >= numIDs){
        printf("Number of updates exceeds the number of keys.\n");
        return;
    }

    int *indices = malloc(numUpdates * sizeof(int));
	int *newKeys = malloc(numUpdates * sizeof(int));
	
	bool *chosen = calloc(numIDs, sizeof(bool));
	
	// key generation
	for(i = 0; i < numUpdates; i++){
	    do{
	        indices[i] = getRandom(0, numIDs - 1);
	    } while(chosen[indices[i]]);
	
	    chosen[indices[i]] = true;
	
	    int oldKey = userIDs[indices[i]];
	    int newKey;
	    do{
	        newKey = getKey(MIN_ID, MAX_ID);
	    } while (avlSearch(*avlRoot, newKey) != NULL || oldKey == newKey);
	    newKeys[i] = newKey;
	}
	free(chosen);
	
    struct timespec startAVL, endAVL, startRB, endRB, startBT, endBT;

    clock_gettime(CLOCK_MONOTONIC, &startAVL);
    for (i = 0; i < numUpdates; i++){
        int oldKey = userIDs[indices[i]];
        int newKey = newKeys[i];
        *avlRoot = avlDelete(*avlRoot, oldKey);
        *avlRoot = avlInsert(*avlRoot, newKey);
    }
    clock_gettime(CLOCK_MONOTONIC, &endAVL);
    double timeAVL = (endAVL.tv_sec - startAVL.tv_sec) + (endAVL.tv_nsec - startAVL.tv_nsec) / 1e9;

   
    clock_gettime(CLOCK_MONOTONIC, &startRB);
    for (i = 0; i < numUpdates; i++){
        int oldKey = userIDs[indices[i]];
        int newKey = newKeys[i];
        rbUpdate(&rbRoot,oldKey,newKey);
    }
    clock_gettime(CLOCK_MONOTONIC, &endRB);
    double timeRB = (endRB.tv_sec - startRB.tv_sec) + (endRB.tv_nsec - startRB.tv_nsec) / 1e9;


    clock_gettime(CLOCK_MONOTONIC, &startBT);
    for (i = 0; i < numUpdates; i++){
        int oldKey = userIDs[indices[i]];
        int newKey = newKeys[i];
        btDelete(btRoot, deg, oldKey);
    	btInsert(btRoot, deg, newKey);
    }
    clock_gettime(CLOCK_MONOTONIC, &endBT);
    double timeBT = (endBT.tv_sec - startBT.tv_sec) + (endBT.tv_nsec - startBT.tv_nsec) / 1e9;
	
	
	for(i = 0; i < numUpdates; i++){
		userIDs[indices[i]] = newKeys[i];
	}
	
    free(indices);
    free(newKeys);

    printf("\nUpdate Analysis:\n\n");
    printf("AVL Tree update time: %.8f seconds\n", timeAVL);
    printf("Red-Black Tree update time: %.8f seconds\n", timeRB);
    printf("B-Tree update time: %.8f seconds\n", timeBT);
}

// function untuk melakukan deletion pada self balancing trees dan analisis waktu
void deleteTrees(int *userIDs, int *numIDs, AVLNode **avlRoot, RBNode *rbRoot, BTNode **btRoot, int deg){
	int i;
    if(*numIDs == 0){
        printf("Trees are empty. No keys to delete.\n");
        return;
    }

    int numDeletions;
    printf("Enter the number of keys to delete: ");
    scanf("%d", &numDeletions);

    if(numDeletions > *numIDs){
        printf("Number of deletions exceeds the number of keys.\n");
		return;
    }

    int *indices = malloc(numDeletions * sizeof(int));
	bool *chosen = calloc(*numIDs, sizeof(bool));
	
	// index generation, data yang didelete harus ada dalam tree/array
	for(i = 0; i < numDeletions; i++) {
    	do{
    	    indices[i] = getRandom(0, *numIDs);
    	} while (chosen[indices[i]] || userIDs[indices[i]] == 0);

    	chosen[indices[i]] = true;
	}
	free(chosen);

    struct timespec startAVL, endAVL, startRB, endRB, startBT, endBT;

    clock_gettime(CLOCK_MONOTONIC, &startAVL);
    for(i = 0; i < numDeletions; i++){
        *avlRoot = avlDelete(*avlRoot, userIDs[indices[i]]);
    }
    clock_gettime(CLOCK_MONOTONIC, &endAVL);
    double timeAVL = (endAVL.tv_sec - startAVL.tv_sec) + (endAVL.tv_nsec - startAVL.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &startRB);
    for(i = 0; i < numDeletions; i++){
        rbDelete(&rbRoot, userIDs[indices[i]]);
    }
    clock_gettime(CLOCK_MONOTONIC, &endRB);
    double timeRB = (endRB.tv_sec - startRB.tv_sec) + (endRB.tv_nsec - startRB.tv_nsec) / 1e9;

    clock_gettime(CLOCK_MONOTONIC, &startBT);
    for(i = 0; i < numDeletions; i++){
        btDelete(btRoot, deg, userIDs[indices[i]]);
    }
    clock_gettime(CLOCK_MONOTONIC, &endBT);
    double timeBT = (endBT.tv_sec - startBT.tv_sec) + (endBT.tv_nsec - startBT.tv_nsec) / 1e9;
	
	// compacting array, mengubah jumlah data yang ada setelah deletion
	for(i = 0; i < numDeletions; i++){
        userIDs[indices[i]] = -1;
    }
	*numIDs -= numDeletions;
	compactArray(userIDs,numIDs);

    free(indices);

    printf("\nDeletion Analysis:\n\n");
    printf("AVL Tree deletion time: %.8f seconds\n", timeAVL);
    printf("Red-Black Tree deletion time: %.8f seconds\n", timeRB);
    printf("B-Tree deletion time: %.8f seconds\n", timeBT);

    if(*numIDs == 0) printf("All elements have been deleted. Trees are now empty.\n");
}

/* konsep program ini adalah menggunakan kasus dunia nyata yaitu mengelola sosial media
   dan user id yang digenerate merupakan kumpulan-kumpulan akun yang dicurigai merupakan
   bot atau melakukan pelanggaran. Sistem ini dapat melakukan operasi CRUD pada dataset
   yang ditentukan yaitu jumlah id user.
*/
void window(int numIDs){
	printf("Current datapoints: %d\n\n",numIDs);
	printf("----------User Management & Analysis----------\n\n");
	printf("\t1. Perform insertion operation.\n");
	printf("\t2. Perform searching operation.\n");
	printf("\t3. Perform update operation.\n");
	printf("\t4. Perform deletion operation.\n");
	printf("\t5. Regenerate data.\n");
	printf("\t6. Exit program.\n\n");
	printf("----------------------------------------------\n");
	printf("Select a command: ");
}

int main(){
	int i;
	int choice = 1;
	int numIDs = 0;
	int range = MAX_ID - MIN_ID + 1;
	int *userIDs = (int *)malloc(range * sizeof(int));
	char buffer[100];
	AVLNode *avlRoot = NULL;
	RBNode *rbRoot = NULL;
	BTNode *btRoot = NULL;
	bool insertionPerformed = false;
	
    if(userIDs == NULL){
    	printf("Memory allocation failed.\n");
        return 1;
    }
    
    while(1){
        printf("Welcome to the Admin Program.\n");
        printf("Enter the number of users to check: ");
        // input validation
        if (fgets(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &numIDs) != 1 || numIDs <= 0){
            printf("\nInvalid input.\nPlease enter a positive integer.\n");
            sleep(1);
            system("cls");
        }
        else if(numIDs > 10000){
        	printf("\nThe amount of data is beyond the scope of this program.\nPlease try again.");
        	sleep(1);
        	system("cls");
		}
		else{
			generateRandomUserIDs(userIDs, numIDs);
			printf("\nSuccessfully generated the data.\n");
			printf("Initializing the menu.");
			sleep(2);
			system("cls");
			break;
		}
    }
    
	while(1){
		window(numIDs);
		scanf("%d",&choice);
		switch(choice){
            case 1:
            	if(insertionPerformed){
            		printf("\nYou've already performed insertion.\nSelect other commands or regenerate data.\n");
            		sleep(2);
            		system("cls");
            		break;
				}
                printf("\nSuccessfully inserted the data.\n");
           		insertTrees(userIDs, numIDs, &avlRoot, &rbRoot, &btRoot, deg);				
				insertionPerformed = true;
                sleep(4);
                system("cls");
                break;
                
            case 2:
            	if(!insertionPerformed){
                    printf("You must perform insertion first.\n");
                    sleep(2);
                    system("cls");
                    break;
                }
                printf("\n");
                searchTrees(userIDs, numIDs, avlRoot, rbRoot, btRoot);
                sleep(4);
                system("cls");
                break;
                
            case 3:
            	if(!insertionPerformed){
                    printf("You must perform insertion first.\n");
                    sleep(2);
                    system("cls");
                    break;
                }
                printf("\n");
	            updateTrees(userIDs, numIDs, &avlRoot, rbRoot, &btRoot, deg);
	            sleep(4);
	            system("cls");
	            break;
	            
	        case 4:
	        	if(!insertionPerformed){
                    printf("You must perform insertion first.\n");
                    sleep(2);
                    system("cls");
                    break;
                }
                printf("\n");
	        	deleteTrees(userIDs, &numIDs, &avlRoot, rbRoot, &btRoot, deg);
	        	sleep(4);
	        	system("cls");
	        	break;
	        
	        // opsi untuk melakukan data generation lagi
	        case 5:
	        	free(userIDs);
	        	userIDs= NULL;
	        	numIDs = 1;
	        	userIDs = (int *)malloc(range * sizeof(int));
	        	if (userIDs == NULL){
        			printf("Memory allocation failed.\n");
        			return 1;
    			}
    			
    			int c;
        		while ((c = getchar()) != '\n' && c != EOF);
    			
    			while(1){
        			printf("Enter the number of users to check: ");
        			fgets(buffer, sizeof(buffer), stdin);
        			if (sscanf(buffer, "%d", &numIDs) != 1 || numIDs <= 0) {
            			printf("\nInvalid input. Please enter a positive integer.\n");
            			sleep(1);
            			system("cls");
        			}
					else if(numIDs > 10000){
            			printf("\nThe amount of data is beyond the scope of this program.\nPlease try again.");
            			sleep(1);
            			system("cls");
        			}
					else{
            			generateRandomUserIDs(userIDs, numIDs);
            			printf("\nSuccessfully generated the data.\n");
            			printf("Initializing the menu.");
            			sleep(2);
            			system("cls");
            			insertionPerformed = false;
            			break;
        			}	
    			}
    			break;
    			
	        case 6:
	        	printf("\nThank you for your work today.\n");
	        	sleep(1);
	        	printf("Exiting the program.\n");
	        	sleep(1);
	        	system("cls");
	        	exit(1);
	        	break;
	        	
	        default:
	        	printf("Invalid command.\nPlease try again.\n");
	        	sleep(1);
	        	system("cls");
	        	break;
		}
    }
    
	return 0;
}

// function untuk mendapatkan nilai tinggi dari suatu node relatif terhadap avl tree
int getHeight(AVLNode *node){
	if(node == NULL) return 0;
	
	return node->height;
}

// function untuk mencari nilai maksimum antara dua integer
int max(int a, int b){
	return(a > b) ? a : b;
}

// function untuk mendapatkan nilai balance factor untuk node
int getBalance(AVLNode *node){
	if(node == NULL) return 0;
	
	return getHeight(node->left) - getHeight(node->right);
}

// mencari node successor terdekat dari node yang didelete
AVLNode *minAVLNode(AVLNode *root){
	AVLNode *temp = root;
	while(temp->left != NULL) temp = temp->left;
	
	return temp;
}

// function untuk membuat node baru dengan mengalokasikan memori dan memasukkan nilai key
AVLNode *createAVLNode(int key){
	AVLNode *newNode = (AVLNode *) malloc(sizeof(AVLNode));
	newNode->key = key;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->height = 1;
	return newNode;
}

// right rotate untuk left left case
AVLNode *avlRotateLL(AVLNode *node){
	AVLNode *leftChild = node->left;
	AVLNode *leftRightChild = leftChild->right;
	
	leftChild->right = node;
	node->left = leftRightChild;
	
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	leftChild->height = max(getHeight(leftChild->left), getHeight(leftChild->right)) + 1;
	
	return leftChild;
}

// left rotate untuk right right case
AVLNode *avlRotateRR(AVLNode *node){
	AVLNode *rightChild = node->right;
	AVLNode *rightLeftChild = rightChild->left;
	
	rightChild->left = node;
	node->right = rightLeftChild;
	
	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	rightChild->height = max(getHeight(rightChild->left), getHeight(rightChild->right)) + 1;
	
	return rightChild;
}

// left right case rotation
AVLNode *avlRotateLR(AVLNode *node){
	node->left = avlRotateRR(node->left);
	return avlRotateLL(node);
}

// right left case rotation
AVLNode *avlRotateRL(AVLNode *node){
	node->right = avlRotateLL(node->right);
	return avlRotateRR(node);
}

// function untuk melakukan insertion pada avl tree dan melakukan balancing
AVLNode *avlInsert(AVLNode *node, int key){
	if(node == NULL) return createAVLNode(key);
	
	if(key < node->key) node->left = avlInsert(node->left, key);
	else if(key > node->key) node->right = avlInsert(node->right,key);
	else return node;
	
	node->height = 1 + max(getHeight(node->left), getHeight(node->right));
	
	int balance = getBalance(node);
	
	if(balance > 1 && key < node->left->key) return avlRotateLL(node);
	if(balance < -1 && key > node->right->key) return avlRotateRR(node);
	if(balance > 1 && key > node->left->key) return avlRotateLR(node);
	if(balance < -1 && key < node->right->key) return avlRotateRL(node);
	
	return node;
}

// function untuk melakukan searching pada avl tree secara rekursif
AVLNode *avlSearch(AVLNode *root, int key){
    if (root == NULL || root->key == key) return root;
    
    if (root->key < key) return avlSearch(root->right, key);
    else return avlSearch(root->left, key);
}

// function untuk melakukan update data pada avl tree dengan delete -> insert
AVLNode *avlUpdate(AVLNode *root, int key, int newKey){
    avlDelete(root, key);
    avlInsert(root, newKey);
    return root;
}

// function untuk melakukan deletion pada avl tree
AVLNode *avlDelete(AVLNode *root, int key){
	if(root == NULL) return root;
	
	if(key < root->key) root->left = avlDelete(root->left, key);
	else if(key > root->key) root->right = avlDelete(root->right, key);
	else{
		if((root->left == NULL) || root->right == NULL){
			AVLNode *temp = root->left ? root->left : root->right;
			// case no children
			if(temp == NULL){
				temp = root;
				root = NULL;
			}
			// case 1 child
			else{
				*root = *temp;
			}
			free(temp);
		}
		// case 2 children, cari successor node dan recursive call
		else{
			AVLNode *temp = minAVLNode(root->right);
			root->key = temp->key;
			root->right = avlDelete(root->right, temp->key);
		}
	}
	
	if(root == NULL) return root;
	root->height = 1 + max(getHeight(root->left), getHeight(root->right));
	
	int balance = getBalance(root);
	
	// balancing tree
	if(balance > 1 && getBalance(root->left) >= 0) return avlRotateLL(root);
	if(balance > 1 && getBalance(root->left) < 0) return avlRotateLR(root);
	if(balance < -1 && getBalance(root->left) <= 0) return avlRotateRR(root);
	if(balance < -1 && getBalance(root->left) > 0) return avlRotateRL(root);
	
	return root;
}

// function untuk membuat node baru pada red black tree dan warnanya merah
RBNode *createRBNode(int key){
    RBNode *newNode = (RBNode*)malloc(sizeof(RBNode));
    newNode->key = key;
    newNode->color = 'R';
    newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
    return newNode;
}

// helper function untuk melakukan insertion secara rekursif
RBNode *rbHelper(RBNode *node, RBNode *temp){
    if(node == NULL) return temp;
 
    if(temp->key < node->key){
        node->left = rbHelper(node->left, temp);
        if(node->left != NULL) node->left->parent = node;
    }
    else if(temp->key > node->key){
        node->right = rbHelper(node->right, temp);
        if(node->right != NULL) node->right->parent = node;
    }
 
    return node;
}

// helper function untuk mencari sibling node
RBNode *rbSibling(RBNode *node){
    if (node == NULL || node->parent == NULL) return NULL;

    if (node->parent->left == node) return node->parent->right;
    else return node->parent->left;
}

// right rotation untuk left left case pada red black tree
void rbRotateLL(RBNode **root, RBNode *buffer){
    RBNode *rightChild = buffer->right;
    buffer->right = rightChild->left;
    
    if(buffer->right) buffer->right->parent = buffer;
    
    rightChild->parent = buffer->parent;
    
	if(!buffer->parent) *root = rightChild;
    else if(buffer == buffer->parent->left) buffer->parent->left = rightChild;
    else buffer->parent->right = rightChild;
    
	rightChild->left = buffer;
    buffer->parent = rightChild;
}

// left rotation untuk right right case pada red black tree
void rbRotateRR(RBNode **root, RBNode *buffer){
    RBNode* leftChild = buffer->left;
    buffer->left = leftChild->right;
    
    if(buffer->left) buffer->left->parent = buffer;
    
	leftChild->parent = buffer->parent;
    
	if(!buffer->parent) *root = leftChild;
    else if(buffer == buffer->parent->left) buffer->parent->left = leftChild;
    else buffer->parent->right = leftChild;
    
	leftChild->right = buffer;
    buffer->parent = leftChild;
}

// function untuk melakukan balancing/fix violation ketika melakukan insertion dan terjadi RR pada parent dan newnode
void rbFixRR(RBNode **root, RBNode *node){
    RBNode* parent = NULL;
    RBNode* grandparent = NULL;

    while((node != *root) && (node->color != 'B') && (node->parent->color == 'R')){
        parent = node->parent;
        grandparent = node->parent->parent;

        if(parent == grandparent->left){

            RBNode *uncle = grandparent->right;
			// case parent adalah left child dari grandparent dan parent sibling berwarna merah
            if(uncle != NULL && uncle->color == 'R'){
                grandparent->color = 'R';
                parent->color = 'B';
                uncle->color = 'B';
                node = grandparent;
            }
            // case parent sibling berwarna hitam atau NULL
			else{
                if(node == parent->right){
                    rbRotateLL(root, parent);
                    node = parent;
                    parent = node->parent;
                }

                rbRotateRR(root, grandparent);
                char temp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp;
                node = parent;
            }
        }
        
		else{
            RBNode *uncle = grandparent->left;
			// case parent adalah right child dari grandparent dan parent sibling berwarna merah
            if(uncle != NULL && uncle->color == 'R'){
                grandparent->color = 'R';
                parent->color = 'B';
                uncle->color = 'B';
                node = grandparent;
            }
            // case parent sibling berwarna hitam atau NULL
			else{
                if(node == parent->left){
                    rbRotateRR(root, parent);
                    node = parent;
                    parent = node->parent;
                }

                rbRotateLL(root, grandparent);
                char temp = parent->color;
                parent->color = grandparent->color;
                grandparent->color = temp;
                node = parent;
            }
        }
    }
}

// function untuk mengganti subtree dengan subtree baru
void rbTransplant(RBNode **root, RBNode *subt, RBNode *newSubt){
    if (subt->parent == NULL) *root = newSubt;
    else if (subt == subt->parent->left) subt->parent->left = newSubt;
    else subt->parent->right = newSubt;
    
	if (newSubt != NULL) newSubt->parent = subt->parent;
}

// function untuk melakukan balancing/fix violation ketika melakukan deletion dan terjadi DB pada parent node yang dihapus
void rbFixDB(RBNode **root, RBNode *node){
    if(node == *root) return;

    RBNode *sibling = rbSibling(node);
	RBNode *parent = node->parent;
	
	// case no sibling, recursive call pada parent
    if(sibling == NULL) rbFixDB(root, parent);
    else{
    	// case sibling berwarna merah, ganti warna dan rotasi relatif terhadap parent kemudian recursive call
        if(sibling->color == 'R'){
            parent->color = 'R';
            sibling->color = 'B';
            
            if (sibling == parent->left) rbRotateRR(root, parent);
            else rbRotateLL(root, parent);
            
            rbFixDB(root, node);
        }
		else{
            // case sibling berwarna hitam
            if((sibling->left != NULL && sibling->left->color == 'R') || (sibling->right != NULL && sibling->right->color == 'R')){
                // case sibling memiliki left child berwarna merah
                if(sibling->left != NULL && sibling->left->color == 'R'){
                    if(sibling == parent->left){
                        // left left case
                        sibling->left->color = sibling->color;
                        sibling->color = parent->color;
                        rbRotateRR(root, parent);
                    }
					else{
                        // right left case
                        sibling->left->color = parent->color;
                        rbRotateRR(root, sibling);
                        rbRotateLL(root, parent);
                    }
                }
                // case sibling memiliki right child berwarna merah
				else{
                    if(sibling == parent->left){
                        // left right case
                        sibling->right->color = parent->color;
                        rbRotateLL(root, sibling);
                        rbRotateRR(root, parent);
                    }
					else{
                        // right right case
                        sibling->right->color = sibling->color;
                        sibling->color = parent->color;
                        rbRotateLL(root, parent);
                    }
                }
                parent->color = 'B';
            }
			else{
                // case 2 children berwarna hitam, ganti warna dan recursive call jika parent berwarna hitam
                sibling->color = 'R';
                if(parent->color == 'B') rbFixDB(root, parent);
                else parent->color = 'B';
            }
        }
    }
}

// mencari node successor pada red black tree
RBNode *rbSuccessor(RBNode *node){
    while (node->left != NULL) node = node->left;
    
    return node;
}

RBNode *rbReplace(RBNode *x){
    if (x->left != NULL && x->right != NULL) return rbSuccessor(x->right);

    if (x->left == NULL && x->right == NULL) return NULL;

    if (x->left != NULL) return x->left;
    else return x->right;
}

// swap value key pada dua node
void swapValues(RBNode *nodeA, RBNode *nodeB){
    int temp = nodeA->key;
    nodeA->key = nodeB->key;
    nodeB->key = temp;
}

// helper function untuk melakukan deletion
void rbDelHelper(RBNode **root, RBNode *node){
    RBNode *successor = rbReplace(node);
    RBNode *parent = node->parent;
    bool existDB = ((successor == NULL || successor->color == 'B') && (node->color == 'B'));

    if(successor == NULL){
        if (node == *root) *root = NULL;
        else{
            if(existDB) rbFixDB(root, node);
            else if(rbSibling(node) != NULL) rbSibling(node)->color = 'R';

            if (node->parent != NULL){
                if (node == node->parent->left) node->parent->left = NULL;
                else node->parent->right = NULL;
            }
        }
        free(node);
        return;
    }

    if (node->left == NULL || node->right == NULL){
        if(node == *root){
            node->key = successor->key;
            node->left = node->right = NULL;
            free(successor);
        }
		else{
            if (node == node->parent->left) node->parent->left = successor;
            else node->parent->right = successor;

            if (successor != NULL) successor->parent = parent;

            free(node);

            if (existDB) rbFixDB(root, successor);
            else successor->color = 'B';
        }
        return;
    }

    swapValues(successor, node);
    rbDelHelper(root, successor);
}

// main function untuk melakukan operasi insertion pada red black tree
RBNode *rbInsert(RBNode **root, int key) {
    RBNode *existingNode = rbSearch(*root, key);
    
    if (existingNode != NULL){
        return *root;
    }
    
    RBNode *newNode = createRBNode(key);
    *root = rbHelper(*root, newNode);
    rbFixRR(root, newNode);
    (*root)->color = 'B';
    
    return *root;
}

// function untuk melakukan searching pada red black tree
RBNode *rbSearch(RBNode *root, int key){
    if (root == NULL || root->key == key) return root;
    
    if (root->key < key) return rbSearch(root->right, key);
    else return rbSearch(root->left, key);
}

// function untuk melakukan update data pada red black tree dengan delete -> insert
void rbUpdate(RBNode **root, int key, int newKey){

    rbDelete(root, key);
    rbInsert(root, newKey);

    return;
}

// function untuk melakukan deletion pada red black tree
void rbDelete(RBNode **rbRoot, int key){
    if(rbRoot == NULL) return;

    RBNode *node = rbSearch(*rbRoot, key);
    if (node == NULL || node->key != key) return;

    rbDelHelper(rbRoot, node);
}

// function untuk membuat node baru pada b-tree, inisialisasi stat(jumlah key) mulai dari 0
BTNode *createBTNode(int t, bool leaf) {
    BTNode *newNode = (BTNode*)malloc(sizeof(BTNode));
    newNode->keys = (int*)malloc(sizeof(int) * (2 * t - 1));
    newNode->children = (BTNode**)malloc(sizeof(BTNode*) * (2 * t));
    newNode->leaf = leaf;
    newNode->stat = 0;
    return newNode;
}

// auxiliary function untuk traversal b-tree
void btTraverse(BTNode* root) {
    if(root){
        int i;
        for (i = 0; i < root->stat; i++) {
            if (!root->leaf) btTraverse(root->children[i]);
            printf(" %d", root->keys[i]);
        }
        if (!root->leaf) btTraverse(root->children[i]);
    }
}

// function untuk melakukan split node ketika satu node sudah melebihi maximum key
void splitChild(BTNode *parent, int t, int cIndex, BTNode *child){
	int i,j;
    BTNode *temp = createBTNode(t, child->leaf);
    temp->stat = t - 1;
    for(j = 0; j < t - 1; j++){
        temp->keys[j] = child->keys[j + t];
	}
	// memindahkan children ke temp
    if(!child->leaf){
        for(j = 0; j < t; j++){
            temp->children[j] = child->children[j + t];
    	}
	}
    child->stat = t - 1;
    
    // memindahkan children pada parent
    for(j = parent->stat; j >= cIndex + 1; j--){
        parent->children[j + 1] = parent->children[j];
	}
    parent->children[cIndex + 1] = temp;
    
    // memindahkan keys pada parent, memasukkan key baru dari median hasil split
    for(j = parent->stat - 1; j >= cIndex; j--){
        parent->keys[j + 1] = parent->keys[j];
    }
    
    parent->keys[cIndex] = child->keys[t - 1];
    parent->stat++;
}

// function untuk melakukan insert pada node yang melebihi maksimum key
void insertNonFull(BTNode *node, int t, int key){
    int i = node->stat - 1;
    if(node->leaf){
    	// insert pada leaf node
        while(i >= 0 && key < node->keys[i]){
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->stat++;
    }
	else{
        while (i >= 0 && key < node->keys[i]) i--;
        i++;
        // call split function jika node full dan recursive call untuk melakukan insert pada node baru
        if(node->children[i]->stat == 2 * t - 1){
            splitChild(node, t, i, node->children[i]);
            if (key > node->keys[i]) i++;
        }
        insertNonFull(node->children[i], t, key);
    }
}

// menghilangkan nilai pada leaf node ketika melakukan deletion
void removeLeaf(BTNode *node, int idx){
	int i;
    for(i = idx + 1; i < node->stat; i++){
        node->keys[i - 1] = node->keys[i];	
	}
    node->stat--;
}

// menghilangkan nilai pada internal node ketika melakukan deletion
void removeNonLeaf(BTNode *node, int t, int idx) {
    int key = node->keys[idx];
	// cek jika left child memiliki jumlah key yang setara atau melebihi minimum, ambil key dari left child
    if(node->children[idx]->stat >= t) {
        int predecessor = getPred(node, idx);
        node->keys[idx] = predecessor;
        btDelHelper(node->children[idx], t, predecessor);
    }
    // cek jika right child memiliki jumlah key yang setara atau melebihi minimum, ambil key dari right child
	else if (node->children[idx + 1]->stat >= t) {
        int successor = getSucc(node, idx);
        node->keys[idx] = successor;
        btDelHelper(node->children[idx + 1], t, successor);
    }
    // ketika kedua children memiliki key yang setara dengan minimum, lakukan merge dan recursive call
	else{
        merge(node, t, idx);
        btDelHelper(node->children[idx], t, key);
    }
}

// mencari nilai predecessor pada leaf node untuk menggantikan key yang didelete
int getPred(BTNode *node, int idx){
    BTNode *temp = node->children[idx];
    while (!temp->leaf) temp = temp->children[temp->stat];
    
    return temp->keys[temp->stat - 1];
}

// mencari nilai successor pada leaf node untuk menggantikan key yang didelete
int getSucc(BTNode *node, int idx){
    BTNode *temp = node->children[idx + 1];
    while (!temp->leaf) temp = temp->children[0];
    
    return temp->keys[0];
}

// function ketika sebuah node perlu meminjam key dari left sibling
void borrowPrev(BTNode *node, int idx) {
	int i;
    BTNode *child = node->children[idx];
    BTNode *sibling = node->children[idx - 1];
    
	// memindahkan key pada child node
    for(i = child->stat - 1; i >= 0; i--){
        child->keys[i + 1] = child->keys[i];
	}
	// memindahkan pointer jika child bukan leaf node
    if(!child->leaf){
        for(i = child->stat; i >= 0; i--){
            child->children[i + 1] = child->children[i];
        }
    }
	// mengubah key pertama dari child node menjadi key dari parent
    child->keys[0] = node->keys[idx - 1];
    
	// mengubah pointer pertama dari child node menjadi pointer terakhir dari sibling
    if(!child->leaf) child->children[0] = sibling->children[sibling->stat];
    
	// memasukkan key terakhir dari sibling ke parent node
    node->keys[idx - 1] = sibling->keys[sibling->stat - 1];
    child->stat += 1;
    sibling->stat -= 1;
}

// function oketika sebuah node perlu meminjam key dari right sibling
void borrowNext(BTNode *node, int idx){
	int i;
    BTNode *child = node->children[idx];
    BTNode *sibling = node->children[idx + 1];
	
	// memindahkan key dari parent ke child node
    child->keys[child->stat] = node->keys[idx];
	
	// mengubah pointer child jika bukan leaf node
    if(!(child->leaf)) child->children[child->stat + 1] = sibling->children[0];
	
	// memindahkan key dari sibling ke parent
    node->keys[idx] = sibling->keys[0];
	
	// memindahkan key sibling ke kiri
    for(i = 1; i < sibling->stat; i++){
        sibling->keys[i - 1] = sibling->keys[i];
	}
	
	// mengubah pointer di sibling jika bukan leaf node
    if(!sibling->leaf){
        for(i = 1; i <= sibling->stat; i++){
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->stat += 1;
    sibling->stat -= 1;
}

// function untuk melakukan merge pada satu node dengan sibling kanan ketika melakukan deletion
void merge(BTNode *node, int t, int idx){
	int i;
    BTNode *child = node->children[idx];
    BTNode *sibling = node->children[idx + 1];
	
    child->keys[t - 1] = node->keys[idx];
	
	// memindahkan key dari sibling ke child
    for(i = 0; i < sibling->stat; i++){
        child->keys[i + t] = sibling->keys[i];
    }
	
	// memindahkan pointer dari sibling ke child node jika bukan merupakan leaf node
    if(!child->leaf){
        for(i = 0; i <= sibling->stat; i++){
            child->children[i + t] = sibling->children[i];
    	}
	}
	
	// mengubah key pada parent node
    for(i = idx + 1; i < node->stat; i++){
        node->keys[i - 1] = node->keys[i];
	}
	
	// mengubah pointer pada parent node
    for(i = idx + 2; i <= node->stat; i++){
        node->children[i - 1] = node->children[i];
	}
	
    child->stat += sibling->stat + 1;
    node->stat--;
	
	// free memori untuk auxiliary node dan array
    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

// function untuk memasukkan key pada node yang memiliki jumlah key yang kurang dari minimum
void fill(BTNode* node, int t, int idx){
    if(idx != 0 && node->children[idx - 1]->stat >= t) borrowPrev(node, idx);
    else if(idx != node->stat && node->children[idx + 1]->stat >= t) borrowNext(node, idx);
    else{
        if(idx != node->stat) merge(node, t, idx);
        else merge(node, t, idx - 1);
    }
}

void btDelHelper(BTNode *root, int t, int key){
    int idx = 0;
    while(idx < root->stat && root->keys[idx] < key) idx++;
	
	// menemukan key pada node
    if(idx < root->stat && root->keys[idx] == key){
        if(root->leaf) removeLeaf(root, idx); // case leaf node
        else removeNonLeaf(root, t, idx); // case internal node
    }
	else{
        if(root->leaf) return; // key tidak ada dalam b-tree
		
		// cek apabila key merupakan key paling kanan pada node
        bool flag = ((idx == root->stat) ? true : false);
		
		// call function fill jika child node memiliki key yang kurang dari minimum
        if(root->children[idx]->stat < t) fill(root, t, idx);
		
		// recursive call jika key ada di subtree left sibling
        if(flag && idx > root->stat) btDelHelper(root->children[idx - 1], t, key);
        // recursive call jika key ada di current child
        else btDelHelper(root->children[idx], t, key);
    }
}

// function untuk melakukan insert pada b-tree
void btInsert(BTNode** root, int t, int key){
    if((*root) == NULL){
        *root = createBTNode(t, true);
        (*root)->keys[0] = key;
        (*root)->stat = 1;
    }
	else{
		// cek jika root node full, lakukan split child dan insert key pada node baru hasil split
        if((*root)->stat == 2 * t - 1){
            BTNode* newRoot = createBTNode(t, false);
            newRoot->children[0] = *root;
            splitChild(newRoot, t, 0, *root);
            int i = 0;
            if(newRoot->keys[0] < key) i++;
            
            insertNonFull(newRoot->children[i], t, key);
            *root = newRoot;
        }
        // jika root node tidak full, masukkan key pada root
		else insertNonFull(*root, t, key);
    }
}

// function untuk melakukan searching pada b-tree secara rekursif
BTNode *btSearch(BTNode *root, int key){
    int i = 0;
    while (i < root->stat && key > root->keys[i]) i++;
    
    if (key == root->keys[i]) return root;
    if (root->leaf) return NULL;
    
    return btSearch(root->children[i], key);
}

// function untuk melakukan update data pada b-tree dengan delete -> insert
void btUpdate(BTNode **root, int t, int key, int newKey){
    BTNode *node = btSearch(*root, key);

    if(node != NULL){
        btDelete(root, t, key);
    	btInsert(root, t, newKey);
	}
}

// main function untuk melakukan deletion pada b-tree
void btDelete(BTNode **root, int t, int key) {
    if(*root == NULL){
        printf("The tree is empty\n");
        return;
    }

    btDelHelper(*root, t, key);
	
	// cek jika root empty setelah deletion
    if((*root)->stat == 0){
        BTNode *tmp = *root;
        if ((*root)->leaf) *root = NULL;
        else *root = (*root)->children[0];

        free(tmp->keys);
        free(tmp->children);
        free(tmp);
    }
    return;
}

