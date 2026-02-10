#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class TestRunner {
   public:
    void run(const string& name, function<void()> test) {
        try {
            test();
            cout << "[PASS] " << name << endl;
            passed_++;
        } catch (const exception& e) {
            cout << "[FAIL] " << name << ": " << e.what() << endl;
            failed_++;
        } catch (...) {
            cout << "[FAIL] " << name << ": Unknown error" << endl;
            failed_++;
        }
    }

    void summary() const {
        cout << "\nTest Summary: " << passed_ << " passed, " << failed_
             << " failed." << endl;
    }

    int getFailed() const { return failed_; }

   private:
    int passed_ = 0;
    int failed_ = 0;
};
