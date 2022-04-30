#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"
#include "Trainer.h"

enum ActionStatus {
    COMPLETED, ERROR
};

//Forward declaration
class Studio;

class BaseAction {
public:

    // Constructor
    BaseAction();

    // Destructor (default)
    virtual ~BaseAction() = default;

    ActionStatus getStatus() const;

    virtual void act(Studio &studio) = 0;

    virtual std::string toString() const = 0;

    virtual BaseAction *clone() const = 0;

protected:
    void complete();

    void error(std::string errorMsg);

    std::string getErrorMsg() const;

private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTrainer : public BaseAction {
public:

    // Constructor
    OpenTrainer(int id, std::vector<Customer *> &customersList);

    // Destructor
    virtual ~OpenTrainer();

    void act(Studio &studio);

    std::string toString() const;

    virtual OpenTrainer *clone() const;


private:
    const int trainerId;
    std::vector<Customer *> customers;

    void clear();
};


class Order : public BaseAction {
public:
    Order(int id);

    void act(Studio &studio);

    std::string toString() const;

    virtual Order *clone() const;


private:
    const int trainerId;
};


class MoveCustomer : public BaseAction {
public:

    // Constructor
    MoveCustomer(int src, int dst, int customerId);

    void act(Studio &studio);

    std::string toString() const;

    virtual MoveCustomer *clone() const;


private:
    const int srcTrainer;
    const int dstTrainer;
    const int id;
};


class Close : public BaseAction {
public:

    // Constructor
    Close(int id);

    void act(Studio &studio);

    std::string toString() const;

    virtual Close *clone() const;


private:
    const int trainerId;
};


class CloseAll : public BaseAction {
public:

    // Constructor
    CloseAll();

    void act(Studio &studio);

    std::string toString() const;

    virtual CloseAll *clone() const;


private:
};


class PrintWorkoutOptions : public BaseAction {
public:

    // Constructor
    PrintWorkoutOptions();

    void act(Studio &studio);

    std::string toString() const;

    virtual PrintWorkoutOptions *clone() const;


private:
};


class PrintTrainerStatus : public BaseAction {
public:

    // Constructor
    PrintTrainerStatus(int id);

    void act(Studio &studio);

    std::string toString() const;

    virtual PrintTrainerStatus *clone() const;


private:
    const int trainerId;
};


class PrintActionsLog : public BaseAction {
public:

    // Constructor
    PrintActionsLog();

    void act(Studio &studio);

    std::string toString() const;

    virtual PrintActionsLog *clone() const;


private:
};


class BackupStudio : public BaseAction {
public:

    // Constructor
    BackupStudio();

    void act(Studio &studio);

    std::string toString() const;

    virtual BackupStudio *clone() const;

private:
};


class RestoreStudio : public BaseAction {
public:

    // Constructor
    RestoreStudio();

    void act(Studio &studio);

    std::string toString() const;

    virtual RestoreStudio *clone() const;

};


#endif