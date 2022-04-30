#include <vector>
#include "Customer.h"
#include "Workout.h"
#include "Trainer.h"

typedef std::pair<int, Workout> OrderPair;

// Constructor
Trainer::Trainer(int _capacity) : capacity(_capacity) {
    open = false;
    customersList = std::vector<Customer *>();
    orderList = std::vector<OrderPair>();
    origCap = _capacity;
    salary = 0;
}

// Destructor
Trainer::~Trainer() { clear(); }


// Copy Constructor
Trainer::Trainer(const Trainer &other) {
    copy(other.capacity, other.open, other.customersList, other.orderList, other.salary, other.origCap);
}

// Move Constructor
Trainer::Trainer(Trainer &&other) : origCap(other.origCap), capacity(other.capacity),
                                    open(other.open), customersList(std::move(other.customersList)),
                                    orderList(std::move(other.orderList)),
                                    salary(other.salary) {
    other.salary = 0;
    other.capacity = 0;
    other.origCap = 0;
    other.open = false;
}


// Copy Assigment
Trainer &Trainer::operator=(const Trainer &other) {
    if (this != &other) {
        clear();
        copy(other.capacity, other.open, other.customersList, other.orderList, other.salary, other.origCap);
    }
    return *this;
}

// Move Assigment
Trainer &Trainer::operator=(Trainer &&other) {
    if (this != &other) {
        clear();
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        origCap = other.origCap;
        for (size_t i = 0; i < customersList.size(); i++)
            customersList[i] = other.customersList[i];
        for (size_t i = 0; i < other.orderList.size(); ++i)
            orderList.push_back(other.orderList[i]);
        other.customersList.clear();
        other.orderList.clear();
        other.open = false;
        other.capacity = 0;
        other.origCap = 0;
        other.salary = 0;
    }
    return *this;
}

int Trainer::getCapacity() const {
    return capacity;
}

// Adding new customer to trainer's customer list
void Trainer::addCustomer(Customer *customer) {
    customersList.push_back(customer);
    capacity--;
}

// Removing a customer from trainer's customer list
void Trainer::removeCustomer(int id) {
    int indForDelete = searchById(id); // Returns the index of customer with id num in the customers list
    customersList.erase(customersList.begin() + indForDelete);
    capacity++;
    std::vector<OrderPair> newOrderList;
    // The next two loop is for deleting the trainer from the order list (it's done like that because from some reason it's not possible to erase from orderList
    for (size_t i = 0; i < orderList.size(); ++i) {
        if (orderList[i].first != id)
            newOrderList.push_back(orderList[i]);
    }
    orderList.clear();
    for (size_t i = 0; i < newOrderList.size(); i++)
        orderList.push_back(newOrderList[i]);
}

Customer *Trainer::getCustomer(int id) {
    for (size_t i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id)
            return customersList[i];
    }
    return nullptr; // In case that the trainer doesn't exist
}

std::vector<Customer *> &Trainer::getCustomers() {
    return customersList;
}

std::vector<OrderPair> &Trainer::getOrders() {
    return orderList;
}

// Adding a specific customer to the order list and update salary
void Trainer::order(const int customer_id, const std::vector<int> &workout_ids,
                    const std::vector<Workout> &workout_options) {
    for (size_t i = 0; i < workout_ids.size(); ++i) {
        OrderPair pairToPush(customer_id, workout_options[workout_ids[i]]);
        orderList.push_back(pairToPush);
        salary += pairToPush.second.getPrice();
    }
}

void Trainer::openTrainer() {
    open = true;
}

// Closing the trainer by deleting all his customers and "restore" his fields except from the salary
void Trainer::closeTrainer() {
    for (size_t i = 0; i < customersList.size(); ++i) { //todo check if other people did that
        if (customersList[i] != nullptr) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
    }
    customersList.clear();
    orderList.clear();
    open = false;
    capacity = origCap;
}

int Trainer::getSalary() const {
    return salary;
}

bool Trainer::isOpen() {
    return open;

}

// Returns the index of customer with id num in the customers list
int Trainer::searchById(int id) const {
    for (size_t i = 0; i < customersList.size(); ++i) {
        if (customersList[i]->getId() == id)
            return i;
    }
    return -1;
}

void Trainer::changSalary(int amount) {
    salary += amount;
}

void Trainer::clear() {
    for (size_t i = 0; i < customersList.size(); i++) {
        if (customersList[i] != nullptr) {
            delete customersList[i];
            customersList[i] = nullptr;
        }
    }
    customersList.clear();
    orderList.clear();
    salary = 0;
    capacity = 0;
    origCap = 0;
    open = false;
}

// Deep Copy
void Trainer::copy(const int other_capacity, const bool other_open, const std::vector<Customer *> &other_customersList,
                   const std::vector<OrderPair> &other_orderList, const int other_salary, const int other_origCap) {
    open = other_open;
    salary = other_salary;
    capacity = other_capacity;
    origCap = other_origCap;
    for (size_t i = 0; i < other_customersList.size(); i++) {
        Customer *customer = other_customersList[i]->clone();
        customersList.push_back(customer);
    }
    for (size_t i = 0; i < other_orderList.size(); i++) {
        Workout tmp = other_orderList[i].second;
        Workout workout(tmp.getId(), tmp.getName(), tmp.getPrice(), tmp.getType());
        int id = other_orderList[i].first;
        OrderPair pair(id, tmp);
        orderList.push_back(pair);
    }
}
