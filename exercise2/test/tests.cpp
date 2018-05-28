// tests.cpp
#include "../src/BoundedBuffer.h"
#include "../src/ProducerConsumer.h"
#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <string>

//Test checking if bounded buffer is empty
TEST(PCTest, Test1) {
    BoundedBuffer *BB = new BoundedBuffer(5);
    EXPECT_TRUE(BB->isEmpty());

    delete BB;
}

//Test checking append() and remove() from buffer
TEST(PCTest, Test2){
    BoundedBuffer *BB = new BoundedBuffer(5);
    BB->append(0);
    ASSERT_EQ(0,BB->remove());

    delete BB;
}

//Testing ProducerConsumer for one thread
TEST(PCTest, ProducerConsumerOneThread){
    InitProducerConsumer(1,5,10,1,3);

    string producer ("Producer");
    string consumer ("Consumer");

    string line = "";

    int cnt_prod = 0;
    int cnt_con = 0;

    std::ifstream myfile ("output.txt");
    if (myfile.is_open()){
        while(getline(myfile,line)){
            cout << line << endl;
            if (line.compare(0, 8, producer) == 0){
                cnt_prod++;
            }
            if (line.compare(0, 8, consumer) == 0){
                cnt_con++;
            }
        }
    }
    myfile.close();

    EXPECT_EQ(5, cnt_prod);
    EXPECT_EQ(5, cnt_con);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
