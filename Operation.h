//
// Created by avivs on 11/6/2021.
//

#ifndef CALCULATORGAMESOLVER_OPERATION_H
#define CALCULATORGAMESOLVER_OPERATION_H

#include <string>

class Operation {
  public:
    // adjusts display and returns whether buttons should be added
    virtual bool action(int& display) = 0;
    virtual ~Operation();
    virtual std::string to_string();
};

class Add : public Operation {
  public:
    explicit Add(int val);
    bool action(int& display) override;
  private:
    int addVal;
    std::string to_string() override;
};

class Subtract : public Operation {
  public:
    explicit Subtract(int val);
    bool action(int& display) override;
  private:
    int subVal;
    std::string to_string() override;
};

class Multiply : public Operation {
  public:
    explicit Multiply(int val);
    bool action(int& display) override;
  private:
    int mulVal;
    std::string to_string() override;
};

class Divide : public Operation {
  public:
    explicit Divide(int val);
    bool action(int& display) override;
  private:
    int divVal;
    std::string to_string() override;
};

class Delete : public Operation {
  public:
    Delete();
    bool action(int& display) override;
  private:
    std::string to_string() override;
};

class Insert : public Operation {
  public:
    explicit Insert(int val);
    bool action(int& display) override;
  private:
    int insVal;
    std::string to_string() override;
};


#endif //CALCULATORGAMESOLVER_OPERATION_H
