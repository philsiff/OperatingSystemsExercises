#include "BoundedBuffer.h"

BoundedBuffer::BoundedBuffer(int N){
	//TODO: constructor to initiliaze all the varibales declared in BoundedBuffer.h
    buffer = new int[N];
    buffer_size = N;
    buffer_cnt = 0;
    buffer_last = 0;
    buffer_lock = PTHREAD_MUTEX_INITIALIZER;
    buffer_full = PTHREAD_COND_INITIALIZER;
    buffer_empty = PTHREAD_COND_INITIALIZER;
}

void BoundedBuffer::append(int data){
	//TODO: append a data item to the circular buffer
    pthread_mutex_lock(&buffer_lock);
    while(buffer_cnt >= buffer_size){
        pthread_cond_wait(&buffer_empty, &buffer_lock);
    }
    //cout << "DATA: " << to_string(data) << endl;
    buffer[buffer_cnt == 0 ? buffer_last : (buffer_last+1)%buffer_size] = data;
    buffer_last = (buffer_cnt == 0 ? buffer_last : buffer_last + 1);
    buffer_cnt++;
    pthread_cond_broadcast(&buffer_full);
    pthread_mutex_unlock(&buffer_lock);
}

int BoundedBuffer::remove(){
	//TODO: remove and return a data item from the circular buffer
    //cout << "REMOVE: HAS LOCK" << endl;
    pthread_mutex_lock(&buffer_lock);  
    while(buffer_cnt <= 0){
        //cout << "REMOVE: IN LOOP SLEEPING" << endl;
        pthread_cond_wait(&buffer_full, &buffer_lock);
        //cout << "REMOVE: IN LOOP AWOKEN" << endl;
    }
    int toReturn = buffer[(buffer_size - buffer_cnt + buffer_last + 1)%buffer_size];
    cout << "----------------BOUNDED BUFFER-----------------" << endl;
    for(int i = 0; i < buffer_size; i++){
        cout << to_string(i) << ": " << to_string(buffer[i]) << endl;
        if(buffer_last == i){
            cout << "^^^^^^^^^^^TAIL^^^^^^^^^^^" << endl;
        }
    }
    cout << "COUNT: " << to_string(buffer_cnt) << endl;
    buffer_cnt--;
    pthread_cond_broadcast(&buffer_empty);
    pthread_mutex_unlock(&buffer_lock);
    cout << "TORETURN: " << to_string(toReturn) << endl;
    return toReturn;
}

bool BoundedBuffer::isEmpty(){
	//TODO: check is the buffer is empty
	return (buffer_cnt == 0);
}
