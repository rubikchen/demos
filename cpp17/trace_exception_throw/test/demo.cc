#include <future>
int main() {
    std::async(std::launch::async, []() noexcept {
        throw 1;
    }).wait();
}