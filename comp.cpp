#include<bits/stdc++.h> 
#define MAX_TREE_HT 100
#include<math.h>
using namespace std;

//structure for each value in the large file
struct table
{
      int data;
      int freq;
      int a[100];
      int huffcode[100];
      int huffdec;
      int bin_size;     
};

//structure info to store the compression factors
struct info
{
	int pad;
	float ratio;
	int length;
	int maxcodelen;
	float compression_ratio;
	float compression_factor;
	float compression_percentage;
};

 struct info info1;
 
// A node of huffman tree 
struct QueueNode
{
	int data;
	unsigned freq;
	struct QueueNode *left, *right;
};

// Structure for Queue: collection of Huffman Tree nodes (or QueueNodes)
struct Queue
{
	int front, rear;
	int capacity;
	struct QueueNode **array;
};

// A utility function to create a new Queuenode
struct QueueNode* newNode(int data, unsigned freq)
{
	struct QueueNode* temp =
	(struct QueueNode*) malloc(sizeof(struct QueueNode));
	temp->left = temp->right = NULL;
	temp->data = data;
	temp->freq = freq;
	return temp;
}

// A utility function to create a Queue of given capacity
struct Queue* createQueue(int capacity)
{
	struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
	queue->front = queue->rear = -1;
	queue->capacity = capacity;
	queue->array = (struct QueueNode**) malloc(queue->capacity * sizeof(struct QueueNode*));
	return queue;
}

void merge(struct table arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    struct table L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i].freq <= R[j].freq)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}
 
/* l is for left index and r is right index of the
   sub-array of arr to be sorted */
void mergeSort(struct table arr[], int l, int r)
{
    if (l < r)
    {
        // Same as (l+r)/2, but avoids overflow for
        // large l and h
        int m = l+(r-l)/2;
        // Sort first and second halves
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr, l, m, r);
    }
}

// A utility function to check if size of given queue is 1
int isSizeOne(struct Queue* queue)
{
	return queue->front == queue->rear && queue->front != -1;
}

// A utility function to check if given queue is empty
int isEmpty(struct Queue* queue)
{
	return queue->front == -1;
}

// A utility function to check if given queue is full
int isFull(struct Queue* queue)
{
	return queue->rear == queue->capacity - 1;
}

// A utility function to add an item to queue
void enQueue(struct Queue* queue, struct QueueNode* item)
{
	if (isFull(queue))
		return;
	queue->array[++queue->rear] = item;
	if (queue->front == -1)
		++queue->front;
}

// A utility function to remove an item from queue
struct QueueNode* deQueue(struct Queue* queue)
{
	if (isEmpty(queue))
		return NULL;
	struct QueueNode* temp = queue->array[queue->front];
	if (queue->front == queue->rear) // If there is only one item in queue
		queue->front = queue->rear = -1;
	else
		++queue->front;
	return temp;
}

// A utility function to get from of queue
struct QueueNode* getFront(struct Queue* queue)
{
	if (isEmpty(queue))
		return NULL;
	return queue->array[queue->front];
}

/* A function to get minimum item from two queues */
struct QueueNode* findMin(struct Queue* firstQueue, struct Queue* secondQueue)
{
	// Step 3.a: If second queue is empty, dequeue from first queue
	if (isEmpty(firstQueue))
		return deQueue(secondQueue);

	// Step 3.b: If first queue is empty, dequeue from second queue
	if (isEmpty(secondQueue))
		return deQueue(firstQueue);

	// Step 3.c: Else, compare the front of two queues and dequeue minimum
	if (getFront(firstQueue)->freq < getFront(secondQueue)->freq)
		return deQueue(firstQueue);
	
	else if(getFront(firstQueue)->freq>getFront(secondQueue)->freq)
		return deQueue(secondQueue);
	
	else if(getFront(firstQueue)->freq==getFront(secondQueue)->freq&&getFront(firstQueue)->data<getFront(secondQueue)->data)
		return deQueue(firstQueue);
	
