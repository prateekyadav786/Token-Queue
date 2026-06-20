/*
 * Automated Token Queuing System
 * Author: Prateek Yadav
 * GitHub: github.com/prateekyadav786
 *
 * Description:
 *   Modular FIFO simulation engine using C++ STL Queue
 *   with buffer-overflow protection, dynamic sequence
 *   tracking, and 4-module architecture.
 *
 * Compile (Termux):
 *   g++ -o token_queue project2_token_queue.cpp
 *
 * Run:
 *   ./token_queue
 */

#include <iostream>
#include <queue>
#include <deque>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdexcept>

using namespace std;

// ─────────────────────────────────────────────
// Token structure
// ─────────────────────────────────────────────
struct Token {
    int         id;
    string      data;
    string      status;   // "PENDING", "PROCESSED", "INVALID"
    int         priority; // 1 = high, 2 = normal, 3 = low

    Token(int i, const string& d, int p = 2)
        : id(i), data(d), status("PENDING"), priority(p) {}
};

// ─────────────────────────────────────────────
// MODULE 1: Intake — validates and enqueues tokens
// ─────────────────────────────────────────────
class IntakeModule {
private:
    int nextId;

public:
    IntakeModule() : nextId(1) {}

    // Validate token data
    bool validate(const string& data) {
        if (data.empty()) return false;
        if (data.length() > 256) return false;
        // Reject tokens with only spaces
        bool hasNonSpace = false;
        for (char c : data) {
            if (c != ' ') { hasNonSpace = true; break; }
        }
        return hasNonSpace;
    }

    // Create token with auto-ID
    Token createToken(const string& data, int priority = 2) {
        if (!validate(data)) {
            throw invalid_argument("Invalid token data: '" + data + "'");
        }
        return Token(nextId++, data, priority);
    }

    int getNextId() const { return nextId; }
};

// ─────────────────────────────────────────────
// MODULE 2: Processor — FIFO queue with buffer-overflow protection
// ─────────────────────────────────────────────
class ProcessorModule {
private:
    deque<Token> buffer;       // STL deque for O(1) front/back ops
    size_t       maxCapacity;  // Buffer overflow protection
    int          totalEnqueued;
    int          totalDequeued;

public:
    ProcessorModule(size_t capacity = 1000)
        : maxCapacity(capacity), totalEnqueued(0), totalDequeued(0) {}

    // O(1) enqueue — buffer overflow safe
    bool enqueue(const Token& token) {
        if (buffer.size() >= maxCapacity) {
            cerr << "  [OVERFLOW PROTECTION] Buffer full ("
                 << maxCapacity << " tokens max). Token "
                 << token.id << " rejected.\n";
            return false;
        }
        buffer.push_back(token);
        totalEnqueued++;
        return true;
    }

    // O(1) dequeue — FIFO order
    bool dequeue(Token& out) {
        if (buffer.empty()) return false;
        out = buffer.front();
        buffer.pop_front();
        totalDequeued++;
        return true;
    }

    // Peek at front without removing
    bool peek(Token& out) const {
        if (buffer.empty()) return false;
        out = buffer.front();
        return true;
    }

    size_t size()     const { return buffer.size(); }
    bool   isEmpty()  const { return buffer.empty(); }
    size_t capacity() const { return maxCapacity; }
    int    getTotalEnqueued() const { return totalEnqueued; }
    int    getTotalDequeued() const { return totalDequeued; }

    // View all tokens in buffer
    void viewBuffer() const {
        if (buffer.empty()) {
            cout << "  [Buffer is empty]\n";
            return;
        }
        cout << "  " << left << setw(6) << "ID"
             << setw(30) << "Data"
             << setw(12) << "Priority"
             << "Status\n";
        cout << "  " << string(60, '-') << "\n";
        for (const auto& t : buffer) {
            string pLabel = (t.priority == 1) ? "HIGH"
                          : (t.priority == 3) ? "LOW" : "NORMAL";
            cout << "  " << left << setw(6) << t.id
                 << setw(30) << t.data.substr(0, 28)
                 << setw(12) << pLabel
                 << t.status << "\n";
        }
    }
};

// ─────────────────────────────────────────────
// MODULE 3: Validator — processes and validates dequeued tokens
// ─────────────────────────────────────────────
class ValidatorModule {
public:
    // Simulate token processing with validation
    Token process(Token token) {
        // Rule: tokens starting with "ERR" are invalid
        if (token.data.length() >= 3 &&
            token.data.substr(0, 3) == "ERR") {
            token.status = "INVALID";
        } else {
            token.status = "PROCESSED";
        }
        return token;
    }
};

// ─────────────────────────────────────────────
// MODULE 4: Output — stores and displays processed results
// ─────────────────────────────────────────────
class OutputModule {
private:
    vector<Token> processedLog;
    vector<Token> invalidLog;

public:
    void store(const Token& token) {
        if (token.status == "PROCESSED") {
            processedLog.push_back(token);
        } else {
            invalidLog.push_back(token);
        }
    }

    void displayResults() const {
        cout << "\n  ── Processed Tokens ──\n";
        if (processedLog.empty()) {
            cout << "  [None]\n";
        } else {
            for (const auto& t : processedLog) {
                cout << "  [" << t.id << "] " << t.data << "\n";
            }
        }

        cout << "\n  ── Invalid Tokens ──\n";
        if (invalidLog.empty()) {
            cout << "  [None]\n";
        } else {
            for (const auto& t : invalidLog) {
                cout << "  [" << t.id << "] " << t.data << " → REJECTED\n";
            }
        }

        cout << "\n  ── Summary ──\n";
        cout << "  Processed : " << processedLog.size() << "\n";
        cout << "  Invalid   : " << invalidLog.size() << "\n";
        cout << "  Total     : " << (processedLog.size() + invalidLog.size()) << "\n";
    }

