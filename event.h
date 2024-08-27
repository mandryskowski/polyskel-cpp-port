#ifndef EVENT_H
#define EVENT_H
#include <memory>
#include <string>
#include <sstream>

#include "vec.h"
#include "lavertex.h"

namespace polyskel {
    class Event {
    public:
        double distance;
        Vec2 intersection_point;

        Event(double dist, Vec2 inter_point) : distance(dist), intersection_point(inter_point) {}

        bool operator<(const Event& other) const {
            return distance < other.distance;
        }

        virtual std::string toString() const = 0;

        virtual ~Event() = default;
    };

    class SplitEvent : public Event {
    public:
        shared_ptr<LAVertex> vertex;
        Edge opposite_edge;

        SplitEvent(double dist, Vec2 inter_point, shared_ptr<LAVertex> vert, Edge opp_edge)
            : Event(dist, inter_point), vertex(vert), opposite_edge(opp_edge) {}


        virtual std::string toString() const override {
            std::ostringstream os;
            os << distance << " Split event @ " << intersection_point << " from " << vertex << " to " << opposite_edge;
            return os.str();
        }
    };

    class EdgeEvent : public Event {
    public:
        shared_ptr<LAVertex> vertex_a;
        shared_ptr<LAVertex> vertex_b;

        EdgeEvent(double dist, Vec2 inter_point, shared_ptr<LAVertex> vert_a, shared_ptr<LAVertex> vert_b)
            : Event(dist, inter_point), vertex_a(vert_a), vertex_b(vert_b) {}

        virtual std::string toString() const override {
            std::ostringstream os;
            os << distance << " Edge event @ " << intersection_point << " between " << vertex_a->toString() << " and " << vertex_b->toString();
            return os.str();
        }
    };
    // Wrapper class to store Event and its insertion index
    class EventWrapper {
    public:
        std::shared_ptr<Event> event;
        size_t index;

        EventWrapper(std::shared_ptr<Event> e, size_t idx) : event(e), index(idx) {}
    };

    class EventQueue {
    public:
        std::vector<EventWrapper> data;
        size_t insertion_counter = 0;

    public:
        EventQueue() = default;

        auto heap_func() {
            return [](const EventWrapper& lhs, const EventWrapper& rhs) {
                if (lhs.event->distance == rhs.event->distance) {
                    return lhs.index > rhs.index; // Min-heap: Earlier index has higher priority
                }
                return lhs.event->distance > rhs.event->distance; // Min-heap: Smaller distance has higher priority
            };
        }

        void put(std::shared_ptr<Event> item) {
            if (item) {
                data.emplace_back(item, insertion_counter++);
                std::push_heap(data.begin(), data.end(), heap_func());
            }
        }

        void put_all(const std::vector<std::shared_ptr<Event>>& items) {
            for (auto& item : items) {
                data.emplace_back(item, insertion_counter++);
                std::push_heap(data.begin(), data.end(), heap_func());
            }
        }

        std::shared_ptr<Event> get() {
            std::pop_heap(data.begin(), data.end(), heap_func());
            auto item = data.back().event;
            data.pop_back();
            return item;
        }

        bool empty() const {
            return data.empty();
        }

        std::shared_ptr<Event> peek() const {
            if (!data.empty()) {
                return data.front().event;
            }
            throw std::out_of_range("Queue is empty");
        }

        void show() const {
            for (const auto& item : data) {
                std::cout << item.event->distance << "(" << item.index << ") ";
            }
            std::cout << std::endl;
        }
    };
} // namespace polyskel
#endif //EVENT_H