	else if(getFront(firstQueue)->freq==getFront(secondQueue)->freq&&getFront(firstQueue)->data>getFront(secondQueue)->data)
		return deQueue(secondQueue);
}

// Utility function to check if this node is leaf
int isLeaf(struct QueueNode* root)
{
	return !(root->left) && !(root->right) ;
}

// A utility function to print the huffman code 
void printArr(int arr[],int n,struct table input[],int x)
{
	int i;
	for (i = 0; i < n; ++i)
	     {
            // printf("%d", arr[i]);
             input[x].a[i]=arr[i];
             input[x].huffcode[i]=arr[i];
        }
             input[x].huffcode[i]=1;
             
        input[x].bin_size=n;
	//printf("\n");
}

// The main function that builds Huffman tree
struct QueueNode* buildHuffmanTree(struct table input[], int size)
{
	struct QueueNode *left, *right, *top;

	// Step 1: Create two empty queues
	struct Queue* firstQueue = createQueue(size);
	struct Queue* secondQueue = createQueue(size);

	// Step 2:Create a leaf node for each unique character and Enqueue it to
	// the first queue in non-decreasing order of frequency. Initially second
	// queue is empty
	int i;
	for (i = 0; i < size; ++i)
		enQueue(firstQueue, newNode(input[i].data, input[i].freq));

	// Run while Queues contain more than one node. Finally, first queue will
	// be empty and second queue will contain only one node
	while (!(isEmpty(firstQueue) && isSizeOne(secondQueue)))
	{
		// Step 3: Dequeue two nodes with the minimum frequency by examining
		// the front of both queues
		left = findMin(firstQueue, secondQueue);
		right = findMin(firstQueue, secondQueue);

		// Step 4: Create a new internal node with frequency equal to the sum
		// of the two nodes frequencies. Enqueue this node to second queue.
		top = newNode('$' , left->freq + right->freq);
		top->left = left;
		top->right = right;
		enQueue(secondQueue, top);
	}

	return deQueue(secondQueue);
}

// Prints huffman codes from the root of Huffman Tree. It uses arr[] to store codes
void printCodes(struct QueueNode* root, int arr[], int top,struct table input[],int n)
{
	// Assign 0 to left edge and recur
	if (root->left)
	{
		arr[top] = 0;
		printCodes(root->left, arr, top + 1,input,n);
	}

	// Assign 1 to right edge and recur
	if (root->right)
	{
		arr[top] = 1;
		printCodes(root->right, arr, top + 1,input,n);
	}

	// If this is a leaf node, then it contains one of the input
	// characters, print the character and its code from arr[]
	if(isLeaf(root))
	{
	   // printf("%d: ", root->data);
	    int x=-1;
	    int i;
	    for(i=0;i<n;i++)
	    {
                if(input[i].data==root->data)
                x=i;
       }
		printArr(arr, top,input,x);
	}
}

// The main function that builds a Huffman Tree and print codes by traversing the built Huffman Tree

void HuffmanCodes(struct table input[100], int size)
{
	struct QueueNode* root = buildHuffmanTree(input, size);
	int arr[MAX_TREE_HT], top = 0;
	printCodes(root, arr, top,input,size);
}

//reordering of data packets in the order they were sent
int Big_Binary_8_Multiple(int seq[30000],int n,struct table input[300],int r,int binary[240000])
{
	int x=0,i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<r;j++)
		{
			if(seq[i]==input[j].data)
			{
            int k;
			for(k=0;k<input[j].bin_size;k++)
			{
			binary[x++]=input[j].a[k];
			}
			break;
			}
		}
	}	
	int m=0;
	while(x%8!=0)
	{
	m++;
	binary[x++]=0;
	}
	info1.pad=m; ///padding in protocol interpretor
	return x;
}

