//
// Created by avivs on 11/6/2021.
//

#ifndef CALCULATORGAMESOLVER_SOLVER_H
#define CALCULATORGAMESOLVER_SOLVER_H


#include <string>
#include <vector>
#include "Operation.h"

class Solver {
  private:
    std::vector<Operation*> buttons;
    int start, goal, moves;

  public:
    explicit Solver(const std::string& level_file) : start(0), goal(0), moves(0) {
        read_file(level_file);
    }

    std::vector<Operation*> solve() {
        std::vector<Operation*> buttons_pushed;
        return *solve_helper(buttons_pushed, start, moves, nullptr);
    }

  private:
    void read_file(const std::string& level_file);

    std::vector<Operation*>* solve_helper(std::vector<Operation*>& buttons_pushed, int display,
                                          int moves_left, Operation* do_this) {
        if (do_this)
            moves_left -= do_this->action(display);

        if (moves_left == 0 && display != goal)
            return nullptr;
        else if (display == goal)
            return &buttons_pushed;

        for (Operation* button : buttons) {
            buttons_pushed.push_back(button);
//            bool useMoves = button->action(display);
            std::vector<Operation*>* result = solve_helper(buttons_pushed, display,
                                                           moves_left, button);
            if (result)
                return result;
            buttons_pushed.pop_back();
        }

        return nullptr;
    }
};


#endif //CALCULATORGAMESOLVER_SOLVER_H
