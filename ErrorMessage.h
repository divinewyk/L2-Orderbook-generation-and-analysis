#pragma once

#include "Order.h"
void display_error_order_id_exist(const Order& o);

void display_error_exist_order_modification(const Order& o);

void display_error_amendment_price(const Order& o);

void display_error_order_not_consistent_deletion(const Order& o);

void display_error_order_not_existing_deletion(const Order& o);