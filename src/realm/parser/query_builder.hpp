////////////////////////////////////////////////////////////////////////////
//
// Copyright 2015 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#ifndef REALM_QUERY_BUILDER_HPP
#define REALM_QUERY_BUILDER_HPP

#include <string>
#include <memory>
#include <vector>

#include <realm/binary_data.hpp>
#include <realm/null.hpp>
#include <realm/string_data.hpp>
#include <realm/timestamp.hpp>
#include <realm/table.hpp>
#include <realm/util/any.hpp>
#include <realm/util/string_buffer.hpp>

namespace realm {
class Query;
class Realm;
class Table;
template<typename> class BasicRowExpr;
using RowExpr = BasicRowExpr<Table>;

namespace parser {
    struct Predicate;
    struct DescriptorOrderingState;
}

namespace query_builder {
class Arguments;

void apply_predicate(Query& query, const parser::Predicate& predicate, Arguments& arguments);
void apply_predicate(Query& query, const parser::Predicate& predicate); // zero out of string args version

void apply_ordering(DescriptorOrdering& ordering, TableRef target, const parser::DescriptorOrderingState& state, Arguments& arguments);
void apply_ordering(DescriptorOrdering& ordering, TableRef target, const parser::DescriptorOrderingState& state);


struct AnyContext
{
    template<typename T>
    T unbox(const util::Any& wrapper) {
        return util::any_cast<T>(wrapper);
    }
    bool is_null(const util::Any& wrapper) {
        if (!wrapper.has_value()) {
            return true;
        }
        if (wrapper.type() == typeid(realm::null)) {
            return true;
        }
        return false;
    }
};

class Arguments {
public:
    virtual bool bool_for_argument(size_t argument_index) = 0;
    virtual long long long_for_argument(size_t argument_index) = 0;
    virtual float float_for_argument(size_t argument_index) = 0;
    virtual double double_for_argument(size_t argument_index) = 0;
    virtual StringData string_for_argument(size_t argument_index) = 0;
    virtual BinaryData binary_for_argument(size_t argument_index) = 0;
    virtual Timestamp timestamp_for_argument(size_t argument_index) = 0;
    virtual size_t object_index_for_argument(size_t argument_index) = 0;
    virtual bool is_argument_null(size_t argument_index) = 0;
    util::StringBuffer buffer_space; // dynamic conversion space with lifetime tied to this
};

template<typename ValueType, typename ContextType>
class ArgumentConverter : public Arguments {
public:
    ArgumentConverter(ContextType& context, const ValueType* arguments, size_t count)
    : m_ctx(context)
    , m_arguments(arguments)
    , m_count(count)
    {}

    bool bool_for_argument(size_t i) override { return get<bool>(i); }
    long long long_for_argument(size_t i) override { return get<int64_t>(i); }
    float float_for_argument(size_t i) override { return get<float>(i); }
    double double_for_argument(size_t i) override { return get<double>(i); }
    StringData string_for_argument(size_t i) override { return get<StringData>(i); }
    BinaryData binary_for_argument(size_t i) override { return get<BinaryData>(i); }
    Timestamp timestamp_for_argument(size_t i) override { return get<Timestamp>(i); }
    size_t object_index_for_argument(size_t i) override { return get<RowExpr>(i).get_index(); }
    bool is_argument_null(size_t i) override { return m_ctx.is_null(at(i)); }

private:
    ContextType& m_ctx;
    const ValueType* m_arguments;
    size_t m_count;

    const ValueType& at(size_t index) const
    {
        if (index >= m_count)
            throw std::out_of_range("vector");
        return m_arguments[index];
    }

    template<typename T>
    T get(size_t index) const
    {
        return m_ctx.template unbox<T>(at(index));
    }
};
} // namespace query_builder
} // namespace realm

#endif // REALM_QUERY_BUILDER_HPP
