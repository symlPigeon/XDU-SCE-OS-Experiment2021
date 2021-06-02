#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct MultiThreadMergeSortArgs{
    int* srcList;
    int* dstList;
    int listLen;
};

void* MultiThreadMergeSort(void* args){
    struct MultiThreadMergeSortArgs* funcArgs = (struct MultiThreadMergeSortArgs*)args;
    int* srcList = funcArgs -> srcList;
    int* dstList = funcArgs -> dstList;
    int listLen = funcArgs -> listLen;
    if (listLen == 1){
        *dstList = *srcList;
        return;
    }
    int* dstListLeft = (int*) malloc(sizeof(int) * listLen / 2);
    int* dstListRight = (int*) malloc(sizeof(int) * (listLen - listLen / 2));
    struct MultiThreadMergeSortArgs leftArgs, rightArgs;
    leftArgs.srcList = srcList;
    leftArgs.dstList = dstListLeft;
    leftArgs.listLen = listLen / 2;
    rightArgs.srcList = (srcList + listLen / 2);
    rightArgs.dstList = dstListRight;
    rightArgs.listLen = listLen - listLen / 2;
    pthread_t pid1, pid2;
    int childThreadLeft = pthread_create(&pid1, NULL, MultiThreadMergeSort, &leftArgs);
    int childThreadRight = pthread_create(&pid2, NULL, MultiThreadMergeSort, &rightArgs);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    int leftPoint = 0, rightPoint = 0;
    for (int i = 0; i < listLen; i++){
        if (leftPoint == listLen / 2)
            dstList[i] = dstListRight[rightPoint++];
        else
            if (rightPoint == listLen - listLen / 2)
                dstList[i] = dstListLeft[leftPoint++];
            else
                dstList[i] = dstListLeft[leftPoint] > dstListRight[rightPoint] ? dstListRight[rightPoint++] : dstListLeft[leftPoint++];
    }
    return;
}

int main(){
    int cnt;
    scanf("%d", &cnt);
    int* list = (int*) malloc(sizeof(int) * cnt);
    for (int i = 0; i < cnt; i++)
        scanf("%d", &list[i]);
    int* ansList = (int*) malloc(sizeof(int) * cnt);
    struct MultiThreadMergeSortArgs args;
    args.srcList = list;
    args.dstList = ansList;
    args.listLen = cnt;
    MultiThreadMergeSort(&args);
    for (int i = 0; i < cnt; i++)
        printf("%d ", ansList[i]);
    return 0;
}

