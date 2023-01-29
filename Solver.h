//
// Created by avivs on 11/6/2021.
//

#ifndef CALCULATORGAMESOLVER_SOLVER_H
#define CALCULATORGAMESOLVER_SOLVER_H


#include <string>
#include <numeric>
#include <iostream>
#include "Operation.h"

#define MAX_DISPLAY 999999

class Solver {
public:
    std::vector<std::shared_ptr<Operation>> buttons;
    int start, goal, moves;
    int portal_in, portal_out;

    explicit Solver(const std::string& level_file) : start(0), goal(0), moves(0), portal_in(-1), portal_out(-1) {
        read_file(level_file);
    }

    std::vector<int> get_solution() {
        std::vector<int> buttons_pushed;
        std::vector<std::shared_ptr<MutatorOperation>> mutators;
        solve_helper(buttons_pushed, start, moves, -1, mutators);
        return buttons_pushed;
    }

    static void solve(const std::string& file_name, bool try_getting_smallest = true) {
        Solver s(file_name);

        int start = s.start;
        int goal = s.goal;
        int moves = s.moves;
        const std::vector<std::shared_ptr<Operation>>& buttons = s.buttons;
        std::vector<std::shared_ptr<Operation>> buttons_to_show;

        for (const std::shared_ptr<Operation>& button : buttons) {
            if (!button->is_memory())
                buttons_to_show.push_back(button);
        }

        std::cout << "Start: " << start << "\nGoal: " << goal << "\nMoves: " << moves << "\n";

        if (s.portal_in >= 0 && s.portal_out >= 0) {
            std::cout << "Portal from position " << (s.portal_in + 1) << " to position " << (s.portal_out + 1) << "\n";
        }

        std::cout << "Buttons: ";

        for (int i = 0; i < buttons_to_show.size() - 1; i++) {
            std::cout << buttons_to_show[i]->to_string() << ", ";
        }

        std::cout << buttons_to_show.back()->to_string()
                  << "\nAttempting to find solution..." << std::endl;

        if (try_getting_smallest) {
            for (int new_moves = 1; new_moves <= moves; new_moves++) {
                s.moves = new_moves;
                std::vector<int> solution = s.get_solution();

                if (!solution.empty()) {
                    std::cout << "Solution found!";
                    int moves_spared = moves - new_moves;
                    if (moves_spared > 0)
                        std::cout << " (with " << moves_spared << " move" << ((moves_spared == 1) ? "" : "s")
                                  << " to spare!)";
                    std::cout << std::endl;

                    s.print_steps(solution);
                    return;
                }
            }

            std::cout << "No solution found. :(" << std::endl;
        } else {
            std::vector<int> solution = s.get_solution();

            if (solution.empty()) {
                std::cout << "No solution found. :(" << std::endl;
            } else {
                std::cout << "Solution found!" << std::endl;
                s.print_steps(solution);
            }
        }
    }

private:
    int portal_out_mul, portal_in_mul;

    void read_file(const std::string& level_file);