//converting final output from sender bit stream to byte decimal
int Final_Output(int binary[240000],int bin_count,int final_output[30000])
{
	FILE *fp2;
	fp2 = fopen("/media/naseem/New Volume/Firu/Studies/NonAcads/projects/inp_project/INP-FTP/sender_output.txt", "w");
	
	int x=0,sum=0,p=1,i;
	for(i=0;i<bin_count;i++)
	{
		if(i%8==0&&i!=0)
		{
		final_output[x++]=sum;
		fprintf(fp2,"%d\n",final_output[x-1]);
		sum=0;
		p=1;
		}
		sum=sum+p*binary[i];
		p=p*2;	
	}
	final_output[x++]=sum;
	fprintf(fp2,"%d\n",final_output[x-1]);
	return x;

}


int main()
{
	FILE *fp;
	int seq[30000],n,hash[300];
    struct table input[300];
	fp = fopen("/media/naseem/New Volume/Firu/Studies/NonAcads/projects/inp_project/INP-FTP/sender_input.txt", "r");
	
	
	
//converting to 8-bit
     int i7,in[30000];
     n=21600;
      int l=n;
     for(i7=0;i7<l;i7++)
      {
        int r;
	     fscanf(fp, "%d",&r);
	     in[i7]=r;
      }
     int dif[n-1],m[n-1],out[n+1];
     int j1,max1=-1,x,sign;

     float y;
     j1=0;

     for(i7=0;i7<l-1;i7++)
     {
      dif[j1]=in[i7+1]-in[i7];
      if(dif[j1]<0)
      m[j1]=-dif[j1];
      else
      m[j1]=dif[j1];
      if(m[j1]>max1)
      max1=m[j1];
      j1++;
     }

     y=log(max1)/log(2);
     int h2=y;
     if((h2-y)!=0)
     sign=h2+1;
     else
     sign=y;

     x=pow(2,sign);
     out[0]=in[0]>>4;
     out[1]=in[0]>>8;

     for(i7=0;i7<l-1;i7++)
     {
      	if(dif[i7]<0)
      	out[i7+2]=-dif[i7]+x;
       	else
        	out[i7+2]=dif[i7];
     }


	info1.length=n;
	
	//initialise frequency of each number to be zero
	int i1;
	for(i1=0;i1<256;i1++)
	hash[i1]=0;
	
	//accept each number and increment the frequency of each number to find frequency of each number
	int i9;
	n++;
	for(i9=0;i9<n;i9++)
	{
	seq[i9]=out[i9];
	hash[seq[i9]]++;
	} 
	
	//store the values of data and freq in the structure called input
	int r=0;
	int i2;
	for(i2=0;i2<256;i2++)
	{
		input[i2].huffdec=0;
		if(hash[i2]>0)
		{
		input[r].data=i2;
		input[r].freq=hash[i2];
		r++;
		}
	}
	
     mergeSort(input,0,r-1);
     
        //create huffmann codes for each and store them in input structure
  HuffmanCodes(input, r);
  
	int max=0;
	int i3;
	
	//storing huff - decimal for huffman codes
	for(i3=0;i3<r;i3++)
	{
            int j1;      
            int p=1;
            int sum=0;
            int j2;
            for(j2=0;j2<=input[i3].bin_size;j2++)
            {
            sum=sum+((input[i3].huffcode[j2])*(p));
            p=p*2;
            } 
            input[i3].huffdec=sum;
            if(input[i3].bin_size>max)
            max=input[i3].bin_size;
                 
         }
         info1.maxcodelen=max;
    
         //store the binary sequence of the present sequence using huffmann coding 
         int binary[240000],bin_count;  
		
		//create a stream of binary output to transfer
    	 bin_count=Big_Binary_8_Multiple(seq,n,input,r,binary);
    	 
    	 //find the compressed decimal output 
     int final_output[30000],fin_count;
    	 fin_count=Final_Output(binary,bin_count,final_output);
    	 
    	 
      info1.ratio=((double)fin_count)/n;
      cout<<"final count sent is "<<fin_count<<"\n";
      cout<<"n value initiallly is "<<n<<"\n";
    	
    	 //calculating compression values
    	 info1.compression_ratio=info1.ratio;

    	 
    	  printf("compressed ratio is %f\n",info1.compression_ratio);
    	
return 0;
}

