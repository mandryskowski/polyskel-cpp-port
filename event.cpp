#include "event.h"

using namespace polyskel;


void EventQueue::put(std::shared_ptr<Event> item) {
    if (item) {
        data.emplace_back(item, insertion_counter++);
        std::push_heap(data.begin(), data.end(), heap_func());
    }
}

void EventQueue::put_all(const std::vector<std::shared_ptr<Event>>& items) {
    for (auto& item : items) {
        data.emplace_back(item, insertion_counter++);
        std::push_heap(data.begin(), data.end(), heap_func());
    }
}

std::shared_ptr<Event> EventQueue::get() {
    std::pop_heap(data.begin(), data.end(), heap_func());
    auto item = data.back().event;
    data.pop_back();
    return item;
}

bool EventQueue::empty() const {
    return data.empty();
}

std::shared_ptr<Event> EventQueue::peek() const {
    if (!data.empty()) {
        return data.front().event;
    }
    throw std::out_of_range("Queue is empty");
}

void EventQueue::show() const {
    for (const auto& item : data) {
        std::cout << item.event->distance << "(" << item.index << ") ";
    }
    std::cout << std::endl;
}