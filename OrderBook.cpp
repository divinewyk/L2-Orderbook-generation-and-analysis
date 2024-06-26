#include "OrderBook.h"
#include "ErrorMessage.h"
#include <algorithm>
#include <string>
#include "UtilityFunctions.h"

OrderAction::OrderAction(Order& o, ACTION a) :order(o), action(a) {}

OrderBook::OrderBook() {

};

void OrderBook::add_order(Order& o) 
{

    // If an order already exists, should not process this order
    // Check if the order id already exists in the map.
    // The map primary key is the order id
    // If the order exists, show error display_error_order_id_exist

    if (order_set.find(o.id) != order_set.end()) {
        display_error_order_id_exist(o);
        return;
    }

    // Based on the field is_bid, store the order to the right map
    if (o.is_bid) {
        bids[o.id] = new Order(o);

    }
    else {
        asks[o.id] = new Order(o);
    }

    // Add order id to order id set
    order_set.insert(o.id);

    // Add order size to respective price levels depth
    if (o.is_bid) {
        auto it2 = bid_volume_level.find(o.price);
        if (it2 != bid_volume_level.end())
        {
            bid_volume_level[o.price] += o.quantity;
        }
        else {
            bid_volume_level[o.price] = o.quantity;
        }
    }
    else {
        auto it2 = ask_volume_level.find(o.price);
        if (it2 != ask_volume_level.end())
        {
            ask_volume_level[o.price] += o.quantity;
        }
        else {
            ask_volume_level[o.price] = o.quantity;
        }
    }


};

void OrderBook::modify_order(struct Order o) {


    // First, if the order id is not in previous orders, we cannot process the modify request
    // Output Error
    if (order_set.find(o.id) == order_set.end()) {
        display_error_exist_order_modification(o);
        return;
    }

    // If the modified order is in the bid book
    if (o.is_bid && bids.find(o.id) != bids.end()) {
        // Second, if the modification coming from the order in the parameter of function has a different price
        // we cannot process the modification and output error
        if (o.price != bids[o.id]->price) {
            display_error_amendment_price(o);
            return;
        }

        // Change respective price levels depth to reflect the order modification
        bid_volume_level[o.price] += o.quantity - bids[o.id]->quantity;

        // Delete the price level if no more quantity
        if (bid_volume_level[o.price] == 0) {
            bid_volume_level.erase(o.price);
        }

        // Update the order with new quantity
        bids[o.id]->quantity = o.quantity;
        bids[o.id]->timestamp = o.timestamp;
        return;

    }

    //If the modified order is in the ask book, perform similar actions
    if (!o.is_bid && asks.find(o.id) != asks.end()) {
        if (o.price != asks[o.id]->price) {
            display_error_amendment_price(o);
            return;
        }
        ask_volume_level[o.price] += o.quantity - asks[o.id]->quantity;

        if (ask_volume_level[o.price] == 0) {
            ask_volume_level.erase(o.price);
        }

        asks[o.id]->quantity = o.quantity;
        asks[o.id]->timestamp = o.timestamp;
        return;
    }

}

void OrderBook::delete_order(struct Order o)
{
    // If the deleted order is in the bid book
    if (o.is_bid && bids.find(o.id) != bids.end()) {
        if (o.quantity == bids[o.id]->quantity && o.price == bids[o.id]->price) {

            // Reduce the quantity in the levels after the deletion
            bid_volume_level[o.price] -= bids[o.id]->quantity;

            // Delete the price level if no more quantity
            if (bid_volume_level[o.price] == 0) {
                bid_volume_level.erase(o.price);
            }
            delete bids[o.id];
            bids.erase(o.id);
        }
        else {
            //If the delete oder is not the same as what is stored previously for the order, reject deletion
            display_error_order_not_consistent_deletion(o);
        }

        return;
    }

    //If the deleted order is in the ask book, perform similar actions
    if (!o.is_bid && asks.find(o.id) != asks.end()) {
        if (o.quantity == asks[o.id]->quantity && o.price == asks[o.id]->price) {

            ask_volume_level[o.price] -= asks[o.id]->quantity;
            if (ask_volume_level[o.price] == 0) {
                ask_volume_level.erase(o.price);
            }
            delete asks[o.id];
            asks.erase(o.id);
        }
        else {
            display_error_order_not_consistent_deletion(o);
        }
        return;
    }
    display_error_order_not_existing_deletion(o);


}

