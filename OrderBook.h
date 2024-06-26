#pragma once

#include "Order.h"
#include <map>
#include <unordered_map>
#include <set>

enum ACTION { ADD, MODIFY, DELETE };

struct OrderAction {
    Order& order;
    ACTION action;
    OrderAction(Order& o, ACTION a);
};

class OrderBook {

private:
    std::unordered_map<order_id_t, Order*> bids;

    std::unordered_map<order_id_t, Order*> asks;

    std::map<float, int> bid_volume_level;

    std::map<float, int> ask_volume_level;

    std::set<order_id_t> order_set;

public:

    // Constructor Initialized order book object
    OrderBook();

    // Add a new order to the order book
    void add_order(Order& o);

    // Modify an existing order
    void modify_order(struct Order o);

    // Delete an existing order
    void delete_order(struct Order o);

    // Exposed Interface to process all three actions above
    void process_order(OrderAction o);

    // Get the top k(5) bid levels of price level and quantity
    std::vector<std::string> get_bid_top_k_levels();

    // Get the top k(5) ask levels of price level and quantity
    std::vector<std::string> get_ask_top_k_levels();

    int total_bid_levels();

    int total_ask_levels();

    // Clear the book after the day finishes; Release all the dynamically allocated memory
    void clear_book();

};

