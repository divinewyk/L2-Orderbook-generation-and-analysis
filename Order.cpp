#include <string>
#include "Order.h"

Order::Order(
    unsigned long long timestamp_,
    bool is_bid_,
    order_id_t id_,
    price_t price_,
    quantity_t quantity_,
    ORDERTYPE type_
) :timestamp(timestamp_),is_bid(is_bid_),id(id_),price(price_),quantity(quantity_),type(type_){

}
Order::Order(const Order& o1) {
    timestamp = o1.timestamp;
    is_bid = o1.is_bid;
    id = o1.id;
    price = o1.price;
    quantity = o1.quantity;
    type = o1.type;
}