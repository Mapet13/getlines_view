#include <fstream>
#include <ranges>
#include <string>

namespace utils {
    class getlines_view : public std::ranges::view_interface<getlines_view> {
        std::istream* in_;
        std::string str_{};
        char delim_;

        struct cursor {
            getlines_view* rng_ = nullptr;

           public:
            cursor() = default;

            explicit cursor(getlines_view* rng)
                : rng_(rng) {}

            void next() {
                rng_->next();
            }
            std::string& read() const noexcept {
                return rng_->str_;
            }
            bool equal(std::default_sentinel_t) const {
                return !rng_->in_;
            }
            bool equal(cursor that) const {
                return !rng_->in_ == !that.rng_->in_;
            }
        };

       public:
        class iterator {
            using Base = const std::string;

            cursor cursor_;

           public:
            using iterator_category = std::input_iterator_tag;
            using reference = Base&;
            using value_type = Base;
            using difference_type = std::ranges::range_difference_t<Base>;

            iterator() = default;

            iterator(cursor cursor)
                : cursor_{ cursor } {}

            decltype(auto) operator*() {
                return cursor_.read();
            }

            decltype(auto) operator*() const {
                return cursor_.read();
            }

            iterator& operator++() {
                cursor_.next();
                return *this;
            }

            void operator++(int) {
                (void)operator++();
            }

            friend bool operator==(const iterator& x,
                                   const iterator& y) {
                return x.cursor_.equal(y.cursor_);
            }

            friend bool operator!=(const iterator& x,
                                   const iterator& y) {
                return !(x == y);
            }

            friend bool operator==(const iterator& x,
                                   const std::default_sentinel_t sentinel) {
                return x.cursor_.equal(sentinel);
            }

            friend bool operator!=(const iterator& x,
                                   const std::default_sentinel_t sentinel) {
                return !(x == sentinel);
            }
        };

       public:
        using iterator_t = iterator;
        using sentinel_t = std::default_sentinel_t;

        getlines_view() = default;
        getlines_view(std::istream& in, char delim = '\n')
            : in_(&in), delim_(delim) {
            this->next();
        }

        void next() {
            if (!std::getline(*in_, str_, delim_)) {
                in_ = nullptr;
            }
        }

        auto begin() {
            return iterator{ cursor{ this } };
        }

        auto end() const noexcept {
            return std::default_sentinel;
        }
    };

    namespace views {

        namespace detail {
            struct getlines_fn {
                getlines_view operator()(std::istream& in, char delim = '\n') const {
                    return getlines_view{ in, delim };
                }

                template <typename T>
                friend constexpr auto operator|(T&& v, getlines_fn fn) {
                    return fn(std::forward<T>(v));
                }
            };
        }  // namespace detail

        inline constexpr auto getlines = detail::getlines_fn{};
    }  // namespace views

}  // namespace utils
