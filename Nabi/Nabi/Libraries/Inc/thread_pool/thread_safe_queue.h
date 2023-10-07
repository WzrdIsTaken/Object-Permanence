#pragma once

#include <concepts>
#include <deque>
//#include <limits>
#include <mutex>
#include <optional>

// https://github.com/DeveloperPaul123/thread-pool/tree/master | Paul T & Adam Lugowski | MIT License

namespace dp {
    /**
     * @brief Simple concept for the Lockable and Basic Lockable types as defined by the C++
     * standard.
     * @details See https://en.cppreference.com/w/cpp/named_req/Lockable and
     * https://en.cppreference.com/w/cpp/named_req/BasicLockable for details.
     */
    template <typename Lock>
    concept is_lockable = requires(Lock && lock) {
        lock.lock();
        lock.unlock();
        { lock.try_lock() } -> std::convertible_to<bool>;
    };

    template <typename T, typename Lock = std::mutex>
    requires is_lockable<Lock>
        class thread_safe_queue {
        public:
            using value_type = T;
            using size_type = typename std::deque<T>::size_type;

            thread_safe_queue() = default;

            void push(T&& value) {
                std::lock_guard lock(mutex_);
                data_.push_back(std::forward<T>(value));
            }

            [[nodiscard]] bool empty() const {
                std::lock_guard lock(mutex_);
                return data_.empty();
            }

            [[nodiscard]] std::optional<T> pop() {
                std::lock_guard lock(mutex_);
                if (data_.empty()) return std::nullopt;

                auto front = std::move(data_.front());
                data_.pop_front();
                return front;
            }

            [[nodiscard]] std::optional<T> steal() {
                std::lock_guard lock(mutex_);
                if (data_.empty()) return std::nullopt;

                auto back = std::move(data_.back());
                data_.pop_back();
                return back;
            }

        private:
            std::deque<T> data_{};
            mutable Lock mutex_{};
    };
}  // namespace dp

/*
* A WIP idea to allow the thead_pool to support tasks with priorities
* However, I think its more important right now to focus on getting the functional stuff in, rather than this optimization which I might never need.
* What I was doing here was pretty close though I think! maybe...

    namespace nabi
    {
        template <typename T, typename Lock = std::mutex>
        requires dp::is_lockable<Lock>
            class thread_safe_priority_queue {
            public:
                using priority_type = unsigned int;
                using value_type = T;
                using size_type = typename std::deque<T>::size_type;
                static priority_type constexpr null_priority = std::numeric_limits<priority_type>::max();

                thread_safe_priority_queue() = default;

                void push(T&& value, priority_type priority = null_priority) {
                    std::lock_guard lock(mutex_);

                    item const item = {
                        priority,
                        std::forward<T>(value)
                    };
                    data_.push_back(item);
                    std::push_heap(data_.begin(), data_.end(), comparator_less_);
                }

                [[nodiscard]] bool empty() const {
                    std::lock_guard lock(mutex_);
                    return data_.empty();
                }

                [[nodiscard]] std::optional<T> pop() {
                    std::lock_guard lock(mutex_);
                    if (data_.empty()) return std::nullopt;

                    auto front = std::move(data_.front());
                    std::pop_heap(data_.begin(), data_.end(), comparator_less_);

                    data_.pop_front();
                    return front.data;
                }

                [[nodiscard]] std::optional<T> steal() {
                    std::lock_guard lock(mutex_);
                    if (data_.empty()) return std::nullopt;

                    auto back = std::move(data_.back());
                    std::pop_heap(data_.begin(), data_.end(), comparator_greater_);

                    data_.pop_back();
                    return back.data;
                }

            private:
                struct item;
                using item_comparator_type = 
                    std::function<bool(item const&, item const&)>;

                struct item final
                {
                    priority_type priority;
                    T data;

                    [[nodiscard]] bool operator<(item const& other) const noexcept
                    {
                        return this->priority < other.priority;
                    }

                    [[nodiscard]] bool operator>(item const& other) const noexcept
                    {
                        return this->priority > other.priority;
                    }
                };

                std::deque<item> data_{};
                mutable Lock mutex_{};

                item_comparator_type comparator_less_ = 
                    [](item const& lhs, item const& rhs) -> bool {
                    return lhs < rhs;
                };
                item_comparator_type comparator_greater_ = 
                    [](item const& lhs, item const& rhs) -> bool {
                    return lhs > rhs;
                };
        };  
    } // namespace nabi
*/
