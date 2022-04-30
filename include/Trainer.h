#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer {
public:

    // Constructor
    Trainer(int t_capacity);

    // Destructor
    virtual ~Trainer();

    // Copy Constructor
    Trainer(const Trainer &aTrainer);

    // Move Constructor
    Trainer(Trainer &&other);

    // Copy Assigment
    Trainer &operator=(const Trainer &trainer);

    // Move Assigment
    Trainer &operator=(Trainer &&other);

    int getCapacity() const;

    void addCustomer(Customer *customer);

    void removeCustomer(int id);

    Customer *getCustomer(int id);

    std::vector<Customer *> &getCustomers();

    std::vector<OrderPair> &getOrders();

    void order(const int customer_id, const std::vector<int> &workout_ids, const std::vector<Workout> &workout_options);

    void openTrainer();

    void closeTrainer();

    int getSalary() const;

    bool isOpen();

    int searchById(int id) const; // Returns the index of customer with id num in the customers list

    void changSalary(int amount); // Change trainer salary

    int origCap; // Field for the original capacity pf the trainer

private:
    int capacity;
    bool open;
    std::vector<Customer *> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    int salary;

    void clear();
    void copy(const int other_capacity, const bool other_open, const std::vector<Customer *> &other_customersList,
              const std::vector<OrderPair> &other_orderList, const int other_salary, const int origCap);
};


#endif