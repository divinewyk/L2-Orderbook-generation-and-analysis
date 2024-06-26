# L2 book and feature generation

This is a completed C++
implementation for building L2 top 5 bid ask levels and other features from L3 +  Python features analysis.

The implementation has key points:
1. Abstraction: Object oriented (order class, orderbook class)
<br/><br/>
2. Encapsulation: Separation of interface and implementation
    - User only interacts with the interface of orderbook to process orders
<br/><br/>
3. Resource memory management: 
    - dynamically allocated memory to store order and release after emptying orderbook
<br/><br/>
4. Error Handling:
    - While the given dataset does not have any data problem, will output error in cases such as adding duplicate orders or modifying/ deleting a non-existent order
<br/><br/>
5. Test cases to test the error handling

Welcome any discussions or comments!
