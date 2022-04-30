#include "Action.h"
#include "Studio.h"
#include "Customer.h"
#include "Trainer.h"
#include <string>
#include <iostream>

extern Studio *backup; //global variable

// Constructor
BaseAction::BaseAction() {}

ActionStatus BaseAction::getStatus() const {
    return status;

}

void BaseAction::error(std::string errorMsg) {
    status = ERROR;
    this->errorMsg = errorMsg;
}

void BaseAction::complete() {
    status = COMPLETED;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}

// Constructor
OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList) : trainerId(id), customers(customersList) {}

// Destructor
OpenTrainer::~OpenTrainer() {
    clear();
}

//Opens a given trainer's workout session and assigns a list of customers
void OpenTrainer::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || trainer->isOpen()) {
        error("Error: Trainer does not exist or is not open");
        std::cout << getErrorMsg() << std::endl;
        clear();// Costumers were created for this trainer, but we don't need them (for example we did open trainer for trainer that already open)
    } else if (trainer->getCapacity() <= 0) {
        error("Error: Not enough capacity.");
        std::cout << getErrorMsg() << std::endl;
        clear();// Costumers were created for this trainer, but we don't need them (for example not enough capacity)
    } else {
        // Adding each customer from customers list
        for (size_t i = 0; i < customers.size() && trainer->getCapacity() > 0; ++i)
            trainer->addCustomer(
                    customers[i]->clone()); // We keep copy of costumers for each trainer to separate deletion in the heap
        trainer->openTrainer();
        complete();
    }
}

//Prints the execution of open trainer
std::string OpenTrainer::toString() const {
    std::string ans = "open ";
    ans.append(std::to_string(trainerId) + " ");
    for (size_t i = 0; i < customers.size(); ++i)
        ans.append(customers[i]->getName() + "," + customers[i]->toString() + " ");
    ans = ans.substr(0, ans.size() - 1);
    if (getStatus() == ERROR)
        ans.append(" " + getErrorMsg());
    return ans;
}

// Creating a deep copy of the object for later backup use
OpenTrainer *OpenTrainer::clone() const {
    std::vector<Customer *> toCopy;
    for (size_t i = 0; i < customers.size(); i++)
        toCopy.push_back(customers[i]->clone());
    OpenTrainer *cloner = new OpenTrainer(trainerId, toCopy);
    return cloner;
}

void OpenTrainer::clear() {
    for (size_t i = 0; i < customers.size(); ++i) {
        if (customers[i] != nullptr) {
            delete customers[i]; // Deletion from heap of each costumer
            customers[i] = nullptr;
        }
    }
    customers.clear();
}


// Constructor
Order::Order(int id) : trainerId(id) {}

// Taking order from each customer in the trainer's group,and each customer order according to his strategy
void Order::act(Studio &studio) {

    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen()) { // Regarding exceptions according to assignment
        this->error("Trainer does not exist or is not open");
        std::cout << getErrorMsg() << std::endl;
    } else {
        std::vector<Customer *> customersList = trainer->getCustomers();
        std::vector<Workout> workout_options = studio.getWorkoutOptions();
        if (!trainer->getOrders().empty()) { // The empty is for a case that "order" will call twice one after one
            std::vector<OrderPair> orderList = trainer->getOrders();
            for (size_t i = 0; i < orderList.size(); ++i) {
                OrderPair pairToPush = orderList[i];
                std::string customerName = trainer->getCustomer(pairToPush.first)->getName();
                std::cout << customerName << " Is Doing " << pairToPush.second.getName()
                          << std::endl;
            }
        } else {
            for (size_t i = 0; i < customersList.size(); ++i) { // Taking order of each customer according to strategy
                std::vector<int> workout_ids = customersList[i]->order(workout_options);
                trainer->order(customersList[i]->getId(), workout_ids, workout_options);
            }
            std::vector<OrderPair> orderList = trainer->getOrders();
            for (size_t i = 0; i < orderList.size(); ++i) {
                OrderPair pairToPush = orderList[i];
                std::string customerName = trainer->getCustomer(pairToPush.first)->getName();
                std::cout << customerName << " Is Doing " << pairToPush.second.getName()
                          << std::endl;
            }
            complete();
        }
    }
}

