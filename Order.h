#pragma once

typedef float price_t;
typedef unsigned int quantity_t;
typedef unsigned int order_id_t;
typedef unsigned int id_t;
enum ORDERTYPE { IOC, GTD, GTC };

struct Order {
    unsigned long long timestamp;
    bool is_bid;
    order_id_t id;
    price_t price;
    quantity_t quantity;
    ORDERTYPE type;

    Order(
        unsigned long long timestamp_,
        bool is_bid_,
        order_id_t id_,
        price_t price_,
        quantity_t quantity_,
        ORDERTYPE type_
    );

    Order(const Order& o1);
};