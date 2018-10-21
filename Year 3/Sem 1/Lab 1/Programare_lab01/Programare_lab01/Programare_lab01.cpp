// Programare_lab01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <ctime>
#include <vector>

#define MAX_THREADS 64

typedef struct _PRODUCT_PRICE
{
    std::string Product;
    int         Quantity;
    int         Price;
}PRODUCT_PRICE;

PRODUCT_PRICE gProductPrices[] =
{
    {"apple",   10,     100},
    {"mango",   7,      50},
    {"banana",  8,      30},
    {"cherry",  9,      300},
    {"lemon",   30,     251},
    {"orange", 700,     10}
};

int gTotalMoney = 0;

std::vector<PRODUCT_PRICE*> gBills;

#define TOTAL_PRODUCTS _countof(gProductPrices) 

std::mutex mtx;

void f(int i)
{
    std::srand(std::time(nullptr)); // use current time as seed for random generator

    /*printf("Thread %d running\n", i);
*/
    std::unique_lock<std::mutex> lck(mtx, std::defer_lock);


    //std::cout << "thread #" << i << '\n';

    int numberOfProductsToBuy = std::rand() % TOTAL_PRODUCTS;
    printf("Thread %d chose %d items to buy\n", i, numberOfProductsToBuy);

    for (int i = 0; i < numberOfProductsToBuy; i++)
    {
        // Choose the products
        int chosenProduct = std::rand() % TOTAL_PRODUCTS;

        // Find an item with quantity > 0

        char lastKnownProduct[255];
        lck.lock();

        for (
            ;
            lastKnownProduct!= nullptr && (strcmp(lastKnownProduct, gProductPrices[chosenProduct].Product.c_str())) && !gProductPrices[chosenProduct].Quantity;
            chosenProduct = std::rand() % TOTAL_PRODUCTS
            );

        lck.unlock();

        PRODUCT_PRICE* productPrice = &gProductPrices[chosenProduct];
        strcpy_s(lastKnownProduct, gProductPrices[chosenProduct].Product.c_str());

        // Chose product gProductPrices[chosenProduct]
        // Now get the total ammount to decrease

        int totalQuantity = /*std::rand() % productPrice->Quantity*/1;
        int totalPrice = totalQuantity * productPrice->Price;

        lck.lock();

        productPrice->Quantity -= totalQuantity;
        productPrice->Price -= totalPrice;

        lck.unlock();

        printf("Thread %d buying %s, quantity %d, resulting in a total price of %d\n", i, productPrice->Product.c_str(), totalQuantity, totalPrice);
        std::this_thread::sleep_for(std::chrono::milliseconds(15));

    }

    // Add a bill with all these payment
    lck.lock();

    lck.unlock();
}

int main()
{

    std::thread threadArray[MAX_THREADS];

    for (int i = 0; i < MAX_THREADS; i++)
    {
        threadArray[i] = std::thread(f, i);
    }

    for (auto & th : threadArray)
    {
        th.join();
    }
}