    bool solve_helper(std::vector<int>& buttons_pushed, int display, int moves_left, int button_to_push,
                      std::vector<std::shared_ptr<MutatorOperation>>& mutators_to_apply) {
#if DEBUG
        std::string s = button_list_to_string(buttons_pushed);
#endif
        if (button_to_push >= 0) {
            std::shared_ptr<Operation> o = buttons[button_to_push];
            for (const std::shared_ptr<MutatorOperation>& m : mutators_to_apply) {
                o->mutate_by(*m, false);
            }
            o->action(display);
            moves_left--;
            for (const std::shared_ptr<MutatorOperation>& m : mutators_to_apply) {
                o->mutate_by(*m, true);
            }

#if DEBUG
            std::string indent(moves - moves_left - 1, '\t');
            std::cout << indent << "Trying button " << buttons[button_to_push]->to_string() << " (index "
                      << button_to_push << "), display is now " << display << ". ";
#endif

            // apply portal
            while (portal_in >= 0 && portal_out >= 0 && display >= portal_in_mul) {
                int ahead = display / portal_in_mul;
                int remaining = display % portal_in_mul;
                int stay_in_front = ahead / 10;
                int to_add_before_mul = ahead % 10;
                display = stay_in_front * portal_in_mul + remaining + to_add_before_mul * portal_out_mul;
#if DEBUG
                std::cout << indent << "Applying portal, display is now " << display << ". ";
#endif
            }
        }

        if (moves_left == 0 && display != goal) {
#if DEBUG
            std::cout << "Out of moves, going up with FALSE.\n";
#endif
            return false;
        } else if (display == INT_MAX || display > MAX_DISPLAY) {
#if DEBUG
            std::cout << "Invalid result, going up with FALSE.\n";
#endif
            return false;
        } else if (display == goal) {
#if DEBUG
            std::cout << "Goal reached, going up with TRUE.\n";
#endif
            return true;
        } else if (button_to_push >= 0) {
#if DEBUG
            std::cout << "Unsuccessful, continuing.\n";
#endif
        }

        for (int i = 0; i < buttons.size(); i++) {
            buttons_pushed.push_back(i);
            std::shared_ptr<MutatorOperation> mut;
            std::shared_ptr<Store> mem_store;
            if (buttons[i]->is_mutator()) {
                mut = static_pointer_cast<MutatorOperation>(buttons[i]);
                mutators_to_apply.push_back(mut);
            } else if (buttons[i]->is_store()) {
                mem_store = std::static_pointer_cast<Store>(buttons[i]);
            }
            bool result = solve_helper(buttons_pushed, display, moves_left, i, mutators_to_apply);
            if (result) {
                return true;
            }
            buttons_pushed.pop_back();
            if (mut) {
                mutators_to_apply.pop_back();
            }
            if (mem_store) {
                mem_store->memory->pop_memory();
            }
        }

        return false;
    }

#if DEBUG
    std::string buttons_list_to_string() {
        std::vector<int> all(buttons.size());
        std::iota(all.begin(), all.end(), 0);
        return button_list_to_string(all);
    }

    std::string button_list_to_string(const std::vector<int>& indices) {
        if (indices.size() == 0)
            return "{}";

        std::string result = "{";

        for (int i = 0; i < indices.size() - 1; i++) {
            result += buttons[indices[i]]->to_string() + ", ";
        }

        return result + buttons[indices.back()]->to_string() + "}";
    }
#endif

    void print_steps(const std::vector<int>& solution) {
        int x = start;
        int move = 0;
        std::vector<std::shared_ptr<MutatorOperation>> mutations_to_apply;
        for (int i : solution) {
            std::shared_ptr<Operation> o = buttons[i];
            std::shared_ptr<MutatorOperation> mutation;
            if (o->is_mutator()) {
                mutation = std::static_pointer_cast<MutatorOperation>(o);
                mutations_to_apply.push_back(mutation);
            }
            for (const std::shared_ptr<MutatorOperation>& m : mutations_to_apply) {
                o->mutate_by(*m, false);
            }
            o->action(x);
            std::cout << "Step " << (++move) << ":\t" << o->to_string() << " --> " << x;
            for (const std::shared_ptr<MutatorOperation>& m : mutations_to_apply) {
                o->mutate_by(*m, true);
            }

            // apply portal
            while (portal_in >= 0 && portal_out >= 0 && x >= portal_in_mul) {
                int ahead = x / portal_in_mul;
                int remaining = x % portal_in_mul;
                int stay_in_front = ahead / 10;
                int to_add_before_mul = ahead % 10;
                x = stay_in_front * portal_in_mul + remaining + to_add_before_mul * portal_out_mul;
                std::cout << " --> " << x;
            }

            std::cout << std::endl;
        }
    }
};


#endif //CALCULATORGAMESOLVER_SOLVER_H