std::string Order::toString() const {
    std::string ans = "order ";
    ans.append(std::to_string(trainerId));
    if (getStatus() == ERROR)
        ans.append(" Error: " + getErrorMsg());
    return ans;
}

Order *Order::clone() const {
    return new Order(trainerId);
}


// Constructor
MoveCustomer::MoveCustomer(int src, int dst, int customerId) : srcTrainer(src), dstTrainer(dst), id(customerId) {}

//Moves a customer from one trainer to another
void MoveCustomer::act(Studio &studio) {
    Trainer *trainerSrc = studio.getTrainer(srcTrainer);
    Trainer *trainerDst = studio.getTrainer(dstTrainer);
    Customer *customer = trainerSrc->getCustomer(id);
    if (trainerSrc == nullptr || trainerDst == nullptr || !trainerSrc->isOpen() || !trainerDst->isOpen()
        || trainerDst->getCapacity() <= 0 || customer == nullptr) { // Regarding exceptions according to assignment
        error("Cannot move customer");
        std::cout << getErrorMsg() << std::endl;
    } else {
        trainerDst->addCustomer(customer);
        std::vector<OrderPair> ordersSRC = trainerSrc->getOrders();
        std::vector<OrderPair> ordersDST = trainerSrc->getOrders();
        // Update each trainer salary and orders list
        for (size_t i = 0; i < ordersSRC.size(); ++i) {
            if (ordersSRC[i].first == id) {
                ordersDST.push_back(ordersSRC[i]);
                trainerSrc->changSalary(-ordersSRC[i].second.getPrice());
                trainerDst->changSalary(ordersSRC[i].second.getPrice());
            }
        }
        trainerSrc->removeCustomer(id);
        complete();
    }
}

std::string MoveCustomer::toString() const {
    std::string ans = "move ";
    ans.append(std::to_string(srcTrainer) + " " + std::to_string(dstTrainer) + " " + std::to_string(id) + " ");
    if (getStatus() == ERROR)
        ans.append("Error: " + getErrorMsg());
    return ans;
}

MoveCustomer *MoveCustomer::clone() const {
    return new MoveCustomer(srcTrainer, dstTrainer, id);
}


// Constructor
Close::Close(int id) : trainerId(id) {}

//Closes a given trainer session
void Close::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    if (trainer == nullptr || !trainer->isOpen()) { // Regarding exceptions according to assignment
        error("Trainer does not exist or is not open");
        std::cout << getErrorMsg() << std::endl;
    } else {
        if (trainer->getCustomers().size() !=
            0) { // We don't want to print in case of  closing a trainer by action of - "Move Customer"
            trainer->closeTrainer();
            std::cout << "Trainer " << trainerId << " closed. " << "Salary " << trainer->getSalary() << "NIS"
                      << std::endl;
            complete();
        } else
            trainer->closeTrainer();
        complete();
    }
}

std::string Close::toString() const {
    std::string ans = "close ";
    ans.append(std::to_string(trainerId));
    if (getStatus() == ERROR)
        ans.append(" Error: " + getErrorMsg());
    return ans;
}

Close *Close::clone() const {
    return new Close(trainerId);
}


// Constructor
CloseAll::CloseAll() {}

// Closes all workout sessions in the studio, and then closes the studio and exits
void CloseAll::act(Studio &studio) {
    int numOfTrainers = studio.getNumOfTrainers();
    // Loo for closing each trainer in the studio
    for (size_t tid = 0; tid < (size_t) numOfTrainers; ++tid) {
        if (studio.getTrainer(tid)->isOpen())
            std::cout << "Trainer " << tid << " closed. Salary " << studio.getTrainer(tid)->getSalary() << "NIS"
                      << std::endl;
        studio.getTrainer(tid)->closeTrainer();
    }
    complete();
}

std::string CloseAll::toString() const {
    return "closeall";
}

CloseAll *CloseAll::clone() const {
    return new CloseAll();
}


