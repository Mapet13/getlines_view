# getlines_view implementation made for learning purposes

### inspired by [range-v3](https://github.com/ericniebler/range-v3) and [TartanLlama - ranges](https://github.com/TartanLlama/ranges)

#### example:
```cpp
std::ifstream input{ "input.txt" };
for (const auto& line: input | utils::views::getlines) {
    std::cout << line << '\n';
}
```
