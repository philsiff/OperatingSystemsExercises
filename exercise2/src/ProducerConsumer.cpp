#include "ProducerConsumer.h"

//TODO: add BoundedBuffer, locks and any global variables here
BoundedBuffer *bb;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
int pSleep;
int cSleep;
int addedItems = 0;
int removedItems = 0;
int maxItems;
ofstream outputFile;

void InitProducerConsumer(int p, int c, int psleep, int csleep, int items){
	//TODO: constructor to initialize variables declared
	//also see instruction for implementation
    bb = new BoundedBuffer(items);
    pthread_t *threads = new pthread_t[p+c];
    pSleep = psleep;
    cSleep = csleep;
    maxItems = items;
    outputFile.open("output.txt");
    
    pthread_t producers[p];
    pthread_t consumers[c];
    int threadID;

    for(int i = 0; i < p; i++){
        threadID = i + 1;
        //cout << to_string(threadID) << endl;
        pthread_create(&producers[i], NULL, producer, (void *) &threadID); 
    }
    for(int j = 0; j < c; j++){
        threadID = j + 1;
        //cout << to_string(threadID) << endl;
        pthread_create(&consumers[j], NULL, consumer, (void *) &threadID); 
    }
    for(int i = 0; i < p; i++){
        pthread_join(producers[i], NULL); 
    }
    for(int i = 0; i < p; i++){
        pthread_join(consumers[i], NULL); 
    }
    outputFile.close();
}

void* producer(void* threadID){
	//TODO: producer thread, see instruction for implementation
    //cout << "PRODUCER ACTIVE!" << endl;
    int *id = (int *) threadID;
    int toAdd;
    while(1){
        usleep(pSleep);
        //cout << "PRODUCER: DONE SLEEPING" << endl;
        if(addedItems >= maxItems){
            pthread_exit(NULL);
        }
        //cout << "PRODUCER: WAITING ON LOCK" << endl;
        pthread_mutex_lock(&lock);
        //cout << "PRODUCER: ACQUIRED LOCK" << endl;
        toAdd = rand();
        //cout << to_string(toAdd) << endl;
        //cout << "PRODUCER: NUMBER GENERATED" << endl;
        bb->append(toAdd);
        //cout << "PRODUCER: NUMBER ADDED" << endl;
        addedItems++;
        pthread_cond_broadcast(&empty);
        //cout << "Producer " << to_string(*id) << ", time = current time" << \
            ", producing data item #" << to_string(addedItems) << ", item value=" << to_string(toAdd) << endl;
        outputFile << "Producer " << to_string(*id) << ", time = current time" << \
            ", producing data item #" << to_string(addedItems) << ", item value=" << to_string(toAdd) << endl;
        pthread_mutex_unlock(&lock);
    }
}

void* consumer(void* threadID){
	//TODO: consumer thread, see instruction for implementation
    int *id = (int *) threadID;
    int returned;
    while(1){
        usleep(cSleep);
        pthread_mutex_lock(&lock);
        while(removedItems == addedItems){
            if(addedItems == maxItems){
                pthread_mutex_unlock(&lock);
                pthread_exit(NULL);
            }
            pthread_cond_wait(&empty, &lock);
        }
        pthread_mutex_unlock(&lock);
        
        //cout << "CONSUMER: HAS LOCK. WAITING FOR REMOVE" << endl;
        returned = bb->remove(); 
        
        //cout << "CONSUMER: REMOVED SUCCESS" << endl;
        pthread_mutex_lock(&lock);
        removedItems++;
        //cout << "Consumer " << to_string(*id) << ", time = current time" << \
            ", consuming data item with value=" << to_string(returned) << endl;
        outputFile << "Consumer " << to_string(*id) << ", time = current time" << \
            ", consuming data item with value=" << to_string(returned) << endl;
        pthread_mutex_unlock(&lock);
    }
}