// Constructor
PrintWorkoutOptions::PrintWorkoutOptions() {}

// Prints the available workout options of the studio
void PrintWorkoutOptions::act(Studio &studio) {
    std::vector<Workout> workoutOptions = studio.getWorkoutOptions();
    for (size_t i = 0; i < workoutOptions.size(); ++i) {
        std::cout << workoutOptions[i].getName() << " " << workoutOptions[i].getStrType() << " "
                  << workoutOptions[i].getPrice() << std::endl;
    }
    complete();
}

std::string PrintWorkoutOptions::toString() const {
    return "workout_options";
}

PrintWorkoutOptions *PrintWorkoutOptions::clone() const {
    return new PrintWorkoutOptions();
}


// Constructor
PrintTrainerStatus::PrintTrainerStatus(int id) : trainerId(id) {}

// Prints a status report of a given trainer.
void PrintTrainerStatus::act(Studio &studio) {
    Trainer *trainer = studio.getTrainer(trainerId);
    std::string toPrint = "Trainer " + std::to_string(trainerId);
    toPrint.append(" status: ");
    if (trainer->isOpen())
        toPrint.append("open");
    else
        toPrint.append("closed");
    std::cout << toPrint << std::endl;
    if (trainer->isOpen() &&
        !trainer->getOrders().empty()) { // If there is no order we print only if the trainer is close/open
        std::vector<Customer *> customerList = trainer->getCustomers();
        std::vector<OrderPair> orderList = trainer->getOrders();
        std::vector<Workout> workoutOptions = studio.getWorkoutOptions();
        std::cout << "Customers:" << std::endl;
        for (size_t i = 0; i < customerList.size(); ++i)
            std::cout << customerList[i]->getId() << " " << customerList[i]->getName() << std::endl;
        std::cout << "Orders:" << std::endl;
        for (size_t i = 0; i < orderList.size(); ++i)
            std::cout << orderList[i].second.getName() << " " << orderList[i].second.getPrice() << "NIS "
                      << orderList[i].first << std::endl;
        std::cout << "Current Trainer’s Salary: " << trainer->getSalary() << "NIS" << std::endl;
        complete();
    }
}

std::string PrintTrainerStatus::toString() const {
    std::string ans = "status ";
    ans.append(std::to_string(trainerId));
    return ans;
}

PrintTrainerStatus *PrintTrainerStatus::clone() const {
    return new PrintTrainerStatus(trainerId);
}

// Constructor
PrintActionsLog::PrintActionsLog() {}

// Prints all the actions that were performed by the user from the first action to the last action
void PrintActionsLog::act(Studio &studio) {
    std::vector<BaseAction *> actionsLog = studio.getActionsLog();
    // Loop for the prints of each action
    for (size_t i = 0; i < actionsLog.size(); ++i) {
        if (actionsLog[i]->getStatus() == COMPLETED)
            std::cout << actionsLog[i]->toString() << " " << "Completed" << std::endl;
        else
            std::cout << actionsLog[i]->toString() << std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log";
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog();
}


// Constructor
BackupStudio::BackupStudio() : BaseAction() {}

// Save all studio information
void BackupStudio::act(Studio &studio) {
    if (backup != nullptr)
        delete backup;
    backup = new Studio(studio);
    complete();
}

std::string BackupStudio::toString() const {
    return "backup";
}

BackupStudio *BackupStudio::clone() const {
    return new BackupStudio();
}


// Constructor
RestoreStudio::RestoreStudio() {}

// Restore the backed-up studio status and overwrite the current studio status
void RestoreStudio::act(Studio &studio) {
    if (backup == nullptr) {
        error("No backup available");
        std::cout << getErrorMsg() << std::endl;
    } else {
        studio = *backup;
        complete();
    }
}

std::string RestoreStudio::toString() const {
    std::string ans = "restore";
    if (getStatus() == ERROR)
        ans.append(" Error: " + getErrorMsg());
    return ans;
}

RestoreStudio *RestoreStudio::clone() const {
    return new RestoreStudio();
}