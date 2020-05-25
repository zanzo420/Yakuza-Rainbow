#pragma once

#include <type_traits>
#include <intrin.h>
#include <random>
#include <array>

namespace hardware_id
{
    namespace random
    {
        namespace detail
        {
            inline static std::mt19937& get_generator()
            {
                // ReSharper disable once CppLocalVariableMayBeConst
                thread_local std::mt19937 generator{ std::random_device{}() };
                return generator;
            }
        }

        inline void set_seed(const unsigned seed)
        {
            detail::get_generator().seed(seed);
        }

        template <typename T>
        inline T get_integer(T min, T max)
        {
            std::uniform_int_distribution<T> distribution{ min, max };
            return distribution(detail::get_generator());
        }

        inline char get_char()
        {
            const auto value = get_integer(0u, 61u);

            if (value <= 9)
                return static_cast<char>(value + 48);

            if (value <= 35)
                return static_cast<char>(value + 55);

            return static_cast<char>(value + 61);
        }

        inline std::string get_string(const unsigned length)
        {
            const auto buffer = std::make_unique<char[]>(length);
            std::generate(buffer.get(), buffer.get() + length, []() -> char { return get_char(); });

            return { buffer.get(), buffer.get() + length };
        }
    }

    namespace hash
    {
        namespace detail
        {
            inline static constexpr uint32_t prime = 0x01000193;
            inline static constexpr uint32_t seed = 0x811C9DC5;

            // ReSharper disable once CppInconsistentNaming
            inline static uint32_t fnv1a_impl(const uint8_t value, const uint32_t hash = seed) noexcept
            {
                return (value ^ hash) * prime;
            }

            // ReSharper disable once CppInconsistentNaming
            template <typename T>
            inline static uint32_t fnv1a_impl(const T value, uint32_t hash = seed) noexcept
            {
                for (auto ptr = reinterpret_cast<const uint8_t*>(&value); ptr < reinterpret_cast<const uint8_t*>(&value + 1); ++ptr)
                    hash = fnv1a_impl(*ptr, hash);

                return hash;
            }
        }

        inline constexpr uint32_t get_seed() noexcept
        {
            return detail::seed;
        }

        // ReSharper disable once CppInconsistentNaming
        template <typename... Ts>
        inline uint32_t fnv1a(Ts... values)
        {
            static_assert(sizeof...(Ts) > 0, "fnv1a() takes one or more arguments");
            static_assert((... && std::is_trivially_copyable<Ts>::value), "Ts... must contain only trivially copyable types");

            std::tuple<Ts...> data{ std::forward<Ts>(values)... };

            return detail::fnv1a_impl(data);
        }

        // ReSharper disable once CppInconsistentNaming
        template <typename... Ts>
        inline uint32_t fnv1a_specify_hash(uint32_t hash, Ts... values)
        {
            static_assert(sizeof...(Ts) > 0, "fnv1a_specify_hash() takes one or more arguments");
            static_assert((... && std::is_trivially_copyable<Ts>::value), "Ts... must contain only trivially copyable types");

            std::tuple<Ts...> data{ std::forward<Ts>(values)... };

            return detail::fnv1a_impl(data, hash);
        }
    }

    namespace cpuid
    {
        namespace detail
        {
            inline static constexpr auto basic_input_start = 0u;
            inline static constexpr auto extended_function_input_start = 0x80000000u;
        }

        using leaf_t = std::array<uint32_t, 4>;

        inline uint32_t get_max_basic_input() noexcept
        {
            static_assert(sizeof(int) == sizeof(uint32_t));

            auto cpu_leaf = leaf_t{ 0u, 0u, 0u, 0u };
            auto& [eax, ebx, ecx, edx] = cpu_leaf;

            __cpuidex(reinterpret_cast<int*>(cpu_leaf.data()), detail::basic_input_start, 0);
            return eax;
        }