void OrderBook::process_order(OrderAction o)
{

    // This function is the exposed interface and is called for any received orders.
    // call the function add_order, delete_order, modify_order depending
    // on the ACTION::ADD, ACTION::DELETE, ACTION::MODIFY
    switch (o.action) {
    case ACTION::ADD:
        add_order(o.order);
        break;

    case ACTION::DELETE:
        delete_order(o.order);
        break;
    case ACTION::MODIFY:
        modify_order(o.order);
        break;
    }

};

std::vector<std::string> OrderBook::get_bid_top_k_levels() {
    // This function will return the top 5 bid price and the total volume associated to this price

    std::vector<std::pair<float, int>> sorted_volume_levels(bid_volume_level.begin(), bid_volume_level.end());
    if (bid_volume_level.size() < 5) {
        std::sort(sorted_volume_levels.begin(), sorted_volume_levels.end(), [](const auto& a, const auto& b) {
            return a.first > b.first; // Sort in descending order of values
            });
    }
    else {
        std::partial_sort(sorted_volume_levels.begin(), sorted_volume_levels.begin() + 5, sorted_volume_levels.end(), [](const auto& a, const auto& b) {
            return a.first > b.first; // Sort in descending order of values
            });
    }
    std::vector<std::string> bid_levels;
    for (size_t i = 0; i < 5; ++i) {
        if (i < bid_volume_level.size()) {
            bid_levels.push_back(floatToStringWithPrecision(sorted_volume_levels[i].first, 2));
            bid_levels.push_back(std::to_string(sorted_volume_levels[i].second));
        }
        else {
            bid_levels.push_back("0");
            bid_levels.push_back("0");
        }

    }
    return bid_levels;
}

std::vector<std::string> OrderBook::get_ask_top_k_levels() {
    // Same as the previous one
    std::vector<std::pair<float, int>> sorted_volume_levels(ask_volume_level.begin(), ask_volume_level.end());

    if (ask_volume_level.size() < 5) {
        std::sort(sorted_volume_levels.begin(), sorted_volume_levels.end(), [](const auto& a, const auto& b) {
            return a.first < b.first; // Sort in ascending order of values
            });
    }
    else {
        std::partial_sort(sorted_volume_levels.begin(), sorted_volume_levels.begin() + 5, sorted_volume_levels.end(), [](const auto& a, const auto& b) {
            return a.first < b.first; // Sort in ascending order of values
            });
    }
    std::vector<std::string> ask_levels;
    for (size_t i = 0; i < 5; ++i) {
        if (i < ask_volume_level.size()) {
            ask_levels.push_back(floatToStringWithPrecision(sorted_volume_levels[i].first, 2));
            ask_levels.push_back(std::to_string(sorted_volume_levels[i].second));
        }
        else {
            ask_levels.push_back("0");
            ask_levels.push_back("0");
        }

    }
    return ask_levels;

}

int OrderBook::total_bid_levels() {
    // Same as the previous one

    int totalSum = 0;

    // Iterate through the map and add the values together
    for (const auto& pair : bid_volume_level) {
        totalSum += pair.second;
    }
    return totalSum;

}

int OrderBook::total_ask_levels() {
    // Same as the previous one

    int totalSum = 0;

    // Iterate through the map and add the values together
    for (const auto& pair : ask_volume_level) {
        totalSum += pair.second;
    }
    return totalSum;

}

void OrderBook::clear_book() {
    // you need to clear the book completely
    // it means you need to remove all the bids
    // all the orders

    // at the end of this function you will display the number of bids and the asks
    // you remove
    int number_of_bids_deleted = 0;
    int number_of_asks_deleted = 0;

    for (auto it = bids.begin(); it != bids.end(); ) {
        delete it->second;
        it = bids.erase(it);
        number_of_bids_deleted++;

    }
    for (auto it = asks.begin(); it != asks.end(); ) {
        delete it->second;
        it = asks.erase(it);
        number_of_asks_deleted++;

    }
}