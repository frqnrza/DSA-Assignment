#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstdint>

using namespace std;

const int Bits_Per_Node = 64;
const int Nodes = 1024 / Bits_Per_Node;
const uint64_t MAX_UINT64 = UINT64_MAX;

struct Node {
    uint64_t value;
    Node* next;
    Node(uint64_t val = 0) : value(val), next(nullptr) {}
};

class LargeNumber {
private:
    Node* head;

    void deepCopy(const LargeNumber& other) {
        Node* otherCurrent = other.head;
        Node** current = &head;
        while (otherCurrent) {
            *current = new Node(otherCurrent->value);
            current = &((*current)->next);
            otherCurrent = otherCurrent->next;
        }
    }

public:
    LargeNumber() : head(nullptr) {}

    LargeNumber(uint64_t val) : head(nullptr) {
        head = new Node(val);
    }

    LargeNumber(const LargeNumber& other) : head(nullptr) {
        deepCopy(other);
    }

    LargeNumber& operator=(const LargeNumber& other) {
        if (this != &other) {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            deepCopy(other);
        }
        return *this;
    }

    void generateRandom() {
        srand(time(nullptr));
        for (int i = 0; i < Nodes; ++i) {
            uint64_t randomValue = ((uint64_t)rand() << 32) | rand();
            if (i == Nodes - 1) {
                randomValue |= (1ULL << 63); // Ensure the number is 1024 bits
            }
            Node* newNode = new Node(randomValue);
            newNode->next = head;
            head = newNode;
        }
    }

    bool isEven() const {
        return head && !(head->value & 1);
    }

    bool isZero() const {
        Node* current = head;
        while (current) {
            if (current->value != 0) return false;
            current = current->next;
        }
        return true;
    }

    bool isOne() const {
        if (!head || head->value != 1) return false;
        Node* current = head->next;
        while (current) {
            if (current->value != 0) return false;
            current = current->next;
        }
        return true;
    }

    void divideByTwo() {
        uint64_t carry = 0;
        Node* current = head;
        while (current) {
            uint64_t newValue = (current->value >> 1) | (carry << 63);
            carry = current->value & 1;
            current->value = newValue;
            current = current->next;
        }
        if (head && head->value == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void Subtract(const LargeNumber& other) {
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;
        uint64_t borrow = 0;

        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value >= otherCurrent->value + borrow) {
                thisCurrent->value -= otherCurrent->value + borrow;
                borrow = 0;
            } else {
                thisCurrent->value = (MAX_UINT64 - otherCurrent->value - borrow + 1 + thisCurrent->value);
                borrow = 1;
            }
            thisCurrent = thisCurrent->next;
            otherCurrent = otherCurrent->next;
        }

        while (thisCurrent && borrow) {
            if (thisCurrent->value >= borrow) {
                thisCurrent->value -= borrow;
                borrow = 0;
            } else {
                thisCurrent->value = MAX_UINT64 - borrow + 1;
                borrow = 1;
            }
            thisCurrent = thisCurrent->next;
        }

        // Remove leading zeros
        while (head && head->next && head->value == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    LargeNumber mod(const LargeNumber& m) const {
        LargeNumber result = *this;
        while (result >= m) {
            result.Subtract(m);
        }
        return result;
    }

    LargeNumber modMul(const LargeNumber& other, const LargeNumber& m) const {
        LargeNumber result;
        LargeNumber temp = *this;

        Node* otherCurrent = other.head;
        while (otherCurrent) {
            for (int i = 0; i < 64; ++i) {
                if (otherCurrent->value & (1ULL << i)) {
                    result = (result + temp).mod(m);
                }
                temp = (temp + temp).mod(m);
            }
            otherCurrent = otherCurrent->next;
        }

        return result;
    }

    LargeNumber modPow(const LargeNumber& exponent, const LargeNumber& m) const {
        if (m.isOne()) return LargeNumber(0);

        LargeNumber result(1);
        LargeNumber base = this->mod(m);
        LargeNumber exp = exponent;

        while (!exp.isZero()) {
            if (!exp.isEven()) {
                result = result.modMul(base, m);
            }
            base = base.modMul(base, m);
            exp.divideByTwo();
        }

        return result;
    }

    bool operator==(const LargeNumber& other) const {
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;

        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value != otherCurrent->value) return false;
            thisCurrent = thisCurrent->next;
            otherCurrent = otherCurrent->next;
        }

        return !thisCurrent && !otherCurrent;
    }