        inline uint32_t get_max_extended_function_input() noexcept
        {
            static_assert(sizeof(int) == sizeof(uint32_t));

            auto cpu_leaf = leaf_t{ 0u, 0u, 0u, 0u };
            auto& [eax, ebx, ecx, edx] = cpu_leaf;

            __cpuidex(reinterpret_cast<int*>(cpu_leaf.data()), detail::extended_function_input_start, 0);
            return eax - detail::extended_function_input_start;
        }

        inline leaf_t get_basic_leaf(const uint32_t leaf) noexcept
        {
            static_assert(sizeof(int) == sizeof(uint32_t));

            auto cpu_leaf = leaf_t{ 0u, 0u, 0u, 0u };
            __cpuidex(reinterpret_cast<int*>(cpu_leaf.data()), static_cast<int>(detail::basic_input_start + leaf), 0);

            return cpu_leaf;
        }

        inline leaf_t get_extended_function_leaf(const uint32_t leaf) noexcept
        {
            static_assert(sizeof(int) == sizeof(uint32_t));

            auto cpu_leaf = leaf_t{ 0u, 0u, 0u, 0u };
            __cpuidex(reinterpret_cast<int*>(cpu_leaf.data()), static_cast<int>(detail::extended_function_input_start + leaf), 0);

            return cpu_leaf;
        }
    }

    namespace detail
    {
        inline uint32_t generate_cpuid_hash()
        {
            auto cpu_hash = hash::get_seed();

            const auto max_basic_input = cpuid::get_max_basic_input();
            const auto max_extended_input = cpuid::get_max_extended_function_input();

            for (auto input = 0u; input <= max_basic_input; ++input) // process basic leaves
            {
                auto basic_leaf = cpuid::get_basic_leaf(input);
                auto& [eax, ebx, ecx, edx] = basic_leaf;

                switch (input)
                {
                case 0x01: // Basic CPUID Information
                    ebx &= 0x00FF'FFFF; // Clear CPUID.01H:EBX[31:24], Initial APIC ID, varies on Intel/AMD/Ryzen
                    break;
                case 0x0B: // Extended Topology Enumeration Leaf
                    edx &= 0x0000'0000; // Clear CPUID.0BH:EDX[31:0], x2APIC ID the current logical processor, varies on Intel
                    break;
                case 0x1F: // V2 Extended Topology Enumeration Leaf
                    eax &= 0xFFFF'FFE0; // Clear CPUID.1FH:EAX[4:0], Number of bits to shift right on x2APIC ID to get a unique topology ID of the next level type, could vary
                    edx &= 0x0000'0000; // Clear CPUID.1FH:EDX[31:0], x2APIC ID the current logical processor, could vary
                    break;
                default:
                    break;
                }

                cpu_hash = hash::fnv1a_specify_hash(cpu_hash, edx, ecx, ebx, eax);
            }

            for (auto input = 0u; input <= max_extended_input; ++input) // process extended function leaves
            {
                auto extended_leaf = cpuid::get_extended_function_leaf(input);
                auto& [eax, ebx, ecx, edx] = extended_leaf;

                switch (input)
                {
                case 0x01: // Feature Identifiers
                    ecx &= 0xFFF8'3FFF; // Clear CPUID.80000001H:ECX[18:14], Reserved, varies on AMD
                    break;
                case 0x1E: // Unknown
                    eax &= 0x0000'0000; // Clear CPUID.8000001EH:EAX[31:0], Unknown, varies on AMD/Ryzen
                    ebx &= 0x0000'0000; // Clear CPUID.8000001EH:EBX[31:0], Unknown, varies on AMD/Ryzen
                    break;
                default:
                    break;
                }

                cpu_hash = hash::fnv1a_specify_hash(cpu_hash, edx, ecx, ebx, eax);
            }

            return cpu_hash;
        }
    }

    inline std::string generate_uuid()
    {
        random::set_seed(detail::generate_cpuid_hash());

        return "{" + random::get_string(8u) + "-" + random::get_string(4u) + "-" + random::get_string(4u) + "-" + random::get_string(4u) + "-" + random::get_string(12u) + "}";
    }
}