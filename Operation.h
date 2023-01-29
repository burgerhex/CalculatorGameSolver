//
// Created by avivs on 11/6/2021.
//

#ifndef CALCULATORGAMESOLVER_OPERATION_H
#define CALCULATORGAMESOLVER_OPERATION_H

#include <string>
#include <memory>
#include <vector>

#define DEBUG 0

// common to all operations
#define OPERATION_METHODS                                           \
public:                                                             \
void action(int& display) override;                                 \
void mutate_by(const MutatorOperation& op, bool unmutate) override; \
private:                                                            \
std::string to_string() override;

// common to all mutators
#define MUTATOR_OPERATION_METHODS                                      \
public:                                                                \
void action(int& display) override {};                                 \
void mutate_by(const MutatorOperation& op, bool unmutate) override {}; \
void mutate(int& val, bool unmutate) const override;                   \
private:                                                               \
std::string to_string() override;

class MutatorOperation;

class Operation {
public:
    // adjusts display and returns whether buttons should be added
    virtual void action(int& display) = 0;

    virtual void mutate_by(const MutatorOperation& op, bool unmutate) = 0;

    virtual ~Operation();

    virtual std::string to_string() = 0;

    bool is_mutator() { return _is_mutator; }

    bool is_memory() { return _is_memory; }

    bool is_store() { return _is_store; }

protected:
    bool _is_mutator = false;
    bool _is_memory = false;
    bool _is_store = false;

    Operation() : Operation(false, false, false) {};

    Operation(bool is_mutator, bool is_memory, bool is_store) : _is_mutator(is_mutator), _is_memory(is_memory),
                                                                _is_store(is_store) {};

    static int safe_to_int(const std::string& str);
};

class MutatorOperation : public Operation {
public:
    // adjusts display and returns whether buttons should be added
    virtual void mutate(int& val, bool unmutate) const = 0;

    virtual ~MutatorOperation();

    virtual std::string to_string();

protected:
    MutatorOperation() : Operation(true, false, false) {};
};

class Add : public Operation {
public:
    explicit Add(int val);

OPERATION_METHODS

private:
    int addVal;
};

class MutatorAdd : public MutatorOperation {
public:
    explicit MutatorAdd(int val);

MUTATOR_OPERATION_METHODS

private:
    int addVal;
};

class Subtract : public Operation {
public:
    explicit Subtract(int val);

OPERATION_METHODS

private:
    int subVal;
};

class Multiply : public Operation {
public:
    explicit Multiply(int val);

OPERATION_METHODS

private:
    int mulVal;
};

class Divide : public Operation {
public:
    explicit Divide(int val);

OPERATION_METHODS

private:
    int divVal;
};

class Delete : public Operation {
public:
    Delete();

OPERATION_METHODS
};

class Insert : public Operation {
public:
    explicit Insert(int val);

OPERATION_METHODS

private:
    int insVal;
};

class Replace : public Operation {
public:
    Replace(int val1, const std::string& val2);

OPERATION_METHODS

private:
    int toReplace;
    std::string replaceWith;
};

class Reverse : public Operation {
public:
    Reverse();

OPERATION_METHODS
};

class PlusMinus : public Operation {
public:
    PlusMinus();

OPERATION_METHODS
};

class Sum : public Operation {
public:
    Sum();

OPERATION_METHODS
};

class Cube : public Operation {
public:
    Cube();

OPERATION_METHODS
};

class Shift : public Operation {
public:
    explicit Shift(bool val);

OPERATION_METHODS

private:
    bool is_left;
};

class Mirror : public Operation {
public:
    Mirror();

OPERATION_METHODS
};

class Memory : public Operation {
public:
    Memory();

    int get_memory();

    void set_memory(int memory);

    void pop_memory();

OPERATION_METHODS

private:
    std::vector<int> past_memories;
};

class Store : public Operation {
public:
    explicit Store(std::shared_ptr<Memory> memory);
    std::shared_ptr<Memory> memory;

OPERATION_METHODS
};

class Inv10 : public Operation {
public:
    Inv10();

OPERATION_METHODS
};


#endif //CALCULATORGAMESOLVER_OPERATION_H
