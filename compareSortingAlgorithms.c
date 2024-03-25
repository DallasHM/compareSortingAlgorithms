#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}




void bubbleSort(int* parData, int n)
{
     for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (parData[j] > parData[j + 1]) {
                int t = parData[j];
                parData[j] = parData[j + 1];
                parData[j + 1] = t;
            }
        }
    }
	
}



	void insertionSort(int* parData, int n)
{
    for (int i = 1; i < n; i++) {
        int key = parData[i];
        int j = i - 1;
        while (j >= 0 && parData[j] > key) {
            parData[j + 1] = parData[j];
            j = j - 1;
        }
        parData[j + 1] = key;
    }
}




void selectionSort(int* parData, int n)
{
       for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (parData[j] < parData[minIndex]) {
                minIndex = j;
            }
        }
        // Swap the elements
        int t = parData[minIndex];
        parData[minIndex] = parData[i];
        parData[i] = t;
    }
	
}


void heapSort(int arr[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--) {
        int parent = i;
        while (parent < n / 2) {
            int maxChild = 2 * parent + 1;
            if (maxChild + 1 < n && arr[maxChild] < arr[maxChild + 1]) {                maxChild++;
            }
            if (arr[parent] < arr[maxChild]) {
                // Swap largest child if parent is smaller
                int t = arr[parent];
                arr[parent] = arr[maxChild];
                arr[maxChild] = t;
            }
            parent = maxChild;
        }
    }

    // Extract elements from heap one by one
    for (int i = n - 1; i > 0; i--) {
        int t = arr[0];
        arr[0] = arr[i];
        arr[i] = t;

        int parent = 0;
        while (parent < i / 2) {
            int maxChild = 2 * parent + 1; // Left child
            if (maxChild + 1 < i && arr[maxChild] < arr[maxChild + 1]) {
                maxChild++;
            }
            if (arr[parent] < arr[maxChild]) {
                int t = arr[parent];
                arr[parent] = arr[maxChild];
                arr[maxChild] = t;
            }
            parent = maxChild;
        }
    }
}




// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int parData[], int l, int r)
{
        if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(parData, l, m);
        mergeSort(parData, m + 1, r);
        int n1 = m - l + 1;
        int n2 = r - m;
        int *L = (int *)Alloc(n1 * sizeof(int));
        int *R = (int *)Alloc(n2 * sizeof(int));
        for (int i = 0; i < n1; i++)
            L[i] = parData[l + i];
        for (int j = 0; j < n2; j++)
            R[j] = parData[m + 1 + j];
        int i = 0, j = 0, k = l;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                parData[k] = L[i];
                i++;
            } else {
                parData[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1) {
            parData[k] = L[i];
            i++;
            k++;
        }
        while (j < n2) {
            parData[k] = R[j];
            j++;
            k++;
        }
        DeAlloc(L);
        DeAlloc(R);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **pparData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*pparData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*pparData = (int *)malloc(sizeof(int) * dataSz);
		if (*pparData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *pparData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

void printArray(int parData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",parData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",parData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *parDataSrc, *parDataCopy;
		int dataSz = parseData(fileNames[i], &parDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		parDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		
		printf("Selection Sort:\n");
		memcpy(parDataCopy, parDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(parDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(parDataCopy, dataSz);
		
		
		
				printf("Insertion Sort:\n");
		memcpy(parDataCopy, parDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(parDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(parDataCopy, dataSz);
		
		printf("Bubble Sort:\n");
		memcpy(parDataCopy, parDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(parDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(parDataCopy, dataSz);
		
		
		printf("Merge Sort:\n");
		memcpy(parDataCopy, parDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(parDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(parDataCopy, dataSz);
		
		
		                printf("Heap Sort:\n");
		memcpy(parDataCopy, parDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(parDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(parDataCopy, dataSz);
		
		free(parDataCopy);
		free(parDataSrc);
	}
	
}