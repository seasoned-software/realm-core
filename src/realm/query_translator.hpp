/*************************************************************************
 *
 * Copyright 2016 Realm Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 **************************************************************************/

#ifndef REALM_QUERY_TRANSLATOR_HPP
#define REALM_QUERY_TRANSLATOR_HPP

#include <realm/query.hpp>
#include <realm/impl/transact_log.hpp>

namespace realm {

enum class QueryInstruction {
    instr_or,
    instr_and,
    instr_gt,
    instr_lt,
    instr_eq,
    instr_gte,
    instr_lte,
    instr_like,
};

class QueryEncoder : _impl::GenericLogEncoder {
public:
    template <class... L>
    void encode(QueryInstruction instruction, L... numbers);
    template <class C>
    void serialize_condition();
    template <typename T>
    void serialze_value(T v);
};

class QueryTranslator {
public:
    Query translate(char* stream, size_t size, TableRef target_table);
    std::unique_ptr<util::Buffer<char>> translate(const Query& q);

protected:
    QueryEncoder m_encoder;
};


template <typename T>
void QueryEncoder::serialze_value(T v)
{
    append_simple_instr(v);
}


} // namespace realm

#endif // REALM_QUERY_TRANSLATOR_HPP