    bool operator>=(const LargeNumber& other) const {
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;

        // Skip leading zeros
        while (thisCurrent && thisCurrent->value == 0) thisCurrent = thisCurrent->next;
        while (otherCurrent && otherCurrent->value == 0) otherCurrent = otherCurrent->next;

        // Compare number of significant nodes
        int thisNodes = 0, otherNodes = 0;
        Node* temp = thisCurrent;
        while (temp) {
            thisNodes++;
            temp = temp->next;
        }
        temp = otherCurrent;
        while (temp) {
            otherNodes++;
            temp = temp->next;
        }

        if (thisNodes > otherNodes) return true;
        if (thisNodes < otherNodes) return false;

        // Compare node values from most significant to least
        while (thisCurrent && otherCurrent) {
            if (thisCurrent->value > otherCurrent->value) return true;
            if (thisCurrent->value < otherCurrent->value) return false;
            thisCurrent = thisCurrent->next;
            otherCurrent = otherCurrent->next;
        }

        return true; // Numbers are equal
    }

    LargeNumber operator+(const LargeNumber& other) const {
        LargeNumber result;
        Node** resultCurrent = &result.head;
        Node* thisCurrent = head;
        Node* otherCurrent = other.head;
        bool carry = false;

        while (thisCurrent || otherCurrent || carry) {
            uint64_t sum = carry ? 1 : 0;
            if (thisCurrent) {
                sum += thisCurrent->value;
                thisCurrent = thisCurrent->next;
            }
            if (otherCurrent) {
                sum += otherCurrent->value;
                otherCurrent = otherCurrent->next;
            }

            *resultCurrent = new Node(sum);
            resultCurrent = &((*resultCurrent)->next);

            carry = (sum < (carry ? 1 : 0)) ||
                    (thisCurrent && sum < thisCurrent->value) ||
                    (otherCurrent && sum < otherCurrent->value);
        }

        return result;
    }


    LargeNumber operator-(const LargeNumber& other) const {
        LargeNumber result = *this;
        result.Subtract(other);
        return result;
    }

    bool IsPrime() {
        if (isEven()) return false;

        LargeNumber two(2);
        LargeNumber one(1);

        LargeNumber d = *this - one;
        int s = 0;
        while (d.isEven()) {
            d.divideByTwo();
            s++;
        }

        for (int i = 0; i < 5; i++) { // 5 iterations for Miller-Rabin
            LargeNumber a;
            a.generateRandom();
            a = a.mod(*this);

            if (a.isZero() || a.isOne()) continue;

            LargeNumber x = a.modPow(d, *this);

            if (x.isOne() || x == *this - one) continue;

            bool composite = true;
            for (int j = 0; j < s - 1; j++) {
                x = x.modMul(x, *this);
                if (x == *this - one) {
                    composite = false;
                    break;
                }
            }

            if (composite) return false;
        }

        return true;
    }

    void Print_Number() const {
        Node* current = head;
        while (current) {
            cout << current->value << " ";
            current = current->next;
        }
        cout << endl;
    }

    ~LargeNumber() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

int main() {
    cout<< "\n\t\tWelcome to C++ Based Prime Number Checker for Large Integers \n"<<endl;
    LargeNumber largeNum;
    largeNum.generateRandom();

    cout << "Generated 1024-bit Number : ";
    largeNum.Print_Number();

    bool isPrime = largeNum.IsPrime();
    cout << "\nIs Number Prime ? : " << (isPrime ? " Yes " : " No ") << endl;
    cout<< "\n\t\tThanks for Using Our System "<<endl;
    return 0;
}