    int getProcessedCount() const { return (int)processedLog.size(); }
    int getInvalidCount()   const { return (int)invalidLog.size(); }
};

// ─────────────────────────────────────────────
// Menu
// ─────────────────────────────────────────────
void printMenu(const ProcessorModule& proc) {
    cout << "\n╔══════════════════════════════════════╗\n";
    cout << "║    Automated Token Queuing System    ║\n";
    cout << "║    Author: Prateek Yadav             ║\n";
    cout << "╚══════════════════════════════════════╝\n";
    cout << "  Buffer: " << proc.size() << "/" << proc.capacity() << " tokens\n";
    cout << "\n  1. Add token to queue\n";
    cout << "  2. Add multiple tokens (batch)\n";
    cout << "  3. Process next token (dequeue)\n";
    cout << "  4. Process all tokens\n";
    cout << "  5. Peek at next token\n";
    cout << "  6. View buffer\n";
    cout << "  7. View results\n";
    cout << "  8. Stats\n";
    cout << "  9. Exit\n";
    cout << "\n  Choice: ";
}

int main() {
    IntakeModule    intake;
    ProcessorModule processor(1000);  // Max 1000 tokens
    ValidatorModule validator;
    OutputModule    output;

    int    choice;
    string input;

    cout << "\n  Welcome to the Automated Token Queuing System\n";
    cout << "  Tip: Tokens starting with 'ERR' will be marked INVALID\n";

    while (true) {
        printMenu(processor);

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  [Invalid input. Enter a number.]\n";
            continue;
        }
        cin.ignore(1000, '\n');

        switch (choice) {

            case 1: {
                cout << "\n  Enter token data: ";
                getline(cin, input);
                cout << "  Priority (1=High, 2=Normal, 3=Low) [default 2]: ";
                string pStr;
                getline(cin, pStr);
                int priority = 2;
                if (!pStr.empty()) {
                    try { priority = stoi(pStr); }
                    catch (...) { priority = 2; }
                    if (priority < 1 || priority > 3) priority = 2;
                }
                try {
                    Token t = intake.createToken(input, priority);
                    if (processor.enqueue(t)) {
                        cout << "  ✓ Token [" << t.id << "] enqueued: " << t.data << "\n";
                    }
                } catch (const invalid_argument& e) {
                    cout << "  ✗ " << e.what() << "\n";
                }
                break;
            }

            case 2: {
                cout << "\n  How many tokens to add? ";
                int n;
                if (!(cin >> n) || n <= 0 || n > 100) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "  [Enter a number between 1 and 100]\n";
                    break;
                }
                cin.ignore(1000, '\n');
                int added = 0;
                for (int i = 0; i < n; i++) {
                    cout << "  Token " << (i + 1) << "/" << n << ": ";
                    getline(cin, input);
                    try {
                        Token t = intake.createToken(input);
                        if (processor.enqueue(t)) added++;
                    } catch (const invalid_argument& e) {
                        cout << "  ✗ Skipped: " << e.what() << "\n";
                    }
                }
                cout << "  ✓ " << added << " tokens added to queue.\n";
                break;
            }

            case 3: {
                Token t(0, "");
                if (processor.dequeue(t)) {
                    t = validator.process(t);
                    output.store(t);
                    cout << "  ✓ Processed Token [" << t.id << "]: "
                         << t.data << " → " << t.status << "\n";
                } else {
                    cout << "  [Queue is empty]\n";
                }
                break;
            }

            case 4: {
                if (processor.isEmpty()) {
                    cout << "  [Queue is empty]\n";
                    break;
                }
                int count = 0;
                Token t(0, "");
                while (processor.dequeue(t)) {
                    t = validator.process(t);
                    output.store(t);
                    count++;
                }
                cout << "  ✓ Processed " << count << " tokens.\n";
                cout << "  Valid: " << output.getProcessedCount()
                     << "  |  Invalid: " << output.getInvalidCount() << "\n";
                break;
            }

            case 5: {
                Token t(0, "");
                if (processor.peek(t)) {
                    cout << "  Next token → [" << t.id << "] " << t.data << "\n";
                } else {
                    cout << "  [Queue is empty]\n";
                }
                break;
            }

            case 6: {
                cout << "\n  ── Queue Buffer ──\n";
                processor.viewBuffer();
                break;
            }

            case 7: {
                output.displayResults();
                break;
            }

            case 8: {
                cout << "\n  ── System Stats ──\n";
                cout << "  Buffer size    : " << processor.size() << "\n";
                cout << "  Total enqueued : " << processor.getTotalEnqueued() << "\n";
                cout << "  Total dequeued : " << processor.getTotalDequeued() << "\n";
                cout << "  Processed OK   : " << output.getProcessedCount() << "\n";
                cout << "  Invalid tokens : " << output.getInvalidCount() << "\n";
                break;
            }

            case 9: {
                cout << "\n  Exiting. Goodbye!\n\n";
                return 0;
            }

            default:
                cout << "  [Invalid choice. Enter 1–9.]\n";
        }
    }

    return 0;
}
