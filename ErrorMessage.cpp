#include "ErrorMessage.h"
#include <iostream>
void display_error_order_id_exist(const Order& o)
{
    std::cout << "order id=" << o.id << " is a duplicate - order skipped" << std::endl;
}

void display_error_exist_order_modification(const Order& o)
{
    std::cout << "order id=" << o.id << " not existing - modification rejected" << std::endl;
}

void display_error_amendment_price(const Order& o)
{
    std::cout << "order id=" << o.id << " cannot have price changed  - amendment rejected" << std::endl;

}

void display_error_order_not_consistent_deletion(const Order& o)
{
    std::cout << "order id=" << o.id << " not the same price or quanitity as previous order - deletion rejected" << std::endl;

}

void display_error_order_not_existing_deletion(const Order& o)
{
    std::cout << "order id=" << o.id << " not existing - deletion rejected" << std::endl;
}