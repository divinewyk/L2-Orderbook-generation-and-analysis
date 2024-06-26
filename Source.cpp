
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "OrderBook.h"

const std::string convert_side(const Order &o)
{
    return (o.is_bid) ? "bid" : "ask";
}

void generate_order_book_output(std::string filename)
{
    std::ifstream file("input/" + filename);
    std::ofstream outputfile("./output_final/output_" + filename);

    // Generate header row
    outputfile << "timestamp,price,side";
    for (int i = 0; i < 5; i++)
    {
        outputfile << ","
                   << "bp" << i;
        outputfile << ","
                   << "bq" << i;
    }
    for (int i = 0; i < 5; i++)
    {
        outputfile << ","
                   << "ap" << i;
        outputfile << ","
                   << "aq" << i;
    }
    outputfile << ","
               << "total_bid_levels";
    outputfile << ","
               << "total_ask_levels";
    outputfile << std::endl;
    if (!file.is_open())
    {
        std::cerr << "Failed to open the file!" << std::endl;
        return;
    }
    std::string line;
    int count = 0;

    // Skip the header row
    std::getline(file, line);

    OrderBook main_order_book;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::vector<std::string> row;
        std::string cell_value;
        count++;
        // Get each line of order update from input
        while (std::getline(ss, cell_value, ','))
        {
            row.push_back(cell_value);
        }

        // Output a count every 100000 as progress bar update
        if (count % 100000 == 0)
        {
            std::cout << count << std::endl;
        }
        unsigned long long timestamp = std::stoull(row[0]);
        bool is_bid = (row[1] == "b") ? true : false;
        order_id_t order_id = std::stoi(row[3]);
        price_t price = std::stof(row[4]);
        quantity_t quantity = std::stoi(row[5]);

        // all of the orders in the exercise are good till day end limit orders
        // assume order venue is all from CME exchange
        Order e(timestamp, is_bid, order_id, price, quantity, ORDERTYPE::GTD);

        // perform three kinds of order actions
        if (row[2] == "a")
        {
            OrderAction oa(e, ACTION::ADD);
            main_order_book.process_order(oa);
        }
        else if (row[2] == "m")
        {
            OrderAction oa(e, ACTION::MODIFY);
            main_order_book.process_order(oa);
        }
        else if (row[2] == "d")
        {
            OrderAction oa(e, ACTION::DELETE);
            main_order_book.process_order(oa);
        }
        else
        {
            std::cerr << "Unknown order action!" << std::endl;
            continue;
        }

        outputfile << timestamp << "," << price << "," << row[1];
        std::vector<std::string> bid_levels = main_order_book.get_bid_top_k_levels();
        std::vector<std::string> ask_levels = main_order_book.get_ask_top_k_levels();
        int total_bid_levels = main_order_book.total_bid_levels();
        int total_ask_levels = main_order_book.total_ask_levels();

        // Output 5 bid levels
        for (const std::string &value : bid_levels)
        {
            outputfile << "," << value;
        }

        // Output 5 ask levels
        for (const std::string &value : ask_levels)
        {
            outputfile << "," << value;
        }

        // Output total sum of bid and ask levels, additonal features
        outputfile << "," << total_bid_levels;
        outputfile << "," << total_ask_levels;
        outputfile << std::endl;
        // if (count > 5) {
        //     break;
        // }
    }
    outputfile.close();
    main_order_book.clear_book();
    std::cout << "finished reading and generated output for " << filename << std::endl;
}

void test_case0()
{
    // test case to test duplicate order creation
    generate_order_book_output("test_case0.csv");
}

void test_case1()
{
    // test case to test deleting non-existent order
    generate_order_book_output("test_case1.csv");
}

int main()
{

    std::vector<std::string> dates = {"20190610", "20190611", "20190612", "20190613", "20190614"};

    /*test_case0();
    test_case1();
    return 0;*/
    for (const std::string date : dates)
    {
        generate_order_book_output("res_" + date + ".csv");
    }
}
